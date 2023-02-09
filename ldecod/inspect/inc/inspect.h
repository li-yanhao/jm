#ifndef _INSPECT_H_
#define _INSPECT_H_


#include "macroblock.h"


typedef struct inspector
{
  float*** residual;
  int height;
  int width;
  int channel;

} Inspector;


void extract_residual(Macroblock* currMB, Slice* currSlice, float*** out_residual);
void init_inspector(Inspector** inspector, VideoParameters* p_Vid);
void free_inspector(Inspector** inspector);
void export_from_inspector(Inspector* inspector);

#endif
