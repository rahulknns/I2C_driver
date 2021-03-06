#include "I2C_device.h"
#include <Arduino.h>

//Constructor for I2C Device
I2CDevice::I2CDevice(byte address, unsigned int port_no = 0){
    
    setupDevice(address,port_no);
    if(!checkConnection()){
        Serial.println(" error getting Data! Check the address, port number and wiring ");
        delay(1000);
    }
}

//setting up device address_ and i2c port
void I2CDevice::setupDevice(byte address, unsigned int port_no){

    address_ = address;
    port_no_ = port_no;
    return;
}


//Checking the validity of given information
bool I2CDevice::checkConnection(){

    byte error = 0;
    switch (port_no_)
    {
    case 0:
        Wire.begin();
        Wire.beginTransmission(address_);
        error = Wire.endTransmission();
        break;
    #ifdef TEENSY
    case 1:
        Wire1.begin();
        Wire1.beginTransmission(address_);
        error = Wire1.endTransmission();
        break;
    case 2:
        Wire2.begin();
        Wire2.beginTransmission(address_);
        error = Wire2.endTransmission();
        break;
  
    default:
        Serial.println("Invalid Port! Teensy has only 3 ports.");
        break;
    #endif
    }
   
        
    
    return (error == 0) ? true : false;
} 


//Reads bytes and updates in provided location
void I2CDevice::readBytesFromReg (byte regadd, unsigned int count,  byte* const values)
{
    switch (port_no_)
    {
    case 0:
        Wire.beginTransmission(address_);
        Wire.write(regadd);
        Wire.endTransmission(false);
        count = Wire.requestFrom(address_,count);
        for (int i = 0; i < count; i++)
        {
            values[i] = Wire.read();
        }
        break;    
    #ifdef TEENSY
    case 1:
        Wire1.beginTransmission(address_);
        Wire1.write(regadd);
        Wire1.endTransmission(false);
        count = Wire1.requestFrom(address_,count);
        for (int i = 0; i < count; i++)
        {
            values[i] = Wire1.read();
        }
        break;     
 

    case 2:
        Wire2.beginTransmission(address_);
        Wire2.write(regadd);
        Wire2.endTransmission(false);
        count = Wire2.requestFrom(address_,count);
        for (int i = 0; i < count; i++)
        {
            values[i] = Wire2.read();
        }
        break; 
        
        
    default:
        Serial.println("Invalid Port. Teensy has only 3 ports");
        break;
    #endif
    }
    
    
    return;    
}
        
       

    


//Returns a byte read from given register
byte I2CDevice::readByteFromReg(byte regadd)
{
    byte Byte;
    readBytesFromReg(regadd,1,&Byte);
    return Byte;
}


//Returns a byte  with values of required bits read from given register
byte I2CDevice::readBitsFromReg(byte regadd,byte bitmask)
{
    int value;
    value = ( readByteFromReg( regadd) & bitmask );
    return value;

}

 //Reads an array of short ints from a reg location 
 void I2CDevice::readShortIntsFromReg( byte regadd,unsigned int count,short int* values)
{
   byte Bytes[2*count];
   readBytesFromReg(regadd,2*count,Bytes);
   for (int i = 0; i < count; i++)
   {
       values[i] = ((Bytes[ (2*i) ]<<8) + Bytes[(2*i) + 1 ]);
   }

   return;
   
} 

 //Reads an short int and returns it
 short int I2CDevice::readShortIntFromReg( byte regadd)
{
    short int short_int;
    readShortIntsFromReg(regadd,1,&short_int);

    return short_int;
} 




// write an array of bytes to a register location
void I2CDevice::writeBytesToReg(byte regadd, unsigned int count,byte* values)
{
    switch (port_no_)
    {
    case 0:
        Wire.beginTransmission(address_);
        Wire.write(regadd);
        for (int i = 0;i < count; i++){
            Wire.write(values[i]);
        }
        Wire.endTransmission(true);
        break;
    #ifdef TEENSY
    case 1:
        Wire1.beginTransmission(address_);
        Wire1.write(regadd);
        for (int i = 0;i < count; i++){
            Wire1.write(values[i]);
        }
        Wire1.endTransmission(true);
        break;

    case 2:
        Wire2.beginTransmission(address_);
        Wire2.write(regadd);
        for (int i = 0;i < count; i++){
            Wire2.write(values[i]);
        }
        Wire2.endTransmission(true);
        break;
    
    default:
        break;
    #endif
    }

       

} 

// write a byte to a register 
void I2CDevice::writeByteToReg(byte regadd,byte  value)
{
    writeBytesToReg(regadd,1,&value);
    return;
} 



//write bits to a register
void I2CDevice::writeBitsToReg(byte regadd,byte bitmask,byte value)
{
    value = ((readByteFromReg(regadd) & (~bitmask)) | (value & bitmask));
    writeByteToReg(regadd,value);
    return;
}