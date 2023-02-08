#include "tracehelper.h"
#include "xmltracefile.h"

int iCurr_frame_id = 0;
int iCurr_gop_number = -1;
int iDisplayNumberOffset = 0;
int iCurr_mb_type = 0;

int iCurr_NAL_number = -1;
NALU_t *curr_NAL, *dpb_NAL, *dpc_NAL;
seq_parameter_set_rbsp_t *sps = NULL;// = AllocSPS();

int getNewFrameID()
{
	iCurr_frame_id++;
	return iCurr_frame_id - 1;
}

void incrementGOP()
{
	iCurr_gop_number++;
}

int getGOPNumber()
{
	return iCurr_gop_number;
}

void setDisplayNumberOffset(int value)
{
	iDisplayNumberOffset = value;
}

int getDisplayNumberOffset()
{
	return iDisplayNumberOffset;
}

void incrementNAL()
{
	iCurr_NAL_number++;
}

void setNAL(NALU_t *nal)
{
	incrementNAL();
	curr_NAL = AllocNALU(MAX_CODED_FRAME_SIZE);
	curr_NAL->nal_unit_type = nal->nal_unit_type;
	curr_NAL->len = nal->len;
}

void setDPBNAL(NALU_t *nal)
{
	incrementNAL();
	dpb_NAL = AllocNALU(MAX_CODED_FRAME_SIZE);
	dpb_NAL->nal_unit_type = nal->nal_unit_type;
	dpb_NAL->len = nal->len;
}

void setDPCNAL(NALU_t *nal)
{
	incrementNAL();
	dpc_NAL = AllocNALU(MAX_CODED_FRAME_SIZE);
	dpc_NAL->nal_unit_type = nal->nal_unit_type;
	dpc_NAL->len = nal->len;
}

void clearNALInfo()
{
	FreeNALU(curr_NAL);
	curr_NAL = NULL;
	FreeNALU(dpb_NAL);
	dpb_NAL = NULL;
	FreeNALU(dpc_NAL);
	dpc_NAL = NULL;
}

void getMbTypeName_I_Slice(int mb_type, Macroblock* currMB, char* typestring, char* predmodstring, int corr)
{

	strcpy(predmodstring, "BLOCK_TYPE_I");

	switch(mb_type - corr)
	{
		case 0: 
			if(currMB->luma_transform_size_8x8_flag == 0) 
				strcpy(typestring, "I_4x4");
			else
				strcpy(typestring, "I_8x8");
			break;
		case 1: strcpy(typestring, "I_16x16_0_0_0"); break;
		case 2: strcpy(typestring, "I_16x16_1_0_0"); break;
		case 3: strcpy(typestring, "I_16x16_2_0_0"); break;
		case 4: strcpy(typestring, "I_16x16_3_0_0"); break;
		case 5: strcpy(typestring, "I_16x16_0_1_0"); break;
		case 6: strcpy(typestring, "I_16x16_1_1_0"); break;
		case 7: strcpy(typestring, "I_16x16_2_1_0"); break;
		case 8: strcpy(typestring, "I_16x16_3_1_0"); break;
		case 9: strcpy(typestring, "I_16x16_0_2_0"); break;
		case 10: strcpy(typestring, "I_16x16_1_2_0"); break;
		case 11: strcpy(typestring, "I_16x16_2_2_0"); break;
		case 12: strcpy(typestring, "I_16x16_3_2_0"); break;
		case 13: strcpy(typestring, "I_16x16_0_0_1"); break;
		case 14: strcpy(typestring, "I_16x16_1_0_1"); break;
		case 15: strcpy(typestring, "I_16x16_2_0_1"); break;
		case 16: strcpy(typestring, "I_16x16_3_0_1"); break;
		case 17: strcpy(typestring, "I_16x16_0_1_1"); break;
		case 18: strcpy(typestring, "I_16x16_1_1_1"); break;
		case 19: strcpy(typestring, "I_16x16_2_1_1"); break;
		case 20: strcpy(typestring, "I_16x16_3_1_1"); break;
		case 21: strcpy(typestring, "I_16x16_0_2_1"); break;
		case 22: strcpy(typestring, "I_16x16_1_2_1"); break;
		case 23: strcpy(typestring, "I_16x16_2_2_1"); break;
		case 24: strcpy(typestring, "I_16x16_3_2_1"); break;
		case 25: strcpy(typestring, "I_PCM"); break;
		default: strcpy(typestring, "UNKNOWN"); break;
	}

}

