//Set mode names
#define STANDARD 0
#define SPECIAL 1

//Momentary mode state names
#define PRESS 0
#define RELEASE 1

//EEPROM byte defines
#define EXP1_UPPER_BYTE_1 0
#define EXP1_UPPER_BYTE_2 1
#define EXP1_LOWER_BYTE_1 2
#define EXP1_LOWER_BYTE_2 3
#define EXP2_UPPER_BYTE_1 4
#define EXP2_UPPER_BYTE_2 5
#define EXP2_LOWER_BYTE_1 6
#define EXP2_LOWER_BYTE_2 7

//Midi channel fallback
#ifndef MIDI_CHANNEL
  #define MIDI_CHANNEL MIDI_CHANNEL_OMNI
#endif

//LOOPER_BANK fallback
#ifndef LOOPER_BANK
  #define LOOPER_BANK BANK + 1
#endif

//Set the default QC mode
#ifndef DEFAULT_MODE
  #define DEFAULT_MODE SCENE_MODE
#endif

//Set the default LED color
#ifndef DEFAULT_COLOR
  #define DEFAULT_COLOR White
#endif
//Set the default subpixel order
#ifndef SUBPIXEL_ORDER
  #define SUBPIXEL_ORDER RGB
#endif
//Set the default LED order
#ifndef LED_ORDER
  #define LED_ORDER {0,1,2,3,4,5,6,7}
#endif
//Default LED brightness
#ifndef LED_BRIGHTNESS
  #define LED_BRIGHTNESS 100
#endif

//TODO: Check if this works
//Set the default footswitch modes
#ifndef FOOTSWITCH_MODES
  #define FOOTSWITCH_MODES
#endif

//TODO: Set to appropriate value
#ifndef EXP_THRESHOLD
  #define EXP_THRESHOLD 50
#endif

#ifdef EXP1_PIN
  #ifndef EXP1_DEADZONE
    #define EXP1_DEADZONE 0
  #endif
#endif
#ifdef EXP2_PIN
  #ifndef EXP2_DEADZONE
    #define EXP2_DEADZONE 0
  #endif
#endif

//Define LED mode names
#define LED_OFF 0 //All LED off
#define LED_ALL 1 //All LEDs on
#define LED_EXCLUSIVE 2 //Only last LED on
#define LED_TOGGLE 3 //LEDs are toggled on and off on press
#define LED_MOMENTARY 4 //LEDs are on when pressed and off when released
#define LED_EXP1 5 //All LEDs are on, brightness according to exp1 position
#define LED_EXP2 6 //All LEDs are on, brightness according to exp2 position

#ifndef HOLD_DURATION
  #define HOLD_DURATION 300
#endif

//Bank LED mode shortcut
#define BANK_LED_MODE(mode) \
  switch(mode){ \
    case LED_EXCLUSIVE: \
      for(uint8_t j = 0; j < LED_NUM; j++){ \
        ledState[j] = false; \
      } \
      ledState[i] = true; \
      break; \
    case LED_TOGGLE: \
      ledState[i] = !ledState[i]; \
      break; \
    case LED_MOMENTARY: \
      ledState[i] = true; \
      break; \
  }

//Define Special actions
//Open/Close tuner
#define TUNER \
  if(!special_action){ \
    special_action = true; \
    if(!tuner_open){ \
      tuner_open = true; \
      TUNER_OPEN; \
    } else { \
      tuner_open = false; \
      TUNER_CLOSE; \
    } \
  }

#define LOOPER_VIEW \
  if(!special_action){ \
    special_action = true; \
    if(!gig_open){ \
      gig_open = true; \
      LOOPER_OPEN \
    } else { \
      gig_open = false; \
      LOOPER_CLOSE \
    } \
  }

#define GIG_VIEW \
  if(!special_action){ \
    special_action = true; \
    if(!gig_open){ \
      gig_open = true; \
      GIG_VIEW_OPEN; \
    } else { \
      gig_open = false; \
      GIG_VIEW_CLOSE; \
    } \
  }

#define GIG_LOOPER_VIEW \
  if(!special_action){ \
    special_action = true; \
    switch(bank){ \
      case LOOPER_BANK: \
        if(!gig_open){ \
          gig_open = true; \
          LOOPER_OPEN; \
        } else { \
          gig_open = false; \
          LOOPER_CLOSE; \
        } \
        break; \
      default: \
        if(!gig_open){ \
          gig_open = true; \
          GIG_VIEW_OPEN; \
        } else { \
          gig_open = false; \
          GIG_VIEW_CLOSE; \
        } \
    } \
  }

//Increment bank
#define NEXT_BANK \
  if(!special_action){ \
    special_action = true; \
    bank++; \
    bank_changed = true; \
    updateLEDflag = true; \
    if(bank >= BANKS){ \
      bank = 0; \
    } \
    switch(bank){ \
      case 0: \
        BANK0_MODE; \
        break; \
      case 1: \
        BANK1_MODE; \
        break; \
      case 2: \
        BANK2_MODE; \
        break; \
      case 3: \
        BANK3_MODE; \
        break; \
      case 4: \
        BANK4_MODE; \
        break; \
      case 5: \
        BANK5_MODE; \
        break; \
      case 6: \
        BANK6_MODE; \
        break; \
      case 7: \
        BANK7_MODE; \
        break; \
      case 8: \
        BANK8_MODE; \
        break; \
      case 9: \
        BANK9_MODE; \
        break; \
      case 10: \
        BANK10_MODE; \
        break; \
      case 11: \
        BANK11_MODE; \
        break; \
      case 12: \
        BANK12_MODE; \
        break; \
      case 13: \
        BANK13_MODE; \
        break; \
      case 14: \
        BANK14_MODE; \
        break; \
      case 15: \
        BANK15_MODE; \
        break; \
    } \
  }

#define PREV_BANK \
  if(!special_action){ \
    special_action = true; \
    bank--; \
    bank_changed = true; \
    updateLEDflag = true; \
    if(bank >= BANKS){ \
      bank = BANKS - 1; \
    } \
    switch(bank){ \
      case 0: \
        BANK0_MODE; \
        break; \
      case 1: \
        BANK1_MODE; \
        break; \
      case 2: \
        BANK2_MODE; \
        break; \
      case 3: \
        BANK3_MODE; \
        break; \
      case 4: \
        BANK4_MODE; \
        break; \
      case 5: \
        BANK5_MODE; \
        break; \
      case 6: \
        BANK6_MODE; \
        break; \
      case 7: \
        BANK7_MODE; \
        break; \
      case 8: \
        BANK8_MODE; \
        break; \
      case 9: \
        BANK9_MODE; \
        break; \
      case 10: \
        BANK10_MODE; \
        break; \
      case 11: \
        BANK11_MODE; \
        break; \
      case 12: \
        BANK12_MODE; \
        break; \
      case 13: \
        BANK13_MODE; \
        break; \
      case 14: \
        BANK14_MODE; \
        break; \
      case 15: \
        BANK15_MODE; \
        break; \
    } \
  }

//Reset bank to 0
#define RESET_BANK \
  if(!special_action){ \
    special_action = true; \
    bank = 0; \
    bank_changed = true; \
    updateLEDflag = true; \
  } \
  BANK0_MODE

//Swap Expression pedals
#define SWAP_PEDALS \
  if(!special_action){ \
    special_action = true; \
    swap_pedals = !swap_pedals; \
  }

#if defined EXP1_PIN && EXP2_PIN
  #define CALIBRATE_PEDAL CALIBRATE_PEDALS
#elif defined EXP1_PIN
  #define CALIBRATE_PEDAL CALIBRATE_PEDAL_1
#elif defined EXP2_PIN
  #define CALIBRATE_PEDAL CALIBRATE_PEDAL_2
#endif

#if LED_NUM > 1
#define CALIBRATE_PEDAL_1 \
  if(!special_action){ \
    special_action = true; \
    bool startCalibration = false; \
    for(uint8_t i = 0; i < LED_NUM; i++){ \
      ledStatePrev[i] = ledState[i]; \
    } \
    while(!startCalibration){ \
      startCalibration = true; \
      buttons.update(); \
      for(uint8_t i = 0; i < FOOTSWITCH_NUM; i++){ \
        if(buttons.held(i)){ \
          startCalibration = false; \
          break; \
        } \
      } \
    } \
    calibrateExp(1); \
  }

