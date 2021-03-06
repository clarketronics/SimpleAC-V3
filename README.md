# Not currently working!!!!!

# SimpleAC-V3
A product and idea by Chimpo McDoodle, supported by a community of biohackers and crazy's around the world.

This code is specifically for the V3 if you are using the older version the code is [here](https://github.com/clarketronics/SimpleAC)

## Required libraries:
**[LinkedList](https://github.com/ivanseidel/LinkedList)**: This is used for managing card UID's.    
**[DFRobotDFPlayerMini](https://github.com/DFRobot/DFRobotDFPlayerMini)**: This is for the MP3 player module.    
**[PN532](https://github.com/clarketronics/PN532)**:This is for the PN532 reader module (includes power saving).    
**SoftwareSerial**: This is required by the DFRobot library above and is part of the default install.    

## Definition options:
The code below contains all the options on how you can set up your simpleAC. Removing the // befor a line to let the code know your using that part.

```c++
//-------Module Definitions- Delete the "//" of the modules you are using----------
//#define debug // Unmark this to enable output printing to the serial monitor, this will not continue without a open serial port.
#define using_LED // Unmark this if you want to enable the RGB LED.
#define using_Buzzer // Unmark this if you want to enable the Buzzer.
#define Sleep // Unmark this if you want to enable sleep mode (conserves battery).
```

## Flash codes:
Within the SimpleAC there are several flash / beep codes, some are specific to start up and others are shown during operation that relay information to the user.

### Startup (fault codes):
**Note:** These codes are always the red LED each flash is accompanied by a beep (if enabled).
| LED flash count | Description |
| :---: | :---: |
| 2 | MP3 module failed |
| 3 | Reader module failed |

### Runtime:
**Note:** Each LED flash will be accompanied by a beep (if enabled).
| LED Sequence | Description |
| :---: | :---: |
| RRR | Not authorised |
| RRB | No access cards defined |
| RRG | No master defined, scan new master now |
| RBR | Card to remove was not found |
| RBB | Unlocking failed (car mode) |
| RBG | |
| RGR | Master scanned (clear mode), starts 10 second countdown |
| RGB | |
| RGG | |
| BRR | Locking failed (car mode) |
| BRB | |
| BRG | |
| BBR | Learning mode, Card removed |
| BBB | Clear mode |
| BBG | Learning mode, Card added |
| BGR | |
| BGB | |
| BGG | |
| GRR | |
| GRB | |
| GRG | |
| GBR | Boot up sequence |
| GBB | Authorised, locking (car mode)|
| GBG | Authorised, unlocking (car mode)|
| GGR | |
| GGB | Learning mode (Master scanned) entered or exited |
| GGG | Authorised (accessory and bike mode) |

**Note:** These are the codes if the LED is not enabled.
| Beep Number | Description |
| :---: | :---: |
| 1 | Authorised, unlocking (car mode), Learning mode (Card added) |
| 2 | Authorised, locking (car mode), Learning mode (Card removed) |
| 3 | Error / Unauthorsed |
| 4 | Clear mode, Master scanned (clear mode), starts 10 second countdown |
| 5 | No access cards defined |
| 6 | No master defined, scan new master now |
| 7 | Learning mode (Leaving or Entering) |