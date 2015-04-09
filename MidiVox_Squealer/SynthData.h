/*
    This file is part of the Aduino Piano Squealer.

    Arduino Piano Squealer is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Arduino Piano Squealer is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Arduino Piano Squealer.  If not, see <http://www.gnu.org/licenses/>.

    Copyright 2008, 2009 Marc Nostromo	
*/



#ifndef _SYNTH_DATA_H_
#define _SYNTH_DATA_H_

#include <inttypes.h>
#include <avr/pgmspace.h>



extern const PROGMEM uint8_t boy1Table[] ;
extern const PROGMEM uint8_t sineTable[] ;
extern const PROGMEM uint8_t triTable[] ;
extern const PROGMEM uint8_t casioTable[] ;
extern const PROGMEM uint8_t noizTable[] ;
extern const PROGMEM uint8_t squareTable[] ;

#endif