void getMbTypeName_SI_Slice(int mb_type, Macroblock* currMB, char* typestring, char* predmodstring, int corr)
{

	strcpy(predmodstring, "BLOCK_TYPE_SI");

	if(mb_type == 0) strcpy(typestring, "SI");
	else getMbTypeName_I_Slice(mb_type, currMB, typestring, predmodstring, 1);

}

void getMbTypeName_P_SP_Slice(int mb_type, Macroblock* currMB, char* typestring, char* predmodstring, int corr)
{

	if(mb_type != 0) mb_type--;

	strcpy(predmodstring, "BLOCK_TYPE_P");

	if(mb_type <= 4)
	{
		switch(mb_type)
		{
			case 0: strcpy(typestring, "P_L0_16x16"); break;
			case 1: strcpy(typestring, "P_L0_L0_16x8"); break;
			case 2: strcpy(typestring, "P_L0_L0_8x16"); break;
			case 3: strcpy(typestring, "P_8x8"); break;
			case 4: strcpy(typestring, "P_8x8ref0"); break;
		}
	}
	else getMbTypeName_I_Slice(mb_type, currMB, typestring, predmodstring, 5);

	if(currMB->skip_flag == 1)
		strcpy(typestring, "P_SKIP");
}

void getMbTypeName_B_Slice(int mb_type, Macroblock* currMB, char* typestring, char* predmodstring, int corr)
{

	strcpy(predmodstring, "BLOCK_TYPE_B");

	if(mb_type <= 22)
	{
		switch(mb_type)
		{
			case 0: strcpy(typestring, "B_Direct_16x16"); break;
			case 1: strcpy(typestring, "B_L0_16x16"); break;
			case 2: strcpy(typestring, "B_L1_16x16"); break;
			case 3: strcpy(typestring, "B_Bi_16x16"); break;
			case 4: strcpy(typestring, "B_L0_L0_16x8"); break;
			case 5: strcpy(typestring, "B_L0_L0_8x16"); break;
			case 6: strcpy(typestring, "B_L1_L1_16x8"); break;
			case 7: strcpy(typestring, "B_L1_L1_8x16"); break;
			case 8: strcpy(typestring, "B_L0_L1_16x8"); break;
			case 9: strcpy(typestring, "B_L0_L1_8x16"); break;
			case 10: strcpy(typestring, "B_L1_L0_16x8"); break;
			case 11: strcpy(typestring, "B_L1_L0_8x16"); break;
			case 12: strcpy(typestring, "B_L0_Bi_16x8"); break;
			case 13: strcpy(typestring, "B_L0_Bi_8x16"); break;
			case 14: strcpy(typestring, "B_L1_Bi_16x8"); break;
			case 15: strcpy(typestring, "B_L1_Bi_8x16"); break;
			case 16: strcpy(typestring, "B_Bi_L0_16x8"); break;
			case 17: strcpy(typestring, "B_Bi_L0_8x16"); break;
			case 18: strcpy(typestring, "B_Bi_L1_16x8"); break;
			case 19: strcpy(typestring, "B_Bi_L1_8x16"); break;
			case 20: strcpy(typestring, "B_Bi_Bi_16x8"); break;
			case 21: strcpy(typestring, "B_Bi_Bi_8x16"); break;
			case 22: strcpy(typestring, "B_8x8"); break;
		}
	}
	else getMbTypeName_I_Slice(mb_type, currMB, typestring, predmodstring, 23);

	if(currMB->skip_flag == 1)
		strcpy(typestring, "B_SKIP");
}

