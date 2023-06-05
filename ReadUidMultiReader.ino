// Adding some imports.
#include <SPI.h>       // We amy be betrayed by SPI.
#include <MFRC522.h>   // Reading RFID reader.

// Defining the pins layout to adhere to the above. 

#define RST_PIN         53       // Configurable, see typical pin layout above
#define NR_OF_READERS   1        // Defining number of readers. In this case, it's 1 but this may change in the future.
#define SS_1_PIN        48       // Configurable, take a unused pin, only HIGH/LOW required, must be different to SS 2.
//#define SS_2_PIN        49     // Configurable, take a unused pin, only HIGH/LOW required, must be different to SS 1.
// /\ Commenting this one out because we are only taking readings from 1 RFID reader. This may change in the future. 

// Defining SDA pin(s).
byte ssPins[] = {SS_1_PIN}; // If you want to get more than 1 pin, change to {SS_1_PIN, SS_2_PIN};  

// Create MFRC522 instance.
MFRC522 mfrc522[NR_OF_READERS];   

// Initialize.
void setup() {

  // Build connection. Why like this?? Because arduino says so...
  Serial.begin(9600);  // Initialize serial communications with the PC
  while (!Serial);     // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  SPI.begin();         // Init SPI bus

  // Iterate through all readers. (Only useful if more than 1 reader.)
  for (uint8_t reader = 0; reader < NR_OF_READERS; reader++) {
    mfrc522[reader].PCD_Init(ssPins[reader], RST_PIN); // Init each MFRC522 card
    Serial.print(F("Reader "));                        // Commenting output like an idiot.
    Serial.print(reader);
    Serial.print(F(": "));
    mfrc522[reader].PCD_DumpVersionToSerial();
  }
}

// Main loop.
void loop() {

  for (uint8_t reader = 0; reader < NR_OF_READERS; reader++) {
    // Look for new cards

    if (mfrc522[reader].PICC_IsNewCardPresent() && mfrc522[reader].PICC_ReadCardSerial()) {
      Serial.print(F("Reader "));
      Serial.print(reader);
      // Show some details of the PICC (that is: the tag/card)
      Serial.print(F(": Card UID:"));
      dump_byte_array(mfrc522[reader].uid.uidByte, mfrc522[reader].uid.size);
      Serial.println();
      Serial.print(F("PICC type: "));
      MFRC522::PICC_Type piccType = mfrc522[reader].PICC_GetType(mfrc522[reader].uid.sak);
      Serial.println(mfrc522[reader].PICC_GetTypeName(piccType));

      // Halt PICC
      mfrc522[reader].PICC_HaltA();
      // Stop encryption on PCD
      mfrc522[reader].PCD_StopCrypto1();

      //// Counting number of Tags/Cards.              ------------------- THIS IS BASICALLY PSEUDO-CODE BEYOND HERE.    -------------------
      
      // Initialisng variables.
      int mainIndex = 30;         // Maximum number of items = 30.
      int rfidArray[mainIndex];          
      byte arrayIndex = 0;
      foundFlag = 0;
      
      // Verify if RFID already counted.
      for(uint8_t loopIndex = 0; loopIndex < mainIndex; loopIndex++) {

        // Verify if currently selected RFID exists in currently selected array position. 
        if mfrc522[reader].PICC_GetTypeName(piccType) == rfidArray[loopIndex] {

          // Flag as found.
          foundFlag = 1;

        }
            
      }

      // Verify whether RFID has been found in the array.
      if foundFlag == 0 {

        // Iterate array until next free position is found.
        for(uint8_t loopIndex2 = 0; loopIndex2 < mainIndex; loopIndex2++) {

          // Verify array position is not empty.
          if strlen(rfidArray[loopIndex2]) == 0 {

            // Add RFID to array.
            rfidArray[loopIndex2]) = mfrc522[reader].PICC_GetTypeName(piccType);

          }
      
        // Reset foundFlag.
        foundFlag = 0;

      }
      
    } //if (mfrc522[reader].PICC_IsNewC

    // Count RFIDs of items in array.
    int rfidCount = 0;
    for(uint8_t loopIndex3 = 0; loopIndex3 < mainIndex; loopIndex3++) {
    
      // Verify position is not empty.
      if strlen(rfidArray[loopIndex2]) != 0 {

        // Count RFID as present if not empty.
        rfidCount = rfidCount + 1;

      }

    }


    // HERE WILL BE CODE THAT UPDATES VISUAL DISPLAY.
    // this entire code will need to run like 30 times a minute and display the final number of tags found.
    // may require multiple tags.

    Serial.println(F("Iteration Complete - "));
    //Serial.print(millis());
    delay(1000);
  } //for(uint8_t reader
}

/**
 * Helper routine to dump a byte array as hex values to Serial.
 */
void dump_byte_array(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}
