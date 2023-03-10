
/*!
 ************************************************************************
 *  \file
 *     global.h
 *  \brief
 *     global definitions for H.264 decoder.
 *  \author
 *     Copyright (C) 1999  Telenor Satellite Services,Norway
 *                         Ericsson Radio Systems, Sweden
 *
 *     Inge Lille-Langoy               <inge.lille-langoy@telenor.com>
 *
 *     Telenor Satellite Services
 *     Keysers gt.13                       tel.:   +47 23 13 86 98
 *     N-0130 Oslo,Norway                  fax.:   +47 22 77 79 80
 *
 *     Rickard Sjoberg                 <rickard.sjoberg@era.ericsson.se>
 *
 *     Ericsson Radio Systems
 *     KI/ERA/T/VV
 *     164 80 Stockholm, Sweden
 *
 ************************************************************************
 */
#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <sys/timeb.h>

#include "win32.h"
#include "defines.h"
#include "ifunctions.h"
#include "parsetcommon.h"
#include "types.h"
#include "io_image.h"
#include "frame.h"
#include "distortion.h"
#include "io_video.h"


int **PicPos;

#define ET_SIZE 300      //!< size of error text buffer
char errortext[ET_SIZE]; //!< buffer for error message for exit with error()

/***********************************************************************
 * T y p e    d e f i n i t i o n s    f o r    J M
 ***********************************************************************
 */

typedef enum
{
  LumaComp = 0,
  CrComp = 1,
  CbComp = 2
} Color_Component;

/***********************************************************************
 * D a t a    t y p e s   f o r  C A B A C
 ***********************************************************************
 */

typedef struct pix_pos
{
  int   available;
  int   mb_addr;
  short x;
  short y;
  short pos_x;
  short pos_y;
} PixelPos;

//! struct to characterize the state of the arithmetic coding engine
typedef struct
{
  unsigned int    Drange;
  unsigned int    Dvalue;
  int             DbitsLeft;
  byte            *Dcodestrm;
  int             *Dcodestrm_len;
} DecodingEnvironment;

typedef DecodingEnvironment *DecodingEnvironmentPtr;

//! definition of motion parameters
typedef struct motion_params
{
  int64 ***   ref_pic_id;    //!< reference picture identifier [list][subblock_y][subblock_x]
  short ****  mv;            //!< motion vector       [list][subblock_x][subblock_y][component]
  char  ***   ref_idx;       //!< reference picture   [list][subblock_y][subblock_x]
  byte **     moving_block;
} MotionParams;

//! struct for context management
typedef struct
{
  uint16 state;         // index into state-table CP
  unsigned char  MPS;           // Least Probable Symbol 0/1 CP
  unsigned char dummy;          // for alignment
} BiContextType;

typedef BiContextType *BiContextTypePtr;


/**********************************************************************
 * C O N T E X T S   F O R   T M L   S Y N T A X   E L E M E N T S
 **********************************************************************
 */

#define NUM_MB_TYPE_CTX  11
#define NUM_B8_TYPE_CTX  9
#define NUM_MV_RES_CTX   10
#define NUM_REF_NO_CTX   6
#define NUM_DELTA_QP_CTX 4
#define NUM_MB_AFF_CTX 4
#define NUM_TRANSFORM_SIZE_CTX 3

// structures that will be declared somewhere else
struct storable_picture;
struct datapartition;
struct syntaxelement;

typedef struct
{
  BiContextType mb_type_contexts [3][NUM_MB_TYPE_CTX];
  BiContextType b8_type_contexts [2][NUM_B8_TYPE_CTX];
  BiContextType mv_res_contexts  [2][NUM_MV_RES_CTX];
  BiContextType ref_no_contexts  [2][NUM_REF_NO_CTX];
  BiContextType delta_qp_contexts[NUM_DELTA_QP_CTX];
  BiContextType mb_aff_contexts  [NUM_MB_AFF_CTX];
} MotionInfoContexts;

#define NUM_IPR_CTX    2
#define NUM_CIPR_CTX   4
#define NUM_CBP_CTX    4
#define NUM_BCBP_CTX   4
#define NUM_MAP_CTX   15
#define NUM_LAST_CTX  15
#define NUM_ONE_CTX    5
#define NUM_ABS_CTX    5


