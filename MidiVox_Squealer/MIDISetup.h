#ifndef _MIDI_SETUP_H_
#define _MIDI_SETUP_H_

#include <inttypes.h>

uint8_t presets[9]={47, 93, 0, 0, 34, 71, 7, 12, 64};

void MIDI_Setup();
void MIDI_Read();
void fillParams(uint8_t controller, uint8_t value);

#endif



