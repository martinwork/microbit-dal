/*
The MIT License (MIT)

Copyright (c) 2016 British Broadcasting Corporation.
This software is provided by Lancaster University by arrangement with the BBC.

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/

#include "mbed.h"
#include "MicroBit.h"
#include "twi_master.h"
#include "nrf_delay.h"

/**
  * Constructor. 
  * Create an instance of i2c
  * @param sda the Pin to be used for SDA
  * @param scl the Pin to be used for SCL
  * Example:
  * @code 
  * MicroBitI2C i2c(MICROBIT_PIN_SDA, MICROBIT_PIN_SCL);
  * @endcode
  * @note this should prevent i2c lockups as well.
  */
MicroBitI2C::MicroBitI2C(PinName sda, PinName scl) : I2C(sda,scl)
{
    this->retries = 0;
}

/**
 * Performs a complete read transaction. The bottom bit of the address is forced to 1 to indicate a read.
 *
 * @address 8-bit I2C slave address [ addr | 1 ]
 * @data Pointer to the byte-array to read data in to
 * @length Number of bytes to read
 * @repeated Repeated start, true - don't send stop at end.
 *
 * @return MICROBIT_OK on success, MICROBIT_I2C_ERROR if an unresolved read failure is detected.
 */
int MicroBitI2C::read(int address, char *data, int length, bool repeated)
{
    int result = I2C::read(address,data,length,repeated);
    
    //0 indicates a success, presume failure
    while(result != 0 && retries < MICROBIT_I2C_MAX_RETRIES)
    {
        _i2c.i2c->EVENTS_ERROR = 0;
        _i2c.i2c->ENABLE       = TWI_ENABLE_ENABLE_Disabled << TWI_ENABLE_ENABLE_Pos; 
        _i2c.i2c->POWER        = 0;
        nrf_delay_us(5);
        _i2c.i2c->POWER        = 1;
        _i2c.i2c->ENABLE       = TWI_ENABLE_ENABLE_Enabled << TWI_ENABLE_ENABLE_Pos;
        twi_master_init_and_clear();
        result = I2C::read(address,data,length,repeated);
        retries++;
    }
    
    if(result != 0)
        return MICROBIT_I2C_ERROR;
   
    retries = 0; 
    return MICROBIT_OK;
}

/**
 * Performs a complete write transaction. The bottom bit of the address is forced to 0 to indicate a write.
 *
 * @address 8-bit I2C slave address [ addr | 0 ]
 * @data Pointer to the byte-arraycontaining the data to write 
 * @length Number of bytes to write
 * @repeated Repeated start, true - don't send stop at end.
 *
 * @return MICROBIT_OK on success, MICROBIT_I2C_ERROR if an unresolved write failure is detected.
 */
int MicroBitI2C::write(int address, const char *data, int length, bool repeated)
{   
    int result = I2C::write(address,data,length,repeated);
    
    //0 indicates a success, presume failure
    while(result != 0 && retries < MICROBIT_I2C_MAX_RETRIES)
    {
        _i2c.i2c->EVENTS_ERROR = 0;
        _i2c.i2c->ENABLE       = TWI_ENABLE_ENABLE_Disabled << TWI_ENABLE_ENABLE_Pos; 
        _i2c.i2c->POWER        = 0;
        nrf_delay_us(5);
        _i2c.i2c->POWER        = 1;
        _i2c.i2c->ENABLE       = TWI_ENABLE_ENABLE_Enabled << TWI_ENABLE_ENABLE_Pos;
        
        twi_master_init_and_clear();
        result = I2C::write(address,data,length,repeated);
        retries++;
    }
    
    if(result != 0)
        return MICROBIT_I2C_ERROR;
        
    retries = 0; 
    return MICROBIT_OK;
}
