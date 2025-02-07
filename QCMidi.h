/* Standard Functions */
/* Commands ending in _R are the "switch released" commands */
//Choose preset across all setlists, bank is calculated
#define CHANGE_PRESET(set, preset) \
 MIDI.sendControlChange(0, preset / 128, MIDI_CHANNEL); \
 MIDI.sendControlChange(32, set, MIDI_CHANNEL); \
 MIDI.sendProgramChange(preset - 1, MIDI_CHANNEL
 
//Choose preset within current setlist, bank is calculated
#define PRESET(preset) \
 MIDI.sendControlChange(0, preset / 128, MIDI_CHANNEL); \
 MIDI.sendProgramChange(preset - 1, MIDI_CHANNEL)

#define CHANGE_BANK(bank) MIDI.sendControlChange(0, bank, MIDI_CHANNEL)
#define CHANGE_BANK_0 MIDI.sendControlChange(0, 0, MIDI_CHANNEL)
#define CHANGE_BANK_1 MIDI.sendControlChange(0, 1, MIDI_CHANNEL) 
//The expression pedal value ranges from 0-127
#define EXP_PEDAL_1(val1) MIDI.sendControlChange(1, val1, MIDI_CHANNEL)
#define EXP_PEDAL_2(val2) MIDI.sendControlChange(2, val2, MIDI_CHANNEL)

//Use 0-indexed setlist (0 is factory presets)
#define SETLIST(set) MIDI.sendControlChange(32, set, MIDI_CHANNEL)
#define SETLIST_0 MIDI.sendControlChange(32, 0, MIDI_CHANNEL)
#define SETLIST_1 MIDI.sendControlChange(32, 1, MIDI_CHANNEL)
#define SETLIST_2 MIDI.sendControlChange(32, 2, MIDI_CHANNEL)
#define SETLIST_3 MIDI.sendControlChange(32, 3, MIDI_CHANNEL)
#define SETLIST_4 MIDI.sendControlChange(32, 4, MIDI_CHANNEL)
#define SETLIST_5 MIDI.sendControlChange(32, 5, MIDI_CHANNEL)
#define SETLIST_6 MIDI.sendControlChange(32, 6, MIDI_CHANNEL)
#define SETLIST_7 MIDI.sendControlChange(32, 7, MIDI_CHANNEL)
#define SETLIST_8 MIDI.sendControlChange(32, 8, MIDI_CHANNEL)
#define SETLIST_9 MIDI.sendControlChange(32, 9, MIDI_CHANNEL)
#define SETLIST_10 MIDI.sendControlChange(32, 10, MIDI_CHANNEL)
#define SETLIST_11 MIDI.sendControlChange(32, 11, MIDI_CHANNEL)
#define SETLIST_12 MIDI.sendControlChange(32, 12, MIDI_CHANNEL)

//Use 0-indexed footswitch number (0 is top left)
#define FOOTSWITCH(footswitch) MIDI.sendControlChange(35 + footswitch, 127, MIDI_CHANNEL)
#define FOOTSWITCH_R(footswitch) MIDI.sendControlChange(35 + footswitch, 0, MIDI_CHANNEL)
#define FOOTSWITCH_A MIDI.sendControlChange(35, 127, MIDI_CHANNEL)
#define FOOTSWITCH_A_R MIDI.sendControlChange(35, 0, MIDI_CHANNEL)
#define FOOTSWITCH_B MIDI.sendControlChange(36, 127, MIDI_CHANNEL)
#define FOOTSWITCH_B_R MIDI.sendControlChange(36, 0, MIDI_CHANNEL)
#define FOOTSWITCH_C MIDI.sendControlChange(37, 127, MIDI_CHANNEL)
#define FOOTSWITCH_C_R MIDI.sendControlChange(37, 0, MIDI_CHANNEL)
#define FOOTSWITCH_D MIDI.sendControlChange(38, 127, MIDI_CHANNEL)
#define FOOTSWITCH_D_R MIDI.sendControlChange(38, 0, MIDI_CHANNEL)
#define FOOTSWITCH_E MIDI.sendControlChange(39, 127, MIDI_CHANNEL)
#define FOOTSWITCH_E_R MIDI.sendControlChange(39, 0, MIDI_CHANNEL)
#define FOOTSWITCH_F MIDI.sendControlChange(40, 127, MIDI_CHANNEL)
#define FOOTSWITCH_F_R MIDI.sendControlChange(40, 0, MIDI_CHANNEL)
#define FOOTSWITCH_G MIDI.sendControlChange(41, 127, MIDI_CHANNEL)
#define FOOTSWITCH_G_R MIDI.sendControlChange(41, 0, MIDI_CHANNEL)
#define FOOTSWITCH_H MIDI.sendControlChange(42, 127, MIDI_CHANNEL)
#define FOOTSWITCH_H_R MIDI.sendControlChange(42, 0, MIDI_CHANNEL)

//Use 0-indexed scene number (0 is top left)
#define SCENE(scene) MIDI.sendControlChange(43, scene, MIDI_CHANNEL)
#define SCENE_A MIDI.sendControlChange(43, 0, MIDI_CHANNEL)
#define SCENE_B MIDI.sendControlChange(43, 1, MIDI_CHANNEL)
#define SCENE_C MIDI.sendControlChange(43, 2, MIDI_CHANNEL)
#define SCENE_D MIDI.sendControlChange(43, 3, MIDI_CHANNEL)
#define SCENE_E MIDI.sendControlChange(43, 4, MIDI_CHANNEL)
#define SCENE_F MIDI.sendControlChange(43, 5, MIDI_CHANNEL)
#define SCENE_G MIDI.sendControlChange(43, 6, MIDI_CHANNEL)
#define SCENE_H MIDI.sendControlChange(43, 7, MIDI_CHANNEL)

#define TAP_TEMPO MIDI.sendControlChange(44, 127, MIDI_CHANNEL)
//Does this work? Probably not
#define BPM_TEMPO(bpm) MIDI.sendControlChange(44, bpm, MIDI_CHANNEL)

#define TUNER_OPEN MIDI.sendControlChange(45, 127, MIDI_CHANNEL)
#define TUNER_CLOSE MIDI.sendControlChange(45, 0, MIDI_CHANNEL)
#define GIG_VIEW_OPEN MIDI.sendControlChange(46, 127, MIDI_CHANNEL)
#define GIG_VIEW_CLOSE MIDI.sendControlChange(46, 0, MIDI_CHANNEL)
#define PRESET_MODE MIDI.sendControlChange(47, 0, MIDI_CHANNEL)
#define SCENE_MODE MIDI.sendControlChange(47, 1, MIDI_CHANNEL)
#define STOMP_MODE MIDI.sendControlChange(47, 2, MIDI_CHANNEL)
#define IGNORE_DUPLICATE_PC_ON MIDI.sendControlChange(62, 127, MIDI_CHANNEL)
#define IGNORE_DUPLICATE_PC_OFF MIDI.sendControlChange(62, 0, MIDI_CHANNEL)

/* Looper actions */
#define LOOPER_OPEN MIDI.sendControlChange(48, 0, MIDI_CHANNEL)
#define LOOPER_CLOSE MIDI.sendControlChange(48, 127, MIDI_CHANNEL)
#define LOOPER_DUPLICATE MIDI.sendControlChange(49, 127, MIDI_CHANNEL)
#define LOOPER_ONESHOT MIDI.sendControlChange(50, 127, MIDI_CHANNEL)
#define LOOPER_HALFSPEED MIDI.sendControlChange(51, 127, MIDI_CHANNEL)
#define LOOPER_PUNCHIN MIDI.sendControlChange(52, 127, MIDI_CHANNEL)
#define LOOPER_PUNCHIN_R MIDI.sendControlChange(52, 0, MIDI_CHANNEL)
#define LOOPER_RECORD MIDI.sendControlChange(53, 127, MIDI_CHANNEL)
#define LOOPER_RECORD_R MIDI.sendControlChange(53, 0, MIDI_CHANNEL)
#define LOOPER_PLAY MIDI.sendControlChange(54, 127, MIDI_CHANNEL)
#define LOOPER_PLAY_R MIDI.sendControlChange(54, 127, MIDI_CHANNEL)
#define LOOPER_REVERSE MIDI.sendControlChange(55, 127, MIDI_CHANNEL)
#define LOOPER_UNDO MIDI.sendControlChange(56, 127, MIDI_CHANNEL)

/* Looper settings */
#define LOOPER_DUPLICATE_PARAM_FREE MIDI.sendControlChange(57, 0, MIDI_CHANNEL)
#define LOOPER_DUPLICATE_PARAM_SYNC MIDI.sendControlChange(57, 1, MIDI_CHANNEL)
#define LOOPER_QUANTIZE_PARAM(quant) MIDI.sendControlChange(58, quant, MIDI_CHANNEL)
#define LOOPER_QUANTIZE_PARAM_0 MIDI.sendControlChange(58, 0, MIDI_CHANNEL)
#define LOOPER_QUANTIZE_PARAM_1 MIDI.sendControlChange(58, 1, MIDI_CHANNEL)
#define LOOPER_QUANTIZE_PARAM_2 MIDI.sendControlChange(58, 2, MIDI_CHANNEL)
#define LOOPER_QUANTIZE_PARAM_3 MIDI.sendControlChange(58, 3, MIDI_CHANNEL)
#define LOOPER_QUANTIZE_PARAM_4 MIDI.sendControlChange(58, 4, MIDI_CHANNEL)
#define LOOPER_QUANTIZE_PARAM_5 MIDI.sendControlChange(58, 5, MIDI_CHANNEL)
#define LOOPER_QUANTIZE_PARAM_6 MIDI.sendControlChange(58, 6, MIDI_CHANNEL)
#define LOOPER_QUANTIZE_PARAM_7 MIDI.sendControlChange(58, 7, MIDI_CHANNEL)
#define LOOPER_QUANTIZE_PARAM_8 MIDI.sendControlChange(58, 8, MIDI_CHANNEL)
#define LOOPER_QUANTIZE_PARAM_16 MIDI.sendControlChange(58, 9, MIDI_CHANNEL)
#define LOOPER_CLOCK_ON MIDI.sendControlChange(59, 0, MIDI_CHANNEL)
#define LOOPER_CLOCK_OFF MIDI.sendControlChange(59, 1, MIDI_CHANNEL)
#define LOOPER_PERFORM_MODE MIDI.sendControlChange(60, 0, MIDI_CHANNEL)
#define LOOPER_PARAMS_MODE MIDI.sendControlChange(60, 1, MIDI_CHANNEL)
#define LOOPER_ROUTING(route) MIDI.sendControlChange(61, route, MIDI_CHANNEL)
#define LOOPER_ROUTING_GRID MIDI.sendControlChange(61, 0, MIDI_CHANNEL)
#define LOOPER_ROUTING_IN1 MIDI.sendControlChange(61, 1, MIDI_CHANNEL)
#define LOOPER_ROUTING_IN2 MIDI.sendControlChange(61, 2, MIDI_CHANNEL)
#define LOOPER_ROUTING_RET1 MIDI.sendControlChange(61, 3, MIDI_CHANNEL)
#define LOOPER_ROUTING_RET2 MIDI.sendControlChange(61, 4, MIDI_CHANNEL)
#define LOOPER_ROUTING_IN12 MIDI.sendControlChange(61, 5, MIDI_CHANNEL)
#define LOOPER_ROUTING_RET12 MIDI.sendControlChange(61, 6, MIDI_CHANNEL)
#define LOOPER_ROUTING_OUT1 MIDI.sendControlChange(61, 7, MIDI_CHANNEL)
#define LOOPER_ROUTING_OUT2 MIDI.sendControlChange(61, 8, MIDI_CHANNEL)
#define LOOPER_ROUTING_OUT3 MIDI.sendControlChange(61, 9, MIDI_CHANNEL)
#define LOOPER_ROUTING_OUT4 MIDI.sendControlChange(61, 10, MIDI_CHANNEL)
#define LOOPER_ROUTING_OUT12 MIDI.sendControlChange(61, 11, MIDI_CHANNEL)
#define LOOPER_ROUTING_OUT34 MIDI.sendControlChange(61, 12, MIDI_CHANNEL)
#define LOOPER_ROUTING_MULTI MIDI.sendControlChange(61, 13, MIDI_CHANNEL)

/* Looper routing options */
#define GRID 0
#define IN1 1
#define IN2 2
#define RET1 3
#define RET2 4
#define IN12 5
#define RET12 6
#define OUT1 7
#define OUT2 8
#define OUT3 9
#define OUT4 10
#define OUT12 11
#define OUT34 12
#define MULTI 13
