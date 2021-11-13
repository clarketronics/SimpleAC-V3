//-------Module Definitions- Delete the "//" of the modules you are using----------
//#define debug // Unmark this to enable output printing to the serial monitor, this will not continue without a open serial port.
#define using_LED // Unmark this if you want to enable the RGB LED.
#define using_Buzzer // Unmark this if you want to enable the Buzzer.
//#define Sleep // Unmark this if you want to enable sleep mode (conserves battery).


//------Door mode variables------
#define statePin 2 // State input pin, locked / unlocked?
#define unlockDelay 500 // Amount of time to wait before turning the relay back off.
#define lockDelay 500 // Amount of time to wait before turning the relay back off.


//------Ignition mode variables------
#define primeTime 3000 // The amount of time after relay 1 triggers to wait for the bike to prime.
#define startTime 5000 // The amount of time to run the starter motor.


// Don't touch anything below, or do im not your mum.

//------Relay setup------
#define relay1 5 // Relay 1.
#define relay2 6 // Relay 2.
#define relay3 9 // Relay 3.

//------Dip setup------
#define dip1 0 // Dip switch 1.
#define dip2 1 // Dip switch 2.

//------Default input state------
#define defaultState HIGH // Default state for inputs.

//------Sleep variables------
#define wakeupPin 3 // The pin that will wake up the device from sleep, by defualt pin 3. This must be an interupt pin.
#define triggerState HIGH // What state should we look for when sleeping.
#define timeToWait  5000 // The amount of time in millis to wait before sleeping.

//------MP3 variables------
#define Volume 30 // Set the volume of the mp3 played 0~30.
#define startupTrack 001 // Track played when MP3 player boots.
#define authorisedTrack 002 // Track played when an autorised card / implant is scanned.
#define unauthorisedTrack 003 // Track played when an autorised card / implant is scanned.
