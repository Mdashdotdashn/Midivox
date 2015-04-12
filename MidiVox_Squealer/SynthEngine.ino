#include <avr/interrupt.h>
#include "SynthData.h"

// Oscillator internal variable

float oscPosition = 0;
float oscSpeed = 0;
const uint8_t *oscDataStart = 0;
int oscDataLen = 0;
int currentNote = 0;
float currentFrequency = 0;
float pitchBendVal = -12;
float freq = 261.6255653006f; //C3
fixed oscFCut = i2fp(1);
fixed oscFRes = 0;
fixed oscFMix = 0;
float oscLoopWidth = 1.0;
fixed ampValue=i2fp(1);
fixed fltHeight=0 ;
fixed fltSpeed=0 ;
fixed fltDelay=0 ;
fixed outValue=0 ; // output for release
fixed nastyval = 0; //

// Oscillator external parameters
int oscShapeParam=0 ;
float oscFrequencyParam=0 ;

fixed clipFPlus=0 ;
fixed clipFMinus=0 ;

//---------------------------------------------------------------------------------------------

// Wave shape data
int oscShapeCount=6 ;

const uint8_t *oscShapeTable[]= 
{
   sineTable,
   boy1Table,
   triTable,
   noizTable,
   casioTable,
   squareTable
};

int oscShapeLength[]= 
{
  0x100,
  0x12,
  0x100,
  0x2AD,
  0x244,
  0x100
};

//---------------------------------------------------------------------------------------------


#define DRIVER_SAMPLERATE 15625.0f

void SE_SetPitchBend(float pitchBend)
{                // EXPERIMENTAL
  pitchBendVal = pitchBend;
  if (oscFrequencyParam==0)  return;
  else{
    float freqFactor=(pow(2.0,(currentNote+pitchBendVal)/12.0)) ;
    currentFrequency = (freq*freqFactor);
    SE_SetFrequency(currentFrequency);
  }
}


//---------------------------------------------------------------------------------------------

void SE_SetFrequency(float frequency)
{
  oscFrequencyParam = frequency;
  oscSpeed=(frequency*oscDataLen)/DRIVER_SAMPLERATE; 
}


//---------------------------------------------------------------------------------------------

void SE_SetNote(int note)
{
  currentNote=note;
  float freqFactor=(pow(2.0,(currentNote+pitchBendVal)/12.0)) ;
  SE_SetFrequency(freq*freqFactor) ;
}


//---------------------------------------------------------------------------------------------

void updateLoopPoints() 
{
  int len=oscShapeLength[oscShapeParam] ;
  oscDataStart = oscShapeTable[oscShapeParam] ;
  oscDataLen = len*oscLoopWidth ;

  SE_SetFrequency(oscFrequencyParam) ;
}


//---------------------------------------------------------------------------------------------

void SE_SetShape(float shape) 
{
  int newShape=shape*oscShapeCount-0.01 ;
  if (newShape == oscShapeParam) return ;

  oscShapeParam = newShape;
  updateLoopPoints();
  oscPosition=0;
}


//---------------------------------------------------------------------------------------------

void SE_SetNasty(float in) 
{
  nastyval = fl2fp(in);
}


//---------------------------------------------------------------------------------------------

void SE_SetWidth(float w) 
{
  oscLoopWidth=w;
  updateLoopPoints() ;                                  // DISABLED LOOP POINTS ... sorta
}


//---------------------------------------------------------------------------------------------

void SE_SetFilterCut(float cut) 
{
  oscFCut=fl2fp(cut*cut) ;
}


//---------------------------------------------------------------------------------------------

void SE_SetFilterMix(float mix) 
{	
  oscFMix=fl2fp(mix) ;
}


//---------------------------------------------------------------------------------------------

void SE_SetOutput(float out) 
{
  outValue=fl2fp(out) ;
}


//---------------------------------------------------------------------------------------------

void SE_SetFilterRes(float res) 
{
  float tmpRes=(1-res) ;
  oscFRes=fl2fp(1-tmpRes*tmpRes*tmpRes) ;
}


//---------------------------------------------------------------------------------------------

void SynthEngine_Setup() 
{
  oscShapeParam = 0;
  oscFrequencyParam = 0;
  SE_SetNote(38);
  SE_SetShape(0.3);

  clipFPlus=i2fp(16-FIXED_SHIFT);
  clipFMinus=-clipFPlus ;
}


//---------------------------------------------------------------------------------------------

// variables for oscillators

uint16_t index = 0;          // index for wave lookup (the upper 8 bits of the accumulator)

//---------------------------------------------------------------------------------------------

// ProcessSample:
// returns a unsigned 16 bit value
// to be sent to the DAC. This routine
// is called for every sample

uint16_t SynthEngine_ProcessSample() 
{
  // calculate new position inside the
  // current wavetable. 

  oscPosition += oscSpeed ;  // add in pith, the higher the number, the faster it rolls over, the more cycles per second
  index=((int)oscPosition) ;

  if (index>=oscDataLen)
  {
    oscPosition-=oscDataLen ;
    index-=oscDataLen ;
  }
  
  uint8_t rawOsc = 0 ;
  memcpy_P(&rawOsc,&oscDataStart[index],1);
  fixed osc = (rawOsc-0x80)<<(FIXED_SHIFT-8+4) ;
 
  fixed lpin = fp_mul(osc,FP_ONE-oscFMix) ;
  fixed hpin = -fp_mul(osc,oscFMix) ;

  fixed difr = fp_sub(lpin,fltHeight);
  fltSpeed = fp_mul(fltSpeed,oscFRes);		//mul by res, it's some kind of inertia. caution to feedback
  fltSpeed = fp_add(fltSpeed,fp_mul(difr,oscFCut)); //mul by cutoff, less cutoff = no sound, so better not be 0.      

  fltHeight += fltSpeed ;
  fltHeight += fltDelay - hpin ;

  osc = fltHeight ;

  if (fltHeight>clipFPlus) 
  {
    fltHeight=clipFPlus ;
  } 
  else if (fltHeight<clipFMinus)
  {
    fltHeight=clipFMinus ;
  }

  fltDelay = hpin; 
  
/*  if (osc<nastyval)  //this is a nasty trick (hence the name) to change a sin to a square, and you can modulate the pulsewith aswell! 
    osc = clipFPlus;
  else if (osc > -nastyval)
    osc = clipFMinus;	
 
  osc = fp_mul(osc,outValue);
*/  
  osc <<= 2;
  return osc+0x8000;   // sample format for DAC is 12 bit, left justified
}



