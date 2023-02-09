#ifndef _INSPECT_H_
#define _INSPECT_H_


#include "macroblock.h"


typedef struct inspector
{
  float*** residual;
  int height;
  int width;
  int channel;

  int num_pic_stream;
  int num_display;

  uint8 is_exported;

} Inspector;


void extract_residual(Macroblock* currMB, Slice* currSlice, float*** out_residual);
void init_inspector(Inspector** inspector, VideoParameters* p_Vid, int num_display);
void free_inspector(Inspector** inspector);
int export_from_inspector(Inspector* inspector);

#endif