typedef struct
{
  BiContextType  transform_size_contexts [NUM_TRANSFORM_SIZE_CTX];
  BiContextType  ipr_contexts [NUM_IPR_CTX];
  BiContextType  cipr_contexts[NUM_CIPR_CTX];
  BiContextType  cbp_contexts [3][NUM_CBP_CTX];
  BiContextType  bcbp_contexts[NUM_BLOCK_TYPES][NUM_BCBP_CTX];
  BiContextType  map_contexts [2][NUM_BLOCK_TYPES][NUM_MAP_CTX];
  BiContextType  last_contexts[2][NUM_BLOCK_TYPES][NUM_LAST_CTX];
  BiContextType  one_contexts [NUM_BLOCK_TYPES][NUM_ONE_CTX];
  BiContextType  abs_contexts [NUM_BLOCK_TYPES][NUM_ABS_CTX];
} TextureInfoContexts;


//*********************** end of data type definition for CABAC *******************

/***********************************************************************
 * N e w   D a t a    t y p e s   f o r    T M L
 ***********************************************************************
 */

//! Buffer structure for decoded reference picture marking commands
typedef struct DecRefPicMarking_s
{
  int memory_management_control_operation;
  int difference_of_pic_nums_minus1;
  int long_term_pic_num;
  int long_term_frame_idx;
  int max_long_term_frame_idx_plus1;
  struct DecRefPicMarking_s *Next;
} DecRefPicMarking_t;


//! definition of pic motion parameters
typedef struct pic_motion_params2
{
  int64    ref_pic_id;    //!< reference picture identifier [list][subblock_y][subblock_x]
  int64    ref_id;        //!< reference picture identifier [list][subblock_y][subblock_x]
  short    mv[2];         //!< motion vector       [list][subblock_x][subblock_y][component]
  char     ref_idx;       //!< reference picture   [list][subblock_y][subblock_x]
  byte     mb_field;      //!< field macroblock indicator
  byte     field_frame;   //!< indicates if co_located is field or frame.
} PicMotionParams2;

//! Motion Vector structure
typedef struct
{
  short mv_x;
  short mv_y;
} MotionVector;

//! Macroblock
typedef struct macroblock
{
  struct slice       *p_Slice;                    //!< pointer to the current slice
  struct video_par   *p_Vid;                      //!< pointer to VideoParameters
  struct inp_par     *p_Inp;
  int                 mbAddrX;                    //!< current MB address
  int mb_x;
  int mb_y;
  int block_x;
  int block_y;
  int block_y_aff;
  int pix_x;
  int pix_y;
  int pix_c_x;
  int pix_c_y;

  int subblock_x;
  int subblock_y;

  int           qp;                    //!< QP luma
  int           qpc[2];                //!< QP chroma
  int           qp_scaled[MAX_PLANE];  //!< QP scaled for all comps.
  Boolean       is_lossless;
  Boolean       is_intra_block;
  Boolean       is_v_block;

  short         slice_nr;
  short         delta_quant;          //!< for rate control

  struct macroblock   *mb_up;   //!< pointer to neighboring MB (CABAC)
  struct macroblock   *mb_left; //!< pointer to neighboring MB (CABAC)

  // some storage of macroblock syntax elements for global access
  int           mb_type;
  short         mvd[2][BLOCK_MULTIPLE][BLOCK_MULTIPLE][2];      //!< indices correspond to [forw,backw][block_y][block_x][x,y]
  int           cbp;
  int64         cbp_blk     [3];
  int64         cbp_bits    [3];
  int64         cbp_bits_8x8[3];

  int           i16mode;
  char          b8mode[4];
  char          b8pdir[4];
  /***** XML_TRACE_BEGIN *****/
  int			b8submbtype[4];
  /****** XML_TRACE_END ******/
  char          ei_flag;             //!< error indicator flag that enables concealment
  char          dpl_flag;            //!< error indicator flag that signals a missing data partition
  char          ipmode_DPCM;

  short         DFDisableIdc;
  short         DFAlphaC0Offset;
  short         DFBetaOffset;

  char          c_ipred_mode;       //!< chroma intra prediction mode
  Boolean       mb_field;

  char          skip_flag;

  int mbAddrA, mbAddrB, mbAddrC, mbAddrD;
  Boolean mbAvailA, mbAvailB, mbAvailC, mbAvailD;

  Boolean       luma_transform_size_8x8_flag;
  Boolean       NoMbPartLessThan8x8Flag;

  void (*itrans_4x4)(struct macroblock *currMB, ColorPlane pl, int ioff, int joff);
  void (*itrans_8x8)(struct macroblock *currMB, ColorPlane pl, int ioff, int joff);

  void (*GetMVPredictor) (struct macroblock *currMB, PixelPos *block, 
    short pmv[2], short ref_frame, char **refPic, short ***tmp_mv, int mb_x, int mb_y, int blockshape_x, int blockshape_y);

  int (*read_and_store_CBP_block_bit) (struct macroblock *currMB, DecodingEnvironmentPtr  dep_dp, int type);
  char (*readRefPictureIdx)   (struct macroblock *currMB, struct syntaxelement *currSE, struct datapartition *dP, char b8mode, int list);

} Macroblock;