void getSubMbTypeName_P_Slice(int submb_type, char* typestring)
{
	switch(submb_type)
	{
		case 0: strcpy(typestring, "P_L0_8x8"); break;
		case 1: strcpy(typestring, "P_L0_8x4"); break;
		case 2: strcpy(typestring, "P_L0_4x8"); break;
		case 3: strcpy(typestring, "P_L0_4x4"); break;
		default: strcpy(typestring, "UNKNOWN"); break;
	}
}

void getSubMbTypeName_B_Slice(int submb_type, char* typestring)
{
	switch(submb_type)
	{
		case 0: strcpy(typestring, "B_Direct_8x8"); break;
		case 1: strcpy(typestring, "B_L0_8x8"); break;
		case 2: strcpy(typestring, "B_L1_8x8"); break;
		case 3: strcpy(typestring, "B_Bi_8x8"); break;
		case 4: strcpy(typestring, "B_L0_8x4"); break;
		case 5: strcpy(typestring, "B_L0_4x8"); break;
		case 6: strcpy(typestring, "B_L1_8x4"); break;
		case 7: strcpy(typestring, "B_L1_4x8"); break;
		case 8: strcpy(typestring, "B_Bi_8x4"); break;
		case 9: strcpy(typestring, "B_Bi_4x8"); break;
		case 10: strcpy(typestring, "B_L0_4x4"); break;
		case 11: strcpy(typestring, "B_L1_4x4"); break;
		case 12: strcpy(typestring, "B_Bi_4x4"); break;
		default: strcpy(typestring, "UNKNOWN"); break;
	}
}

void addMVInfoToTrace(Macroblock *currMB)
{
	int j0, i0, j, i, list;
	int kk, step_h, step_v;
	char typestring[256];

	if(currMB->mb_type == P8x8)
	{
		//Loop through every submacroblock
		for(j0 = 0; j0 < 4; j0 += 2)	//vertical
		{
			for(i0 = 0; i0 < 4; i0 += 2)	//horizontal
			{
				kk = 2 * (j0 >> 1) + (i0 >> 1);
				xml_write_start_element("SubMacroBlock");
					xml_write_int_attribute("num", kk);
					xml_write_start_element("Type");
						xml_write_int(currMB->b8submbtype[kk]);
					xml_write_end_element();
					xml_write_start_element("TypeString");
						if(currMB->p_Slice->slice_type == B_SLICE)
							getSubMbTypeName_B_Slice(currMB->b8submbtype[kk], typestring);
						else
							getSubMbTypeName_P_Slice(currMB->b8submbtype[kk], typestring);
						xml_write_text(typestring);
					xml_write_end_element();

					if(xml_get_log_level() >= 3)
					{
						//Loop through every block inside the submacroblock to get the motion vectors
						step_h = BLOCK_STEP [currMB->b8mode[kk]][0];
						step_v = BLOCK_STEP [currMB->b8mode[kk]][1];
						if(currMB->b8mode[kk] != 0) //Has forward vector
						{
							for (j = j0; j < j0 + 2; j += step_v)
							{
								for (i = i0; i < i0 + 2; i += step_h)
								{
									//Loop through LIST0 and LIST1
									for (list = LIST_0; list <= LIST_1; list++)
									{
										if ((currMB->b8pdir[kk]== list || currMB->b8pdir[kk]== BI_PRED) && (currMB->b8mode[kk] !=0))//has forward vector
										{
											xml_write_start_element("MotionVector");
												xml_write_int_attribute("list", list);
												xml_write_start_element("RefIdx");
													xml_write_int(currMB->p_Vid->dec_picture->motion.ref_id[list][currMB->block_y+j0][currMB->block_x+i0]);
												xml_write_end_element();
												xml_write_start_element("Difference");
													xml_write_start_element("X");
														xml_write_int(currMB->mvd[list][j][i][0]);
													xml_write_end_element();
													xml_write_start_element("Y");
														xml_write_int(currMB->mvd[list][j][i][1]);
													xml_write_end_element();
												xml_write_end_element();
												xml_write_start_element("Absolute");
													xml_write_start_element("X");
														xml_write_int(currMB->p_Vid->dec_picture->motion.mv[list][currMB->block_y + j][currMB->block_x + i][0]);
													xml_write_end_element();
													xml_write_start_element("Y");
														xml_write_int(currMB->p_Vid->dec_picture->motion.mv[list][currMB->block_y + j][currMB->block_x + i][1]);
													xml_write_end_element();
												xml_write_end_element();
											xml_write_end_element();
										}
									}
								}
							}
						}
					}
				xml_write_end_element();
			}
		}
	}
}

