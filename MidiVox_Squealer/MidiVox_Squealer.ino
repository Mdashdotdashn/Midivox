#include <MIDI.h>

/***********************************************************************************
 *   HEALER MIDI SYNTH v1.0
 *         written for use with Arduino MidiVox - http://narbotic.com/kits/midivox
 *         based on Arduino Piano Squealer by Marc Nostromo - http://bit.ly/2xrZn5
 *
 *   NOTES:
 *    - responds to MIDI note values 0-84
 *    - synth parameters are mapped to MIDI control like so:
 *
 *         CC#____Parameter______
 *          0_____Waveform Select  
 *          1_____Filter Resonance
 *          2_____Filter Cutoff
 *          3_____Filter Type
 *          4_____Filter Envelope (set to 0 to disable)
 *          5_____Wave Loop/Truncate
 *          6_____Attack Time
 *          7_____Release Time
 *          
 ***********************************************************************************/
 
#include <EEPROM.h>
#include "MIDISetup.h"
#include "SynthController.h"

void(* resetFunc) (void) = 0;               //declare reset function @ address 0

//---------------------------------------------------------------------------------------------

void setup()
{
  pinMode(7, OUTPUT);
  pinMode(6, INPUT);
  digitalWrite(6, HIGH);
  digitalWrite(7, LOW);

  MIDI_Setup();                           // Set up MIDI data handlers and such
  SynthController_Setup();                // Initialize synth parameters
  Hardware_Setup();                       // Setup the arduino hardware, interrupts,etc..
  loadPresets();						  // Load preset parameters from eeprom 
} 


//---------------------------------------------------------------------------------------------

void loop()
{
  MIDI_Read();            // poll for MIDI data
  
  if ((PIND&B1000000)==0) // reset if needed
  {
    saveAndReset();
  }  
}


//---------------------------------------------------------------------------------------------

void flashLED(int pause)
{	
  PORTD |= (1<<7);     //digitalWrite(LED, HIGH);
  delay(pause);
  PORTD &= ~(1<<7);    //digitalWrite(7, LOW);
  delay(pause);
  PORTD |= (1<<7);     //digitalWrite(LED, HIGH);
  delay(pause);
  PORTD &= ~(1<<7);    //digitalWrite(7, LOW);
}


//---------------------------------------------------------------------------------------------

void saveAndReset()
{
  uint8_t val=0;
  int index=0;
  for (index = 0; index <=7; index++)
  {
    val = (presets[index]);
    EEPROM.write(index, val);
  }
  resetFunc();
}


//---------------------------------------------------------------------------------------------

void loadPresets()
{  
  unsigned int val=0;
  int index=0;
  for (index = 0; index <=7; index++)
  {
    val = EEPROM.read(index);			// Load a value from EEPROM
    if (val == 255)
    {					// If a preset hasn't been saved,
	val = presets[index];			// then load the default
    }
    fillParams(index, val);			
  }
  flashLED(100);
}