#define CALIBRATE_PEDAL_2 \
  if(!special_action){ \
    special_action = true; \
    bool startCalibration = false; \
    for(uint8_t i = 0; i < LED_NUM; i++){ \
      ledStatePrev[i] = ledState[i]; \
    } \
    while(!startCalibration){ \
      startCalibration = true; \
      buttons.update(); \
      for(uint8_t i = 0; i < FOOTSWITCH_NUM; i++){ \
        if(buttons.held(i)){ \
          startCalibration = false; \
        } \
      } \
    } \
    calibrateExp(2); \
  }

#define CALIBRATE_PEDALS \
  if(!special_action){ \
    special_action = true; \
    bool calibrationFinished = false; \
    bool startCalibration1 = false; \
    bool startCalibration2 = false; \
    for(uint8_t i = 0; i < LED_NUM; i++){ \
      ledStatePrev[i] = ledState[i]; \
    } \
    for(uint8_t i = 0; i < FOOTSWITCH_NUM; i++){ \
      ledState[i] = false; \
    } \
    ledState[EXP1_CALIBRATION_KEY] = true; \
    ledState[EXP2_CALIBRATION_KEY] = true; \
    updateLEDs(); \
    while(!calibrationFinished){ \
      calibrationFinished = true; \
      buttons.update(); \
      for(uint8_t i = 0; i < FOOTSWITCH_NUM; i++){ \
        if(buttons.held(i)){ \
          calibrationFinished = false; \
        } \
      } \
    } \
    calibrationFinished = false; \
    while(!calibrationFinished){ \
      calibrationFinished = false; \
      buttons.update(); \
      if(buttons.held(EXP1_CALIBRATION_KEY)){ \
        startCalibration1 = true; \
        calibrationFinished = true; \
      } else if(buttons.held(EXP2_CALIBRATION_KEY)){ \
        startCalibration2 = true; \
        calibrationFinished = true; \
      } \
      for(uint8_t i = 0; i < FOOTSWITCH_NUM; i++){ \
        if(i == EXP1_CALIBRATION_KEY || i == EXP2_CALIBRATION_KEY){ \
          continue; \
        } \
        if(buttons.fell(i)){ \
          while(!calibrationFinished){ \
            buttons.update(); \
            if(buttons.rose(i)){ \
              calibrationFinished = true; \
              ledState[EXP1_CALIBRATION_KEY] = false; \
              ledState[EXP2_CALIBRATION_KEY] = false; \
              for(uint8_t j = 0; j < FOOTSWITCH_NUM; j++){ \
                btn_held[j] = false; \
              } \
              updateLEDs(); \
              break; \
            } \
          } \
        } \
      } \
    } \
    if(startCalibration1){ \
      calibrateExp(1); \
    } else if(startCalibration2){ \
      calibrateExp(2); \
    } \
  }
#else
#define CALIBRATE_PEDAL_1 \
  if(!special_action){ \
    special_action = true; \
    while(!startCalibration){ \
      startCalibration = true; \
      buttons.update(); \
      for(uint8_t i = 0; i < FOOTSWITCH_NUM; i++){ \
        if(buttons.held(i)){ \
          startCalibration = false; \
        } \
      } \
    } \
    calibrateExp(1); \
  }

#define CALIBRATE_PEDAL_2 \
  if(!special_action){ \
    special_action = true; \
    bool startCalibration = false; \
    while(!startCalibration){ \
      startCalibration = true; \
      buttons.update(); \
      for(uint8_t i = 0; i < FOOTSWITCH_NUM; i++){ \
        if(buttons.held(i)){ \
          startCalibration = false; \
        } \
      } \
    } \
    calibrateExp(2); \
  }

#define CALIBRATE_PEDALS \
  if(!special_action){ \
    special_action = true; \
    bool calibrationFinished = false; \
    bool startCalibration1 = false; \
    bool startCalibration2 = false; \
    while(!calibrationFinished){ \
      calibrationFinished = true; \
      buttons.update(); \
      for(uint8_t i = 0; i < FOOTSWITCH_NUM; i++){ \
        if(buttons.held(i)){ \
          calibrationFinished = false; \
        } \
      } \
    } \
    calibrationFinished = false; \
    while(!calibrationFinished){ \
      calibrationFinished = false; \
      buttons.update(); \
      if(buttons.held(EXP1_CALIBRATION_KEY)){ \
        startCalibration1 = true; \
        calibrationFinished = true; \
      } else if(buttons.held(EXP2_CALIBRATION_KEY)){ \
        startCalibration2 = true; \
        calibrationFinished = true; \
      } \
      for(uint8_t i = 0; i < FOOTSWITCH_NUM; i++){ \
        if(i == EXP1_CALIBRATION_KEY || i == EXP2_CALIBRATION_KEY){ \
          continue; \
        } \
        if(buttons.fell(i)){ \
          while(!calibrationFinished){ \
            buttons.update(); \
            if(buttons.rose(i)){ \
              calibrationFinished = true; \
            } \
          } \
        } \
      } \
    } \
    if(startCalibration1){ \
      calibrateExp(1); \
    } else if(startCalibration2){ \
      calibrateExp(2); \
    } \
  }
#endif //if LED_NUM > 1

#define SET_BANK(num) \
  if(!special_action){ \
    special_action = true; \
    bank = num; \
    bank_changed = true; \
    updateLEDflag = true; \
    switch(num){ \
      case 0: \
        BANK0_MODE; \
        break; \
      case 1: \
        BANK1_MODE; \
        break; \
      case 2: \
        BANK2_MODE; \
        break; \
      case 3: \
        BANK3_MODE; \
        break; \
      case 4: \
        BANK4_MODE; \
        break; \
      case 5: \
        BANK5_MODE; \
        break; \
      case 6: \
        BANK6_MODE; \
        break; \
      case 7: \
        BANK7_MODE; \
        break; \
      case 8: \
        BANK8_MODE; \
        break; \
      case 9: \
        BANK9_MODE; \
        break; \
      case 10: \
        BANK10_MODE; \
        break; \
      case 11: \
        BANK11_MODE; \
        break; \
      case 12: \
        BANK12_MODE; \
        break; \
      case 13: \
        BANK13_MODE; \
        break; \
      case 14: \
        BANK14_MODE; \
        break; \
      case 15: \
        BANK15_MODE; \
        break; \
    } \
  }

#define MOMENTARY_BANK(mom_bank) \
  if(!special_action){ \
    special_action = true; \
    prev_bank = bank; \
    bank = mom_bank; \
    bank_changed = true; \
    updateLEDflag = true; \
  }


#ifdef EXP1_PIN
#ifndef EXP1_CALIBRATION_KEY
  #define EXP1_CALIBRATION_KEY 0
#endif
#endif
#ifdef EXP2_PIN
#ifndef EXP2_CALIBRATION_KEY
  #define EXP2_CALIBRATION_KEY 1
#endif
#endif

#ifndef MAX_BRIGHTNESS
  #define MAX_BRIGHTNESS 240
#endif

#ifndef MIN_BRIGHTNESS
  #define MIN_BRIGHTNESS 1
#endif

#ifndef SPECIAL_PRESS_ACTION
  #define SPECIAL_PRESS_ACTION
#endif
#ifndef SPECIAL_HOLD_ACTION
  #define SPECIAL_HOLD_ACTION
#endif
#ifndef SPECIAL_HOLD_ACTION_RELEASE
  #define SPECIAL_HOLD_ACTION_RELEASE
#endif

//Fallback LED_NUM
#ifndef LED_NUM
  #define NO_LED
  #define LED_NUM 1
  #undef LED_ORDER
  #define LED_ORDER {0}
#endif
#if LED_NUM == 0
  #define NO_LED
  #define LED_NUM 1
  #undef LED_ORDER
  #define LED_ORDER {0}
#endif
#ifndef LED_ORDER
  #if LED_NUM == 1
    #define LED_ORDER {0}
  #elif LED_NUM == 2
    #define LED_ORDER {0,1}
  #elif LED_NUM == 4
    #define LED_ORDER {0,1,2,3}
  #elif LED_NUM == 6
    #define LED_ORDER {0,1,2,3,4,5}
  #elif LED_NUM == 8
    #define LED_ORDER {0,1,2,3,4,5,6,7}
  #elif LED_NUM == 10
    #define LED_ORDER {0,1,2,3,4,5,6,7,8,9}
  #elif LED_NUM == 12
    #define LED_ORDER {0,1,2,3,4,5,6,7,8,9,10,11}
  #elif LED_NUM == 14
    #define LED_ORDER {0,1,2,3,4,5,6,7,8,9,10,11,12,13}
  #elif LED_NUM == 16
    #define LED_ORDER {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15}
  #endif
