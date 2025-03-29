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
'''CHANGE_PRESET(set, preset)'''  
Loads the corresponding preset from **all** setlists according to the parameters. Look up "Incoming MIDI Messages" in the QC manual https://downloads.neuraldsp.com/file/quad-cortex/Quad%20Cortex%20User%20Manual%20v3.1.0.pdf for more info.

'''PRESET(preset)'''  
Loads the corresponding preset from the **current** setlist.

'''CHANGE_BANK(bank)'''  
Changes only the Quad Cortex preset bank (without loading a new preset).

'''SETLIST(set)'''  
Changes only the Quad Cortex setlist (without loading a new preset).

'''EXP_PEDAL_1(val1)'''  
Sends an expression pedal 1 position (0-127) to the QC.

'''EXP_PEDAL_2(val2)'''  
Sends an expression pedal 2 position (0-127) to the QC.

'''FOOTSWITCH(x)'''  
Sends a "footswitch x pressed" command to the QC. Uses numbers 0-7 instead of letters.

'''FOOTSWITCH_R(x)'''  
Sends a "footswitch x released" command to the QC. Uses numbers 0-7 instead of letters.

'''FOOTSWITCH_A'''  
Sends a "footswitch A pressed" command to the QC. A can be substituted for A-H.

'''FOOTSWITCH_A_R'''  
Sends a "footswitch A released" command to the QC. A can be substituted for A-H.

'''SCENE(x)'''  
Sends a scene change command to the QC. Uses numbers 0-7.

'''SCENE_A'''  
Sends a scene change command to the QC. Uses letters A-H.

'''TAP_TEMPO'''  
Sends a tap tempo command to the QC. _NEEDS TESTING_

'''TUNER_OPEN'''  
Opens the Tuner on the QC. _Special action TUNER is recommended._

'''TUNER_CLOSE'''  
Closes the Tuner on the QC. _Special action TUNER is recommended._

'''GIG_VIEW_OPEN'''  
Opens the Gig View on the QC. _Special action GIG_VIEW is recommended._

'''GIG_VIEW_CLOSE'''  
Closes the Gig View on the QC. _Special action GIG_VIEW is recommended._

'''PRESET_MODE'''  
Sets the QC Gig View mode to _PRESET_.

'''SCENE_MODE'''  
Sets the QC Gig View mode to _SCENE_.

'''STOMP_MODE'''  
Sets the QC Gig View mode to _STOMP_.

### QC Looper commands
'''LOOPER_OPEN'''  
Opens the Looper on the QC. _Special action GIG_LOOPER_VIEW is recommended._

'''LOOPER_CLOSE'''  
Closes the Looper on the QC. _Special action GIG_LOOPER_VIEW is recommended._

'''LOOPER_RECORD'''  
Starts/Stops recording the Looper.

'''LOOPER_RECORD_R'''  
If recording is set to momentary mode, you can assign this command to the release action.

'''LOOPER_PLAY'''  
Starts/Stops playing the Recording.

'''LOOPER_PUNCHIN'''  
Starts/Stops the Punch-In function.

'''LOOPER_PUNCHIN_R'''  
If Punch-In is set to momentary mode, you can assign this command to the release action.

'''LOOPER_ONESHOT'''  
Turns the Looper Oneshot function on/off.

'''LOOPER_DUPLICATE'''  
Turns the Looper Duplicate function on/off.

'''LOOPER_REVERSE'''  
Turns the Looper Reverse function on/off.

'''LOOPER_HALFSPEED'''  
Turns the Looper Halfspeed function on/off.

'''LOOPER_UNDO'''  
Triggers the Looper Undo function.

## QC Looper settings
Read the QCmidi.h file to change Looper settings and routing options over Midi.
 

### Special actions
'''TUNER'''  
Opens/Closes the Tuner window when triggered.

'''LOOPER_VIEW'''  
Opens/Closes the Looper window when triggered. A looper block needs to exist in the active preset.

'''GIG_VIEW'''  
Opens/Closes the Gig View when triggered.

'''GIG_LOOPER_VIEW'''  
Opens/Closes the Gig View when triggered. If the current controller bank matches the LOOPER_BANK define, this Opens/Closes the Looper View instead.

