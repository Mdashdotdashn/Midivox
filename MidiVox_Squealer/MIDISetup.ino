//MIDI Setup

MIDI_CREATE_DEFAULT_INSTANCE();

#include "SynthController.h"

byte noteOut = 255;
float pitchVal = 0.0;
float paramVal = 0.0;
float filterVal = 0.0;
float attackVal = 0.0;
float releaseVal = 0.0;
int v, pos, i = 0;

byte noteBuf[5] = {255,255,255,255,255};  // set up keyboard buffer for 5 notes

void handleNoteOff(byte channel, byte note, byte velocity)
{
  note_delete(note);
  playNote();
}

void handleNoteOn(byte channel, byte note, byte velocity)
{
  if (!velocity)  // this will be a note off message
  {
    note_delete(note);
  }

  else if (note < 84)            // check if the note isn't too high for the synth engine
  {
    note_insert(note);
  }

  playNote();
}

void handlePitchBend(byte channel, int pitch)
{
  pitch = map(pitch,0,127,0,128);
  pitchVal = ((pitch)/float(690))-23.95;
  SE_SetPitchBend(pitchVal);
}

void handleControlChange(byte channel, byte controller, byte value)
{ 
  fillParams(controller, value);
}

void handleProgramChange(byte channel, byte program)
{
  randomSeed(program);
  fillParams((unsigned int)0, (unsigned int) random(0,127));
  fillParams((unsigned int)1, (unsigned int) random(0,127));
  fillParams((unsigned int)2, (unsigned int) random(0,127));
  fillParams((unsigned int)3, (unsigned int) random(0,127));
  fillParams((unsigned int)4, (unsigned int) random(0,127));
  fillParams((unsigned int)5, (unsigned int) random(0,127));
  fillParams((unsigned int)6, (unsigned int) random(0,127/4));
  fillParams((unsigned int)7, (unsigned int) random(0,127/4));
  fillParams((unsigned int)8, (unsigned int) random(0,127));
  fillParams((unsigned int)9, (unsigned int) random(0,127));
  fillParams((unsigned int)10, (unsigned int) random(0,110));
  saveAndReset();
}


void playNote()
{ 	
  if (noteBuf[0]!=255){  
    PORTD |= (1<<7);                                         // DATA LED ON
    noteOut = (noteBuf[0]);                                        
  }	
  else
  {   
    PORTD &= ~(1<<7);                                        //DATA LED OFF
    noteOut = 255;
  }	
}

void note_insert(byte note)
{	
  for (v=0; v<=4; v++) // remove the note if it's already in the buffer
  {  
    if (noteBuf[v] == note) 
    {
      for(pos=v; pos<=4; pos++) 
      {
  	noteBuf[pos]=noteBuf[pos+1];
      }
    }
  }

  for(i=4; i>0; i--) 
  {
    noteBuf[i] = noteBuf[i-1];       // move each note in the buffer up one position
  }
	
  noteBuf[0]=note;                   // place our new note in the beginning of the note buffer
}

void note_delete(byte note)
{
  for (v=0; v<=4; v++)
  {  
    if (noteBuf[v] == note)
    {
      pos=v;
      break;  
    }
  }
	
  for(v=pos; v<=4; v++) 
  {
    noteBuf[v]=noteBuf[v+1];
  }
  noteBuf[v-1]=255;
}

void MIDI_Setup()
{
  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleNoteOff(handleNoteOff); 
  MIDI.setHandleControlChange(handleControlChange); 
  MIDI.setHandlePitchBend(handlePitchBend); 
  MIDI.setHandleProgramChange(handleProgramChange); 
  MIDI.begin();
 // midi.setParam(Midi::PARAM_SEND_FULL_COMMANDS, 0);
}

void MIDI_Read()
{
  MIDI.read();
  SynthController_ProcessEnvelopes();        // process synth data
  SynthController_Trigger(noteOut, filterVal, attackVal, releaseVal);          // send audio data
}

void fillParams(uint8_t controller, uint8_t value)
{
    switch (controller)
    {
    case 0:
      presets[0] = value;
      paramVal= value/float(127);
      SC_SetParam(0, paramVal);           // Waveform Select
      break;
    case 1:
      presets[1] = value;
      paramVal= value/float(127);
      SC_SetParam(4, paramVal);           // Filter Resonance
      break;
    case 2:
      presets[2] = value;
      paramVal= (value+10)/float(137);
      SC_SetParam(3, paramVal);           // Filter Cutoff
      break;
    case 3:
      presets[3] = value; 
      paramVal= value/float(127);
      SC_SetParam(2, paramVal);           // Filter Mix  
      break;
    case 4:
      presets[4] = value;					// Filter Envelope
      filterVal= value/float(137);
      break;
    case 5:
      presets[5] = value;
      paramVal= (value+8)/float(135);
      SC_SetParam(1, paramVal);           // Wave Loop/Truncate
      break;
    case 6:
      presets[6] = value;
      attackVal = value/float(48);           // Attack Time
      break;
    case 7:
      presets[7] = value;
      releaseVal = value/float(32);         // Release Time
      break;
    case 8:
      presets[8] = value;                 // Velocity Sensitivity - DISABLED!
	  //veloWeight = value;
      break;
    case 9:
       presets[9] = value;
       paramVal= value/float(127);
         SC_SetParam(6, paramVal);           // NASTY  
    break;
      case 10:
       presets[10] = value;
       paramVal= value/float(127);
       SC_SetParam(7, paramVal);           // Feedback  
    break;
      
    default:
      break;
    }
}