#endif
//Set default bank modes
#ifndef BANK0_MODE
  #define BANK0_MODE DEFAULT_MODE
#endif
#ifndef BANK1_MODE
  #define BANK1_MODE DEFAULT_MODE
#endif
#ifndef BANK2_MODE
  #define BANK2_MODE DEFAULT_MODE
#endif
#ifndef BANK3_MODE
  #define BANK3_MODE DEFAULT_MODE
#endif
#ifndef BANK4_MODE
  #define BANK4_MODE DEFAULT_MODE
#endif
#ifndef BANK5_MODE
  #define BANK5_MODE DEFAULT_MODE
#endif
#ifndef BANK5_MODE
  #define BANK5_MODE DEFAULT_MODE
#endif
#ifndef BANK6_MODE
  #define BANK6_MODE DEFAULT_MODE
#endif
#ifndef BANK7_MODE
  #define BANK7_MODE DEFAULT_MODE
#endif
#ifndef BANK8_MODE
  #define BANK8_MODE DEFAULT_MODE
#endif
#ifndef BANK9_MODE
  #define BANK9_MODE DEFAULT_MODE
#endif
#ifndef BANK10_MODE
  #define BANK10_MODE DEFAULT_MODE
#endif
#ifndef BANK11_MODE
  #define BANK11_MODE DEFAULT_MODE
#endif
#ifndef BANK12_MODE
  #define BANK12_MODE DEFAULT_MODE
#endif
#ifndef BANK13_MODE
  #define BANK13_MODE DEFAULT_MODE
#endif
#ifndef BANK14_MODE
  #define BANK14_MODE DEFAULT_MODE
#endif
#ifndef BANK15_MODE
  #define BANK15_MODE DEFAULT_MODE
#endif

//Define default bank colors
#ifndef BANK0_COLOR
  #define BANK0_COLOR DEFAULT_COLOR
#endif
#ifndef BANK1_COLOR
  #define BANK1_COLOR DEFAULT_COLOR
#endif
#ifndef BANK2_COLOR
  #define BANK2_COLOR DEFAULT_COLOR
#endif
#ifndef BANK3_COLOR
  #define BANK3_COLOR DEFAULT_COLOR
#endif
#ifndef BANK4_COLOR
  #define BANK4_COLOR DEFAULT_COLOR
#endif
#ifndef BANK5_COLOR
  #define BANK5_COLOR DEFAULT_COLOR
#endif
#ifndef BANK6_COLOR
  #define BANK6_COLOR DEFAULT_COLOR
#endif
#ifndef BANK7_COLOR
  #define BANK7_COLOR DEFAULT_COLOR
#endif
#ifndef BANK8_COLOR
  #define BANK8_COLOR DEFAULT_COLOR
#endif
#ifndef BANK9_COLOR
  #define BANK9_COLOR DEFAULT_COLOR
#endif
#ifndef BANK10_COLOR
  #define BANK10_COLOR DEFAULT_COLOR
#endif
#ifndef BANK11_COLOR
  #define BANK11_COLOR DEFAULT_COLOR
#endif
#ifndef BANK12_COLOR
  #define BANK12_COLOR DEFAULT_COLOR
#endif
#ifndef BANK13_COLOR
  #define BANK13_COLOR DEFAULT_COLOR
#endif
#ifndef BANK14_COLOR
  #define BANK14_COLOR DEFAULT_COLOR
#endif
#ifndef BANK15_COLOR
  #define BANK15_COLOR DEFAULT_COLOR
#endif

//Default LED mode fallback
#ifndef DEFAULT_LED_MODE
  #define DEFAULT_LED_MODE LED_TOGGLE
#endif

//Set bank LED mode fallback
#ifndef BANK0_LED_MODE
  #define BANK0_LED_MODE DEFAULT_LED_MODE
#endif
#ifndef BANK1_LED_MODE
  #define BANK1_LED_MODE DEFAULT_LED_MODE
#endif
#ifndef BANK2_LED_MODE
  #define BANK2_LED_MODE DEFAULT_LED_MODE
#endif
#ifndef BANK3_LED_MODE
  #define BANK3_LED_MODE DEFAULT_LED_MODE
#endif
#ifndef BANK4_LED_MODE
  #define BANK4_LED_MODE DEFAULT_LED_MODE
#endif
#ifndef BANK5_LED_MODE
  #define BANK5_LED_MODE DEFAULT_LED_MODE
#endif
#ifndef BANK6_LED_MODE
  #define BANK6_LED_MODE DEFAULT_LED_MODE
#endif
#ifndef BANK7_LED_MODE
  #define BANK7_LED_MODE DEFAULT_LED_MODE
#endif
#ifndef BANK8_LED_MODE
  #define BANK8_LED_MODE DEFAULT_LED_MODE
#endif
#ifndef BANK9_LED_MODE
  #define BANK9_LED_MODE DEFAULT_LED_MODE
#endif
#ifndef BANK10_LED_MODE
  #define BANK10_LED_MODE DEFAULT_LED_MODE
#endif
#ifndef BANK11_LED_MODE
  #define BANK11_LED_MODE DEFAULT_LED_MODE
#endif
#ifndef BANK12_LED_MODE
  #define BANK12_LED_MODE DEFAULT_LED_MODE
#endif
#ifndef BANK13_LED_MODE
  #define BANK13_LED_MODE DEFAULT_LED_MODE
#endif
#ifndef BANK14_LED_MODE
  #define BANK14_LED_MODE DEFAULT_LED_MODE
#endif
#ifndef BANK15_LED_MODE
  #define BANK15_LED_MODE DEFAULT_LED_MODE
#endif

//Define SPECIAL combo defaults
#ifndef SPECIAL_0_1
  #define SPECIAL_0_1
#endif
#ifndef SPECIAL_1_2
  #define SPECIAL_1_2
#endif
#ifndef SPECIAL_2_3
  #define SPECIAL_2_3
#endif
#ifndef SPECIAL_3_4
  #define SPECIAL_3_4
#endif
#ifndef SPECIAL_4_5
  #define SPECIAL_4_5
#endif
#ifndef SPECIAL_5_6
  #define SPECIAL_5_6
#endif
#ifndef SPECIAL_6_7
  #define SPECIAL_6_7
#endif
#ifndef SPECIAL_7_8
  #define SPECIAL_7_8
#endif
#ifndef SPECIAL_8_9
  #define SPECIAL_8_9
#endif
#ifndef SPECIAL_9_10
  #define SPECIAL_9_10
#endif
#ifndef SPECIAL_10_11
  #define SPECIAL_10_11
#endif
#ifndef SPECIAL_11_12
  #define SPECIAL_11_12
#endif
#ifndef SPECIAL_12_13
  #define SPECIAL_12_13
#endif
#ifndef SPECIAL_13_14
  #define SPECIAL_13_14
#endif
#ifndef SPECIAL_14_15
  #define SPECIAL_14_15
#endif
//Vertical for 4 buttons
#ifndef SPECIAL_0_2
  #define SPECIAL_0_2
#endif
#ifndef SPECIAL_1_3
  #define SPECIAL_1_3
#endif
//Vertical for 6 buttons
#ifndef SPECIAL_0_3
  #define SPECIAL_0_3
#endif
#ifndef SPECIAL_1_4
  #define SPECIAL_1_4
#endif
#ifndef SPECIAL_2_5
  #define SPECIAL_2_5
#endif
//Vertical for 8 buttons
#ifndef SPECIAL_0_4
  #define SPECIAL_0_4
#endif
#ifndef SPECIAL_1_5
  #define SPECIAL_1_5
#endif
#ifndef SPECIAL_2_6
  #define SPECIAL_2_6
#endif
#ifndef SPECIAL_3_7
  #define SPECIAL_3_7
#endif
//Vertical for 10 buttons
#ifndef SPECIAL_0_5
  #define SPECIAL_0_5
#endif
#ifndef SPECIAL_1_6
  #define SPECIAL_1_6
#endif
#ifndef SPECIAL_2_7
  #define SPECIAL_2_7
