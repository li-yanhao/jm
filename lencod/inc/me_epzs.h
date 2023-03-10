
/*!
************************************************************************
* \file
*     me_epzs.h
*
* \author
*    Alexis Michael Tourapis        <alexis.tourapis@dolby.com>
*
* \date
*    11. August 2006
*
* \brief
*    Headerfile for EPZS motion estimation
**************************************************************************
*/


#ifndef _ME_EPZS_H_
#define _ME_EPZS_H_
#include "me_epzs_common.h"

static const MotionVector search_point_hp[10] = {{0,0},{-2,0}, {0,2}, {2,0},  {0,-2}, {-2,2},  {2,2},  {2,-2}, {-2,-2}, {-2,2}};
static const MotionVector search_point_qp[10] = {{0,0},{-1,0}, {0,1}, {1,0},  {0,-1}, {-1,1},  {1,1},  {1,-1}, {-1,-1}, {-1,1}};

// Functions
extern distblk EPZSPelBlockMotionSearch      (Macroblock *, MotionVector *, MEBlock *, distblk, int);
extern distblk EPZSPelBlockMotionSearchSubMB (Macroblock *, MotionVector *, MEBlock *, distblk, int);
extern distblk EPZSSubPelBlockMotionSearch   (Macroblock *, MotionVector *, MEBlock *mv_block, distblk, int*);
extern distblk EPZSSubPelBlockSearchBiPred   (Macroblock *,  MEBlock *mv_block, int list, 
                                         MotionVector *pred_mv1, MotionVector *pred_mv2, MotionVector *mv1, MotionVector *mv2, distblk min_mcost, int *lambda_factor);
extern distblk EPZSBiPredBlockMotionSearch   (Macroblock *, int, MotionVector *, MotionVector *, MotionVector *, MotionVector *, MEBlock *,  int, distblk, int);

#endif

