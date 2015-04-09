//SHARP FILTER CUTOFF CHANGES ARE RISKY

#include "SynthEngine.h"

byte lastKeys = 255;
//float velocityVal = 1.0;
int transposeVal = 48; // -3[transpose multiplier] * 12
byte note = 255;

typedef void (*ControllerFunction) (float);

enum ControllerParamStatus {
  CPS_INACTIVE=0,
  CPS_UPDATE,
  CPS_ENVUP,
  CPS_ENVDOWN
};

enum ControllerParameters {
  CP_WAVE_SHAPE = 0,
  CP_WAVE_LOOP =1 ,
  CP_FILTER_MIX =2 ,
  CP_FILTER_CUT =3 ,
  CP_FILTER_RES = 4,
  CP_OUT_GAIN =5 ,
  CP_OUT_NASTY=6 ,
  CP_OUT_FEED =7,
  CP_LAST
};

struct params {
  ControllerParamStatus status_ ;
  ControllerFunction update_ ;
  float envelope_ ;
  float current_ ;
  float target_ ;
  float speed_ ;
};

struct patch {
  float attack_;
  float release_;
  float filterEnvelope_ ;
};

struct params controllerParams[CP_LAST];

struct patch controllerPatch;

void SC_SetParam(byte which,float value, bool reset){
  struct params *current=controllerParams+which ;
  current->current_=value ;
  if ((current->status_==CPS_INACTIVE)||(reset)) {
    current->status_=CPS_UPDATE;
    current->envelope_=0 ;  
  }
};

void SC_SetAttack(float attackVal){
  controllerPatch.attack_=attackVal;
};

void SC_SetRelease(float releaseVal){
  controllerPatch.release_=releaseVal;
};

void SC_SetFilterEnvelope(float filtEnvVal){
			controllerPatch.filterEnvelope_ = filtEnvVal;
};

void SC_SetEnvelope(byte which,float target,float time){

  struct params * current = controllerParams + which;
  if (time < 0.001) {
    current -> envelope_= target ;
    current->status_=CPS_UPDATE ;
  } 
  else {
    current -> target_= target ;
    if (target<current->envelope_) {
      float sp=(current->envelope_-target)/time/200.0 ;
      current->speed_=sp ;
      current->status_=CPS_ENVDOWN ;
    } 
    else {
      float sp=(target-current->envelope_)/time/200.0 ;
      current->speed_=sp ;
      current->status_=CPS_ENVUP ;
    }
  }
};

void SynthController_ProcessEnvelopes(){
  struct params * current = controllerParams ;
  for (byte i=0;i<CP_LAST;i++) {
    if (current->status_!=CPS_INACTIVE) {
      float value=current->current_+current->envelope_ ;
      if (value>1.0) value=1.0;
      if (value<0) value=0.0;
      current->update_(value);
      switch(current->status_){
      case CPS_UPDATE:
        current->status_==CPS_INACTIVE ;
        break ;
      case CPS_ENVUP:
        current->envelope_+=current->speed_ ;
        if (current->envelope_>current->target_) {
          current->envelope_=current->target_ ;
          current->status_=CPS_UPDATE ;
        }
        break ;
      case CPS_ENVDOWN:
        current->envelope_-= current -> speed_ ;
        if (current->envelope_< current->target_) {
          current->envelope_= current->target_ ;
          current->status_= CPS_UPDATE ;
        }
        break ;
      }
    };
    current++ ;
  }
};  

void SynthController_Setup(){
  SynthEngine_Setup() ;
  controllerPatch.attack_ = 0.02;
  controllerPatch.release_ = 0.02;
  controllerPatch.filterEnvelope_ = 0.0;

  struct params *current=controllerParams ;

  for (byte i=0;i<CP_LAST;i++) {
    switch(i) {
    case CP_WAVE_SHAPE:
      current->current_= 0.0;
      current->update_= SE_SetShape ;
      break ;
    case CP_WAVE_LOOP:
      current->current_= 1.0;
      current->update_= SE_SetWidth ;
      break ;
    case CP_FILTER_MIX:
      current->current_= 0.0;
      current->update_= SE_SetFilterMix ;
      break ;
    case CP_FILTER_CUT:
      current->current_= 1.0;
      current->update_= SE_SetFilterCut ;
      break ;
    case CP_FILTER_RES:
      current->current_= 0.0;
      current->update_= SE_SetFilterRes;
      break ;
    case CP_OUT_GAIN:
      current->current_= 0.0;
      current->update_= SE_SetOutput;
      break ;
    case CP_OUT_NASTY:
      current->current_= 0.0;
      current->update_= SE_SetNasty;
      break ;
    case CP_OUT_FEED:
      current->current_= 0.0;
      current->update_= SE_SetFeedBackVal;
      break ;
    default:
      current->update_= 0;
      break ;      
    }
    if (current->update_) {
      current->update_(current->current_) ;
    }
    current->status_ = CPS_INACTIVE;
    current->envelope_ = 0;
    current++;
  }
  SC_SetParam(CP_OUT_GAIN,0.0,true);
};

void SynthController_Trigger(byte keys, float filterVal, float attackVal, float releaseVal){
	
	
  //velocityVal = (velocity/float(127-weight));
  //if (velocityVal > 1.0) velocityVal = 1.0;
	
    if (lastKeys!=keys){
		note = keys; 
		lastKeys = keys;
		
	  if (filterVal > 0.0) {
			  if (note < 84){	// notecap could be <255
				  SE_SetNote(note - transposeVal);
				  SC_SetEnvelope(CP_FILTER_CUT, 1.0, filterVal); // if Filter Envelope
				  // FOR VELOCITY - SC_SetEnvelope(CP_OUT_GAIN, velocityVal, attackVal);
				  SC_SetEnvelope(CP_OUT_GAIN, 1.0, attackVal);
			  }
			  else {
				  SC_SetEnvelope(CP_FILTER_CUT, 0.0, filterVal);
				  SC_SetEnvelope(CP_OUT_GAIN,0.0,releaseVal);
			  }
	  }
		
		else{
		if (note < 84){										// notecap could be <255
				  SE_SetNote(note - transposeVal);
				  // FOR VELOCITY - SC_SetEnvelope(CP_OUT_GAIN, velocityVal, attackVal);
				  SC_SetEnvelope(CP_OUT_GAIN, 1.0, attackVal);
			  }
			  else {
				  SC_SetEnvelope(CP_OUT_GAIN,0.0,releaseVal) ;  // CHANGED
			  }
	  }
    }
};
  




