#endif
#ifndef SPECIAL_3_8
  #define SPECIAL_3_8
#endif
#ifndef SPECIAL_4_9
  #define SPECIAL_4_9
#endif
//Vertical for 12 buttons
#ifndef SPECIAL_0_6
  #define SPECIAL_0_6
#endif
#ifndef SPECIAL_1_7
  #define SPECIAL_1_7
#endif
#ifndef SPECIAL_2_8
  #define SPECIAL_2_8
#endif
#ifndef SPECIAL_3_9
  #define SPECIAL_3_9
#endif
#ifndef SPECIAL_4_10
  #define SPECIAL_4_10
#endif
#ifndef SPECIAL_5_11
  #define SPECIAL_5_11
#endif
//Vertical for 14 buttons
#ifndef SPECIAL_0_7
  #define SPECIAL_0_7
#endif
#ifndef SPECIAL_1_8
  #define SPECIAL_1_8
#endif
#ifndef SPECIAL_2_9
  #define SPECIAL_2_9
#endif
#ifndef SPECIAL_3_10
  #define SPECIAL_3_10
#endif
#ifndef SPECIAL_4_11
  #define SPECIAL_4_11
#endif
#ifndef SPECIAL_5_12
  #define SPECIAL_5_12
#endif
#ifndef SPECIAL_6_13
  #define SPECIAL_6_13
#endif
//Vertical for 16 buttons
#ifndef SPECIAL_0_8
  #define SPECIAL_0_8
#endif
#ifndef SPECIAL_1_9
  #define SPECIAL_1_9
#endif
#ifndef SPECIAL_2_10
  #define SPECIAL_2_10
#endif
#ifndef SPECIAL_3_11
  #define SPECIAL_3_11
#endif
#ifndef SPECIAL_4_12
  #define SPECIAL_4_12
#endif
#ifndef SPECIAL_5_13
  #define SPECIAL_5_13
#endif
#ifndef SPECIAL_6_14
  #define SPECIAL_6_14
#endif
#ifndef SPECIAL_7_15
  #define SPECIAL_7_15
#endif

