
/*!
 ************************************************************************
 * \file image.h
 *
 * \brief
 *    prototypes for image.c
 *
 ************************************************************************
 */

#ifndef _IMAGE_H_
#define _IMAGE_H_

#include "mbuffer.h"

/****** INSPECT_BEGIN ******/
#include "inspect.h"
/****** INSPECT_END ******/

extern void calculate_frame_no(VideoParameters *p_Vid, StorablePicture *p);
extern void find_snr          (VideoParameters *p_Vid, StorablePicture *p, int *p_ref);
extern int  picture_order     (VideoParameters *p_Vid);


/****** INSPECT_BEGIN ******/
// extern void decode_one_slice  (Slice *currSlice);
extern void decode_one_slice(Slice *currSlice, Inspector* inspector);
/****** INSPECT_END ******/

extern int  read_new_slice    (Slice *currSlice);
extern void exit_picture      (VideoParameters *p_Vid, StorablePicture **dec_picture);
extern int  decode_one_frame  (VideoParameters *p_Vid);

extern int  is_new_picture(StorablePicture *dec_picture, Slice *currSlice, OldSliceParams *p_old_slice);
extern void init_old_slice(OldSliceParams *p_old_slice);
// For 4:4:4 independent mode
extern void copy_dec_picture_JV (VideoParameters *p_Vid, StorablePicture *dst, StorablePicture *src );

extern void frame_postprocessing(VideoParameters *p_Vid);
extern void field_postprocessing(VideoParameters *p_Vid);

#endif