//! Syntaxelement
typedef struct syntaxelement
{
  int           type;                  //!< type of syntax element for data part.
  int           value1;                //!< numerical value of syntax element
  int           value2;                //!< for blocked symbols, e.g. run/level
  int           len;                   //!< length of code
  int           inf;                   //!< info part of CAVLC code
  unsigned int  bitpattern;            //!< CAVLC bitpattern
  int           context;               //!< CABAC context
  int           k;                     //!< CABAC context for coeff_count,uv

#if TRACE
  #define       TRACESTRING_SIZE 100           //!< size of trace string
  char          tracestring[TRACESTRING_SIZE]; //!< trace string
#endif

  //! for mapping of CAVLC to syntaxElement
  void  (*mapping)(int len, int info, int *value1, int *value2);
  //! used for CABAC: refers to actual coding method of each individual syntax element type
  void  (*reading)(Macroblock *currMB, struct syntaxelement *, DecodingEnvironmentPtr);
} SyntaxElement;


//! Bitstream
typedef struct bit_stream
{
  // CABAC Decoding
  int           read_len;           //!< actual position in the codebuffer, CABAC only
  int           code_len;           //!< overall codebuffer length, CABAC only
  // CAVLC Decoding
  int           frame_bitoffset;    //!< actual position in the codebuffer, bit-oriented, CAVLC only
  int           bitstream_length;   //!< over codebuffer lnegth, byte oriented, CAVLC only
  // ErrorConcealment
  byte          *streamBuffer;      //!< actual codebuffer for read bytes
  int           ei_flag;            //!< error indication, 0: no error, else unspecified error
} Bitstream;

//! DataPartition
typedef struct datapartition
{

  Bitstream           *bitstream;
  DecodingEnvironment de_cabac;

  int     (*readSyntaxElement)(Macroblock *currMB, SyntaxElement *, struct datapartition *);
          /*!< virtual function;
               actual method depends on chosen data partition and
               entropy coding method  */
} DataPartition;

