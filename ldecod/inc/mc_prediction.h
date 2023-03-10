
/*!
 *************************************************************************************
 * \file mc_prediction.h
 *
 * \brief
 *    definitions for motion compensated prediction
 *
 * \author
 *      Main contributors (see contributors.h for copyright, 
 *                         address and affiliation details)
 *      - Alexis Michael Tourapis  <alexismt@ieee.org>
 *
 *************************************************************************************
 */

#ifndef _MC_PREDICTION_H_
#define _MC_PREDICTION_H_

#include "global.h"
#include "mbuffer.h"

extern int  allocate_pred_mem(Slice *currSlice);
extern void free_pred_mem    (Slice *currSlice);

extern void get_block_luma  (Macroblock *currMB, ColorPlane pl, StorablePicture *list, int x_pos, int y_pos, int ver_block_size, int hor_block_size, imgpel **block);
extern void get_block_chroma(Macroblock *currMB, int uv, StorablePicture *list, int x_pos, int y_pos, int hor_block_size, int ver_block_size, imgpel **block);

extern void intra_cr_decoding    (Macroblock *currMB, int yuv);
extern void prepare_direct_params(Macroblock *currMB, StorablePicture *dec_picture, short pmvl0[2], short pmvl1[2],char *l0_rFrame, char *l1_rFrame);
extern void perform_mc           (Macroblock *currMB, ColorPlane pl, StorablePicture *dec_picture, int pred_dir, int i, int j, int list_offset,   int block_size_x, int block_size_y, int curr_mb_field);
#endif

