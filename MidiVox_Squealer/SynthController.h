#ifndef _SYNTH_CONTROLLER_H_
#define _SYNTH_CONTROLLER_H_

#include <inttypes.h>

void SynthController_Setup() ;
void SynthController_Trigger
(byte keys, float filterVal, float attackVal, float releaseVal);
void SynthController_ProcessEnvelopes();
void SC_SetParam(byte which, float value, bool reset=false);
void SC_SetAttack(float attackVal);
void SC_SetRelease(float releaseVal);
void SC_SetFilterEnvelope(float filtEnvVal);

#endif