//! Slice
typedef struct slice
{
  struct video_par    *p_Vid;
  struct inp_par      *p_Inp;
  pic_parameter_set_rbsp_t *active_pps;
  seq_parameter_set_rbsp_t *active_sps;

  struct colocated_params *p_colocated;
  struct colocated_params *Co_located_JV[MAX_PLANE];  //!< p_colocated to be used during 4:4:4 independent mode decoding

  int                 mb_aff_frame_flag;
  int                 direct_spatial_mv_pred_flag;       //!< Indicator for direct mode type (1 for Spatial, 0 for Temporal)
  int                 num_ref_idx_l0_active;             //!< number of available list 0 references
  int                 num_ref_idx_l1_active;             //!< number of available list 1 references

  int                 ei_flag;       //!< 0 if the partArr[0] contains valid information
  int                 qp;
  int                 slice_qp_delta;
  int                 qs;
  int                 slice_qs_delta;
  int                 slice_type;    //!< slice type
  int                 model_number;  //!< cabac model number
  PictureStructure    structure;     //!< Identify picture structure type
  int                 start_mb_nr;   //!< MUST be set by NAL even in case of ei_flag == 1
  int                 max_part_nr;
  int                 dp_mode;       //!< data partitioning mode
  int                 next_header;
  int                 last_dquant;

  //  int                 last_mb_nr;    //!< only valid when entropy coding == CABAC
  DataPartition       *partArr;      //!< array of partitions
  MotionInfoContexts  *mot_ctx;      //!< pointer to struct of context models for use in CABAC
  TextureInfoContexts *tex_ctx;      //!< pointer to struct of context models for use in CABAC

  int mvscale[6][MAX_REFERENCE_PICTURES];

  int                 ref_pic_list_reordering_flag_l0;
  int                 *reordering_of_pic_nums_idc_l0;
  int                 *abs_diff_pic_num_minus1_l0;
  int                 *long_term_pic_idx_l0;
  int                 ref_pic_list_reordering_flag_l1;
  int                 *reordering_of_pic_nums_idc_l1;
  int                 *abs_diff_pic_num_minus1_l1;
  int                 *long_term_pic_idx_l1;
  

  short               DFDisableIdc;     //!< Disable deblocking filter on slice
  short               DFAlphaC0Offset;  //!< Alpha and C0 offset for filtering slice
  short               DFBetaOffset;     //!< Beta offset for filtering slice

  int                 pic_parameter_set_id;   //!<the ID of the picture parameter set the slice is reffering to

  int                 dpB_NotPresent;    //!< non-zero, if data partition B is lost
  int                 dpC_NotPresent;    //!< non-zero, if data partition C is lost

  imgpel ***mb_pred;
  imgpel ***mb_rec;
  int    ***mb_rres;
  int    ***cof;
  int    ***fcf;

  int cofu[16];

  imgpel **tmp_block_l0;
  imgpel **tmp_block_l1;  
  int    **tmp_res;

  // Scaling matrix info
  int  InvLevelScale4x4_Intra[3][6][4][4];
  int  InvLevelScale4x4_Inter[3][6][4][4];
  int  InvLevelScale8x8_Intra[3][6][8][8];
  int  InvLevelScale8x8_Inter[3][6][8][8];

  int  *qmatrix[12];

  // Cabac
  int  coeff[64]; // one more for EOB
  int  coeff_ctr;
  int  pos;  


  //weighted prediction
  unsigned int apply_weights;
  unsigned int luma_log2_weight_denom;
  unsigned int chroma_log2_weight_denom;
  int ***wp_weight;  // weight in [list][index][component] order
  int ***wp_offset;  // offset in [list][index][component] order
  int ****wbp_weight; //weight in [list][fw_index][bw_index][component] order
  int wp_round_luma;
  int wp_round_chroma;


  void (*read_CBP_and_coeffs_from_NAL) (Macroblock *currMB);
  int  (*decode_one_component     ) (Macroblock *currMB, ColorPlane curr_plane, imgpel **currImg, struct storable_picture *dec_picture);
  int  (*readSlice                ) (struct video_par *, struct inp_par *);  
  int  (*nal_startcode_follows    ) (struct slice*, int );
  void (*read_motion_info_from_NAL) (Macroblock *currMB);
  void (*read_one_macroblock      ) (Macroblock *currMB);
  void (*interpret_mb_mode        ) (Macroblock *currMB);
  void (*compute_colocated        ) (struct slice *currSlice, struct colocated_params *p, struct storable_picture **listX[6]);

  void (*linfo_cbp_intra) (int len,int info,int *cbp, int *dummy);
  void (*linfo_cbp_inter) (int len,int info,int *cbp, int *dummy);
} Slice;

//****************************** ~DM ***********************************