'''MOMENTARY_BANK(bank)'''  
Loads the controller bank passed as an argument for the next button press only, then immediately loads the previously active bank.

'''NEXT_BANK'''  
Loads the next bank on the midi controller.

'''PREV_BANK'''  
Loads the previous bank on the midi controller.

'''RESET_BANK'''  
Loads Bank 0 on the midi controller.

'''SET_BANK(bank)'  
Loads the controller bank passed as an argument.

'''CALIBRATE_PEDAL'''  
If only 1 pedal is connected to the controller, start calibration for that pedal. If both pedals are connected, press EXP1_CALIBRATION_KEY to start calibrating pedal 1 or EXP2_CALIBRATION_KEY to start calibrating pedal 2. Pressing any other key cancels the calibration.

'''CALIBRATE_PEDAL_1'''  
Starts calibration for pedal 1.

'''CALIBRATE_PEDAL_2'''  
Starts calibration for pedal 2.

## Expression Pedal functionality
This firmware allows for the use of two expression pedals simultaneously. Each of them can be calibrated separately, and their calibration data is stored on the controller.

### Expression Pedal calibration
If a pedal is connected, calibration can be triggered by holding the EXPx_CALIBRATION_KEY defined in config.h during startup. Otherwise, calibration can started by calling the CALIBRATE_PEDAL function.
During calibration, move your pedal up and down. If you have LEDs enabled, their brightness will change according to the pedal position.
To stop calibrating and to save the data to the Arduinos memory, press any button. This will save to upper and lower limits to the EEPROM, and the next time you use the pedal, the data is recalled from memory, so you don't have to calibrate the pedal each time. If you use multiple pedals, make sure to plug each pedal into the same slot each time.

 


## Configuration Options

MISCELLANIOUS DEFINES
#define NO_CALIBRATION_ANIMATION
#define SPECIAL_BUTTON
#define MIDI_CHANNEL
#define LOOPER_BANK
#define DEFAULT_MODE
#define DEFAULT_COLOR
#define SUBPIXEL_ORDER
#define LED_ORDER
#define LED_BRIGHTNESS
#define EXP_THRESHOLD
#define EXPx_DEADZONE
#define EXPx_UPPER_DEADZONE
#define EXPx_LOWER_DEADZONE
#define HOLD_DURATION
#define MAX_BRIGHTNESS
#define MIN_BRIGHTNESS
#define BANK_COLOR
#define DEFAULT_LED_MODE
#define BANK_LED_MODE
#define SPECIAL_X_Y
#define EXP1_CALIBRATION_KEY
#define EXP2_CALIBRATION_KEY
#define NO_EEPROM
#define NO_EEPROM_WRITE
#define EXP_HOT_PLUG
BANK0_SHORTCUT etc

Special Actions
-TUNER
-LOOPER_VIEW
-GIG_VIEW
-GIG_LOOPER_VIEW
-SET_BANK(bank)
-NEXT_BANK
-PREV_BANK
-RESET_BANK
-MOMENTARY_BANK(bank)
-SWAP_PEDALS
-CALIBRATE_PEDAL
-CALIBRATE_PEDAL_1
-CALIBRATE_PEDAL_2
-CALIBRATE_PEDALS

LED MODES
LED_OFF
LED_ALL
LED_EXCLUSIVE
LED_TOGGLE
LED_MOMENTARY
LED_EXP1
LED_EXP2

STANDARD ACTIONS
CHANGE_PRESET(set, preset)
PRESET(preset)
CHANGE_BANK(bank)
CHANGE_BANK_0
CHANGE_BANK_1
EXP_PEDAL_1(val1)
EXP_PEDAL_2(val2)
SETLIST(set)
FOOTSWITCH(switch)
FOOTSWITCH_R(switch)
SCENE(scene)
TAP_TEMPO //Needs checking
TUNER_OPEN
TUNER_CLOSE
GIG_VIEW_OPEN
GIG_VIEW_CLOSE
PRESET_MODE
SCENE_MODE
STOMP_MODE
IGNORE_DUPLICATE_PC_ON