#ifndef _INSPECT_H_
#define _INSPECT_H_


#include "macroblock.h"

// NB: Global variable
int g_mb_type;

enum {
  UNDEFINED_MB = 0,  // others
  I_MB         = 1,  // I MB
  P_MB         = 2,  // non skip P MB
  S_MB         = 3   // skip MB (valid for P or B MB)
} MB_TYPE;


typedef struct inspector
{
  float*** coeffs;
  float*** residual;
  uint8** img_type;

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

void inspect_pic_type(Inspector* inspector, int type);
void init_inspector(Inspector** inspector, VideoParameters* p_Vid, int num_display);
void free_inspector(Inspector** inspector);
int export_from_inspector(Inspector* inspector);
void inspect_poc_offset(Inspector* inspector, int offset);

void extract_mb_type(Macroblock* currMB, Slice* currSlice, int mb_type, uint8** img_type);

void save_mb_type(int mb_type);

#endif