// video parameters
typedef struct video_par
{
  struct inp_par      *p_Inp;
  pic_parameter_set_rbsp_t *active_pps;
  seq_parameter_set_rbsp_t *active_sps;
  seq_parameter_set_rbsp_t SeqParSet[MAXSPS];
  pic_parameter_set_rbsp_t PicParSet[MAXPPS];

  struct sei_params        *p_SEI;

  struct old_slice_par *old_slice;
  struct snr_par       *snr;
  int number;                                 //!< frame number
  unsigned int current_mb_nr; // bitstream order
  unsigned int num_dec_mb;
  short        current_slice_nr;
  int *intra_block;
  
  int qp;                                     //!< quant for the current frame

  int sp_switch;                              //!< 1 for switching sp, 0 for normal sp  
  int type;                                   //!< image type INTER/INTRA
  int width;
  int height;
  int width_cr;                               //!< width chroma  
  int height_cr;                              //!< height chroma
  int mb_x;
  int mb_y;
  int block_x;
  int block_y;
  int pix_c_x;
  int pix_c_y;

  int allrefzero;

  byte **ipredmode;                  //!< prediction type [90][74]
  byte ****nz_coeff;
  int **siblock;
  int cod_counter;                   //!< Current count of number of skipped macroblocks in a row

  int newframe;
  int structure;                     //!< Identify picture structure type

  Slice      *currentSlice;          //!< pointer to current Slice data struct
  Macroblock *mb_data;               //!< array containing all MBs of a whole frame
  Macroblock *mb_data_JV[MAX_PLANE]; //!< mb_data to be used for 4:4:4 independent mode
  int colour_plane_id;               //!< colour_plane_id of the current coded slice
  int ChromaArrayType;

  // For MB level frame/field coding
  int mb_aff_frame_flag;

  // for signalling to the neighbour logic that this is a deblocker call
  int DeblockCall;
  byte mixedModeEdgeFlag;

  // picture error concealment
  // concealment_head points to first node in list, concealment_end points to
  // last node in list. Initialize both to NULL, meaning no nodes in list yet
  struct concealment_node *concealment_head;
  struct concealment_node *concealment_end;

  DecRefPicMarking_t *dec_ref_pic_marking_buffer;                    //!< stores the memory management control operations

  int num_ref_idx_l0_active;             //!< number of forward reference
  int num_ref_idx_l1_active;             //!< number of backward reference

  int slice_group_change_cycle;

  int redundant_pic_cnt;

  unsigned int pre_frame_num;           //!< store the frame_num in the last decoded slice. For detecting gap in frame_num.
  int non_conforming_stream;

  // End JVT-D101
  // POC200301: from unsigned int to int
  int toppoc;      //poc for this top field // POC200301
  int bottompoc;   //poc of bottom field of frame
  int framepoc;    //poc of this frame // POC200301
  unsigned int frame_num;   //frame_num for this frame
  unsigned int field_pic_flag;
  byte         bottom_field_flag;

  //the following is for slice header syntax elements of poc
  // for poc mode 0.
  unsigned int pic_order_cnt_lsb;
  int delta_pic_order_cnt_bottom;
  // for poc mode 1.
  int delta_pic_order_cnt[3];

  // ////////////////////////
  // for POC mode 0:
  signed   int PrevPicOrderCntMsb;
  unsigned int PrevPicOrderCntLsb;
  signed   int PicOrderCntMsb;

  // for POC mode 1:
  unsigned int AbsFrameNum;
  signed int ExpectedPicOrderCnt, PicOrderCntCycleCnt, FrameNumInPicOrderCntCycle;
  unsigned int PreviousFrameNum, FrameNumOffset;
  int ExpectedDeltaPerPicOrderCntCycle;
  int PreviousPOC, ThisPOC;
  int PreviousFrameNumOffset;
  // /////////////////////////

  int idr_flag;
  int nal_reference_idc;                       //!< nal_reference_idc from NAL unit

  int idr_pic_id;

  int MaxFrameNum;

  unsigned int PicWidthInMbs;
  unsigned int PicHeightInMapUnits;
  unsigned int FrameHeightInMbs;
  unsigned int PicHeightInMbs;
  unsigned int PicSizeInMbs;
  unsigned int FrameSizeInMbs;
  unsigned int oldFrameSizeInMbs;

  int no_output_of_prior_pics_flag;
  int long_term_reference_flag;
  int adaptive_ref_pic_buffering_flag;

  int last_has_mmco_5;
  int last_pic_bottom_field;

  // Fidelity Range Extensions Stuff
  int pic_unit_bitsize_on_disk;
  short bitdepth_luma;
  short bitdepth_chroma;
  int bitdepth_scale[2];
  int bitdepth_luma_qp_scale;
  int bitdepth_chroma_qp_scale;
  unsigned int dc_pred_value_comp[MAX_PLANE]; //!< component value for DC prediction (depends on component pel bit depth)
  int max_pel_value_comp[MAX_PLANE];       //!< max value that one picture element (pixel) can take (depends on pic_unit_bitdepth)
  int Transform8x8Mode;
  int profile_idc;
  int yuv_format;
  int lossless_qpprime_flag;
  int num_blk8x8_uv;
  int num_uv_blocks;
  int num_cdc_coeff;
  int mb_cr_size_x;
  int mb_cr_size_y;
  int mb_cr_size_x_blk;
  int mb_cr_size_y_blk;
  int mb_size[3][2];                         //!< component macroblock dimensions
  int mb_size_blk[3][2];                     //!< component macroblock dimensions 
  int mb_size_shift[3][2];
  int subpel_x;
  int subpel_y;
  int shiftpel_x;
  int shiftpel_y;
  int total_scale;


  int max_vmv_r;                             //!< maximum vertical motion vector range in luma quarter frame pixel units for the current level_idc
  int max_mb_vmv_r;                          //!< maximum vertical motion vector range in luma quarter pixel units for the current level_idc

  int idr_psnr_number;
  int psnr_number;

  // Timing related variables
  TIME_T start_time;
  TIME_T end_time;

  // picture error concealment
  int last_ref_pic_poc;
  int ref_poc_gap;
  int poc_gap;
  int conceal_mode;
  int earlier_missing_poc;
  unsigned int frame_to_conceal;
  int IDR_concealment_flag;
  int conceal_slice_type;

  // random access point decoding
  int recovery_point;
  int recovery_point_found;
  int recovery_frame_cnt;
  int recovery_frame_num;
  int recovery_poc;

  int separate_colour_plane_flag;
  int pic_unit_size_on_disk;

  byte *buf;
  byte *ibuf;

  ImageData imgData;           //!< Image data to be encoded (dummy variable for now)
  ImageData imgData0;          //!< base layer input
  ImageData imgData1;          //!< temp buffer for left de-muxed view
  ImageData imgData2;          //!< temp buffer for right de-muxed view

  int frame_number;
  int init_bl_done;

  // Redundant slices. Should be moved to another structure and allocated only if extended profile
  unsigned int previous_frame_num; //!< frame number of previous slice
  int ref_flag[17];                //!< 0: i-th previous frame is incorrect
  //!< non-zero: i-th previous frame is correct
  int Is_primary_correct;          //!< if primary frame is correct, 0: incorrect
  int Is_redundant_correct;        //!< if redundant frame is correct, 0:incorrect
  int redundant_slice_ref_idx;     //!< reference index of redundant slice

  // Time 
  int64 tot_time;

  // files
  int p_out;                       //!< file descriptor to output YUV file
  int p_ref;                       //!< pointer to input original reference YUV file file

  //FILE *p_log;                     //!< SNR file
  int LastAccessUnitExists;
  int NALUCount;

  // B pictures
  int  Bframe_ctr;
  int  frame_no;

  int  g_nFrame;
  Boolean global_init_done;

  // global picture format dependent buffers, memory allocation in decod.c
  imgpel **imgY_ref;                              //!< reference frame find snr
  imgpel ***imgUV_ref;

  int *qp_per_matrix;
  int *qp_rem_matrix;

  struct frame_store *last_out_fs;
  int pocs_in_dpb[100];


  struct storable_picture *dec_picture;
  struct storable_picture *dec_picture_JV[MAX_PLANE];  //!< dec_picture to be used during 4:4:4 independent mode decoding
  struct storable_picture *no_reference_picture; //!< dummy storable picture for recovery point
  struct storable_picture **listX[6];

  // Error parameters
  struct object_buffer  *erc_object_list;
  struct ercVariables_s *erc_errorVar;

  int erc_mvperMB;
  struct video_par *erc_img;
  int ec_flag[SE_MAX_ELEMENTS];        //!< array to set errorconcealment

  struct annex_b_struct *annex_b;
  struct sBitsFile *bitsfile;


  struct frame_store *out_buffer;

  struct storable_picture *pending_output;
  int    pending_output_state;
  int    recovery_flag;

  int BitStreamFile;
  // dpb
  struct decoded_picture_buffer *p_Dpb;

  char listXsize[6];
  // report
  char cslice_type[9];  
  // FMO
  int *MbToSliceGroupMap;
  int *MapUnitToSliceGroupMap;
  int  NumberOfSliceGroups;    // the number of slice groups -1 (0 == scan order, 7 == maximum)

#if (ENABLE_OUTPUT_TONEMAPPING)
  struct tone_mapping_struct_s *seiToneMapping;
#endif


  void (*buf2img)          (imgpel** imgX, unsigned char* buf, int size_x, int size_y, int o_size_x, int o_size_y, int symbol_size_in_bytes, int bitshift);
  void (*getNeighbour)     (Macroblock *currMB, int xN, int yN, int mb_size[2], PixelPos *pix);
  void (*get_mb_block_pos) (int mb_addr, short *x, short *y);
  void (*GetStrength)      (byte Strength[16], Macroblock *MbQ, int dir,int edge, int mvlimit, struct storable_picture *p);
  void (*EdgeLoopLuma)     (ColorPlane pl, imgpel** Img, byte Strength[16], Macroblock *MbQ, int dir, int edge, struct storable_picture *p);
  void (*EdgeLoopChroma)   (imgpel** Img, byte Strength[16], Macroblock *MbQ, int dir, int edge, int uv, struct storable_picture *p);
  void (*img2buf)          (imgpel** imgX, unsigned char* buf, int size_x, int size_y, int symbol_size_in_bytes, int crop_left, int crop_right, int crop_top, int crop_bottom);
} VideoParameters;

