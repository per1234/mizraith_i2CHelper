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
  
  BSD license, all text above must be included in any redistribution
 **********************************************************************/

#ifndef _i2CHelper_H_
#define _i2CHelper_H_

// Don't forget the Wire library
class i2CHelper {
  public:
	  void begin(uint8_t addr);        
	  
	  //Utility functions
	  void setBit(uint8_t reg, uint8_t bitpos, uint8_t bitval);
	  void setRegister(uint8_t reg, uint8_t val);
	  void set2Registers(uint8_t reg, uint16_t val);
	  uint8_t getBit(uint8_t reg, uint8_t bitpos)
	  uint8_t getRegister(uint8_t reg);
	  uint16_t get2Registers(uint8_t reg);


  private:
	  uint8_t i2caddr;     //complete i2C address for the device
	  
	  static inline void wireSend(uint8_t z); 
	  static inline void wireReceive(); 
	  
};



#endif
