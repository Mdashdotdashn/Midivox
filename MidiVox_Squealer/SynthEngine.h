
#ifndef _SYNTH_ENGINE_H_
#define _SYNTH_ENGINE_H_

#include <inttypes.h>
#include "Fixed.h"

//extern fixed maxSpeed ;

void SynthEngine_Setup() ;
uint16_t SynthEngine_ProcessSample() ;

void SE_SetShape(float) ;
void SE_SetNasty(float) ;
void SE_SetPitchBend(float pitchBend) ;
void SE_SetNote(int note) ; // -1 to kill note
void SE_SetWidth(float) ;
void SE_SetFilterCut(float) ;
void SE_SetFilterRes(float) ;
void SE_SetFilterMix(float) ;
void SE_SetOutput(float) ;
void SE_SetFeedBackVal(float in);

#endif




