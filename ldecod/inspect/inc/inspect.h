#ifndef _INSPECT_H_
#define _INSPECT_H_


#include "macroblock.h"


// NB: Global variables
int g_mb_type;
char g_save_dir[255];

enum {
  UNDEFINED_MB = 0,  // others
  I_MB         = 1,  // I MB
  P_MB         = 2,  // non skip P MB
  S_MB         = 3   // skip MB (valid for P or B MB)
} MB_TYPE;


typedef struct inspector
{
  float*** coeffs; // of size (H, W, 3)
  float*** residual; // of size (H, W, 3)
  uint8** img_type; // of size (H, W)
  uint8*** img_mv; // of size (H, W, 2)

  int height;
  int width;
  int channel;
  int pic_type;

  int num_pic_stream;
  int num_display;
  int poc_offset;

  uint8 is_exported;

} Inspector;


void extract_coeffs(Macroblock* currMB, Slice* currSlice, float*** out_coeffs);
void extract_residual(Macroblock* currMB, Slice* currSlice, float*** out_residual);
void extract_mb_type(Macroblock* currMB, Slice* currSlice, int mb_type, uint8** img_type);


void inspect_pic_type(Inspector* inspector, int type);
void init_inspector(Inspector** inspector, VideoParameters* p_Vid, int num_display);
void free_inspector(Inspector** inspector);
int export_from_inspector(Inspector* inspector);
void inspect_poc_offset(Inspector* inspector, int offset);


void save_mb_type(int mb_type);
void inspect_set_savedir(char* location);

#endif
