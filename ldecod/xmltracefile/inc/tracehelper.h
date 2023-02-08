#ifndef _TRACEHELPER_H_
#define _TRACEHELPER_H_

#include "macroblock.h"
#include "nalucommon.h"
#include "parsetcommon.h"

//Function declarations
int getNewFrameID();
void incrementGOP();
int getGOPNumber();
void setDisplayNumberOffset(int value);
int getDisplayNumberOffset();
void setNAL(NALU_t *nal);
void incrementNAL();
void setDPBNAL(NALU_t *nal);
void setDPCNAL(NALU_t *nal);
void getMbTypeName_I_Slice(int, Macroblock*, char* , char* , int );
void getMbTypeName_SI_Slice(int, Macroblock*, char* , char* , int );
void getMbTypeName_P_SP_Slice(int, Macroblock*, char* , char* , int );
void getMbTypeName_B_Slice(int, Macroblock*, char* , char* , int );
void getSubMbTypeName_P_Slice(int submb_type, char* typestring);
void getSubMbTypeName_B_Slice(int submb_type, char* typestring);
void addMVInfoToTrace(Macroblock*);
void addCoeffsToTrace(Macroblock* currMB, Slice* currSlice);
void writeMBInfo(Macroblock* currMB, Slice* currSlice);
void writeNALInfo(Slice* currSlice);
void getNALTypeName(int, char*);
void setCurrentMBType(int value);
void clearNALInfo();
void copySPS(pic_parameter_set_rbsp_t *pps_orig);
//End function declarations

#endif