// signal to noise ratio parameters
typedef struct snr_par
{
  int   frame_ctr;
  float snr[3];                                //!< current SNR (component)
  float snr1[3];                               //!< SNR (dB) first frame (component)
  float snra[3];                               //!< Average component SNR (dB) remaining frames
  float sse[3];                                //!< component SSE 
  float msse[3];                                //!< Average component SSE 
} SNRParameters;

// input parameters from configuration file
typedef struct inp_par
{
  char infile[FILE_NAME_SIZE];                       //!< H.264 inputfile
  char outfile[FILE_NAME_SIZE];                      //!< Decoded YUV 4:2:0 output
  char reffile[FILE_NAME_SIZE];                      //!< Optional YUV 4:2:0 reference file for SNR measurement

  int FileFormat;                         //!< File format of the Input file, PAR_OF_ANNEXB or PAR_OF_RTP
  int ref_offset;
  int poc_scale;
  int write_uv;
  int silent;
  int intra_profile_deblocking;               //!< Loop filter usage determined by flags and parameters in bitstream 

  // Input/output sequence format related variables
  FrameFormat source;                   //!< source related information
  FrameFormat output;                   //!< output related information

  int  ProcessInput;
  VideoDataFile   input_file1;          //!< Input video file1
  VideoDataFile   input_file2;          //!< Input video file2
  VideoDataFile   input_file3;          //!< Input video file3


#ifdef _LEAKYBUCKET_
  unsigned long R_decoder;                //!< Decoder Rate in HRD Model
  unsigned long B_decoder;                //!< Decoder Buffer size in HRD model
  unsigned long F_decoder;                //!< Decoder Initial buffer fullness in HRD model
  char LeakyBucketParamFile[FILE_NAME_SIZE];         //!< LeakyBucketParamFile
#endif

  // picture error concealment
  int conceal_mode;
  int ref_poc_gap;
  int poc_gap;

  // dummy for encoder
  int start_frame;
  int bEncoderLog;
  char encoderLogName[FILE_NAME_SIZE];
  FILE *encoderLog;
} InputParameters;

