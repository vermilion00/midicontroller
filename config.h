/* Standard Config */
//What device do you want to control? (QUAD_CORTEX/NEURAL_PLUGIN/etc)
#define DEVICE QUAD_CORTEX
//How many banks do you want to use?
#define BANKS 2
//Which Midi channel should be used? Set to MIDI_CHANNEL_OMNI to broadcast on all channels
#define MIDI_CHANNEL 1
//Default QC gig view mode (PRESET_MODE/SCENE_MODE/STOMP_MODE)
//Leave blank to keep the previously set mode on bank change
#define DEFAULT_MODE SCENE_MODE
//Change the QC gig view mode when a bank is activated
//Defaults to DEFAULT_MODE when uncommented
//Leave blank to avoid setting the QC mode on bank change
#define BANK0_MODE SCENE_MODE
#define BANK1_MODE SCENE_MODE
#define BANK2_MODE PRESET_MODE
#define BANK3_MODE STOMP_MODE
//Do you have a Looper bank? Opening gig view on this bank will open the Looper view instead
//If GIG_LOOPER_VIEW is assigned to an action
#define LOOPER_VIEW_BANK 0

/* LED config */
//TODO: Look up what I need to do to make this function with different LED types
//How many LEDs do you use? Comment out if you don't use any
#define LED_NUM 8
//What kind of LEDs do you use?
#define LED_TYPE WS2812
//What subpixel order do the LEDs use?
#define SUBPIXEL_ORDER GRB
//What order, from left to right, are the LEDs in? Comment out if not needed
#define LED_ORDER {4,5,6,7,0,1,2,3}
//Which pin is being used as the data pin?
#define DATA_PIN 11
//How bright should the LEDs be? (0-255)
#define LED_BRIGHTNESS 30
//Set the bank LED color
//Check https://fastled.io/docs/struct_c_r_g_b.html#aeb40a08b7cb90c1e21bd408261558b99 for color names
#define BANK0_COLOR Red
#define BANK1_COLOR Purple
#define BANK2_COLOR Green
#define BANK3_COLOR Blue
//Set the default LED mode (LED_OFF/LED_ALL/LED_EXCLUSIVE/LED_TOGGLE/LED_MOMENTARY/LED_EXP1/LED_EXP2)
#define DEFAULT_LED_MODE LED_EXCLUSIVE
//Set the bank LED modes (LED_OFF/LED_ALL/LED_EXCLUSIVE/LED_TOGGLE/LED_MOMENTARY/LED_EXP1/LED_EXP2)
//Undefined banks are set to the default LED mode
#define BANK0_LED_MODE LED_TOGGLE
#define BANK1_LED_MODE LED_EXP1
#define BANK2_LED_MODE LED_EXCLUSIVE
#define BANK3_LED_MODE LED_TOGGLE

/* Expression Pedal config */
//Do you have expression pedals connected, and if so, to which pins?
//Comment out if not connected
//It is recommended to pull the Pin to GND with a ~50k resistor
#define EXP1_PIN A0
//#define EXP2_PIN
//Do you want a deadzone for the expression pedals?
#define EXP1_DEADZONE 10
#define EXP1_UPPER_DEADZONE 50
//Set the keys pressed to calibrate the expression pedals
#define EXP1_CALIBRATION_KEY 4
#define EXP2_CALIBRATION_KEY 5
//#define EXP2_DEADZONE 10
//TODO: Check if pedal detection even works
//Set the threshold to a value below what the ADC reads when a pedal is connected for pedal detection to work
#define EXP_THRESHOLD 100
//Allow EXP pedals to be connected during operation
//#define EXP_HOT_PLUG

/* Special Button config */
//Do you have a special button? To which Pin is that button connected?
//Comment it out if you don't use one
//#define SPECIAL_PIN 10
//The hold action is called when the special button is held for longer than HOLD_DURATION
#define SPECIAL_HOLD_ACTION NEXT_BANK
//The press action is called otherwise
#define SPECIAL_PRESS_ACTION TUNER
//The duration to hold the button for to call SPECIAL_HOLD_ACTION in ms
#define HOLD_DURATION 300

/* EEPROM Config */
//Uncomment to complete disable EEPROM access, read and write
//#define NO_EEPROM
//Uncomment to disable EEPROM Write access, will still read the saved data
//#define NO_EEPROM_WRITE