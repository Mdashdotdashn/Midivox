#ifndef _MIDI_SETUP_H_
#define _MIDI_SETUP_H_

#include <inttypes.h>

// Midi CC number. Trying to match the shruti-1 map as close as possible

enum MidiCCValues
{
  MCC_WAVE_SHAPE = 20,
  MCC_WAVE_LOOP = 21,

  MCC_FILTER_MIX = 92,
  MCC_FILTER_CUT = 14,
  MCC_FILTER_RES = 15,
  MCC_FILTER_ENV = 102,
  
  MCC_ENV_ATTACK = 104,
  MCC_ENV_RELEASE = 107,

  MCC_OUT_GAIN = 7,
  MCC_OUT_NASTY = 9,
  MCC_LAST  
};

uint8_t presets[9]={47, 93, 0, 0, 34, 71, 7, 12, 64};

void MIDI_Setup();
void MIDI_Read();
void fillParams(uint8_t controller, uint8_t value);

#endif



