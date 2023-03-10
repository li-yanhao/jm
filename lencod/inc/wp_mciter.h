/*!
 ***************************************************************************
 * \file
 *    wp_mciter.h
 *
 * \author
 *    Alexis Michael Tourapis
 *
 * \date
 *    22. February 2008
 *
 * \brief
 *    Headerfile for weighted prediction support
 **************************************************************************
 */

#ifndef _WP_MCITERM_H_
#define _WP_MCITERM_H_


extern void EstimateWPBSliceAlg2(Slice *currSlice);
extern void EstimateWPPSliceAlg2(Slice *currSlice, int offset);
extern int  TestWPPSliceAlg2    (VideoParameters *p_Vid, int offset);
extern int  TestWPBSliceAlg2    (VideoParameters *p_Vid, int method);

extern void compute_offset      (VideoParameters *p_Vid);

#endif

