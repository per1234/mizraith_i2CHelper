/*********************************************************************** 
  This is a library for the Arduino and acts as
  a basic i2C Helper (utility methods) for i2C devices.
  I noticed a lot of common patterns/methods and 
  decided to centralize them here.
  
  Be sure to read method comments because...well...
  sometimes assumptions are made (like sequential modes)
  
  On most Arduinos, the I2C pins are A4/A5.
 
  AUTHOR: Red Byer (www.redstoyland.com) 
  DATE:   7/29/2013
  Code available on github: https://github.com/mizraith/
  
  BSD license, complete text above must be included in any redistribution
 **********************************************************************/

#include <Wire.h>
#include <avr/pgmspace.h>
#include "i2CHelper.h"

//BASIC stuff
#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif



/********************************************************
* INITIALIZATION ROUTINES
*********************************************************/
/**
 * Takes address of A2A1A0 and ORs that with the MCP23017 address
 * to begin i2C transmission.
 *
 * Starts by setting all ports to input mode (less likely to cause harm).
 */
void i2CHelper::begin(uint8_t addr) {
  i2caddr = addr;
}



/********************************************************
* LOW-LEVEL HELPER METHODS
*********************************************************/
/**
 * HELPER METHOD so we can use the same calls
 */
static inline void i2CHelper::wireSend(uint8_t z) {
#if ARDUINO >= 100
  Wire.write((uint8_t)z);
#else
  Wire.send(z);
#endif
}
/**
 * HELPER METHOD so we can use the same calls
 */
static inline uint8_t i2CHelper::wireReceive() {
#if ARDUINO >= 100
  return Wire.read();
#else
  return Wire.receive();
#endif
}


/********************************************************
* BASIC i2C COMMANDS -- SETTERS
*********************************************************/

/**
 * Given a register (reg) and a bit position (bitpos [0-7])
 * and a bit value (0/1), left shift the bit value into
 * the register.  NOTE:   LSB is bit 0. MSB is bit 7.
 * Assumes user knows what they want and writes the
 * register out regardless--even if the same value as previous.
 */
void i2CHelper::setBit(uint8_t reg, uint8_t bitpos, uint8_t bitval) {
	uint8_t current;
	current = getRegister( reg , 1);
	
	if ( bitval == 0 ) {
	    current = current & ( 0xFF & (0 << bitpos) );
	} else {
	    current = current | (1 << bitpos);
	}
}

/**
 * Set a register of the chip to val
 */
void i2CHelper::setRegister(uint8_t reg, uint8_t, val) {
  Wire.beginTransmission( i2caddr );
  wireSend(reg);	
  wireSend(val);
  Wire.endTransmission();
}

/** 
 * Sets 2 sequential registers starting with address of reg
 * Commonly used for setting XY register pairs
 * ASSUMES sequential operation of the register address pointer
 */
void i2CHelper::set2Registers(uint8_t reg, uint16_t val) {
  Wire.beginTransmission( i2caddr );
  wireSend(reg);	
  wireSend(val & 0xFF);
  wireSend(val >> 8);
  Wire.endTransmission();
}


/********************************************************
* BASIC i2C COMMANDS -- GETTERS
*********************************************************/
/**
 * Given a register (reg) and a bit position (bitpos [0-7])
 * return the bit value (0/1) as the LSB of a uint8_t 
 * NOTE:   LSB is bit 0. MSB is bit 7.
 */
uint8_t i2CHelper::getBit(uint8_t reg, uint8_t bitpos) {
	uint8_t current;
	current = getRegister( reg );
	
	//mask off all the other bits
	current = current & ( 1 << bitpos);
	//put into LSB
	current = current >> bitpos; 
	return current; 
}


/**
 * Read a register of the chip
 */
uint8_t i2CHelper::getRegister(uint8_t reg) {
  uint8_t val;
  
  Wire.beginTransmission( i2caddr );
  wireSend(reg);	
  Wire.endTransmission();
  
  Wire.requestFrom( i2caddr , 1);
  val = wireReceive();
  return val;
}

/** 
 * Gets 2 sequential registers starting with address of reg
 * Commonly used for getting XY register values
 * ASSUMES sequential operation of the register address pointer
 */
uint16_t i2CHelper::get2Registers(uint8_t reg) {
  uint16_t yx = 0;
  uint8_t x;
  
  //set the initial address pointer
  Wire.beginTransmission( i2caddr );
  wireSend(reg);	
  Wire.endTransmission();
  
  //no read the 2 bytes and concatenate
  Wire.requestFrom( i2caddr , 2);
  x = wireReceive();
  yx = wireReceive();
  yx <<= 8;
  yx |= x;

  return yx;
}