typedef struct old_slice_par
{
   unsigned field_pic_flag;   
   unsigned frame_num;
   int      nal_ref_idc;
   unsigned pic_oder_cnt_lsb;
   int      delta_pic_oder_cnt_bottom;
   int      delta_pic_order_cnt[2];
   byte     bottom_field_flag;
   byte     idr_flag;
   int      idr_pic_id;
   int      pps_id;   
} OldSliceParams;

typedef struct decoder_params
{
  InputParameters   *p_Inp;          //!< Input Parameters
  VideoParameters   *p_Vid;          //!< Image Parameters
  int64              bufferSize;     //!< buffersize for tiff reads (not currently supported)
  int                UsedBits;      // for internal statistics, is adjusted by se_v, ue_v, u_1
  FILE              *p_trace;        //!< Trace file
  int                bitcounter;
} DecoderParams;

extern DecoderParams  *p_Dec;

// prototypes

extern void error(char *text, int code);

// dynamic mem allocation
extern int  init_global_buffers(VideoParameters *p_Vid);
extern void free_global_buffers(VideoParameters *p_Vid);

extern int RBSPtoSODB(byte *streamBuffer, int last_byte_pos);
extern int EBSPtoRBSP(byte *streamBuffer, int end_bytepos, int begin_bytepos);

void FreePartition (DataPartition *dp, int n);
DataPartition *AllocPartition(int n);

void tracebits(const char *trace_str,  int len,  int info,int value1);
void tracebits2(const char *trace_str, int len, int info);

unsigned CeilLog2   ( unsigned uiVal);
unsigned CeilLog2_sf( unsigned uiVal);

// For 4:4:4 independent mode
extern void change_plane_JV( VideoParameters *p_Vid, int nplane );
extern void make_frame_picture_JV(VideoParameters *p_Vid);


#endif
