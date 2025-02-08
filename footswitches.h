//How many footswitches do you use? (4-16)
#define FOOTSWITCH_NUM 8

//Which Pins are the footswitches connected to, in order?
#define FOOTSWITCH_PINS 5,6,7,8,9,2,3,4

//What are the modes of the footswitches?
//0 = momentary, 1 = special
    /* Footswitch number 0,1,2,3,4,5,6,7 */
#define FOOTSWITCH_MODES 0,0,1,1,0,0,1,1

//Set the special actions, leave empty if not needed
//The corresponding switches need to be set to SPECIAL mode
#define SPECIAL_2_3 NEXT_BANK
#define SPECIAL_2_6 CALIBRATE_PEDALS
#define SPECIAL_3_7 TUNER
#define SPECIAL_6_7 GIG_LOOPER_VIEW

//How long should the switches be debounced? Set it as low as possible without getting double inputs
#define DEBOUNCE 50

//Set the actions per bank, empty actions will have no effect
//The option ending in "P" is called when the switch is pressed
//The option ending in "R" is called when the switch is released (only in momentary mode)
//Switches set to "special" mode only support the press options
//Use the _R action option if the action is set to momentary on the QC
//You can also call the press option again on release to emulate momentary behavior on actions
//That only support latching modes, like Playing the looper
#define BANK0BTN0P LOOPER_DUPLICATE;
#define BANK0BTN0R
#define BANK0BTN1P LOOPER_PUNCHIN;
#define BANK0BTN1R LOOPER_PUNCHIN_R;
#define BANK0BTN2P LOOPER_HALFSPEED;
#define BANK0BTN2R
#define BANK0BTN3P LOOPER_ONESHOT;
#define BANK0BTN3R
#define BANK0BTN4P LOOPER_RECORD;
#define BANK0BTN4R LOOPER_RECORD_R;
#define BANK0BTN5P LOOPER_PLAY;
#define BANK0BTN5R
#define BANK0BTN6P LOOPER_REVERSE;
#define BANK0BTN6R
#define BANK0BTN7P LOOPER_UNDO;
#define BANK0BTN7R

//Set the actions in bank 1
#define BANK1BTN0P SCENE_A;
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
#define BANK1BTN7R

//Set the actions in bank 2
#define BANK2BTN0P PRESET(1);
#define BANK2BTN0R
#define BANK2BTN1P PRESET(2);
#define BANK2BTN1R
#define BANK2BTN2P PRESET(3);
#define BANK2BTN2R
#define BANK2BTN3P PRESET(4);
#define BANK2BTN3R
#define BANK2BTN4P PRESET(5);
#define BANK2BTN4R
#define BANK2BTN5P PRESET(6);
#define BANK2BTN5R
#define BANK2BTN6P PRESET(7);
#define BANK2BTN6R
#define BANK2BTN7P PRESET(8);
#define BANK2BTN7R

//Set the actions in bank 3
#define BANK3BTN0P FOOTSWITCH_A;
#define BANK3BTN0R
#define BANK3BTN1P FOOTSWITCH_B;
#define BANK3BTN1R
#define BANK3BTN2P FOOTSWITCH_C;
#define BANK3BTN2R
#define BANK3BTN3P FOOTSWITCH_D;
#define BANK3BTN3R
#define BANK3BTN4P FOOTSWITCH_E;
#define BANK3BTN4R
#define BANK3BTN5P FOOTSWITCH_F;
#define BANK3BTN5R
#define BANK3BTN6P FOOTSWITCH_G;
#define BANK3BTN6R
#define BANK3BTN7P FOOTSWITCH_H;
#define BANK3BTN7R
