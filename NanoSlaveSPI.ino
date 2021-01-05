/*
 * NanoSlaveSPI.ino
 * Author: Everton A. Gomes
 * Created on January 3rd, 2021
 */

/*
 * This code is based on Nick Gammon's project, available in https://gist.github.com/chrismeyersfsu/3317769
 * The link above was accessed on January 3rd, 2021.
 */
 
// Written by Nick Gammon
// February 2011
/**
 * Send arbitrary number of bits at whatever clock rate (tested at 500 KHZ and 500 HZ).
 * This script will capture the SPI bytes, when a '\n' is recieved it will then output
 * the captured byte stream via the serial.
 */

/*
 * Arduino Nano SPI Pinout:
 * SS - Pin 10
 * MOSI - Pin 11
 * MISO - Pin 12
 * SCK - Pin 13
 */

#include <SPI.h>
#include <string.h>


char buf [100];
char recData;
volatile byte pos;
volatile boolean process_it;

byte send_data[100];


enum adds {nanoID, reg1, reg2, reg3};


typedef struct {
  byte nanoID; //Arduino Nano ID, read only
  byte reg1;
  byte reg2;
  byte reg3;
} Regs;

Regs myRegisters;


void setup (void)
{
  Serial.begin (115200);   // debugging

  // have to send on master in, *slave out*
  pinMode(MISO, OUTPUT);
  pinMode(MOSI, INPUT);
  
  // turn on SPI in slave mode
  SPCR |= _BV(SPE);
  
  // get ready for an interrupt 
  pos = 0;   // buffer empty
  process_it = false;

  // now turn on interrupts
  SPI.attachInterrupt();

  //strcpy(send_data, "ItWasReceived!\n");

  //Registers initialization
  myRegisters.nanoID = 0x39;
  myRegisters.reg1 = 0x24;
  myRegisters.reg2 = 'C';
  myRegisters.reg3 = 0x01;

}  // end of setup


/*
 * SPI Interrupt Service Routine:
 * Reads a byte from SPI and verify whether it's a valid register or not.
 * In positive case, the ISR calls a function that writes data register on SPI line.
 */
ISR(SPI_STC_vect)
{
  recData = SPDR;
  spi_send_register(recData);
}


void spi_send_register(byte add)
{
  //SPI.detachInterrupt();
  switch(add) {
    case nanoID : {
      SPDR = myRegisters.nanoID;
      Serial.println(myRegisters.nanoID);
      break;
    }
    case reg1 : {
      SPDR = myRegisters.reg1;
      Serial.println(myRegisters.reg1);
      break;
    }
    case reg2: {
      SPDR = myRegisters.reg2;
      Serial.println(myRegisters.reg2);
      break;
    }
    case reg3: {
      SPDR = myRegisters.reg3;
      Serial.println(myRegisters.reg3);
      break;
    }
    default : SPDR = 0xFF;
  }
  //SPI.attachInterrupt();
}



void loop()
{
  //Serial.println(reg2);
}



// SPI interrupt routine
//ISR (SPI_STC_vect)
//{
//  byte c = SPDR;  // grab byte from SPI Data Register
//  
//  // add to buffer if room
//  if (pos < sizeof buf)
//    {
//    buf [pos++] = c;
//    
//    // example: newline means time to process buffer
//    if (c == '\n')
//      process_it = true;
//      
//    }  // end of room available
//}  // end of interrupt routine SPI_STC_vect


//void spi_send_data(byte* data)
//{
//  SPI.detachInterrupt();  //disable SPI interrupt in order to send SPI data to master
//  int i = 0;
//  while(data[i] != '\n'){
//    //Serial.write(data[i]);
//    SPDR = data[i];
//    i++;
//  }
//  //Serial.println("");
//  SPI.attachInterrupt();  //enable SPI interrupt again
//  //SPDR = data;
//}

// main loop - wait for flag set in interrupt routine
//void loop (void)
//{
//  //Serial.println(buf);
//  if (process_it)
//  {
//      buf [pos] = 0;  
//      Serial.println (buf);
//      pos = 0;
//      process_it = false;
//
//      if(buf[0] == 'r') {
//        spi_send_data(send_data);
//      }
//
//  }  // end of flag set
//    
//}  // end of loop