void addCoeffsToTrace(Macroblock* currMB, Slice* currSlice)
{
	int pl, i, j;
	xml_write_start_element("Coeffs");
		if(currMB->luma_transform_size_8x8_flag)
		{
			//Luma
			xml_write_start_element("Plane");
			xml_write_int_attribute("type", 0);
				for(i = 0; i < 16; i++)
				{
					xml_write_start_element("Row");
					for(j = 0; j < 16; j++)
					{
						if(j > 0) xml_write_text(",");
						xml_write_int((&currSlice->mb_rres[0][0])[i][j]);
					}
					xml_write_end_element();
				}
			xml_write_end_element();
			//Chroma
			for(pl = 1; pl <= 2; pl++)
			{
				xml_write_start_element("Plane");
				xml_write_int_attribute("type", pl);
					for( i = 0; i < 8; i++)
					{
						xml_write_start_element("Row");
						for(j = 0; j < 8; j++)
						{
							if(j > 0) xml_write_text(",");
							xml_write_int(currSlice->cof[pl][i][j]);
						}
						xml_write_end_element();
					}
				xml_write_end_element();
			}
		}
		else
		{
			//Luma
			xml_write_start_element("Plane");
			xml_write_int_attribute("type", 0);
				for(i = 0; i < 16; i++)
				{
					xml_write_start_element("Row");
					for(j = 0; j < 16; j++)
					{
						if(j > 0) xml_write_text(",");
						xml_write_int(currSlice->cof[0][i][j]);
					}
					xml_write_end_element();
				}
			xml_write_end_element();
			//Chroma
			for(pl = 1; pl <= 2; pl++)
			{
				xml_write_start_element("Plane");
				xml_write_int_attribute("type", pl);
					for( i = 0; i < 8; i++)
					{
						xml_write_start_element("Row");
						for(j = 0; j < 8; j++)
						{
							if(j > 0) xml_write_text(",");
							xml_write_int(currSlice->cof[pl][i][j]);
						}
						xml_write_end_element();
					}
				xml_write_end_element();
			}
		}
	xml_write_end_element();
}

void writeMBInfo(Macroblock* currMB, Slice* currSlice)
{
	char typestring[255];
	char predmodstring[255];
	switch (currSlice->slice_type)
	{
		case P_SLICE: 
			getMbTypeName_P_SP_Slice(iCurr_mb_type, currMB, typestring, predmodstring, 0);
			break;
		case SP_SLICE:
			getMbTypeName_P_SP_Slice(iCurr_mb_type, currMB, typestring, predmodstring, 0);
			break;
		case B_SLICE:
			getMbTypeName_B_Slice(iCurr_mb_type, currMB, typestring, predmodstring, 0);
			break;
		case I_SLICE: 
			getMbTypeName_I_Slice(iCurr_mb_type, currMB, typestring, predmodstring, 0);
			break;
		case SI_SLICE: 
			getMbTypeName_SI_Slice(iCurr_mb_type, currMB, typestring, predmodstring, 0);
			break;
	}
	xml_write_start_element("Type");
		switch (currSlice->slice_type)
		{
			case P_SLICE: 
			case SP_SLICE:
				if(currMB->skip_flag == 1)
					xml_write_int(-1);
				else
				{
					if(iCurr_mb_type != 0)
						xml_write_int(iCurr_mb_type-1);
					else
						xml_write_int(iCurr_mb_type);
				}
				break;
			case B_SLICE:
				if(currMB->skip_flag == 1)
					xml_write_int(-1);
				else
					xml_write_int(iCurr_mb_type);
				break;
			case I_SLICE: 
			case SI_SLICE:
				xml_write_int(iCurr_mb_type);
				break;
		}
	xml_write_end_element();
	xml_write_start_element("TypeString");
		xml_write_text(typestring);
	xml_write_end_element();
	xml_write_start_element("PredModeString");
		xml_write_text(predmodstring);
	xml_write_end_element();
	xml_write_start_element("SkipFlag");
		if(typestring[0] == 'I')
			xml_write_int(0);
		else
			xml_write_int(currMB->skip_flag);
	xml_write_end_element();
}

