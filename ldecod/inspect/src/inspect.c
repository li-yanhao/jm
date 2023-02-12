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
    //  xml_write_start_element("Plane");
    //  xml_write_int_attribute("type", 0);
    for (i = 0; i < 16; i++) {
      // xml_write_start_element("Row");
      for (j = 0; j < 16; j++) {
        // if(j > 0) xml_write_text(",");
        out_coeffs[0][pos_y + i][pos_x + j] = (&currSlice->mb_rres[0][0])[i][j];
        // printf("8x8: out_coeffs[0][pos_y + i][pos_x + j] = (&currSlice->mb_rres[0][0])[i][j]; \n");
        // xml_write_int((&currSlice->mb_rres[0][0])[i][j]);
      }
      // xml_write_end_element();
    }
    // xml_write_end_element();
    // Chroma
    for (pl = 1; pl <= 2; pl++) {
      // xml_write_start_element("Plane");
      // xml_write_int_attribute("type", pl);
      for (i = 0; i < 8; i++) {
        // xml_write_start_element("Row");
        for (j = 0; j < 8; j++) {
          // if(j > 0) xml_write_text(",");
          // xml_write_int(currSlice->cof[pl][i][j]);
          out_coeffs[pl][pos_y + i][pos_x + j] = currSlice->cof[pl][i][j];
        }
        // xml_write_end_element();
      }
      // xml_write_end_element();
    }
  } else {
    // Luma
    //  xml_write_start_element("Plane");
    //  xml_write_int_attribute("type", 0);
    for (i = 0; i < 16; i++) {
      // xml_write_start_element("Row");
      for (j = 0; j < 16; j++) {
        // if(j > 0) xml_write_text(",");
        // xml_write_int(currSlice->cof[0][i][j]);
        // printf("4x4: out_coeffs[0][pos_y + i][pos_x + j] = currSlice->cof[0][i][j]; \n");
        // printf("i=%d, j=%d \n", i, j);
        out_coeffs[0][pos_y + i][pos_x + j] = currSlice->cof[0][i][j];
      }
      // xml_write_end_element();
    }
    // xml_write_end_element();
    // Chroma
    for (pl = 1; pl <= 2; pl++) {
      // xml_write_start_element("Plane");
      // xml_write_int_attribute("type", pl);
      for (i = 0; i < 8; i++) {
        // xml_write_start_element("Row");
        for (j = 0; j < 8; j++) {
          // if(j > 0) xml_write_text(",");
          // xml_write_int(currSlice->cof[pl][i][j]);
          out_coeffs[pl][pos_y + i][pos_x + j] = currSlice->cof[pl][i][j];
        }
        // xml_write_end_element();
      }
      // xml_write_end_element();
    }
  }
  // xml_write_end_element();
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

  (*inspector)->is_exported = 0;

}


void free_inspector(Inspector** inspector)
{
  if (*inspector) {
    free_mem3Dfloat((*inspector)->residual);
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

    sprintf(fname, "imgY_s%03d_d%03d_%c.npy", inspector->num_pic_stream, inspector->num_display, pic_type);
    iio_write_image_float(fname, &(inspector->residual[0][0][0]), inspector->width, inspector->height);
    
    sprintf(fname, "imgU_s%03d_d%03d_%c.npy", inspector->num_pic_stream, inspector->num_display, pic_type);
    iio_write_image_float(fname, &(inspector->residual[1][0][0]), inspector->width, inspector->height);
    
    sprintf(fname, "imgV_s%03d_d%03d_%c.npy", inspector->num_pic_stream, inspector->num_display, pic_type);
    iio_write_image_float(fname, &(inspector->residual[2][0][0]), inspector->width, inspector->height);
    
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