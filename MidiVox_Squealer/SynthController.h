#pragma once

#include <inttypes.h>

//---------------------------------------------------------------------------------------------

enum ControllerParameters
{
  CP_WAVE_SHAPE = 0,
  CP_WAVE_LOOP =1 ,
  CP_FILTER_MIX =2 ,
  CP_FILTER_CUT =3 ,
  CP_FILTER_RES = 4,
  CP_OUT_GAIN =5 ,
  CP_OUT_NASTY=6 ,
  CP_LAST
};

//---------------------------------------------------------------------------------------------

void SynthController_Setup() ;
void SynthController_Trigger(byte keys, float filterVal, float attackVal, float releaseVal);
void SynthController_ProcessEnvelopes();
void SC_SetParam(enum ControllerParameters which, float value, bool reset=false);
void SC_SetAttack(float attackVal);
void SC_SetRelease(float releaseVal);
void SC_SetFilterEnvelope(float filtEnvVal);





