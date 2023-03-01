#include "inspect.h"
#include "memalloc.h"
#include "iio.h"

/**
 * \param currMB
 * \param currSlice
 * \param out_coeffs the output frame residual, of size (planes, H, W)
 */
void extract_coeffs(Macroblock* currMB, Slice* currSlice, float*** out_coeffs)
{
  int pos_x = currMB->mb_x * MB_BLOCK_SIZE;
  int pos_y = currMB->mb_y * MB_BLOCK_SIZE;

  // printf("pos_x = %d \n", pos_x);
  // printf("pos_y = %d \n", pos_y);

  int pl, i, j;
  // xml_write_start_element("Coeffs");
  if (currMB->luma_transform_size_8x8_flag) {
    // Luma
    for (i = 0; i < 16; i++) {
      for (j = 0; j < 16; j++) {
        out_coeffs[0][pos_y + i][pos_x + j] = (&currSlice->mb_rres[0][0])[i][j];
      }
    }
    // Chroma
    for (pl = 1; pl <= 2; pl++) {
      for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
          out_coeffs[pl][pos_y + i][pos_x + j] = currSlice->cof[pl][i][j];
        }
      }
    }
  } else {
    // Luma
    for (i = 0; i < 16; i++) {
      for (j = 0; j < 16; j++) {
        out_coeffs[0][pos_y + i][pos_x + j] = currSlice->cof[0][i][j];
      }
    }
    // Chroma
    for (pl = 1; pl <= 2; pl++) {
      for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
          out_coeffs[pl][pos_y + i][pos_x + j] = currSlice->cof[pl][i][j];
        }
      }
    }
  }
}


/**
 * \param currMB
 * \param currSlice
 * \param out_residual the output frame residual, of size (planes, H, W)
 */
void extract_residual(Macroblock* currMB, Slice* currSlice, float*** out_residual)
{
  int pos_x = currMB->mb_x * MB_BLOCK_SIZE;
  int pos_y = currMB->mb_y * MB_BLOCK_SIZE;


  int pl, i, j;
  float DQ_factor = (1 << DQ_BITS);
  for (pl = 0; pl < 3; pl++) {
    for (i = 0; i < 16; i++) {
      for (j = 0; j < 16; j++) {
        out_residual[pl][pos_y + i][pos_x + j] = currSlice->mb_rres[pl][i][j] / DQ_factor;
      }
    }

  }
  
}


/**
 * \param currMB
 * \param img_type the macroblock type (0 for undefined, 1 for I, 2 for P, and 3 for skip block)
 *  of size (H, W)
 */
void extract_mb_type(Macroblock* currMB, Slice* currSlice, int mb_type, uint8** img_type)
{
  const int pos_x = currMB->mb_x * MB_BLOCK_SIZE;
  const int pos_y = currMB->mb_y * MB_BLOCK_SIZE;

  int value = UNDEFINED_MB;
  switch (currSlice->slice_type)
	{
		case SP_SLICE:
		case P_SLICE:
      // printf("mb_type: %d\n", mb_type);
      if(mb_type != 0) mb_type--;
      if(mb_type <= 4) {
        value = P_MB;
      } else {
        // printf("I_MB \n");
        value = I_MB;
      }
      if(currMB->skip_flag == 1) {
        value = S_MB;
      }
			break;

		case B_SLICE:
      // TODO
			// getMbTypeName_B_Slice(iCurr_mb_type, currMB, typestring, predmodstring, 0);
			break;

		case I_SLICE:
		case SI_SLICE: 
      value = I_MB;
			break;

    default:
      value = UNDEFINED_MB;
      break;
	}


  for (int i = 0; i < 16; i++) {
    for (int j = 0; j < 16; j++) {
      img_type[pos_y + i][pos_x + j] = value;
    }
  }
}





/**
 * \param currMB
 * \param img_mv the motion vectors, of size (H, W). Each pixel is asigned with a mv.
 */
void extract_motion_vector(Macroblock* currMB, uint8*** img_mv)
{
  const int pos_x = currMB->mb_x * MB_BLOCK_SIZE;
  const int pos_y = currMB->mb_y * MB_BLOCK_SIZE;
  
  int kk, step_h, step_v;

  if(currMB->mb_type == P8x8) {
    for(int j0 = 0; j0 < 4; j0 += 2) {	//vertical
      for(int i0 = 0; i0 < 4; i0 += 2) {	//horizontal
        kk = 2 * (j0 >> 1) + (i0 >> 1);
        step_h = BLOCK_STEP [currMB->b8mode[kk]][0];
        step_v = BLOCK_STEP [currMB->b8mode[kk]][1];
        for (int j = j0; j < j0 + 2; j += step_v) {
          for (int i = i0; i < i0 + 2; i += step_h) {
            //Loop through LIST0 and LIST1
            for (int list = LIST_0; list <= LIST_1; list++) {
              if ((currMB->b8pdir[kk] == list || currMB->b8pdir[kk] == BI_PRED) && (currMB->b8mode[kk] !=0 )) {
                // img_mv[pos_y + ]
                // currMB->p_Vid->dec_picture->motion.mv[list][currMB->block_y + j][currMB->block_x + i][0];

              }
            }
          }
        }
      }
    }
  }

  // for (int i = 0; i < 16; i++) {
  //   for (int j = 0; j < 16; j++) {
  //     img_type[pos_y + i][pos_x + j] = value;
  //   }
  // }
}



