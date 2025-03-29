# An easily configurable Arduino Midicontroller Firmware for the Quad Cortex
This firmware for the Arduino Uno is designed to be configurable for up to 16 switches, 16 banks, 16 LEDs and 2 Expression Pedal inputs.
It supports every midi command that the Quad Cortex understands, and midi commands for other receivers should be easily implementable.
The QC midi commands can be found here: https://downloads.neuraldsp.com/file/quad-cortex/Quad%20Cortex%20User%20Manual%20v3.1.0.pdf  
Every command can be mapped to any switch on any bank.

## Special Features
Additional Features include:  
-Pressing multiple buttons simultaneously for even more actions (also remappable)  
-Momentary banks  
-Automatic Expression Pedal detection  
-Expression Pedal Deadzones  
-Expression Pedal calibration  
-Per Bank led color and mode assignment

## Command explanations
### QC Commands
```CHANGE_PRESET(set, preset)```  
Loads the corresponding preset from **all** setlists according to the parameters. Look up "Incoming MIDI Messages" in the QC manual https://downloads.neuraldsp.com/file/quad-cortex/Quad%20Cortex%20User%20Manual%20v3.1.0.pdf for more info.

```PRESET(preset)```  
Loads the corresponding preset from the **current** setlist.

```CHANGE_BANK(bank)```  
Changes only the Quad Cortex preset bank (without loading a new preset).

```SETLIST(set)```  
Changes only the Quad Cortex setlist (without loading a new preset).

```EXP_PEDAL_1(val1)```  
Sends an expression pedal 1 position (0-127) to the QC.

```EXP_PEDAL_2(val2)```  
Sends an expression pedal 2 position (0-127) to the QC.

```FOOTSWITCH(x)```  
Sends a "footswitch x pressed" command to the QC. Uses numbers 0-7 instead of letters.

```FOOTSWITCH_R(x)```  
Sends a "footswitch x released" command to the QC. Uses numbers 0-7 instead of letters.

```FOOTSWITCH_A```  
Sends a "footswitch A pressed" command to the QC. A can be substituted for A-H.

```FOOTSWITCH_A_R```  
Sends a "footswitch A released" command to the QC. A can be substituted for A-H.

```SCENE(x)```  
Sends a scene change command to the QC. Uses numbers 0-7.

```SCENE_A```  
Sends a scene change command to the QC. Uses letters A-H.

```TAP_TEMPO```  
Sends a tap tempo command to the QC. _NEEDS TESTING_

```TUNER_OPEN```  
Opens the Tuner on the QC. _Special action TUNER is recommended._

```TUNER_CLOSE```  
Closes the Tuner on the QC. _Special action TUNER is recommended._

```GIG_VIEW_OPEN```  
Opens the Gig View on the QC. _Special action GIG_VIEW is recommended._

```GIG_VIEW_CLOSE```  
Closes the Gig View on the QC. _Special action GIG_VIEW is recommended._

```PRESET_MODE```  
Sets the QC Gig View mode to _PRESET_.

```SCENE_MODE```  
Sets the QC Gig View mode to _SCENE_.

```STOMP_MODE```  
Sets the QC Gig View mode to _STOMP_.

### QC Looper commands
```LOOPER_OPEN```  
Opens the Looper on the QC. _Special action GIG_LOOPER_VIEW is recommended._

```LOOPER_CLOSE```  
Closes the Looper on the QC. _Special action GIG_LOOPER_VIEW is recommended._

```LOOPER_RECORD```  
Starts/Stops recording the Looper.

```LOOPER_RECORD_R```  
If recording is set to momentary mode, you can assign this command to the release action.

```LOOPER_PLAY```  
Starts/Stops playing the Recording.

```LOOPER_PUNCHIN```  
Starts/Stops the Punch-In function.

```LOOPER_PUNCHIN_R```  
If Punch-In is set to momentary mode, you can assign this command to the release action.

```LOOPER_ONESHOT```  
Turns the Looper Oneshot function on/off.

```LOOPER_DUPLICATE```  
Turns the Looper Duplicate function on/off.

```LOOPER_REVERSE```  
Turns the Looper Reverse function on/off.

```LOOPER_HALFSPEED```  
Turns the Looper Halfspeed function on/off.

```LOOPER_UNDO```  
Triggers the Looper Undo function.

### QC Looper settings
Read the QCmidi.h file to change Looper settings and routing options over Midi.
 

## Special commands
***Special commands are only tested when called by a combo action. Assigning them to a standard footswitch action might not work.***  
```TUNER```  
Opens/Closes the Tuner window when triggered.

```LOOPER_VIEW```  
Opens/Closes the Looper window when triggered. A looper block needs to exist in the active preset.

```GIG_VIEW```  
Opens/Closes the Gig View when triggered.

```GIG_LOOPER_VIEW```  
Opens/Closes the Gig View when triggered. If the current controller bank matches the LOOPER_BANK define, this Opens/Closes the Looper View instead.

```MOMENTARY_BANK(bank)```  
Loads the controller bank passed as an argument for the next button press only, then immediately loads the previously active bank.