void writeNALInfo(Slice* currSlice)
{
	char typestring[256];

	//What about slice type 20?
	//Write NonPicture element when needed
	if(curr_NAL->nal_unit_type > 5) 
	{
		xml_check_and_write_end_element("Picture"); //End the picture tag when needed
		xml_write_start_element("NonPicture");
	}

	xml_write_start_element("NAL");
		xml_write_start_element("Num");
			if(dpc_NAL != NULL && dpb_NAL != NULL)
			{
				xml_write_int(iCurr_NAL_number - 2);
			}
			else
			{
				if(dpc_NAL != NULL || dpb_NAL != NULL)
					xml_write_int(iCurr_NAL_number - 1);
				else
					xml_write_int(iCurr_NAL_number);
			}
		xml_write_end_element();
		xml_write_start_element("Type");
			xml_write_int(curr_NAL->nal_unit_type);
		xml_write_end_element();
		xml_write_start_element("TypeString");
			getNALTypeName(curr_NAL->nal_unit_type, typestring);
			xml_write_text(typestring);
		xml_write_end_element();
		xml_write_start_element("Length");
			xml_write_int(curr_NAL->len);
		xml_write_end_element();
	xml_write_end_element();
    
    //Output SPS info
    if(curr_NAL->nal_unit_type == 7)
    {
        if(sps != NULL)
        {
            xml_write_start_element("SPS");
            
            //width = ((pic_width_in_mbs_minus1 +1)*16) - frame_crop_left_offset*2 - frame_crop_right_offset*2;
            //height= ((2 - frame_mbs_only_flag)* (pic_height_in_map_units_minus1 +1) * 16) - (frame_crop_top_offset * 2) - (frame_crop_bottom_offset * 2);
            
            xml_write_start_element("pic_width_in_mbs_minus1");
                xml_write_int(sps->pic_width_in_mbs_minus1);
            xml_write_end_element();
            xml_write_start_element("pic_height_in_map_units_minus1");
                xml_write_int(sps->pic_height_in_map_units_minus1);
            xml_write_end_element();
            xml_write_start_element("frame_mbs_only_flag");
                xml_write_int(sps->frame_mbs_only_flag);
            xml_write_end_element();
            //if(sps->frame_cropping_flag)
            //{
                xml_write_start_element("frame_cropping");
                
                    xml_write_start_element("frame_crop_left_offset");
                        xml_write_int(sps->frame_cropping_rect_left_offset);
                    xml_write_end_element();
                    xml_write_start_element("frame_crop_right_offset");
                        xml_write_int(sps->frame_cropping_rect_right_offset);
                    xml_write_end_element();
                    xml_write_start_element("frame_crop_top_offset");
                        xml_write_int(sps->frame_cropping_rect_top_offset);
                    xml_write_end_element();
                    xml_write_start_element("frame_crop_bottom_offset");
                        xml_write_int(sps->frame_cropping_rect_bottom_offset);
                    xml_write_end_element();
                
                xml_write_end_element();
            //}

            
            if(sps->vui_parameters_present_flag && sps->vui_parameters_present_flag)
            {
                xml_write_start_element("vui_parameters");
                
                    xml_write_start_element("timing_info");
                        xml_write_start_element("num_units_in_tick");
                            xml_write_int(sps->vui_seq_parameters.num_units_in_tick);
                        xml_write_end_element();
                        xml_write_start_element("time_scale");
                            xml_write_int(sps->vui_seq_parameters.time_scale);
                        xml_write_end_element();
                        xml_write_start_element("fixed_frame_rate_flag");
                            xml_write_int(sps->vui_seq_parameters.fixed_frame_rate_flag);
                        xml_write_end_element();
                    xml_write_end_element();
                
                xml_write_end_element();
                
            }
            
            xml_write_end_element();
        }
    }

	//Write DPB NAL info
	if(currSlice->dpB_NotPresent == 0 && dpb_NAL != NULL)
	{
		xml_write_start_element("NAL");
			xml_write_start_element("Num");
				if(dpc_NAL != NULL)
					xml_write_int(iCurr_NAL_number - 1);
				else
					xml_write_int(iCurr_NAL_number);
			xml_write_end_element();
			xml_write_start_element("Type");
				xml_write_int(dpb_NAL->nal_unit_type);
			xml_write_end_element();
			xml_write_start_element("TypeString");
				getNALTypeName(dpb_NAL->nal_unit_type, typestring);
				xml_write_text(typestring);
			xml_write_end_element();
			xml_write_start_element("Length");
				xml_write_int(dpb_NAL->len);
			xml_write_end_element();
		xml_write_end_element();
	}

	//Write DPC NAL info
	if(currSlice->dpC_NotPresent == 0 && dpc_NAL != NULL)
	{
		xml_write_start_element("NAL");
			xml_write_start_element("Num");
				xml_write_int(iCurr_NAL_number);
			xml_write_end_element();
			xml_write_start_element("Type");
				xml_write_int(dpc_NAL->nal_unit_type);
			xml_write_end_element();
			xml_write_start_element("TypeString");
				getNALTypeName(dpc_NAL->nal_unit_type, typestring);
				xml_write_text(typestring);
			xml_write_end_element();
			xml_write_start_element("Length");
				xml_write_int(dpc_NAL->len);
			xml_write_end_element();
		xml_write_end_element();
	}

	if(curr_NAL->nal_unit_type > 5) xml_write_end_element();
}

