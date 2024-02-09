/*
  SD card read/write

 This example shows how to read and write data to and from an SD card file
 The circuit:
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13

 created   Nov 2010
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe

 This example code is in the public domain.

 */

#include <SPI.h>

#include "SdFat.h"

#include "Adafruit_SPIFlash.h"

// for flashTransport definition
#include "flash_config.h"

Adafruit_SPIFlash flash(&flashTransport);

// file system object from SdFat
FatVolume fatfs;
File32 myFile;
File32 gFile;

char buf[20];
//String inString = "";

// current gear
int savedGear;
int currentGear = 3;

inline const char * typeStr (int   var) { return "int"; }
inline const char * typeStr (long  var) { return "long"; }
inline const char * typeStr (float var) { return "float"; }
inline const char * typeStr (const char *var) { return "char"; }

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    delay(10); // wait for serial port to connect. Needed for native USB port only
  }

  Init_Flash_Mem();

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  //myFile = fatfs.open("current_gear.txt", O_READ | O_WRITE | O_CREAT);

  Read_From_Flash();

  delay(5000);
}

void loop() {
  /*
  // nothing happens after setup
  myFile = fatfs.open("current_gear.txt", O_READ | O_WRITE | O_TRUNC);
  //String c = String(myGear);
  Save_To_Flash(myGear);
  //Serial.println(myGear);
  delay(1000);
  myFile.close();
*/
}

void Init_Flash_Mem(){
  Serial.println("Initializing Filesystem on external flash...");

  // Init external flash
  flash.begin();

  // Open file system on the flash
  if (!fatfs.begin(&flash)) {
    Serial.println("Error: filesystem is not existed. Please try SdFat_format "
                   "example to make one.");
    while (1) {
      yield();
      delay(1);
    }
  } else {
    return;
  }

}

void Read_From_Flash() {

  Serial.println("Reading current_gear.txt...");
  gFile = fatfs.open("current_gear.txt", O_READ);
  if (gFile.available()) {
    char ch = gFile.read();    // read the first character
    //gFile.read(buf, rlen - 1); // read the remaining to buffer
    gFile.close();

    if (isDigit(ch)) {
      savedGear = ch - '0';
      Serial.print("savedGear = ");
      Serial.println(savedGear);
    } else {
      Serial.println("Saved Gear isnt a Digit...");
      Serial.println("ch = ");
      Serial.print(ch); Serial.println("");
    }

    if (currentGear != savedGear) {
      Serial.println("current gear doesnt match saved gear: do something!");
      Shift_To_Gear(currentGear);
    } else {
      Serial.println("current gear matches saved gear: do nothing.");
    }

    
  } else {
    Serial.println(F("Flash Mem: error on opening file"));
  }

}

void Save_To_Flash(int theGear){
  myFile = fatfs.open("current_gear.txt", FILE_WRITE | O_TRUNC);

  // if the file opened okay, write to it:
  String newGear = "";
  newGear += theGear;

  if (myFile) {
    Serial.print("updating current_gear.txt...");
    myFile.println(newGear);
    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening current_gear.txt");
  }
}

void Shift_To_Gear(int gotoGear) {
  Serial.print("Shifitng into Gear: ");
  Serial.println(gotoGear);
  // add function to aactually shift the gear to servo position
  Serial.println(" ");
  currentGear = gotoGear;
  // now save current gear into flash memory
  Save_To_Flash(gotoGear);

}