```NEXT_BANK```  
Loads the next bank on the midi controller.

```PREV_BANK```  
Loads the previous bank on the midi controller.

```RESET_BANK```  
Loads Bank 0 on the midi controller.

```SET_BANK(bank)```  
Loads the controller bank passed as an argument.

```CALIBRATE_PEDAL```  
If only 1 pedal is connected to the controller, start calibration for that pedal. If both pedals are connected, press EXP1_CALIBRATION_KEY to start calibrating pedal 1 or EXP2_CALIBRATION_KEY to start calibrating pedal 2. Pressing any other key cancels the calibration.

```CALIBRATE_PEDAL_1```  
Starts calibration for pedal 1.

```CALIBRATE_PEDAL_2```  
Starts calibration for pedal 2.

## Expression Pedal functionality
This firmware allows for the use of two expression pedals simultaneously. Each of them can be calibrated separately, and their calibration data is stored on the controller.

### Expression Pedal calibration
If a pedal is connected, calibration can be triggered by holding the EXPx_CALIBRATION_KEY defined in config.h during startup. Otherwise, calibration can started by calling the CALIBRATE_PEDAL function.
During calibration, move your pedal up and down. If you have LEDs enabled, their brightness will change according to the pedal position.
To stop calibrating and to save the data to the Arduinos memory, press any button. This will save to upper and lower limits to the EEPROM, and the next time you use the pedal, the data is recalled from memory, so you don't have to calibrate the pedal each time. If you use multiple pedals, make sure to plug each pedal into the same slot each time.

## Footswitch Configuration
Every following action is defined in _footswitches.h_.
Blank templates for 4x2, 5x2, 6x2 and 4x3 layouts can be found in the templates folder. Banks that you don't need can simply be deleted from your file.
### Action assignments
The firmware supports up to 16 banks (0-15) and 16 switches (0-15). An action is assigned to each switch in each bank. The assignment looks like this:  
```#define BANK0BTN0P SCENE_A;```
This means that if you press switch 0 while on bank 0, the controller will tell the QC to switch to Scene A. Each combination of bank and switch has a #define like this.  
There are also release actions that are called when you release the switch:  
```#define BANK3BTN7R TUNER_CLOSE;```
The "R" at the end means that releasing switch 7 while bank 3 (remember, 0-indexed) is active will close the Tuner window.  
This allows you to use double bindings, e.g. to open the Gig View when a button is held and close it when it's released, you'd use this binding:  
```#define BANK2BTN7P GIG_VIEW_OPEN;```
```#define BANK2BTN7R GIG_VIEW_CLOSE;```
However, release actions are only called if the footswitch mode for that switch is set to 0 (standard).

### Combo actions
You can also set the footswitch mode to 1 on a per-switch basis, allowing you to use this switch in Combos. A combo is any 2 neighboring switches that are set to mode 1. Pressing these together triggers a separate action, assigned using the following define in _footswitches.h_:  
```#define SPECIAL_6_7 TUNER;```
This means that pressing switches 6 and 7 together doesn't call their individual actions, but instead opens/closes the Tuner. These combo actions exist for every combination of neighboring switches, so that you can easily use additional actions with one foot. 
***To use combo actions, the switch mode needs to be to 1. This calls the normal press action on release instead and disables the release action.***  

### Basic configuration
```#define FOOTSWITCH_NUM 8```
Set this to the amount of footswitches you have.  
```#define FOOTSWITCH_PINS 2,3,4,5,6,7,8,9```
Set the order of the switches here. The numbers correspond to the digital Pins that each switch is connected to. The leftmost number is switch 0.  
```#define DEBOUNCE 20```
Set the debounce time in milliseconds. It's recommended to use the smallest number that doesn't result in double inputs.  
```#define FOOTSWITCH_MODES 0,0,1,1,0,0,1,1```
Set the footswitch modes for each switch individually.  
***To use combo actions, the switch mode needs to be to 1. This calls the normal press action on release instead and disables the release action. If you don't use a switch with combo actions, set the mode to 0.***   

### Bank Shortcuts
There exist bank shortcuts for Looper, Preset, Scene and Stomp banks.
If e.g. your bank 1 just needs the standard scene switching actions, you can use this to skip setting each action individually:  
```#define BANK1_SHORTCUT SCENE_BANK```  
This is equivalent to the following assignments:  
```#define BANK1BTN0P SCENE_A;
#define BANK1BTN0R
#define BANK1BTN1P SCENE_B;
#define BANK1BTN1R
#define BANK1BTN2P SCENE_C;
#define BANK1BTN2R
#define BANK1BTN3P SCENE_D;
#define BANK1BTN3R
#define BANK1BTN4P SCENE_E;
#define BANK1BTN4R
#define BANK1BTN5P SCENE_F;
#define BANK1BTN5R
#define BANK1BTN6P SCENE_G;
#define BANK1BTN6R
#define BANK1BTN7P SCENE_H;
#define BANK1BTN7R```  