void init_inspector(Inspector** inspector, VideoParameters* p_Vid, int num_display)
{
  // printf("p_Vid->height = %d \n", p_Vid->height);
  // printf("p_Vid->width = %d \n", p_Vid->width);
  if (! *inspector) {
    *inspector = (Inspector*) calloc(1, sizeof(Inspector));
  }

  (*inspector)->height = p_Vid->height;
  (*inspector)->width = p_Vid->width;
  (*inspector)->channel = 3;
  (*inspector)->num_pic_stream = p_Vid->dec_picture->frame_id;
  (*inspector)->num_display = num_display + (*inspector)->poc_offset;

  // printf("p_Vid->dec_picture->frame_id = %d \n", p_Vid->dec_picture->frame_id);

  if (! (*inspector)->residual) {
    get_mem3Dfloat(&((*inspector)->residual), 3, p_Vid->height, p_Vid->width);
  }
  float* data = &((*inspector)->residual[0][0][0]);
  for (size_t i=0; i < 3 * p_Vid->height * p_Vid->width; i++) {
    data[i] = 0.0f;
  }

  if (! (*inspector)->coeffs) {
    get_mem3Dfloat(&((*inspector)->coeffs), 3, p_Vid->height, p_Vid->width);
  }
  data = &((*inspector)->coeffs[0][0][0]);
  for (size_t i=0; i < 3 * p_Vid->height * p_Vid->width; i++) {
    data[i] = 0.0f;
  }

  if (! (*inspector)->img_type) {
    get_mem2D(&((*inspector)->img_type), p_Vid->height, p_Vid->width);
  }
  uint8* data_uint8 = &((*inspector)->img_type[0][0]);
  for (size_t i=0; i < p_Vid->height * p_Vid->width; i++) {
    data_uint8[i] = 0;
  }

  if (! (*inspector)->img_mv) {
    get_mem3D(&((*inspector)->img_mv), p_Vid->height, p_Vid->width, 2);
  }
  data_uint8 = &((*inspector)->img_mv[0][0][0]);
  for (size_t i=0; i < p_Vid->height * p_Vid->width * 2; i++) {
    data_uint8[i] = 0;
  }
  

  (*inspector)->is_exported = 0;

}


void free_inspector(Inspector** inspector)
{
  if (*inspector) {
    free_mem3Dfloat((*inspector)->coeffs);
    free_mem3Dfloat((*inspector)->residual);
    free_mem2D((*inspector)->img_type);
    free_mem3D((*inspector)->img_mv);
    free(*inspector);
  }
}

void inspect_pic_type(Inspector* inspector, int type)
{
  inspector->pic_type = type;
}


int export_from_inspector(Inspector* inspector)
{
  printf("export_from_inspector(): \n");
  
  if (inspector && inspector->is_exported == 0) {
    printf("num_stream=%d, num_display=%d \n", inspector->num_pic_stream, inspector->num_display);
    float* data = &(inspector->residual[0][0][0]);
    

    char pic_type;
    switch (inspector->pic_type)
    {
    case B_SLICE:
      pic_type = 'B';
      break;
    case I_SLICE:
      pic_type = 'I';
      break;
    case P_SLICE:
      pic_type = 'P';
      break;
    case SI_SLICE:
      pic_type = 'I';
      break;
    case SP_SLICE:
      pic_type = 'P';
      break;
    default:
      break;
    }

    char fname[100];

    if(strcmp(g_save_dir, "\0") == 0) {
      strcpy(g_save_dir, ".");
    }
    sprintf(fname, "%s/imgY_s%04d_d%04d_%c.npy", g_save_dir, inspector->num_pic_stream, inspector->num_display, pic_type);
    iio_write_image_float(fname, &(inspector->residual[0][0][0]), inspector->width, inspector->height);
    
    sprintf(fname, "%s/imgU_s%04d_d%04d_%c.npy", g_save_dir, inspector->num_pic_stream, inspector->num_display, pic_type);
    iio_write_image_float(fname, &(inspector->residual[1][0][0]), inspector->width, inspector->height);
    
    sprintf(fname, "%s/imgV_s%04d_d%04d_%c.npy", g_save_dir, inspector->num_pic_stream, inspector->num_display, pic_type);
    iio_write_image_float(fname, &(inspector->residual[2][0][0]), inspector->width, inspector->height);

    sprintf(fname, "%s/imgMBtype_s%04d_d%04d_%c.png", g_save_dir, inspector->num_pic_stream, inspector->num_display, pic_type);
    iio_write_image_uint8_matrix(fname, inspector->img_type, inspector->width, inspector->height);
    printf("img_*.npy is created. \n");

    inspector->is_exported = 1;

    return 1;
  }

  return 0;
}


void inspect_poc_offset(Inspector* inspector, int offset)
{
  inspector->poc_offset = offset;
}


void save_mb_type(int mb_type)
{
  g_mb_type = mb_type;
}

void inspect_set_savedir(char* location)
{
  strcpy(g_save_dir, location);
}