
//---------------------------------------------------------------------------------------------
//!
//  This is the method that needs to be defined so that it produce a sample

extern uint16_t SynthEngine_ProcessSample();

//---------------------------------------------------------------------------------------------

//! Hardware Setup. Setup the interrupts driving the audio playback.

void Hardware_Setup()
{
  //Timer2 setup  This is the audio rate timer, fires an interrupt at 15625 Hz sampling rate

  TIMSK2 = 1<<OCIE2A;  // interrupt enable audio timer
  OCR2A = 127;
  TCCR2A = 2;               // CTC mode, counts up to 127 then resets
  TCCR2B = 0<<CS22 | 1<<CS21 | 0<<CS20;   // different for atmega8 (no 'B' i think)
    
  SPCR = 0x50;   // set up SPI port  SPCR = 01010000, 
  SPSR = 0x01;  //
  DDRB |= 0x2E;       // PB output for DAC CS, and SPI port
  PORTB |= (1<<1);   // CS high
  
  sei();			// global interrupt enable
}


//---------------------------------------------------------------------------------------------

//! Timer 2 interrupt routine calling the synth engine for every sample

ISR(TIMER2_COMPA_vect)
{
  OCR2A = 127;
  
  //PORTB &= ~(1<<1); // Frame sync low for SPI (making it low here so that we can measure length of interrupt with scope)
 
  uint16_t sample = SynthEngine_ProcessSample();
  
  // format sample for SPI port
  uint8_t dacSPI0 = sample >> 8;
  dacSPI0 >>= 4;
  //dacSPI0 |= 0x30; // Write to DAC-A, unbuffered VREF, gain=1x, Output Power Down Control bit (low impedence out?), 
  dacSPI0 |= 0x70;  //buffered VRef
  uint8_t dacSPI1 = sample >> 4;  

  // transmit value out the SPI port
  PORTB &= ~(1<<1); // Frame sync low
  SPDR = dacSPI0;  //The SPI Data Register - Writing to the register initiates data transmission.
  while (!(SPSR & (1<<SPIF))); //Wait for data to be sent over SPI, flag raised
  SPDR = dacSPI1;
  while (!(SPSR & (1<<SPIF)));
  PORTB |= (1<<1); // Frame sync high
}