The LOOPER_BANK shortcut uses the following assignments:  
```#define BANKxBTN0P LOOPER_DUPLICATE;
#define BANKxBTN0R
#define BANKxBTN1P LOOPER_ONESHOT;
#define BANKxBTN1R
#define BANKxBTN2P LOOPER_HALFSPEED;
#define BANKxBTN2R
#define BANKxBTN3P LOOPER_PUNCHIN;
#define BANKxBTN3R LOOPER_PUNCHIN_R;
#define BANKxBTN4P LOOPER_RECORD;
#define BANKxBTN4R LOOPER_RECORD_R;
#define BANKxBTN5P LOOPER_PLAY;
#define BANKxBTN5R
#define BANKxBTN6P LOOPER_REVERSE;
#define BANKxBTN6R
#define BANKxTN7P LOOPER_UNDO;
#define BANKxBTN7R```

If a bank is dedicated to Looper controls, you can also set 
```#define LOOPER_VIEW_BANK x```
When the controller is on this bank, using the GIG_LOOPER_VIEW command will open the Looper view instead of the Gig view.

## Controller Configuration
All following configuration options are defined in the _config.h_ file.
### Basics
```#define BANKS 2```
Sets the amount of Banks that the NEXT_BANK and PREV_BANK actions loop through.  
It is possible to have additional banks that are accessible through the SET_BANK and MOMENTARY_BANK commands.

```#define MIDI_CHANNEL 1```
Sets the output MIDI_CHANNEL, set to MIDI_CHANNEL_OMNI to broadcast on all channels.  

```#define DEFAULT_MODE SCENE_MODE```
Sets the default QC Gig View mode. (PRESET_MODE/SCENE_MODE/STOMP_MODE)  

```#define BANKx_MODE PRESET_MODE```
Sets the QC Gig View mode on a per-bank basis.

```#define LOOPER_VIEW_BANK 0```
If the controller is on this bank, the GIG_LOOPER_VIEW command opens the Looper view.  

### LED configuration
```#define LED_NUM 8```
How many LEDs you're using. Comment out if you don't use any.  

```#define LED_TYPE WS2812```
***NEEDS TO BE TESTED, PROBABLY DOESN'T WORK***  

```#define SUBPIXEL_ORDER RGB```
Sets the LED subpixel order. If the colors aren't working properly, try changing the order of R,G,B.  

```#define LED_ORDER {4,5,6,7,0,1,2,3}```
If the order of LEDs doesn't match the order of footswitches, you can change this here.  

```#define DATA_PIN 11```
Which pin is used as the data pin for your LED strip?  

```#define LED_BRIGHTNESS 30```
How bright should the LEDs be? (0-255)

```#define BANKx_COLOR Red```
Set the LED color per bank. Check https://fastled.io/docs/struct_c_r_g_b.html#aeb40a08b7cb90c1e21bd408261558b99 for color names.  

```#define BANKx_LED_MODE LED_TOGGLE```
Sets the LED mode on per bank. The options are:  
```LED_EXCLUSIVE``` (Only one LED can be active at a time, e.g. for switching Scenes)  
```LED_TOGGLE``` (LEDs are turned on/off if the corresponding switch is pressed)  
```LED_MOMENTARY``` (LEDs are turned on when the corresponding switch is pressed, and off when released)  
```LED_EXP1``` (All LEDs are on, brightness changes according to pedal 1 position)  
```LED_EXP2``` (All LEDs are on, brightness changes according to pedal 2 position)  
```LED_ALL``` (All LEDs are on)  
```LED_OFF``` (All LEDs are off)

```#define EXP1_PIN A0```
Sets the analog pin to which the Pedal 1 input is connected.  
_It is recommended to pull the pin to GND with a ~50k resistor, to make sure pedal detection works reliably_.  

```#define EXP1_CALIBRATION_KEY 4```
This key is used to start pedal 1 calibration, check the pedal calibration section for info.  

```#define EXP1_UPPER_DEADZONE 50  
#define EXP1_LOWER_DEADZONE 10```
Set Deadzones for the upper and lower limits of the pedal.  

```#define EXP_THRESHOLD 100```
If the adc reads a value lower than this at startup, the controller assumes that no pedal is connected and skips related code. Since the pin should be pulled low, the reading should be around 0 if no pedal is connected.  

### Miscellaneous Configuration
```#define NO_EEPROM```
If this option is defined in config.h, the pedal values aren't written and read to/from the EEPROM.  

```#define NO_EEPROM_WRITE```
If this option is defined in config.h, write access to the EEPROM is blocked, but stored values can still be read.  

```#define SPECIAL_PIN 10```
Allows for a separate button on your controller. If you don't need this, comment it out.  

```#define SPECIAL_PRESS_ACTION TUNER```
Set the command to be executed when the special button is pressed.  

```#define SPECIAL_HOLD_ACTION CALIBRATE_PEDALS```
Set the command to be executed when the special button is held.  

```#define HOLD_DURATION 300```
How long the special button needs to be held to execute the hold action.

```#define MIN_BRIGHTNESS 10```
Set the minimum brightness value for the LED_EXPx modes. (0-255)    

```#define MAX_BRIGHTNESS 250```
Set the maximum brightness value for the LED_EXPx modes. (0-255)  