//Button action fallbacks
//Bank 0
#ifndef BANK0BTN0P
#define BANK0BTN0P
#endif
#ifndef BANK0BTN0R
#define BANK0BTN0R
#endif
#ifndef BANK0BTN1P
#define BANK0BTN1P
#endif
#ifndef BANK0BTN1R
#define BANK0BTN1R
#endif
#ifndef BANK0BTN2P
#define BANK0BTN2P
#endif
#ifndef BANK0BTN2R
#define BANK0BTN2R
#endif
#ifndef BANK0BTN3P
#define BANK0BTN3P
#endif
#ifndef BANK0BTN3R
#define BANK0BTN3R
#endif
#ifndef BANK0BTN4P
#define BANK0BTN4P
#endif
#ifndef BANK0BTN4R
#define BANK0BTN4R
#endif
#ifndef BANK0BTN5P
#define BANK0BTN5P
#endif
#ifndef BANK0BTN5R
#define BANK0BTN5R
#endif
#ifndef BANK0BTN6P
#define BANK0BTN6P
#endif
#ifndef BANK0BTN6R
#define BANK0BTN6R
#endif
#ifndef BANK0BTN7P
#define BANK0BTN7P
#endif
#ifndef BANK0BTN7R
#define BANK0BTN7R
#endif
#ifndef BANK0BTN8P
#define BANK0BTN8P
#endif
#ifndef BANK0BTN8R
#define BANK0BTN8R
#endif
#ifndef BANK0BTN9P
#define BANK0BTN9P
#endif
#ifndef BANK0BTN9R
#define BANK0BTN9R
#endif
#ifndef BANK0BTN10P
#define BANK0BTN10P
#endif
#ifndef BANK0BTN10R
#define BANK0BTN10R
#endif
#ifndef BANK0BTN11P
#define BANK0BTN11P
#endif
#ifndef BANK0BTN11R
#define BANK0BTN11R
#endif
#ifndef BANK0BTN12P
#define BANK0BTN12P
#endif
#ifndef BANK0BTN12R
#define BANK0BTN12R
#endif
#ifndef BANK0BTN13P
#define BANK0BTN13P
#endif
#ifndef BANK0BTN13R
#define BANK0BTN13R
#endif
#ifndef BANK0BTN14P
#define BANK0BTN14P
#endif
#ifndef BANK0BTN14R
#define BANK0BTN14R
#endif
#ifndef BANK0BTN15P
#define BANK0BTN15P
#endif
#ifndef BANK0BTN15R
#define BANK0BTN15R
#endif
//Bank 1
#ifndef BANK1BTN0P
#define BANK1BTN0P
#endif
#ifndef BANK1BTN0R
#define BANK1BTN0R
#endif
#ifndef BANK1BTN1P
#define BANK1BTN1P
#endif
#ifndef BANK1BTN1R
#define BANK1BTN1R
#endif
#ifndef BANK1BTN2P
#define BANK1BTN2P
#endif
#ifndef BANK1BTN2R
#define BANK1BTN2R
#endif
#ifndef BANK1BTN3P
#define BANK1BTN3P
#endif
#ifndef BANK1BTN3R
#define BANK1BTN3R
#endif
#ifndef BANK1BTN4P
#define BANK1BTN4P
#endif
#ifndef BANK1BTN4R
#define BANK1BTN4R
#endif
#ifndef BANK1BTN5P
#define BANK1BTN5P
#endif
#ifndef BANK1BTN5R
#define BANK1BTN5R
#endif
#ifndef BANK1BTN6P
#define BANK1BTN6P
#endif
#ifndef BANK1BTN6R
#define BANK1BTN6R
#endif
#ifndef BANK1BTN7P
#define BANK1BTN7P
#endif
#ifndef BANK1BTN7R
#define BANK1BTN7R
#endif
#ifndef BANK1BTN8P
#define BANK1BTN8P
#endif
#ifndef BANK1BTN8R
#define BANK1BTN8R
#endif
#ifndef BANK1BTN9P
#define BANK1BTN9P
#endif
#ifndef BANK1BTN9R
#define BANK1BTN9R
#endif
#ifndef BANK1BTN10P
#define BANK1BTN10P
#endif
#ifndef BANK1BTN10R
#define BANK1BTN10R
#endif
#ifndef BANK1BTN11P
#define BANK1BTN11P
#endif
#ifndef BANK1BTN11R
#define BANK1BTN11R
#endif
#ifndef BANK1BTN12P
#define BANK1BTN12P
#endif
#ifndef BANK1BTN12R
#define BANK1BTN12R
#endif
#ifndef BANK1BTN13P
#define BANK1BTN13P
#endif
#ifndef BANK1BTN13R
#define BANK1BTN13R
#endif
#ifndef BANK1BTN14P
#define BANK1BTN14P
#endif
#ifndef BANK1BTN14R
#define BANK1BTN14R
#endif
#ifndef BANK1BTN15P
#define BANK1BTN15P
#endif
#ifndef BANK1BTN15R
#define BANK1BTN15R
#endif
//Bank 2
#ifndef BANK2BTN0P
#define BANK2BTN0P
#endif
#ifndef BANK2BTN0R
#define BANK2BTN0R
#endif
#ifndef BANK2BTN1P
#define BANK2BTN1P
#endif
#ifndef BANK2BTN1R
#define BANK2BTN1R
#endif
#ifndef BANK2BTN2P
#define BANK2BTN2P
#endif
#ifndef BANK2BTN2R
#define BANK2BTN2R
#endif
#ifndef BANK2BTN3P
#define BANK2BTN3P
#endif
#ifndef BANK2BTN3R
#define BANK2BTN3R
#endif
#ifndef BANK2BTN4P
#define BANK2BTN4P
#endif
#ifndef BANK2BTN4R
#define BANK2BTN4R
#endif
#ifndef BANK2BTN5P
#define BANK2BTN5P
#endif
#ifndef BANK2BTN5R
#define BANK2BTN5R
#endif
#ifndef BANK2BTN6P
#define BANK2BTN6P
#endif
#ifndef BANK2BTN6R
#define BANK2BTN6R
#endif
#ifndef BANK2BTN7P
#define BANK2BTN7P
#endif
#ifndef BANK2BTN7R
#define BANK2BTN7R
#endif
#ifndef BANK2BTN8P
#define BANK2BTN8P
#endif
#ifndef BANK2BTN8R
#define BANK2BTN8R
#endif
#ifndef BANK2BTN9P
#define BANK2BTN9P
#endif
#ifndef BANK2BTN9R
#define BANK2BTN9R
#endif
#ifndef BANK2BTN10P
#define BANK2BTN10P
#endif
#ifndef BANK2BTN10R
#define BANK2BTN10R
#endif
#ifndef BANK2BTN11P
#define BANK2BTN11P
#endif
#ifndef BANK2BTN11R
#define BANK2BTN11R
#endif
#ifndef BANK2BTN12P
#define BANK2BTN12P
#endif
#ifndef BANK2BTN12R
#define BANK2BTN12R
#endif
#ifndef BANK2BTN13P
#define BANK2BTN13P
#endif
#ifndef BANK2BTN13R
#define BANK2BTN13R
#endif
#ifndef BANK2BTN14P
#define BANK2BTN14P
#endif
#ifndef BANK2BTN14R
#define BANK2BTN14R
#endif
#ifndef BANK2BTN15P
#define BANK2BTN15P
#endif
#ifndef BANK2BTN15R
#define BANK2BTN15R
#endif
//Bank 3
#ifndef BANK3BTN0P
#define BANK3BTN0P
#endif
#ifndef BANK3BTN0R
#define BANK3BTN0R
#endif
#ifndef BANK3BTN1P
#define BANK3BTN1P
#endif
#ifndef BANK3BTN1R
#define BANK3BTN1R
#endif
#ifndef BANK3BTN2P
#define BANK3BTN2P
#endif
#ifndef BANK3BTN2R
#define BANK3BTN2R
#endif
#ifndef BANK3BTN3P
#define BANK3BTN3P
#endif
#ifndef BANK3BTN3R
#define BANK3BTN3R
#endif
#ifndef BANK3BTN4P
#define BANK3BTN4P
#endif
#ifndef BANK3BTN4R
#define BANK3BTN4R
#endif
#ifndef BANK3BTN5P
#define BANK3BTN5P
#endif
#ifndef BANK3BTN5R
#define BANK3BTN5R
#endif
#ifndef BANK3BTN6P
#define BANK3BTN6P
#endif
#ifndef BANK3BTN6R
#define BANK3BTN6R
#endif
#ifndef BANK3BTN7P
#define BANK3BTN7P
#endif
#ifndef BANK3BTN7R
#define BANK3BTN7R
#endif
#ifndef BANK3BTN8P
#define BANK3BTN8P
#endif
#ifndef BANK3BTN8R
#define BANK3BTN8R
#endif
#ifndef BANK3BTN9P
#define BANK3BTN9P
#endif
#ifndef BANK3BTN9R
#define BANK3BTN9R
#endif
#ifndef BANK3BTN10P
#define BANK3BTN10P
#endif
#ifndef BANK3BTN10R
#define BANK3BTN10R
#endif
#ifndef BANK3BTN11P
#define BANK3BTN11P
#endif
#ifndef BANK3BTN11R
#define BANK3BTN11R
#endif
#ifndef BANK3BTN12P
#define BANK3BTN12P
#endif
#ifndef BANK3BTN12R
#define BANK3BTN12R
#endif
#ifndef BANK3BTN13P
#define BANK3BTN13P
#endif
#ifndef BANK3BTN13R
#define BANK3BTN13R
#endif
#ifndef BANK3BTN14P
#define BANK3BTN14P
#endif
#ifndef BANK3BTN14R
#define BANK3BTN14R
#endif
#ifndef BANK3BTN15P
#define BANK3BTN15P
#endif
#ifndef BANK3BTN15R
#define BANK3BTN15R
#endif
//Bank 4
#ifndef BANK4BTN0P
#define BANK4BTN0P
#endif
#ifndef BANK4BTN0R
#define BANK4BTN0R
#endif
#ifndef BANK4BTN1P
#define BANK4BTN1P
#endif
#ifndef BANK4BTN1R
#define BANK4BTN1R
#endif
#ifndef BANK4BTN2P
#define BANK4BTN2P
#endif
#ifndef BANK4BTN2R
#define BANK4BTN2R
#endif
#ifndef BANK4BTN3P
#define BANK4BTN3P
#endif
#ifndef BANK4BTN3R
#define BANK4BTN3R
#endif
#ifndef BANK4BTN4P
#define BANK4BTN4P
#endif
#ifndef BANK4BTN4R
#define BANK4BTN4R
#endif
#ifndef BANK4BTN5P
#define BANK4BTN5P
#endif
#ifndef BANK4BTN5R
#define BANK4BTN5R
#endif
#ifndef BANK4BTN6P
#define BANK4BTN6P
#endif
#ifndef BANK4BTN6R
#define BANK4BTN6R
#endif
#ifndef BANK4BTN7P
#define BANK4BTN7P
#endif
#ifndef BANK0BTN7R
#define BANK0BTN7R
#endif
#ifndef BANK0BTN8P
#define BANK0BTN8P
#endif
#ifndef BANK4BTN8R
#define BANK4BTN8R
#endif
#ifndef BANK4BTN9P
#define BANK4BTN9P
#endif
#ifndef BANK4BTN9R
#define BANK4BTN9R
#endif
#ifndef BANK4BTN10P
#define BANK4BTN10P
#endif
#ifndef BANK4BTN10R
#define BANK4BTN10R
#endif
#ifndef BANK4BTN11P
#define BANK4BTN11P
#endif
#ifndef BANK4BTN11R
#define BANK4BTN11R
#endif
#ifndef BANK4BTN12P
#define BANK4BTN12P
#endif
#ifndef BANK4BTN12R
#define BANK4BTN12R
#endif
#ifndef BANK4BTN13P
#define BANK4BTN13P
#endif
#ifndef BANK4BTN13R
#define BANK4BTN13R
#endif
#ifndef BANK4BTN14P
#define BANK4BTN14P
#endif
#ifndef BANK4BTN14R
#define BANK4BTN14R
#endif
#ifndef BANK4BTN15P
#define BANK4BTN15P
#endif
#ifndef BANK4BTN15R
#define BANK4BTN15R
#endif
//Bank 5
#ifndef BANK5BTN0P
#define BANK5BTN0P
#endif
#ifndef BANK5BTN0R
#define BANK5BTN0R
#endif
#ifndef BANK5BTN1P
#define BANK5BTN1P
#endif
#ifndef BANK5BTN1R
#define BANK5BTN1R
#endif
#ifndef BANK5BTN2P
#define BANK5BTN2P
#endif
#ifndef BANK5BTN2R
#define BANK5BTN2R
#endif
#ifndef BANK5BTN3P
#define BANK5BTN3P
#endif
#ifndef BANK5BTN3R
#define BANK5BTN3R
#endif
#ifndef BANK5BTN4P
#define BANK5BTN4P
#endif
#ifndef BANK5BTN4R
#define BANK5BTN4R
#endif
#ifndef BANK5BTN5P
#define BANK5BTN5P
#endif
#ifndef BANK5BTN5R
#define BANK5BTN5R
#endif
#ifndef BANK5BTN6P
#define BANK5BTN6P
#endif 
#ifndef BANK5BTN6R
#define BANK5BTN6R
#endif
#ifndef BANK5BTN7P
#define BANK5BTN7P
#endif
#ifndef BANK5BTN7R
#define BANK5BTN7R
#endif
#ifndef BANK5BTN8P
#define BANK5BTN8P
#endif
#ifndef BANK5BTN8R
#define BANK5BTN8R
#endif
#ifndef BANK5BTN9P
#define BANK5BTN9P
#endif
#ifndef BANK5BTN9R
#define BANK5BTN9R
#endif
#ifndef BANK5BTN10P
#define BANK5BTN10P
#endif
#ifndef BANK5BTN10R
#define BANK5BTN10R
#endif
#ifndef BANK5BTN11P
#define BANK5BTN11P
#endif
#ifndef BANK5BTN11R
#define BANK5BTN11R
#endif
#ifndef BANK5BTN12P
#define BANK5BTN12P
#endif
#ifndef BANK5BTN12R
#define BANK5BTN12R
#endif
#ifndef BANK5BTN13P
#define BANK5BTN13P
#endif
#ifndef BANK5BTN13R
#define BANK5BTN13R
#endif
#ifndef BANK5BTN14P
#define BANK5BTN14P
#endif
#ifndef BANK5BTN14R
#define BANK5BTN14R
#endif
#ifndef BANK5BTN15P
#define BANK5BTN15P
#endif
#ifndef BANK5BTN15R
#define BANK5BTN15R
#endif
//Bank 6
#ifndef BANK6BTN0P
#define BANK6BTN0P
#endif
#ifndef BANK6BTN0P
#define BANK6BTN0R
#endif
#ifndef BANK6BTN1P
#define BANK6BTN1P
#endif
#ifndef BANK6BTN1R
#define BANK6BTN1R
#endif
#ifndef BANK6BTN2P
#define BANK6BTN2P
#endif
#ifndef BANK6BTN2R
#define BANK6BTN2R
#endif
#ifndef BANK6BTN3P
#define BANK6BTN3P
#endif
#ifndef BANK6BTN3R
#define BANK6BTN3R
#endif
#ifndef BANK6BTN4P
#define BANK6BTN4P
#endif
#ifndef BANK6BTN4R
#define BANK6BTN4R
#endif
#ifndef BANK6BTN5P
#define BANK6BTN5P
#endif
#ifndef BANK6BTN5R
#define BANK6BTN5R
#endif
#ifndef BANK6BTN6P
#define BANK6BTN6P
#endif
#ifndef BANK6BTN6R
#define BANK6BTN6R
#endif
#ifndef BANK6BTN7P
#define BANK6BTN7P
#endif
#ifndef BANK6BTN7R
#define BANK6BTN7R
#endif
#ifndef BANK6BTN8P
#define BANK6BTN8P
#endif
#ifndef BANK6BTN8R
#define BANK6BTN8R
#endif
#ifndef BANK6BTN9P
#define BANK6BTN9P
#endif
#ifndef BANK6BTN9R
#define BANK6BTN9R
#endif
#ifndef BANK6BTN10P
#define BANK6BTN10P
#endif
#ifndef BANK6BTN10R
#define BANK6BTN10R
#endif
#ifndef BANK6BTN11P
#define BANK6BTN11P
#endif
#ifndef BANK6BTN11R
#define BANK6BTN11R
#endif
#ifndef BANK6BTN12P
#define BANK6BTN12P
#endif
#ifndef BANK6BTN12R
#define BANK6BTN12R
#endif
#ifndef BANK6BTN13P
#define BANK6BTN13P
#endif
#ifndef BANK6BTN13R
#define BANK6BTN13R
#endif
#ifndef BANK6BTN14P
#define BANK6BTN14P
#endif
#ifndef BANK6BTN14R
#define BANK6BTN14R
#endif
#ifndef BANK6BTN15P
#define BANK6BTN15P
#endif
#ifndef BANK6BTN15R
#define BANK6BTN15R
#endif
//Bank 7
#ifndef BANK7BTN0P
#define BANK7BTN0P
#endif
#ifndef BANK7BTN0R
#define BANK7BTN0R
#endif
#ifndef BANK7BTN1P
#define BANK7BTN1P
#endif
#ifndef BANK7BTN1R
#define BANK7BTN1R
#endif
#ifndef BANK7BTN2P
#define BANK7BTN2P
#endif
#ifndef BANK7BTN2R
#define BANK7BTN2R
#endif
#ifndef BANK7BTN3P
#define BANK7BTN3P
#endif
#ifndef BANK7BTN3R
#define BANK7BTN3R
#endif
#ifndef BANK7BTN4P
#define BANK7BTN4P
#endif
#ifndef BANK7BTN4R
#define BANK7BTN4R
#endif
#ifndef BANK7BTN5P
#define BANK7BTN5P
#endif
#ifndef BANK7BTN5R
#define BANK7BTN5R
#endif
#ifndef BANK7BTN6P
#define BANK7BTN6P
#endif 
#ifndef BANK7BTN6R
#define BANK7BTN6R
#endif
#ifndef BANK7BTN7P
#define BANK7BTN7P
#endif
#ifndef BANK7BTN7R
#define BANK7BTN7R
#endif
#ifndef BANK7BTN8P
#define BANK7BTN8P
#endif
#ifndef BANK7BTN8R
#define BANK7BTN8R
#endif
#ifndef BANK7BTN9P
#define BANK7BTN9P
#endif
#ifndef BANK7BTN9R
#define BANK7BTN9R
#endif
#ifndef BANK7BTN10P
#define BANK7BTN10P
#endif
#ifndef BANK7BTN10R
#define BANK7BTN10R
#endif
#ifndef BANK7BTN11P
#define BANK7BTN11P
#endif
#ifndef BANK7BTN11R
#define BANK7BTN11R
#endif
#ifndef BANK7BTN12P
#define BANK7BTN12P
#endif
#ifndef BANK7BTN12R
#define BANK7BTN12R
#endif
#ifndef BANK7BTN13P
#define BANK7BTN13P
#endif
#ifndef BANK7BTN13R
#define BANK7BTN13R
#endif
#ifndef BANK7BTN14P
#define BANK7BTN14P
#endif
#ifndef BANK7BTN14R
#define BANK7BTN14R
#endif
#ifndef BANK7BTN15P
#define BANK7BTN15P
#endif
#ifndef BANK7BTN15R
#define BANK7BTN15R
#endif
//Bank 8
#ifndef BANK8BTN0P
#define BANK8BTN0P
#endif
#ifndef BANK8BTN0R
#define BANK8BTN0R
#endif
#ifndef BANK8BTN1P
#define BANK8BTN1P
#endif
#ifndef BANK8BTN1R
#define BANK8BTN1R
#endif
#ifndef BANK8BTN2P
#define BANK8BTN2P
#endif
#ifndef BANK8BTN2R
#define BANK8BTN2R
#endif
#ifndef BANK8BTN3P
#define BANK8BTN3P
#endif
#ifndef BANK8BTN3R
#define BANK8BTN3R
#endif
#ifndef BANK8BTN4P
#define BANK8BTN4P
#endif
#ifndef BANK8BTN4R
#define BANK8BTN4R
#endif
#ifndef BANK8BTN5P
#define BANK8BTN5P
#endif
#ifndef BANK8BTN5R
#define BANK8BTN5R
#endif
#ifndef BANK8BTN6P
#define BANK8BTN6P
#endif
#ifndef BANK8BTN6R
#define BANK8BTN6R
#endif
#ifndef BANK8BTN7P
#define BANK8BTN7P
#endif
#ifndef BANK8BTN7R
#define BANK8BTN7R
#endif
#ifndef BANK8BTN8P
#define BANK8BTN8P
#endif
#ifndef BANK8BTN8R
#define BANK8BTN8R
#endif
#ifndef BANK8BTN9P
#define BANK8BTN9P
#endif
#ifndef BANK8BTN9R
#define BANK8BTN9R
#endif
#ifndef BANK8BTN10P
#define BANK8BTN10P
#endif
#ifndef BANK8BTN10R
#define BANK8BTN10R
#endif
#ifndef BANK8BTN11P
#define BANK8BTN11P
#endif
#ifndef BANK8BTN11R
#define BANK8BTN11R
#endif
#ifndef BANK8BTN12P
#define BANK8BTN12P
#endif
#ifndef BANK8BTN12R
#define BANK8BTN12R
#endif
#ifndef BANK8BTN13P
#define BANK8BTN13P
#endif
#ifndef BANK8BTN13R
#define BANK8BTN13R
#endif
#ifndef BANK8BTN14P
#define BANK8BTN14P
#endif
#ifndef BANK8BTN14R
#define BANK8BTN14R
#endif
#ifndef BANK8BTN15P
#define BANK8BTN15P
#endif
#ifndef BANK8BTN15R
#define BANK8BTN15R
#endif
//Bank 9
#ifndef BANK9BTN0P
#define BANK9BTN0P
#endif
#ifndef BANK9BTN0R
#define BANK9BTN0R
#endif
#ifndef BANK9BTN1P
#define BANK9BTN1P
#endif
#ifndef BANK9BTN1R
#define BANK9BTN1R
#endif
#ifndef BANK9BTN2P
#define BANK9BTN2P
#endif
#ifndef BANK9BTN2R
#define BANK9BTN2R
#endif
#ifndef BANK9BTN3P
#define BANK9BTN3P
#endif
#ifndef BANK9BTN3R
#define BANK9BTN3R
#endif
#ifndef BANK9BTN4P
#define BANK9BTN4P
#endif
#ifndef BANK9BTN4R
#define BANK9BTN4R
#endif
#ifndef BANK9BTN5P
#define BANK9BTN5P
#endif
#ifndef BANK9BTN5R
#define BANK9BTN5R
#endif
#ifndef BANK9BTN6P
#define BANK9BTN6P
#endif
#ifndef BANK9BTN6R
#define BANK9BTN6R
#endif
#ifndef BANK9BTN7P
#define BANK9BTN7P
#endif
#ifndef BANK9BTN7R
#define BANK9BTN7R
#endif
#ifndef BANK9BTN8P
#define BANK9BTN8P
#endif
#ifndef BANK9BTN8R
#define BANK9BTN8R
#endif
#ifndef BANK9BTN9P
#define BANK9BTN9P
#endif
#ifndef BANK9BTN9R
#define BANK9BTN9R
#endif
#ifndef BANK9BTN10P
#define BANK9BTN10P
#endif
#ifndef BANK9BTN10R
#define BANK9BTN10R
#endif
#ifndef BANK9BTN11P
#define BANK9BTN11P
#endif
#ifndef BANK9BTN11R
#define BANK9BTN11R
#endif
#ifndef BANK9BTN12P
#define BANK9BTN12P
#endif
#ifndef BANK9BTN12R
#define BANK9BTN12R
#endif
#ifndef BANK9BTN13P
#define BANK9BTN13P
#endif
#ifndef BANK9BTN13R
#define BANK9BTN13R
#endif
#ifndef BANK9BTN14P
#define BANK9BTN14P
#endif
#ifndef BANK9BTN14R
#define BANK9BTN14R
#endif
#ifndef BANK9BTN15P
#define BANK9BTN15P
#endif
#ifndef BANK9BTN15R
#define BANK9BTN15R
#endif
//Bank 10
#ifndef BANK10BTN0P
#define BANK10BTN0P
#endif
#ifndef BANK10BTN0R
#define BANK10BTN0R
#endif
#ifndef BANK10BTN1P
#define BANK10BTN1P
#endif
#ifndef BANK10BTN1R
#define BANK10BTN1R
#endif
#ifndef BANK10BTN2P
#define BANK10BTN2P
#endif
#ifndef BANK10BTN2R
#define BANK10BTN2R
#endif
#ifndef BANK10BTN3P
#define BANK10BTN3P
#endif
#ifndef BANK10BTN3R
#define BANK10BTN3R
#endif
#ifndef BANK10BTN4P
#define BANK10BTN4P
#endif
#ifndef BANK10BTN4R
#define BANK10BTN4R
#endif
#ifndef BANK10BTN5P
#define BANK10BTN5P
#endif
#ifndef BANK10BTN5R
#define BANK10BTN5R
#endif
#ifndef BANK10BTN6P
#define BANK10BTN6P
#endif
#ifndef BANK10BTN6R
#define BANK10BTN6R
#endif
#ifndef BANK10BTN7P
#define BANK10BTN7P
#endif
#ifndef BANK10BTN7R
#define BANK10BTN7R
#endif
#ifndef BANK10BTN8P
#define BANK10BTN8P
#endif
#ifndef BANK10BTN8R
#define BANK10BTN8R
#endif
#ifndef BANK10BTN9P
#define BANK10BTN9P
#endif
#ifndef BANK10BTN9R
#define BANK10BTN9R
#endif
#ifndef BANK10BTN10P
#define BANK10BTN10P
#endif
#ifndef BANK10BTN10R
#define BANK10BTN10R
#endif
#ifndef BANK10BTN11P
#define BANK10BTN11P
#endif
#ifndef BANK10BTN11R
#define BANK10BTN11R
#endif
#ifndef BANK10BTN12P
#define BANK10BTN12P
#endif
#ifndef BANK10BTN12R
#define BANK10BTN12R
#endif
#ifndef BANK10BTN13P
#define BANK10BTN13P
#endif
#ifndef BANK10BTN13R
#define BANK10BTN13R
#endif
#ifndef BANK10BTN14P
#define BANK10BTN14P
#endif
#ifndef BANK10BTN14R
#define BANK10BTN14R
#endif
#ifndef BANK10BTN15P
#define BANK10BTN15P
#endif
#ifndef BANK10BTN15R
#define BANK10BTN15R
#endif
//Bank 11
#ifndef BANK11BTN0P
#define BANK11BTN0P
#endif
#ifndef BANK11BTN0R
#define BANK11BTN0R
#endif
#ifndef BANK11BTN1P
#define BANK11BTN1P
#endif
#ifndef BANK11BTN1R
#define BANK11BTN1R
#endif
#ifndef BANK11BTN2P
#define BANK11BTN2P
#endif
#ifndef BANK11BTN2R
#define BANK11BTN2R
#endif
#ifndef BANK11BTN3P
#define BANK11BTN3P
#endif
#ifndef BANK11BTN3R
#define BANK11BTN3R
#endif
#ifndef BANK11BTN4P
#define BANK11BTN4P
#endif
#ifndef BANK11BTN4R
#define BANK11BTN4R
#endif
#ifndef BANK11BTN5P
#define BANK11BTN5P
#endif
#ifndef BANK11BTN5R
#define BANK11BTN5R
#endif
#ifndef BANK11BTN6P
#define BANK11BTN6P
#endif
#ifndef BANK11BTN6R
#define BANK11BTN6R
#endif
#ifndef BANK11BTN7P
#define BANK11BTN7P
#endif
#ifndef BANK11BTN7R
#define BANK11BTN7R
#endif
#ifndef BANK11BTN8P
#define BANK11BTN8P
#endif
#ifndef BANK11BTN8R
#define BANK11BTN8R
#endif
#ifndef BANK11BTN9P
#define BANK11BTN9P
#endif
#ifndef BANK11BTN9R
#define BANK11BTN9R
#endif
#ifndef BANK11BTN10P
#define BANK11BTN10P
#endif
#ifndef BANK11BTN10R
#define BANK11BTN10R
#endif
#ifndef BANK11BTN11P
#define BANK11BTN11P
#endif
#ifndef BANK11BTN11R
#define BANK11BTN11R
#endif
#ifndef BANK11BTN12P
#define BANK11BTN12P
#endif
#ifndef BANK11BTN12R
#define BANK11BTN12R
#endif
#ifndef BANK11BTN13P
#define BANK11BTN13P
#endif
#ifndef BANK11BTN13R
#define BANK11BTN13R
#endif
#ifndef BANK11BTN14P
#define BANK11BTN14P
#endif
#ifndef BANK11BTN14R
#define BANK11BTN14R
#endif
#ifndef BANK11BTN15P
#define BANK11BTN15P
#endif
#ifndef BANK11BTN15R
#define BANK11BTN15R
#endif
//Bank 12
#ifndef BANK12BTN0P
#define BANK12BTN0P
#endif
#ifndef BANK12BTN0R
#define BANK12BTN0R
#endif
#ifndef BANK12BTN1P
#define BANK12BTN1P
#endif
#ifndef BANK12BTN1R
#define BANK12BTN1R
#endif
#ifndef BANK12BTN2P
#define BANK12BTN2P
#endif
#ifndef BANK12BTN2R
#define BANK12BTN2R
#endif
#ifndef BANK12BTN3P
#define BANK12BTN3P
#endif
#ifndef BANK12BTN3R
#define BANK12BTN3R
#endif
#ifndef BANK12BTN4P
#define BANK12BTN4P
#endif
#ifndef BANK12BTN4R
#define BANK12BTN4R
#endif
#ifndef BANK12BTN5P
#define BANK12BTN5P
#endif
#ifndef BANK12BTN5R
#define BANK12BTN5R
#endif
#ifndef BANK12BTN6P
#define BANK12BTN6P
#endif
#ifndef BANK12BTN6R
#define BANK12BTN6R
#endif
#ifndef BANK12BTN7P
#define BANK12BTN7P
#endif
#ifndef BANK12BTN7R
#define BANK12BTN7R
#endif
#ifndef BANK12BTN8P
#define BANK12BTN8P
#endif
#ifndef BANK12BTN8R
#define BANK12BTN8R
#endif
#ifndef BANK12BTN9P
#define BANK12BTN9P
#endif
#ifndef BANK12BTN9R
#define BANK12BTN9R
#endif
#ifndef BANK12BTN10P
#define BANK12BTN10P
#endif
#ifndef BANK12BTN10R
#define BANK12BTN10R
#endif
#ifndef BANK12BTN11P
#define BANK12BTN11P
#endif
#ifndef BANK12BTN11R
#define BANK12BTN11R
#endif
#ifndef BANK12BTN12P
#define BANK12BTN12P
#endif
#ifndef BANK12BTN12R
#define BANK12BTN12R
#endif
#ifndef BANK12BTN13P
#define BANK12BTN13P
#endif
#ifndef BANK12BTN13R
#define BANK12BTN13R
#endif
#ifndef BANK12BTN14P
#define BANK12BTN14P
#endif
#ifndef BANK12BTN14R
#define BANK12BTN14R
#endif
#ifndef BANK12BTN15P
#define BANK12BTN15P
#endif
#ifndef BANK12BTN15R
#define BANK12BTN15R
#endif
//Bank 13
#ifndef BANK13BTN0P
#define BANK13BTN0P
#endif
#ifndef BANK13BTN0R
#define BANK13BTN0R
#endif
#ifndef BANK13BTN1P
#define BANK13BTN1P
#endif
#ifndef BANK13BTN1R
#define BANK13BTN1R
#endif
#ifndef BANK13BTN2P
#define BANK13BTN2P
#endif
#ifndef BANK13BTN2R
#define BANK13BTN2R
#endif
#ifndef BANK13BTN3P
#define BANK13BTN3P
#endif
#ifndef BANK13BTN3R
#define BANK13BTN3R
#endif
#ifndef BANK13BTN4P
#define BANK13BTN4P
#endif
#ifndef BANK13BTN4R
#define BANK13BTN4R
#endif
#ifndef BANK13BTN5P
#define BANK13BTN5P
#endif
#ifndef BANK13BTN5R
#define BANK13BTN5R
#endif
#ifndef BANK13BTN6P
#define BANK13BTN6P
#endif
#ifndef BANK13BTN6R
#define BANK13BTN6R
#endif
#ifndef BANK13BTN7P
#define BANK13BTN7P
#endif
#ifndef BANK13BTN7R
#define BANK13BTN7R
#endif
#ifndef BANK13BTN8P
#define BANK13BTN8P
#endif
#ifndef BANK13BTN8R
#define BANK13BTN8R
#endif
#ifndef BANK13BTN9P
#define BANK13BTN9P
#endif
#ifndef BANK13BTN9R
#define BANK13BTN9R
#endif
#ifndef BANK13BTN10P
#define BANK13BTN10P
#endif
#ifndef BANK13BTN10R
#define BANK13BTN10R
#endif
#ifndef BANK13BTN11P
#define BANK13BTN11P
#endif
#ifndef BANK13BTN11R
#define BANK13BTN11R
#endif
#ifndef BANK13BTN12P
#define BANK13BTN12P
#endif
#ifndef BANK13BTN12R
#define BANK13BTN12R
#endif
#ifndef BANK13BTN13P
#define BANK13BTN13P
#endif
#ifndef BANK13BTN13R
#define BANK13BTN13R
#endif
#ifndef BANK13BTN14P
#define BANK13BTN14P
#endif
#ifndef BANK13BTN14R
#define BANK13BTN14R
#endif
#ifndef BANK13BTN15P
#define BANK13BTN15P
#endif
#ifndef BANK13BTN15R
#define BANK13BTN15R
#endif
//Bank 14
#ifndef BANK14BTN0P
#define BANK14BTN0P
#endif
#ifndef BANK14BTN0R
#define BANK14BTN0R
#endif
#ifndef BANK14BTN1P
#define BANK14BTN1P
#endif
#ifndef BANK14BTN1R
#define BANK14BTN1R
#endif
#ifndef BANK14BTN2P
#define BANK14BTN2P
#endif
#ifndef BANK14BTN2R
#define BANK14BTN2R
#endif
#ifndef BANK14BTN3P
#define BANK14BTN3P
#endif
#ifndef BANK14BTN3R
#define BANK14BTN3R
#endif
#ifndef BANK14BTN4P
#define BANK14BTN4P
#endif
#ifndef BANK14BTN4R
#define BANK14BTN4R
#endif
#ifndef BANK14BTN5P
#define BANK14BTN5P
#endif
#ifndef BANK14BTN5R
#define BANK14BTN5R
#endif
#ifndef BANK14BTN6P
#define BANK14BTN6P
#endif
#ifndef BANK14BTN6R
#define BANK14BTN6R
#endif
#ifndef BANK14BTN7P
#define BANK14BTN7P
#endif
#ifndef BANK14BTN7R
#define BANK14BTN7R
#endif
#ifndef BANK14BTN8P
#define BANK14BTN8P
#endif
#ifndef BANK14BTN8R
#define BANK14BTN8R
#endif
#ifndef BANK14BTN9P
#define BANK14BTN9P
#endif
#ifndef BANK14BTN9R
#define BANK14BTN9R
#endif
#ifndef BANK14BTN10P
#define BANK14BTN10P
#endif
#ifndef BANK14BTN10R
#define BANK14BTN10R
#endif
#ifndef BANK14BTN11P
#define BANK14BTN11P
#endif
#ifndef BANK14BTN11R
#define BANK14BTN11R
#endif
#ifndef BANK14BTN12P
#define BANK14BTN12P
#endif
#ifndef BANK14BTN12R
#define BANK14BTN12R
#endif
#ifndef BANK14BTN13P
#define BANK14BTN13P
#endif
#ifndef BANK14BTN13R
#define BANK14BTN13R
#endif
#ifndef BANK14BTN14P
#define BANK14BTN14P
#endif
#ifndef BANK14BTN14R
#define BANK14BTN14R
#endif
#ifndef BANK14BTN15P
#define BANK14BTN15P
#endif
#ifndef BANK14BTN15R
#define BANK14BTN15R
#endif
//Bank 15
#ifndef BANK15BTN0P
#define BANK15BTN0P
#endif
#ifndef BANK15BTN0R
#define BANK15BTN0R
#endif
#ifndef BANK15BTN1P
#define BANK15BTN1P
#endif
#ifndef BANK15BTN1R
#define BANK15BTN1R
#endif
#ifndef BANK15BTN2P
#define BANK15BTN2P
#endif
#ifndef BANK15BTN2R
#define BANK15BTN2R
#endif
#ifndef BANK15BTN3P
#define BANK15BTN3P
#endif
#ifndef BANK15BTN3R
#define BANK15BTN3R
#endif
#ifndef BANK15BTN4P
#define BANK15BTN4P
#endif
#ifndef BANK15BTN4R
#define BANK15BTN4R
#endif
#ifndef BANK15BTN5P
#define BANK15BTN5P
#endif
#ifndef BANK15BTN5R
#define BANK15BTN5R
#endif
#ifndef BANK15BTN6P
#define BANK15BTN6P
#endif
#ifndef BANK15BTN6R
#define BANK15BTN6R
#endif
#ifndef BANK15BTN7P
#define BANK15BTN7P
#endif
#ifndef BANK15BTN7R
#define BANK15BTN7R
#endif
#ifndef BANK15BTN8P
#define BANK15BTN8P
#endif
#ifndef BANK15BTN8R
#define BANK15BTN8R
#endif
#ifndef BANK15BTN9P
#define BANK15BTN9P
#endif
#ifndef BANK15BTN9R
#define BANK15BTN9R
#endif
#ifndef BANK15BTN10P
#define BANK15BTN10P
#endif
#ifndef BANK15BTN10R
#define BANK15BTN10R
#endif
#ifndef BANK15BTN11P
#define BANK15BTN11P
#endif
#ifndef BANK15BTN11R
#define BANK15BTN11R
#endif
#ifndef BANK15BTN12P
#define BANK15BTN12P
#endif
#ifndef BANK15BTN12R
#define BANK15BTN12R
#endif
#ifndef BANK15BTN13P
#define BANK15BTN13P
#endif
#ifndef BANK15BTN13R
#define BANK15BTN13R
#endif
#ifndef BANK15BTN14P
#define BANK15BTN14P
#endif
#ifndef BANK15BTN14R
#define BANK15BTN14R
#endif
#ifndef BANK15BTN15P
#define BANK15BTN15P
#endif
#ifndef BANK15BTN15R
#define BANK15BTN15R
#endif