void getNALTypeName(int nal_type, char* typestring)
{
	switch(nal_type)
	{
		case 0: strcpy(typestring, "Unspecified"); break;
		case 1: strcpy(typestring, "NALU_TYPE_SLICE"); break;
		case 2: strcpy(typestring, "NALU_TYPE_DPA"); break;
		case 3: strcpy(typestring, "NALU_TYPE_DPB"); break;
		case 4: strcpy(typestring, "NALU_TYPE_DPC"); break;
		case 5: strcpy(typestring, "NALU_TYPE_IDR"); break;
		case 6: strcpy(typestring, "NALU_TYPE_SEI"); break;
		case 7: strcpy(typestring, "NALU_TYPE_SPS"); break;
		case 8: strcpy(typestring, "NALU_TYPE_PPS"); break;
		case 9: strcpy(typestring, "NALU_TYPE_AUD"); break;
		case 10: strcpy(typestring, "NALU_TYPE_EOSEQ"); break;
		case 11: strcpy(typestring, "NALU_TYPE_EOSTREAM"); break;
		case 12: strcpy(typestring, "NALU_TYPE_FILL"); break;
		case 13: strcpy(typestring, "NALU_TYPE_SPSEXT"); break;
		case 14: strcpy(typestring, "NALU_TYPE_PREFIX"); break;
		case 15: strcpy(typestring, "NALU_TYPE_SPSSUBSET"); break;
		case 16:
		case 17:
		case 18: strcpy(typestring, "Reserved"); break;
		case 19: strcpy(typestring, "NALU_TYPE_AUX"); break;
		case 20: strcpy(typestring, "NALU_TYPE_SLICEEXT"); break;
		case 21:
		case 22:
		case 23: strcpy(typestring, "Reserved"); break;
		case 24:
		case 25:
		case 26:
		case 27:
		case 28:
		case 29:
		case 30:
		case 31: strcpy(typestring, "Unspecified"); break;
		default: strcpy(typestring, "UNKNOWN"); break;
	}
}

void setCurrentMBType(int value)
{
	iCurr_mb_type = value;
}

void copySPS(pic_parameter_set_rbsp_t *sps_orig)
{
    if(sps == NULL)
        free(sps);
    
    sps = calloc (1, sizeof (seq_parameter_set_rbsp_t));
    
    memcpy (sps, sps_orig, sizeof (seq_parameter_set_rbsp_t));
}