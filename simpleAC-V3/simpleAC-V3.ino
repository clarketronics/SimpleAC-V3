/* 
Required libraries:
LinkedList https://github.com/ivanseidel/LinkedList This is used for managing card UID's.    
DFRobotDFPlayerMini https://github.com/DFRobot/DFRobotDFPlayerMini This is for the MP3 player module.        
PN532 https://github.com/clarketronics/PN532 This is for the PN532 reader module (includes power saving).    
SoftwareSerial This is required by the DFRobot library above and is part of the default install.    

NOTES: 
  * If LED's and Buzzer are enabled the authorised and unautorised blinks / buzzes 
    will be which ever value is greater.

  * If LED's and Buzzer are enabled the interval between blinks / buzzes 
    will be which ever value is greater.

  * Pull LT pin (D2) LOW at startup and you will enter a clear mode
    (this is shown by serial messages, 3 blue LED flashes and beeps) to clear the eeprom.
*/

//-------Global variables and includes-------
#include <Arduino.h>
#include <EEPROM.h>
#include "Unauthorised.h"
#include "Authorised.h"
#include "Config.h"
#include "FlashBeep.h"
#include "Helpers.h"
#include "Data.h"
#include "Learn.h"
#include "Sleep.h"
#include "MP3.h"

// Create instance of custom libraries.
FlashBeep feedback;
Helpers helpers;
Data data;

//------Generic reader wrapper------
PN532_SPI pn532spi(SPI, 10); // Create an SPI instance of the PN532, (Interface, SS pin).
NFCReader nfcReader(pn532spi); // Create the readers class for addressing it directly.


//------MP3 setup------
SoftwareSerial softSerial(8, 7); // Create SoftwareSerial instance (RX pin, TX pin).
DFRobotDFPlayerMini DFPlayer;

// Setup.
void setup() {

  // Setup the reader for use.
  helpers.setupReader(feedback, nfcReader);

  // feedback to used (Buzzer enable, LED enable).
  #if defined(using_Buzzer) && defined(using_LED)
    feedback.begin(true, true);
  #endif

  // feedback to used (Buzzer disabled, LED enable).
  #if !defined(using_Buzzer) && defined(using_LED)
    feedback.begin(false, true);
  #endif

  // feedback to used (Buzzer enabled, LED disabled).
  #if defined(using_Buzzer) && !defined(using_LED)
    feedback.begin(true, false);
  #endif

  // Initiate a serial communication for debug.
  #ifdef debug
    Serial.begin(115200); // Initiate a serial communication at 115200 baud.
    helpers.waitForSerial(5000); // Wait for serial to be open or 5 seconds.
  #endif

  mp3Begin(DFPlayer, softSerial, feedback, data);

  // Setup relays.
  pinMode(relay1, OUTPUT); // Declaring relay 1 as an output.
  digitalWrite(relay1, LOW); // Setting it to OFF.
  pinMode(relay2, OUTPUT); //Declaring relay 2 as output.
  digitalWrite(relay2, LOW); // Setting it to OFF.  
  pinMode(relay3, OUTPUT); //Declaring relay 2 as output.
  digitalWrite(relay3, LOW); // Setting it to OFF.

  // Setup dip's
  pinMode(dip1, INPUT_PULLUP); // Declaring dip1 as an input with pullup enabled.
  pinMode(dip2, INPUT_PULLUP); // Declaring dip2 as an input with pullup enabled.

  // Set state of input pins (input or input with pullup enabled)
  if (defaultState == HIGH){
    #ifdef Sleep
      pinMode(wakeupPin, INPUT_PULLUP);
    #endif

    #ifdef car
      pinMode(statePin, INPUT_PULLUP); // Declare state pin as an input.
    #endif

    pinMode(clearButton, INPUT_PULLUP); // Declaring clear button as an input.
  } else {
    #ifdef Sleep
      pinMode(wakeupPin, INPUT);
    #endif

    #ifdef car
      pinMode(statePin, INPUT); // Declare state pin as an input.
    #endif
    
    pinMode(clearButton, INPUT); // Declaring clear button as an input.
    
  }
  
}

// Main loop.
void loop() {
  // State machine!
  switch (data.state) {
    case startOfDay:
      helpers.onBoot(data, nfcReader, feedback);
      data.state = waitingOnCard;
      #ifdef Sleep
        // Reset Sleep timer.
        data.startMillis = millis();
      #endif
    break;
    case waitingOnCard:
      // Try to read a card.
      if (helpers.readCard(data, nfcReader)) {
        data.state = cardReadSuccessfully;
      } else {
        data.state = waitingOnCard;
      }
    break;
    case cardReadSuccessfully:
      // Check to see if card is 4 byte or not.
      if (helpers.check4Byte(data)) {
        data.state = cardIs4Byte;
      } else {
        data.state = cardIs7Byte;
      }
    break;
    case cardIs4Byte:
      // Check whether the card read is master or even known.
      if (helpers.checkmaster(data.smallCard, data)) {
        data.state = cardIsMaster;
      } else if (helpers.checkCard(fourByte, data)) {
        data.state = cardIs4ByteAccess;
      } else {
        data.state = noMatch;
      }      
    break;
    case cardIs7Byte:
      // Check whether the card read is master or even known.
      if (helpers.checkmaster(data.readCard, data)) {
        data.state = cardIsMaster;
      } else if (helpers.checkCard(sevenByte, data)) {
        data.state = cardIs7ByteAccess;
      } else {
        data.state = noMatch;
      }
    break;
    case cardIsMaster:
      // Output to show entered learing mode.
      #ifdef debug
        Serial.println(F("-------------------"));
        Serial.println(F("Master scanned."));
        Serial.println(F("Scan access card to ADD or REMOVE."));
      #endif 

      #if defined(using_Buzzer) && defined(using_LED)
        // Flash the red LED and beep 3 times.
        feedback.output(SHORT_PERIOD, 2, RGBgreen);
        feedback.output(SHORT_PERIOD, 1, RGBblue);
      #endif

      #if defined(using_Buzzer) && !defined(using_LED)
          // Beep seven times
          feedback.output(SHORT_PERIOD, 7);
      #endif

      // Clear read card arrays.
      helpers.cleanup(data);

      // Enter learning mode (this is blocking).
      learn(data, nfcReader, feedback, helpers);

      #ifdef Sleep
        // Reset Sleep timer.
        data.startMillis = millis();
      #endif

      // Clear data.readCard arrays.
      helpers.cleanup(data);
    break;
    case cardIs4ByteAccess:
    case cardIs7ByteAccess:
      // If a match is found and the device is not already in an enabled state activate.
      authorised(data, feedback, DFPlayer);
      helpers.cleanup(data);
      data.state = waitingOnCard;
    break;
    case noMatch:
      // If a match is not found unauthorised.
      unauthorised(data, feedback, DFPlayer);
      helpers.cleanup(data);
      data.state = waitingOnCard;
    break;
    case goToSleep:
      #ifdef Sleep
        sleepNow(nfcReader, data);
      #endif
    break;
  }

  // Should we Sleep yet?
  #ifdef Sleep
    data.currentMillis = millis();
    if(data.currentMillis >= data.startMillis + timeToWait) {
      data.state = goToSleep;
    }
  #endif
}