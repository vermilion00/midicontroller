#include <EEPROM.h>
#include <MIDI.h> //V5.0.2
#include <FastLED.h> //V3.9.13
#include <Bugtton.h> //V1.0.6
#include "QCMIDI.h"
#include "config.h"
#include "footswitches.h"
#include "actions.h"

/*TODO:
 * Add momentary mode (special action) for switching presets (like one shot in QMK)
 * Allow momentary LED mode for single switches? Not much point otherwise
 * Fix Switches/LEDs getting stuck occasionally
 * Clean up and generalize code
 * Simplify footswitch assignments to an array to make calling easier?
 * Maybe even a 2D array where rows are banks and cols are footswitches in that bank?
 * Would probs make individual footswitch led modes easier, also the code more compact
 * Allow for held combos (shouldn't be too difficult)
 * Make special button actions assignable
 */

MIDI_CREATE_DEFAULT_INSTANCE();

void action(uint8_t i, uint8_t bank, bool state);

#ifdef SPECIAL_BUTTON
const int8_t buttonPins[FOOTSWITCH_NUM + 1] = {FOOTSWITCH_PINS,SPECIAL_PIN};
Bugtton buttons(FOOTSWITCH_NUM + 1, buttonPins, DEBOUNCE);
bool special_button_held = false;
#else
const int8_t buttonPins[FOOTSWITCH_NUM] = {FOOTSWITCH_PINS};
Bugtton buttons(FOOTSWITCH_NUM, buttonPins, DEBOUNCE);
#endif
const uint8_t footswitch_mode[FOOTSWITCH_NUM] = {FOOTSWITCH_MODES};
bool swap_pedals = false;

const uint8_t led_order[LED_NUM] = LED_ORDER;
CRGB leds[LED_NUM];
#ifndef NO_LED
void updateLEDs(void);
uint8_t buttonToLed(uint8_t);
#endif

#if defined EXP1_PIN || EXP2_PIN
bool exp1_connected = false;
bool exp2_connected = false;
uint16_t exp1_val = 0;
uint16_t conv1_val = 0;
uint16_t prev1_val = 0;
uint16_t exp2_val = 0;
uint16_t conv2_val = 0;
uint16_t prev2_val = 0;
uint16_t exp1_upper = 0;
uint16_t exp1_lower = 0;
uint16_t exp2_upper = 0;
uint16_t exp2_lower = 0;
void calibrateExp(uint8_t pedal);
uint8_t convertVal1(uint16_t raw_val);
uint8_t convertVal2(uint16_t raw_val);
#endif

bool ledState[LED_NUM];
bool ledStatePrev[LED_NUM];
uint8_t bank = 0;  
bool gig_open = false;
bool tuner_open = false;
bool bank_changed = false;
#ifdef SPECIAL_PIN
bool btn_held[FOOTSWITCH_NUM + 1];
#else
bool btn_held[FOOTSWITCH_NUM];
#endif
bool special_action = false;
bool any_button_held = false;
bool updateLEDflag = false;
bool turn_off_leds = false;
uint8_t bank_led_mode[16] = {BANK0_LED_MODE, BANK1_LED_MODE, BANK2_LED_MODE, BANK3_LED_MODE, BANK4_LED_MODE, BANK5_LED_MODE, BANK6_LED_MODE, BANK7_LED_MODE, BANK8_LED_MODE, BANK9_LED_MODE, BANK10_LED_MODE, BANK11_LED_MODE, BANK12_LED_MODE, BANK13_LED_MODE, BANK14_LED_MODE, BANK15_LED_MODE};

void setup(){
  #ifndef NO_LED
  FastLED.addLeds<LED_TYPE, DATA_PIN, SUBPIXEL_ORDER>(leds, LED_NUM);
  FastLED.setBrightness(LED_BRIGHTNESS);
  #endif
  //Serial.begin(38400);
  MIDI.begin(MIDI_CHANNEL_OMNI);
  //Check if expression pedals are connected
  #ifdef EXP1_PIN
  //Set ADC prescaler to lower value
  ADCSRA = (ADCSRA & B11111000) | 4;
  //Dummy analog conversion
  exp1_val = analogRead(EXP1_PIN);
  exp1_val = analogRead(EXP1_PIN);
  if(exp1_val > EXP_THRESHOLD){
    exp1_connected = true;
    //Start calibration if calibration key is held at startup
    if(!digitalRead(buttonPins[EXP1_CALIBRATION_KEY])){
      bool released_startup = false;
      #ifndef NO_LED
      for(uint8_t i = 0; i < LED_NUM; i++){
        ledState[i] = true;
      }
      updateLEDs();
      #endif //ifndef NO_LED
      while(!released_startup){
        released_startup = true;
        if(!digitalRead(buttonPins[EXP1_CALIBRATION_KEY])){
          released_startup = false;
        }
      }
      calibrateExp(1); 
    #ifndef NO_EEPROM
    } else {
      //Read limits from EEPROM if no calibration happened
      exp1_upper = EEPROM.read(0) << 8 | EEPROM.read(1);
      exp1_lower = EEPROM.read(2) << 8 | EEPROM.read(3);
    #endif //ifndef NO_EEPROM
    }
  }
  #endif
  #ifdef EXP2_PIN
  //Set ADC prescaler to lower value
  ADCSRA = (ADCSRA & B11111000) | 4;
  //Dummy analog conversion
  exp2_val = analogRead(EXP2_PIN);
  exp2_val = analogRead(EXP2_PIN);
  if(exp2_val > EXP_THRESHOLD){
    exp2_connected = true;
    //Start calibration if calibration key is held at startup
    if(!digitalRead(buttonPins[EXP2_CALIBRATION_KEY])){
      bool released_startup = false;
      #ifndef NO_LED
      for(uint8_t i = 0; i < LED_NUM; i++){
        ledState[i] = true;
      }
      updateLEDs();
      #endif //ifndef NO_LED
      while(!released_startup){ 
        released_startup = true; 
        if(!digitalRead(buttonPins[EXP2_CALIBRATION_KEY])){ 
          released_startup = false;
        } 
      }
      calibrateExp(2); 
    #ifndef NO_EEPROM
    } else {
      //Read limits from EEPROM if no calibration happened
      //TODO: Read saved limits from EEPROM
      exp2_upper = EEPROM.read(4) << 8 | EEPROM.read(5);
      exp2_lower = EEPROM.read(6) << 8 | EEPROM.read(7);
    #endif //ifndef NO_EEPROM
    }
  }
  #endif
  //Turn on all LEDs at the start if the mode is set to LED_ALL, LED_EXP1 or LED_EXP2
  #ifndef NO_LED
  #if BANK0_LED_MODE == LED_ALL || BANK0_LED_MODE == LED_EXP1 || BANK0_LED_MODE == LED_EXP2
  for(uint8_t i = 0; i < FOOTSWITCH_NUM; i++){
    ledState[i] = true;
  }
  #else
  turn_off_leds = true;
  updateLEDs();
  #endif
  #endif
}

void loop(){
  //Update buttons
  buttons.update();

  /*BEGIN SPECIAL BUTTON ACTIONS*/
  #ifdef SPECIAL_BUTTON
  if(buttons.heldUntil(FOOTSWITCH_NUM, HOLD_DURATION)){
    special_button_held = true;
    btn_held[FOOTSWITCH_NUM] = true;
    SPECIAL_HOLD_ACTION
    // bank++;
    // bank_changed = true;
    // #ifndef NO_LED
    // updateLEDflag = true;
    // #endif
    
    // //Reset bank if bank bound has been reached
    // if(bank >= BANKS){
    //   bank = 0;
    // }
    
    // //Change QC mode
    // switch(bank){
    //   //Looper = Scene mode
    //   case 0:
    //     BANK0_MODE;
    //     break;
    //   //Scene Mode
    //   case 1:
    //     BANK1_MODE;
    //     break;
    //   //Preset Mode
    //   case 2:
    //     BANK2_MODE;
    //     break;
    //   //Stomp Mode
    //   case 3:
    //     BANK3_MODE;
    //     break;
    //   #if BANKS > 4
    //   case 4:
    //     BANK4_MODE;
    //     break;
    //   #endif
    //   #if BANKS > 5
    //   case 5:
    //     BANK5_MODE;
    //     break;
    //   #endif
    //   #if BANKS > 6
    //   case 6:
    //     BANK6_MODE;
    //     break;
    //   #endif
    //   #if BANKS > 7
    //   case 7:
    //     BANK7_MODE;
    //     break;
    //   #endif
    //   #if BANKS > 8
    //   case 8:
    //     BANK8_MODE;
    //     break;
    //   #endif
    //   #if BANKS > 9
    //   case 9:
    //     BANK4_MODE;
    //     break;
    //   #endif
    //   #if BANKS > 10
    //   case 10:
    //     BANK4_MODE;
    //     break;
    //   #endif
    //   #if BANKS > 11
    //   case 11:
    //     BANK11_MODE;
    //     break;
    //   #endif
    //   #if BANKS > 12
    //   case 12:
    //     BANK12_MODE;
    //     break;
    //   #endif
    //   #if BANKS > 13
    //   case 13:
    //     BANK13_MODE;
    //     break;
    //   #endif
    //   #if BANKS > 14
    //   case 14:
    //     BANK14_MODE;
    //     break;
    //   #endif
    //   #if BANKS > 15
    //   case 15:
    //     BANK15_MODE;
    //     break;
    //   #endif
    // }
    
    //Button released after bank change
  } else if(buttons.rose(FOOTSWITCH_NUM)){
    if(special_button_held){
      btn_held[FOOTSWITCH_NUM] = false;
      special_button_held = false;
      SPECIAL_HOLD_ACTION_RELEASE
    // if(bank_changed){
    //   bank_changed = false;
    //   updateLEDflag = true;
    //   if(bank_led_mode[bank] != LED_ALL && bank_led_mode[bank] != LED_EXP1 && bank_led_mode[bank] != LED_EXP2){
    //     turn_off_leds = true;
    //   }
      
    //Button pressed normally
    } else {
      btn_held[FOOTSWITCH_NUM] = false;
      SPECIAL_PRESS_ACTION
    }
  }
  #endif //SPECIAL_BUTTON
  /*END SPECIAL BUTTON ACTIONS*/

  //Call footswitch actions
  for(uint8_t i = 0; i < FOOTSWITCH_NUM; i++){
    switch(footswitch_mode[i]){
      case SPECIAL:
        //Since this is the special mode, only call action on release
        if(buttons.fell(i)){
          btn_held[i] = true;
          if(bank_led_mode[bank] == LED_MOMENTARY){
            ledState[i] = true;
            updateLEDflag = true;
          }
          updateLEDflag = true;
        } else if(buttons.rose(i)){
          btn_held[i] = false;
          if(bank_led_mode[bank] == LED_MOMENTARY){
            ledState[i] = false;
            updateLEDflag = true;
          }
          //Call the press action if the switch is released without special action
          if(!special_action){
            if(bank_led_mode[bank] == LED_MOMENTARY){
              //Is translated to press action in the action call
              action(i, bank, RELEASE);
            } else {
              action(i, bank, PRESS);
            }
            updateLEDflag = true;
          }
        }
        break;
        
      default:
        //Switch set to momentary mode, call actions on press and release
        if(buttons.fell(i)){
          action(i, bank, PRESS);
          updateLEDflag = true;
        } else if(buttons.rose(i)){
          action(i, bank, RELEASE);
          if(bank_led_mode[bank] == LED_MOMENTARY){
            updateLEDflag = true;
          }
        }
        break;
    }
  }
  
  //Check if special actions need to be called
  //Horizontal combos
  if(btn_held[0] && btn_held[1]){
    SPECIAL_0_1
  } else if(btn_held[1] && btn_held[2]){
    SPECIAL_1_2
  } else if(btn_held[2] && btn_held[3]){
    SPECIAL_2_3
  } else if(btn_held[3] && btn_held[4]){
    SPECIAL_3_4
  } else if(btn_held[4] && btn_held[5]){
    SPECIAL_4_5
  } else if(btn_held[5] && btn_held[6]){
    SPECIAL_5_6
  } else if(btn_held[6] && btn_held[7]){
    SPECIAL_6_7
  #if FOOTSWITCH_NUM > 8
  } else if(btn_held[7] && btn_held[8]){
    SPECIAL_7_8
  } else if(btn_held[8] && btn_held[9]){
    SPECIAL_8_9
  #endif
  #if FOOTSWITCH_NUM > 10
  } else if(btn_held[9] && btn_held[10]){
    SPECIAL_9_10
  } else if(btn_held[10] && btn_held[11]){
    SPECIAL_10_11
  #endif
  #if FOOTSWITCH_NUM > 12
  } else if(btn_held[11] && btn_held[12]){
    SPECIAL_11_12
  } else if(btn_held[12] && btn_held[13]){
    SPECIAL_12_13
  #endif
  #if FOOTSWITCH_NUM > 14
  } else if(btn_held[13] && btn_held[14]){
    SPECIAL_13_14
  } else if(btn_held[14] && btn_held[15]){
    SPECIAL_14_15
  #endif
  
  //Vertical combos
  #if FOOTSWITCH_NUM == 4
  } else if(btn_held[0] && btn_held[2]){
    SPECIAL_0_2
  } else if(btn_held[1] && btn_held[3]){
    SPECIAL_1_3
  #elif FOOTSWITCH_NUM == 6
  } else if(btn_held[0] && btn_held[3]){
    SPECIAL_0_3
  } else if(btn_held[1] && btn_held[4]){
    SPECIAL_1_4
  } else if(btn_held[2] && btn_held[5]){
    SPECIAL_2_5
  #elif FOOTSWITCH_NUM == 8
  } else if(btn_held[0] && btn_held[4]){
    SPECIAL_0_4
  } else if(btn_held[1] && btn_held[5]){
    SPECIAL_1_5
  } else if(btn_held[2] && btn_held[6]){
    SPECIAL_2_6
  } else if(btn_held[3] && btn_held[7]){
    SPECIAL_3_7  
  #elif FOOTSWITCH_NUM == 10
  } else if(btn_held[0] && btn_held[5]){
    SPECIAL_0_5
  } else if(btn_held[1] && btn_held[6]){
    SPECIAL_1_6
  } else if(btn_held[2] && btn_held[7]){
    SPECIAL_2_7
  } else if(btn_held[3] && btn_held[8]){
    SPECIAL_3_8
  } else if(btn_held[4] && btn_held[9]){
    SPECIAL_4_9
  #elif FOOTSWITCH_NUM == 12
  } else if(btn_held[0] && btn_held[6]){
    SPECIAL_0_6
  } else if(btn_held[1] && btn_held[7]){
    SPECIAL_1_7
  } else if(btn_held[2] && btn_held[8]){
    SPECIAL_2_8
  } else if(btn_held[3] && btn_held[9]){
    SPECIAL_3_9
  } else if(btn_held[4] && btn_held[10]){
    SPECIAL_4_10
  } else if(btn_held[5] && btn_held[11]){
    SPECIAL_5_11
  #elif FOOTSWITCH_NUM == 14
  } else if(btn_held[0] && btn_held[7]){
    SPECIAL_0_7
  } else if(btn_held[1] && btn_held[8]){
    SPECIAL_1_8
  } else if(btn_held[2] && btn_held[9]){
    SPECIAL_2_9
  } else if(btn_held[3] && btn_held[10]){
    SPECIAL_3_10
  } else if(btn_held[4] && btn_held[11]){
    SPECIAL_4_11
  } else if(btn_held[5] && btn_held[12]){
    SPECIAL_5_12
  } else if(btn_held[6] && btn_held[13]){
    SPECIAL_6_13
  #elif FOOTSWITCH_NUM == 16
  } else if(btn_held[0] && btn_held[8]){
    SPECIAL_0_8
  } else if(btn_held[1] && btn_held[9]){
    SPECIAL_1_9
  } else if(btn_held[2] && btn_held[10]){
    SPECIAL_2_10
  } else if(btn_held[3] && btn_held[11]){
    SPECIAL_3_11
  } else if(btn_held[4] && btn_held[12]){
    SPECIAL_4_12
  } else if(btn_held[5] && btn_held[13]){
    SPECIAL_5_13
  } else if(btn_held[6] && btn_held[14]){
    SPECIAL_6_14
  } else if(btn_held[7] && btn_held[15]){
    SPECIAL_7_15
  #endif
  }

  //Check if any button is pressed
  #ifdef SPECIAL_PIN
  for(uint8_t i = 0; i < FOOTSWITCH_NUM + 1; i++){
    if(btn_held[i]){
      any_button_held = true;
    }
  }
  #else
  for(uint8_t i = 0; i < FOOTSWITCH_NUM; i++){
    if(btn_held[i]){
      any_button_held = true;
    }
  }
  #endif
  //Reset special actions
  if(bank_changed && !any_button_held){
    bank_changed = false;
    updateLEDflag = true;
    if(bank_led_mode[bank] != LED_ALL && bank_led_mode[bank] != LED_EXP1 && bank_led_mode[bank] != LED_EXP2){
      turn_off_leds = true;
    }
  } 
  if(special_action && !any_button_held){
    special_action = false;
  }
  any_button_held = false;
  
  //Update Expression Pedals
  #ifdef EXP1_PIN
  if(exp1_connected){
    exp1_val = analogRead(EXP1_PIN);
    conv1_val = convertVal1(exp1_val);
    if(prev1_val != conv1_val){
      prev1_val = conv1_val;
      if(!swap_pedals){
        EXP_PEDAL_1(conv1_val);
      } else {
        EXP_PEDAL_2(conv1_val);
      }
      //Update LED brightness based on EXP1 position
      #ifndef NO_LED
        if(bank_led_mode[bank] == LED_EXP1){
          FastLED.setBrightness(map(exp1_val, exp1_lower, exp1_upper, MIN_BRIGHTNESS, MAX_BRIGHTNESS));
          updateLEDflag = true;
        }
      #endif //ifndef NO_LED
    }
  }
  #endif //ifdef EXP1_PIN
  #ifdef EXP2_PIN
  if(exp2_connected){
    exp2_val = analogRead(EXP2_PIN);
    conv2_val = convertVal2(exp2_val);
    if(prev2_val != conv2_val){
      prev2_val = conv2_val;
      if(!swap_pedals){
        EXP_PEDAL_2(conv2_val);
      } else {
        EXP_PEDAL_1(conv2_val);
      }
      //Update LED brightness based on EXP2 position
      #ifndef NO_LED
        if(bank_led_mode[bank] == LED_EXP2){
          FastLED.setBrightness(map(exp2_val, exp2_lower, exp2_upper, MIN_BRIGHTNESS, MAX_BRIGHTNESS));
          updateLEDflag = true;
        }
      #endif //ifndef NO_LED
    }
  }
  #endif //ifdef EXP2_PIN
  
  //Update LEDs
  #ifndef NO_LED
  if(updateLEDflag){
    updateLEDs();
    updateLEDflag = false;
  }
  #endif
}

#ifndef NO_LED
void updateLEDs(void){
  if(bank_changed){
    //Set led brightness to the normal brightness if not in EXP mode
    if((bank_led_mode[bank] != LED_EXP1) && (bank_led_mode[bank] != LED_EXP2)){
      FastLED.setBrightness(LED_BRIGHTNESS);
    #ifdef EXP1_PIN
    //Set brightness according to EXP1 position when bank is changed
    } else if(bank_led_mode[bank] == LED_EXP1){
      exp1_val = analogRead(EXP1_PIN);
      exp1_val = constrain(exp1_val, exp1_lower, exp1_upper);
      FastLED.setBrightness(map(exp1_val, exp1_lower, exp1_upper, MIN_BRIGHTNESS, MAX_BRIGHTNESS));
      FastLED.show();
    #endif
    #ifdef EXP2_PIN
    //Set brightness according to EXP2 position when bank is changed
    } else if(bank_led_mode[bank] == LED_EXP2){
      exp2_val = analogRead(EXP2_PIN);
      exp2_val = constrain(exp2_val, exp2_lower, exp2_upper);
      FastLED.setBrightness(map(exp2_val, exp2_lower, exp2_upper, MIN_BRIGHTNESS, MAX_BRIGHTNESS));
      FastLED.show();
    #endif
    }
    for(short i = 0; i < LED_NUM; i++){
      ledState[i] = true;
    }
  } else if(turn_off_leds){
    for(short i = 0; i < LED_NUM; i++){
      ledState[i] = false;
    }
    turn_off_leds = false;
  }
  for(short i = 0; i < LED_NUM; i++){
    if(ledState[i]){
      switch(bank){
        case 0:
          leds[led_order[i]] = CRGB::BANK0_COLOR;
          break;
        case 1:
          leds[led_order[i]] = CRGB::BANK1_COLOR;
          break;
        case 2:
          leds[led_order[i]] = CRGB::BANK2_COLOR;
          break;
        #if BANKS > 3
        case 3:
          leds[led_order[i]] = CRGB::BANK3_COLOR;
          break;
        #endif
        #if BANKS > 4
        case 4:
          leds[led_order[i]] = CRGB::BANK4_COLOR;
          break;
        #endif
        #if BANKS > 5
        case 5:
          leds[led_order[i]] = CRGB::BANK5_COLOR;
          break;
        #endif
        #if BANKS > 6
        case 6:
          leds[led_order[i]] = CRGB::BANK6_COLOR;
          break;
        #endif
        #if BANKS > 7
        case 7:
          leds[led_order[i]] = CRGB::BANK7_COLOR;
          break;
        #endif
        #if BANKS > 8
        case 8:
          leds[led_order[i]] = CRGB::BANK8_COLOR;
          break;
        #endif
        #if BANKS > 9
        case 9:
          leds[led_order[i]] = CRGB::BANK9_COLOR;
          break;
        #endif
        #if BANKS > 10
        case 10:
          leds[led_order[i]] = CRGB::BANK10_COLOR;
          break;
        #endif
        #if BANKS > 11
        case 11:
          leds[led_order[i]] = CRGB::BANK11_COLOR;
          break;
        #endif
        #if BANKS > 12
        case 12:
          leds[led_order[i]] = CRGB::BANK12_COLOR;
          break;
        #endif
        #if BANKS > 13
        case 13:
          leds[led_order[i]] = CRGB::BANK13_COLOR;
          break;
        #endif
        #if BANKS > 14
        case 14:
          leds[led_order[i]] = CRGB::BANK14_COLOR;
          break;
        #endif
        #if BANKS > 15
        case 15:
          leds[led_order[i]] = CRGB::BANK15_COLOR;
          break;
        #endif
        default:
          leds[led_order[i]] = CRGB::DEFAULT_COLOR;
          break;
      }
    } else {
      leds[led_order[i]] = CRGB::Black;
    }
  }
  FastLED.show();
}
#endif //ifndef NO_LED

//Expression Pedal stuff
#ifdef EXP1_PIN
//Convert the read adc value to midi range
uint8_t convertVal1(uint16_t raw_val){
  raw_val = constrain(raw_val, exp1_lower, exp1_upper);
  return(map(raw_val, exp1_lower, exp1_upper, 0, 127));
}
#endif
#ifdef EXP2_PIN
uint8_t convertVal2(uint16_t raw_val){
  raw_val = constrain(raw_val, exp2_lower, exp2_upper);
  return(map(raw_val, exp2_lower, exp2_upper, 0, 127));
}
#endif

#if defined EXP1_PIN || EXP2_PIN
void calibrateExp(uint8_t pedal){
  #ifndef NO_LED
  //Turn on all LEDs
  for(uint8_t i = 0; i < LED_NUM; i++){
    ledState[i] = true;
  }
  updateLEDs();
  #endif
  bool calibration_finished = false;
  uint16_t val = 0;
  switch(pedal){
    case 1:
      #ifdef EXP1_PIN
      //Dummy read
      val = analogRead(EXP1_PIN);
      exp1_upper = analogRead(EXP1_PIN);
      exp1_lower = exp1_upper;
      while(!calibration_finished){
        val = analogRead(EXP1_PIN);
        if(val > exp1_upper){
          exp1_upper = val;
        }else if(val < exp1_lower){
          exp1_lower = val;
        }
        //Set LED brightness according to pedal position
        #ifndef NO_CALIBRATION_ANIMATION
        #ifndef NO_LED
        FastLED.setBrightness(map(val, exp1_lower, exp1_upper, MIN_BRIGHTNESS, MAX_BRIGHTNESS));
        FastLED.show();
        #endif //ifndef NO_LED
        //Send position over MIDI during calibration
        val = map(val, exp1_lower, exp1_upper, 0, 127);
        EXP_PEDAL_1(val);
        #endif //ifndef NO_CALIBRATION_ANIMATION
        buttons.update();
        for(uint8_t i = 0; i < FOOTSWITCH_NUM; i++){
          if(buttons.fell(i)){
            while(!calibration_finished){
              buttons.update();
              if(buttons.rose(i)){
                calibration_finished = true;
                exp1_upper -= EXP1_DEADZONE;
                exp1_lower += EXP1_DEADZONE;
                //Reset btn_held to fix an issue
                for(uint8_t j = 0; j < FOOTSWITCH_NUM; j++){
                  btn_held[j] = false;
                }
                break;
              }
            }
          }
        }
      }
      #ifndef NO_EEPROM
      #ifndef NO_EEPROM_WRITE
      EEPROM.update(0, exp1_upper >> 8);
      EEPROM.update(1, exp1_upper & 255);
      EEPROM.update(2, exp1_lower >> 8);
      EEPROM.update(3, exp1_lower & 255);
      #endif //ifndef NO_EEPROM_WRITE
      #endif //ifndef NO_EEPROM
      #endif //ifdef EXP1_PIN
      break;
    case 2:
      #ifdef EXP2_PIN
      //Dummy Read
      val = analogRead(EXP2_PIN);
      exp2_upper = analogRead(EXP2_PIN);
      exp2_lower = exp2_upper;
      while(!calibration_finished){
        val = analogRead(EXP2_PIN);
        if(val > exp2_upper){
          exp2_upper = val;
        }else if(val < exp2_lower){
          exp2_lower = val;
        }
        //Set LED brightness according to pedal position
        #ifndef NO_CALIBRATION_ANIMATION
        #ifndef NO_LED
        FastLED.setBrightness(map(val, exp2_lower, exp2_upper, MIN_BRIGHTNESS, MAX_BRIGHTNESS));
        FastLED.show();
        #endif //ifndef NO_LED
        val = map(val, exp2_lower, exp2_upper, 0, 127);
        EXP_PEDAL_2(val);
        #endif //ifndef NO_CALIBRATION_ANIMATION
        buttons.update();
        for(uint8_t i = 0; i < FOOTSWITCH_NUM; i++){
          if(buttons.fell(i)){
            while(!calibration_finished){
              buttons.update(i);
              if(buttons.rose(i)){
                calibration_finished = true;
                exp2_upper -= EXP2_DEADZONE;
                exp2_lower += EXP2_DEADZONE
                //Reset btn_held to fix an issue
                for(uint8_t j = 0; j < FOOTSWITCH_NUM; j++){
                  btn_held[j] = false;
                }
                break;
              }
            }
          }
        }
      }
      #ifndef NO_EEPROM
      #ifndef NO_EEPROM_WRITE
      EEPROM.update(4, exp2_upper >> 8);
      EEPROM.update(5, exp2_upper & 255);
      EEPROM.update(6, exp2_lower >> 8);
      EEPROM.update(7, exp2_lower & 255);
      #endif //ifndef NO_EEPROM_WRITE
      #endif //ifndef NO_EEPROM
      #endif //ifdef EXP2_PIN
      break;
  }
  #ifndef NO_LED
  //Reset LEDs to previous state
  if(bank_led_mode[bank] == LED_ALL || bank_led_mode[bank] == LED_EXP1 || bank_led_mode[bank] == LED_EXP2){
    for(uint8_t i = 0; i < LED_NUM; i++){
      ledState[i] = true;
    }
  } else {
    for(uint8_t i = 0; i < LED_NUM; i++){
      ledState[i] = ledStatePrev[i];
    }
  }
  //Reset the brightness to the default level
  if((bank_led_mode[bank] != LED_EXP1) || bank_led_mode[bank] != LED_EXP2){
    FastLED.setBrightness(LED_BRIGHTNESS);
  }
  updateLEDs();
  #endif //ifdef NO_LED
}
#endif //if defined EXP1_PIN || EXP2_PIN

//Standard footswitch action calls
void action(uint8_t i, uint8_t bank, bool state){
  switch(bank){
    case 0:
      switch(i){
        case 0:
          if(state == PRESS){
            BANK0BTN0P
            BANK_LED_MODE(BANK0_LED_MODE)
          } else {
            #if BANK0_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK0BTN0P
            #else
              BANK0BTN0R
            #endif
          }
          break;
        case 1:
          if(state == PRESS){
            BANK0BTN1P
            BANK_LED_MODE(BANK0_LED_MODE)
          } else {
            #if BANK0_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK0BTN1P
            #else
              BANK0BTN1R
            #endif
          }
          break;
        case 2:
          if(state == PRESS){
            BANK0BTN2P
            BANK_LED_MODE(BANK0_LED_MODE)
          } else {
            #if BANK0_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK0BTN2P
            #else
              BANK0BTN2R
            #endif
          }
          break;
        case 3:
          if(state == PRESS){
            BANK0BTN3P
            BANK_LED_MODE(BANK0_LED_MODE)
          } else {
            #if BANK0_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK0BTN3P
            #else
              BANK0BTN3R
            #endif
          }
          break;
        case 4:
          if(state == PRESS){
            BANK0BTN4P
            BANK_LED_MODE(BANK0_LED_MODE)
          } else {
            #if BANK0_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK0BTN4P
            #else
              BANK0BTN4R
            #endif
          }
          break;
        case 5:
          if(state == PRESS){
            BANK0BTN5P
            BANK_LED_MODE(BANK0_LED_MODE)
          } else {
            #if BANK0_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK0BTN5P
            #else
              BANK0BTN5R
            #endif
          }
          break;
        case 6:
          if(state == PRESS){
            BANK0BTN6P
            BANK_LED_MODE(BANK0_LED_MODE)
          } else {
           #if BANK0_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK0BTN6P
            #else
              BANK0BTN6R
            #endif
          }
          break;
        case 7:
          if(state == PRESS){
            BANK0BTN7P
            BANK_LED_MODE(BANK0_LED_MODE)
          } else {
            #if BANK0_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK0BTN7P
            #else
              BANK0BTN7R
            #endif
          }
          break;
      }
      break;
    case 1:
      switch(i){
        case 0:
          if(state == PRESS){
            BANK1BTN0P
            BANK_LED_MODE(BANK1_LED_MODE)
          } else {
            #if BANK1_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK1BTN0P
            #else
              BANK1BTN0R
            #endif
          }
          break;
        case 1:
          if(state == PRESS){
            BANK1BTN1P
            BANK_LED_MODE(BANK1_LED_MODE)
          } else {
            #if BANK1_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK1BTN1P
            #else
              BANK1BTN1R
            #endif
          }
          break;
        case 2:
          if(state == PRESS){
            BANK1BTN2P
            BANK_LED_MODE(BANK1_LED_MODE)
          } else {
            #if BANK1_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK1BTN1P
            #else
              BANK1BTN1R
            #endif
          }
          break;
        case 3:
          if(state == PRESS){
            BANK1BTN3P
            BANK_LED_MODE(BANK1_LED_MODE)
          } else {
            #if BANK1_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK1BTN3P
            #else
              BANK1BTN3R
            #endif
          }
          break;
        case 4:
          if(state == PRESS){
            BANK1BTN4P
            BANK_LED_MODE(BANK1_LED_MODE)
          } else {
            #if BANK1_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK1BTN4P
            #else
              BANK1BTN4R
            #endif
          }
          break;
        case 5:
          if(state == PRESS){
            BANK1BTN5P
            BANK_LED_MODE(BANK1_LED_MODE)
          } else {
            #if BANK1_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK1BTN5P
            #else
              BANK1BTN5R
            #endif
          }
          break;
        case 6:
          if(state == PRESS){
            BANK1BTN6P
            BANK_LED_MODE(BANK1_LED_MODE)
          } else {
            #if BANK1_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK1BTN6P
            #else
              BANK1BTN6R
            #endif
          }
          break;
        case 7:
          if(state == PRESS){
            BANK1BTN7P
            BANK_LED_MODE(BANK1_LED_MODE)
          } else {
            #if BANK1_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK1BTN7P
            #else
              BANK1BTN7R
            #endif
          }
          break;
      }
      break;     
    case 2:
      switch(i){
        case 0:
          if(state == PRESS){
            BANK2BTN0P
            BANK_LED_MODE(BANK2_LED_MODE)
          } else {
            #if BANK2_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK2BTN0P
            #else
              BANK2BTN0R
            #endif
          }
          break;
        case 1:
          if(state == PRESS){
            BANK2BTN1P
            BANK_LED_MODE(BANK2_LED_MODE)
          } else {
            #if BANK2_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK2BTN1P
            #else
              BANK2BTN1R
            #endif
          }
          break;
        case 2:
          if(state == PRESS){
            BANK2BTN2P
            BANK_LED_MODE(BANK2_LED_MODE)
          } else {
            #if BANK2_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK2BTN2P
            #else
              BANK2BTN2R
            #endif
          }
          break;
        case 3:
          if(state == PRESS){
            BANK2BTN3P
            BANK_LED_MODE(BANK2_LED_MODE)
          } else {
            #if BANK2_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK2BTN3P
            #else
              BANK2BTN3R
            #endif
          }
          break;
        case 4:
          if(state == PRESS){
            BANK2BTN4P
            BANK_LED_MODE(BANK2_LED_MODE)
          } else {
            #if BANK2_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK2BTN4P
            #else
              BANK2BTN4R
            #endif
          }
          break;
        case 5:
          if(state == PRESS){
            BANK2BTN5P
            BANK_LED_MODE(BANK2_LED_MODE)
          } else {
            #if BANK2_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK2BTN5P
            #else
              BANK2BTN5R
            #endif
          }
          break;
        case 6:
          if(state == PRESS){
            BANK2BTN6P
            BANK_LED_MODE(BANK2_LED_MODE)
          } else {
            #if BANK2_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK2BTN6P
            #else
              BANK2BTN6R
            #endif
          }
          break;
        case 7:
          if(state == PRESS){
            BANK2BTN7P
            BANK_LED_MODE(BANK2_LED_MODE)
          } else {
            #if BANK2_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK2BTN7P
            #else
              BANK2BTN7R
            #endif
          }
          break;
      }
      break;
    case 3:
      switch(i){
        case 0:
          if(state == PRESS){
            BANK3BTN0P
            BANK_LED_MODE(BANK3_LED_MODE)
          } else {
            #if BANK3_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK3BTN0P
            #else
              BANK3BTN0R
            #endif
          }
          break;
        case 1:
          if(state == PRESS){
            BANK3BTN1P
            BANK_LED_MODE(BANK3_LED_MODE)
          } else {
            #if BANK3_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK3BTN1P
            #else
              BANK3BTN1R
            #endif
          }
          break;
        case 2:
          if(state == PRESS){
            BANK3BTN2P
            BANK_LED_MODE(BANK3_LED_MODE)
          } else {
            #if BANK3_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK3BTN2P
            #else
              BANK3BTN2R
            #endif
          }
          break;
        case 3:
          if(state == PRESS){
            BANK3BTN3P
            BANK_LED_MODE(BANK3_LED_MODE)
          } else {
            #if BANK3_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK3BTN3P
            #else
              BANK3BTN3R
            #endif
          }
          break;
        case 4:
          if(state == PRESS){
            BANK3BTN4P
            BANK_LED_MODE(BANK3_LED_MODE)
          } else {
            #if BANK3_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK3BTN4P
            #else
              BANK3BTN4R
            #endif
          }
          break;
        case 5:
          if(state == PRESS){
            BANK3BTN5P
            BANK_LED_MODE(BANK3_LED_MODE)
          } else {
            #if BANK3_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK3BTN5P
            #else
              BANK3BTN5R
            #endif
          }
          break;
        case 6:
          if(state == PRESS){
            BANK3BTN6P
            BANK_LED_MODE(BANK3_LED_MODE)
          } else {
            #if BANK3_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK3BTN6P
            #else
              BANK3BTN6R
            #endif
          }
          break;
        case 7:
          if(state == PRESS){
            BANK3BTN7P
            BANK_LED_MODE(BANK3_LED_MODE)
          } else {
            #if BANK3_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK3BTN7P
            #else
              BANK3BTN7R
            #endif
          }
          break;
      }
      break;
    #if BANKS > 4
    case 4:
      switch(i){
        case 0:
          if(state == PRESS){
            BANK4BTN0P
            BANK_LED_MODE(BANK4_LED_MODE)
          } else {
            #if BANK4_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK4BTN0P
            #else
              BANK4BTN0R
            #endif
          }
          break;
        case 1:
          if(state == PRESS){
            BANK4BTN1P
            BANK_LED_MODE(BANK4_LED_MODE)
          } else {
            #if BANK4_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK4BTN1P
            #else
              BANK4BTN1R
            #endif
          }
          break;
        case 2:
          if(state == PRESS){
            BANK4BTN2P
            BANK_LED_MODE(BANK4_LED_MODE)
          } else {
            #if BANK4_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK4BTN2P
            #else
              BANK4BTN2R
            #endif
          }
          break;
        case 3:
          if(state == PRESS){
            BANK4BTN3P
            BANK_LED_MODE(BANK4_LED_MODE)
          } else {
            #if BANK4_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK4BTN3P
            #else
              BANK4BTN3R
            #endif
          }
          break;
        case 4:
          if(state == PRESS){
            BANK4BTN4P
            BANK_LED_MODE(BANK4_LED_MODE)
          } else {
            #if BANK4_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK4BTN4P
            #else
              BANK4BTN4R
            #endif
          }
          break;
        case 5:
          if(state == PRESS){
            BANK4BTN5P
            BANK_LED_MODE(BANK4_LED_MODE)
          } else {
            #if BANK4_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK4BTN5P
            #else
              BANK4BTN5R
            #endif
          }
          break;
        case 6:
          if(state == PRESS){
            BANK4BTN6P
            BANK_LED_MODE(BANK4_LED_MODE)
          } else {
            #if BANK4_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK4BTN6P
            #else
              BANK4BTN6R
            #endif
          }
          break;
        case 7:
          if(state == PRESS){
            BANK4BTN7P
            BANK_LED_MODE(BANK4_LED_MODE)
          } else {
            #if BANK4_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK4BTN7P
            #else
              BANK4BTN7R
            #endif
          }
          break;
      }
      break;
    #endif
    #if BANKS > 5
    case 5:
      switch(i){
        case 0:
          if(state == PRESS){
            BANK5BTN0P
            BANK_LED_MODE(BANK5_LED_MODE)
          } else {
            #if BANK5_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK5BTN0P
            #else
              BANK5BTN0R
            #endif
          }
          break;
        case 1:
          if(state == PRESS){
            BANK5BTN1P
            BANK_LED_MODE(BANK5_LED_MODE)
          } else {
            #if BANK5_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK5BTN1P
            #else
              BANK5BTN1R
            #endif
          }
          break;
        case 2:
          if(state == PRESS){
            BANK5BTN2P
            BANK_LED_MODE(BANK5_LED_MODE)
          } else {
            #if BANK5_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK5BTN2P
            #else
              BANK5BTN2R
            #endif
          }
          break;
        case 3:
          if(state == PRESS){
            BANK5BTN3P
            BANK_LED_MODE(BANK5_LED_MODE)
          } else {
            #if BANK5_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK5BTN3P
            #else
              BANK5BTN3R
            #endif
          }
          break;
        case 4:
          if(state == PRESS){
            BANK5BTN4P
            BANK_LED_MODE(BANK5_LED_MODE)
          } else {
            #if BANK5_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK5BTN4P
            #else
              BANK5BTN4R
            #endif
          }
          break;
        case 5:
          if(state == PRESS){
            BANK5BTN5P
            BANK_LED_MODE(BANK5_LED_MODE)
          } else {
            #if BANK5_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK5BTN5P
            #else
              BANK5BTN5R
            #endif
          }
          break;
        case 6:
          if(state == PRESS){
            BANK5BTN6P
            BANK_LED_MODE(BANK5_LED_MODE)
          } else {
            #if BANK5_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK5BTN6P
            #else
              BANK5BTN6R
            #endif
          }
          break;
        case 7:
          if(state == PRESS){
            BANK5BTN7P
            BANK_LED_MODE(BANK5_LED_MODE)
          } else {
            #if BANK5_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK5BTN7P
            #else
              BANK5BTN7R
            #endif
          }
          break;
      }
      break;
    #endif
    #if BANKS > 5
    case 5:
      switch(i){
        case 0:
          if(state == PRESS){
            BANK5BTN0P
            BANK_LED_MODE(BANK5_LED_MODE)
          } else {
            #if BANK5_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK5BTN0P
            #else
              BANK5BTN0R
            #endif
          }
          break;
        case 1:
          if(state == PRESS){
            BANK5BTN1P
            BANK_LED_MODE(BANK5_LED_MODE)
          } else {
            #if BANK5_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK5BTN1P
            #else
              BANK5BTN1R
            #endif
          }
          break;
        case 2:
          if(state == PRESS){
            BANK5BTN2P
            BANK_LED_MODE(BANK5_LED_MODE)
          } else {
            #if BANK5_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK5BTN2P
            #else
              BANK5BTN2R
            #endif
          }
          break;
        case 3:
          if(state == PRESS){
            BANK5BTN3P
            BANK_LED_MODE(BANK5_LED_MODE)
          } else {
            #if BANK5_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK5BTN3P
            #else
              BANK5BTN3R
            #endif
          }
          break;
        case 4:
          if(state == PRESS){
            BANK5BTN4P
            BANK_LED_MODE(BANK5_LED_MODE)
          } else {
            #if BANK5_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK5BTN4P
            #else
              BANK5BTN4R
            #endif
          }
          break;
        case 5:
          if(state == PRESS){
            BANK5BTN5P
            BANK_LED_MODE(BANK5_LED_MODE)
          } else {
            #if BANK5_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK5BTN5P
            #else
              BANK5BTN5R
            #endif
          }
          break;
        case 6:
          if(state == PRESS){
            BANK5BTN6P
            BANK_LED_MODE(BANK5_LED_MODE)
          } else {
            #if BANK5_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK5BTN6P
            #else
              BANK5BTN6R
            #endif
          }
          break;
        case 7:
          if(state == PRESS){
            BANK5BTN7P
            BANK_LED_MODE(BANK5_LED_MODE)
          } else {
            #if BANK5_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK5BTN7P
            #else
              BANK5BTN7R
            #endif
          }
          break;
      }
      break;
    #endif
    #if BANKS > 6
    case 6:
      switch(i){
        case 0:
          if(state == PRESS){
            BANK6BTN0P
            BANK_LED_MODE(BANK6_LED_MODE)
          } else {
            #if BANK6_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK6BTN0P
            #else
              BANK6BTN0R
            #endif
          }
          break;
        case 1:
          if(state == PRESS){
            BANK6BTN1P
            BANK_LED_MODE(BANK6_LED_MODE)
          } else {
            #if BANK6_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK6BTN1P
            #else
              BANK6BTN1R
            #endif
          }
          break;
        case 2:
          if(state == PRESS){
            BANK6BTN2P
            BANK_LED_MODE(BANK6_LED_MODE)
          } else {
            #if BANK6_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK6BTN2P
            #else
              BANK6BTN2R
            #endif
          }
          break;
        case 3:
          if(state == PRESS){
            BANK6BTN3P
            BANK_LED_MODE(BANK6_LED_MODE)
          } else {
            #if BANK6_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK6BTN3P
            #else
              BANK6BTN3R
            #endif
          }
          break;
        case 4:
          if(state == PRESS){
            BANK6BTN4P
            BANK_LED_MODE(BANK6_LED_MODE)
          } else {
            #if BANK6_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK6BTN4P
            #else
              BANK6BTN4R
            #endif
          }
          break;
        case 5:
          if(state == PRESS){
            BANK6BTN5P
            BANK_LED_MODE(BANK6_LED_MODE)
          } else {
            #if BANK6_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK6BTN5P
            #else
              BANK6BTN5R
            #endif
          }
          break;
        case 6:
          if(state == PRESS){
            BANK6BTN6P
            BANK_LED_MODE(BANK6_LED_MODE)
          } else {
            #if BANK6_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK6BTN6P
            #else
              BANK6BTN6R
            #endif
          }
          break;
        case 7:
          if(state == PRESS){
            BANK6BTN7P
            BANK_LED_MODE(BANK6_LED_MODE)
          } else {
            #if BANK6_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK6BTN7P
            #else
              BANK6BTN7R
            #endif
          }
          break;
      }
      break;
    #endif
    #if BANKS > 7
    case 7:
      switch(i){
        case 0:
          if(state == PRESS){
            BANK7BTN0P
            BANK_LED_MODE(BANK7_LED_MODE)
          } else {
            #if BANK7_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK7BTN0P
            #else
              BANK7BTN0R
            #endif
          }
          break;
        case 1:
          if(state == PRESS){
            BANK7BTN1P
            BANK_LED_MODE(BANK7_LED_MODE)
          } else {
            #if BANK7_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK7BTN1P
            #else
              BANK7BTN1R
            #endif
          }
          break;
        case 2:
          if(state == PRESS){
            BANK7BTN2P
            BANK_LED_MODE(BANK7_LED_MODE)
          } else {
            #if BANK7_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK7BTN2P
            #else
              BANK7BTN2R
            #endif
          }
          break;
        case 3:
          if(state == PRESS){
            BANK7BTN3P
            BANK_LED_MODE(BANK7_LED_MODE)
          } else {
            #if BANK7_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK7BTN3P
            #else
              BANK7BTN3R
            #endif
          }
          break;
        case 4:
          if(state == PRESS){
            BANK7BTN4P
            BANK_LED_MODE(BANK7_LED_MODE)
          } else {
            #if BANK7_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK7BTN4P
            #else
              BANK7BTN4R
            #endif
          }
          break;
        case 7:
          if(state == PRESS){
            BANK7BTN5P
            BANK_LED_MODE(BANK7_LED_MODE)
          } else {
            #if BANK7_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK7BTN5P
            #else
              BANK7BTN5R
            #endif
          }
          break;
        case 6:
          if(state == PRESS){
            BANK7BTN6P
            BANK_LED_MODE(BANK7_LED_MODE)
          } else {
            #if BANK7_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK7BTN6P
            #else
              BANK7BTN6R
            #endif
          }
          break;
        case 7:
          if(state == PRESS){
            BANK7BTN7P
            BANK_LED_MODE(BANK7_LED_MODE)
          } else {
            #if BANK7_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK7BTN7P
            #else
              BANK7BTN7R
            #endif
          }
          break;
      }
      break;
    #endif
    #if BANKS > 8
    case 8:
      switch(i){
        case 0:
          if(state == PRESS){
            BANK8BTN0P
            BANK_LED_MODE(BANK8_LED_MODE)
          } else {
            #if BANK8_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK8BTN0P
            #else
              BANK8BTN0R
            #endif
          }
          break;
        case 1:
          if(state == PRESS){
            BANK8BTN1P
            BANK_LED_MODE(BANK8_LED_MODE)
          } else {
            #if BANK8_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK8BTN1P
            #else
              BANK8BTN1R
            #endif
          }
          break;
        case 2:
          if(state == PRESS){
            BANK8BTN2P
            BANK_LED_MODE(BANK8_LED_MODE)
          } else {
            #if BANK8_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK8BTN2P
            #else
              BANK8BTN2R
            #endif
          }
          break;
        case 3:
          if(state == PRESS){
            BANK8BTN3P
            BANK_LED_MODE(BANK8_LED_MODE)
          } else {
            #if BANK8_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK8BTN3P
            #else
              BANK8BTN3R
            #endif
          }
          break;
        case 4:
          if(state == PRESS){
            BANK8BTN4P
            BANK_LED_MODE(BANK8_LED_MODE)
          } else {
            #if BANK8_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK8BTN4P
            #else
              BANK8BTN4R
            #endif
          }
          break;
        case 8:
          if(state == PRESS){
            BANK8BTN5P
            BANK_LED_MODE(BANK8_LED_MODE)
          } else {
            #if BANK8_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK8BTN5P
            #else
              BANK8BTN5R
            #endif
          }
          break;
        case 6:
          if(state == PRESS){
            BANK8BTN6P
            BANK_LED_MODE(BANK8_LED_MODE)
          } else {
            #if BANK8_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK8BTN6P
            #else
              BANK8BTN6R
            #endif
          }
          break;
        case 7:
          if(state == PRESS){
            BANK8BTN7P
            BANK_LED_MODE(BANK8_LED_MODE)
          } else {
            #if BANK8_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK8BTN7P
            #else
              BANK8BTN7R
            #endif
          }
          break;
      }
      break;
    #endif
    #if BANKS > 9
    case 9:
      switch(i){
        case 0:
          if(state == PRESS){
            BANK9BTN0P
            BANK_LED_MODE(BANK9_LED_MODE)
          } else {
            #if BANK9_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK9BTN0P
            #else
              BANK9BTN0R
            #endif
          }
          break;
        case 1:
          if(state == PRESS){
            BANK9BTN1P
            BANK_LED_MODE(BANK9_LED_MODE)
          } else {
            #if BANK9_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK9BTN1P
            #else
              BANK9BTN1R
            #endif
          }
          break;
        case 2:
          if(state == PRESS){
            BANK9BTN2P
            BANK_LED_MODE(BANK9_LED_MODE)
          } else {
            #if BANK9_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK9BTN2P
            #else
              BANK9BTN2R
            #endif
          }
          break;
        case 3:
          if(state == PRESS){
            BANK9BTN3P
            BANK_LED_MODE(BANK9_LED_MODE)
          } else {
            #if BANK9_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK9BTN3P
            #else
              BANK9BTN3R
            #endif
          }
          break;
        case 4:
          if(state == PRESS){
            BANK9BTN4P
            BANK_LED_MODE(BANK9_LED_MODE)
          } else {
            #if BANK9_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK9BTN4P
            #else
              BANK9BTN4R
            #endif
          }
          break;
        case 5:
          if(state == PRESS){
            BANK9BTN5P
            BANK_LED_MODE(BANK9_LED_MODE)
          } else {
            #if BANK9_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK9BTN5P
            #else
              BANK9BTN5R
            #endif
          }
          break;
        case 6:
          if(state == PRESS){
            BANK9BTN6P
            BANK_LED_MODE(BANK9_LED_MODE)
          } else {
            #if BANK9_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK9BTN6P
            #else
              BANK9BTN6R
            #endif
          }
          break;
        case 7:
          if(state == PRESS){
            BANK9BTN7P
            BANK_LED_MODE(BANK9_LED_MODE)
          } else {
            #if BANK9_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK9BTN7P
            #else
              BANK9BTN7R
            #endif
          }
          break;
      }
      break;
    #endif
    #if BANKS > 10
    case 10:
      switch(i){
        case 0:
          if(state == PRESS){
            BANK10BTN0P
            BANK_LED_MODE(BANK10_LED_MODE)
          } else {
            #if BANK10_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK10BTN0P
            #else
              BANK10BTN0R
            #endif
          }
          break;
        case 1:
          if(state == PRESS){
            BANK10BTN1P
            BANK_LED_MODE(BANK10_LED_MODE)
          } else {
            #if BANK10_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK10BTN1P
            #else
              BANK10BTN1R
            #endif
          }
          break;
        case 2:
          if(state == PRESS){
            BANK10BTN2P
            BANK_LED_MODE(BANK10_LED_MODE)
          } else {
            #if BANK10_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK10BTN2P
            #else
              BANK10BTN2R
            #endif
          }
          break;
        case 3:
          if(state == PRESS){
            BANK10BTN3P
            BANK_LED_MODE(BANK10_LED_MODE)
          } else {
            #if BANK10_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK10BTN3P
            #else
              BANK10BTN3R
            #endif
          }
          break;
        case 4:
          if(state == PRESS){
            BANK10BTN4P
            BANK_LED_MODE(BANK10_LED_MODE)
          } else {
            #if BANK10_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK10BTN4P
            #else
              BANK10BTN4R
            #endif
          }
          break;
        case 5:
          if(state == PRESS){
            BANK10BTN5P
            BANK_LED_MODE(BANK10_LED_MODE)
          } else {
            #if BANK10_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK10BTN5P
            #else
              BANK10BTN5R
            #endif
          }
          break;
        case 6:
          if(state == PRESS){
            BANK10BTN6P
            BANK_LED_MODE(BANK10_LED_MODE)
          } else {
            #if BANK10_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK10BTN6P
            #else
              BANK10BTN6R
            #endif
          }
          break;
        case 7:
          if(state == PRESS){
            BANK10BTN7P
            BANK_LED_MODE(BANK10_LED_MODE)
          } else {
            #if BANK10_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK10BTN7P
            #else
              BANK10BTN7R
            #endif
          }
          break;
      }
      break;
    #endif
    #if BANKS > 11
    case 11:
      switch(i){
        case 0:
          if(state == PRESS){
            BANK11BTN0P
            BANK_LED_MODE(BANK11_LED_MODE)
          } else {
            #if BANK11_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK11BTN0P
            #else
              BANK11BTN0R
            #endif
          }
          break;
        case 1:
          if(state == PRESS){
            BANK11BTN1P
            BANK_LED_MODE(BANK11_LED_MODE)
          } else {
            #if BANK11_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK11BTN1P
            #else
              BANK11BTN1R
            #endif
          }
          break;
        case 2:
          if(state == PRESS){
            BANK11BTN2P
            BANK_LED_MODE(BANK11_LED_MODE)
          } else {
            #if BANK11_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK11BTN2P
            #else
              BANK11BTN2R
            #endif
          }
          break;
        case 3:
          if(state == PRESS){
            BANK11BTN3P
            BANK_LED_MODE(BANK11_LED_MODE)
          } else {
            #if BANK11_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK11BTN3P
            #else
              BANK11BTN3R
            #endif
          }
          break;
        case 4:
          if(state == PRESS){
            BANK11BTN4P
            BANK_LED_MODE(BANK11_LED_MODE)
          } else {
            #if BANK11_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK11BTN4P
            #else
              BANK11BTN4R
            #endif
          }
          break;
        case 5:
          if(state == PRESS){
            BANK11BTN5P
            BANK_LED_MODE(BANK11_LED_MODE)
          } else {
            #if BANK11_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK11BTN5P
            #else
              BANK11BTN5R
            #endif
          }
          break;
        case 6:
          if(state == PRESS){
            BANK11BTN6P
            BANK_LED_MODE(BANK11_LED_MODE)
          } else {
            #if BANK11_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK11BTN6P
            #else
              BANK11BTN6R
            #endif
          }
          break;
        case 7:
          if(state == PRESS){
            BANK11BTN7P
            BANK_LED_MODE(BANK11_LED_MODE)
          } else {
            #if BANK11_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK11BTN7P
            #else
              BANK11BTN7R
            #endif
          }
          break;
      }
      break;
    #endif
    #if BANKS > 12
    case 12:
      switch(i){
        case 0:
          if(state == PRESS){
            BANK12BTN0P
            BANK_LED_MODE(BANK12_LED_MODE)
          } else {
            #if BANK12_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK12BTN0P
            #else
              BANK12BTN0R
            #endif
          }
          break;
        case 1:
          if(state == PRESS){
            BANK12BTN1P
            BANK_LED_MODE(BANK12_LED_MODE)
          } else {
            #if BANK12_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK12BTN1P
            #else
              BANK12BTN1R
            #endif
          }
          break;
        case 2:
          if(state == PRESS){
            BANK12BTN2P
            BANK_LED_MODE(BANK12_LED_MODE)
          } else {
            #if BANK12_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK12BTN2P
            #else
              BANK12BTN2R
            #endif
          }
          break;
        case 3:
          if(state == PRESS){
            BANK12BTN3P
            BANK_LED_MODE(BANK12_LED_MODE)
          } else {
            #if BANK12_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK12BTN3P
            #else
              BANK12BTN3R
            #endif
          }
          break;
        case 4:
          if(state == PRESS){
            BANK12BTN4P
            BANK_LED_MODE(BANK12_LED_MODE)
          } else {
            #if BANK12_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK12BTN4P
            #else
              BANK12BTN4R
            #endif
          }
          break;
        case 5:
          if(state == PRESS){
            BANK12BTN5P
            BANK_LED_MODE(BANK12_LED_MODE)
          } else {
            #if BANK12_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK12BTN5P
            #else
              BANK12BTN5R
            #endif
          }
          break;
        case 6:
          if(state == PRESS){
            BANK12BTN6P
            BANK_LED_MODE(BANK12_LED_MODE)
          } else {
            #if BANK12_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK12BTN6P
            #else
              BANK12BTN6R
            #endif
          }
          break;
        case 7:
          if(state == PRESS){
            BANK12BTN7P
            BANK_LED_MODE(BANK12_LED_MODE)
          } else {
            #if BANK12_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK12BTN7P
            #else
              BANK12BTN7R
            #endif
          }
          break;
      }
      break;
    #endif
    #if BANKS > 13
    case 13:
      switch(i){
        case 0:
          if(state == PRESS){
            BANK13BTN0P
            BANK_LED_MODE(BANK13_LED_MODE)
          } else {
            #if BANK13_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK13BTN0P
            #else
              BANK13BTN0R
            #endif
          }
          break;
        case 1:
          if(state == PRESS){
            BANK13BTN1P
            BANK_LED_MODE(BANK13_LED_MODE)
          } else {
            #if BANK13_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK13BTN1P
            #else
              BANK13BTN1R
            #endif
          }
          break;
        case 2:
          if(state == PRESS){
            BANK13BTN2P
            BANK_LED_MODE(BANK13_LED_MODE)
          } else {
            #if BANK13_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK13BTN2P
            #else
              BANK13BTN2R
            #endif
          }
          break;
        case 3:
          if(state == PRESS){
            BANK13BTN3P
            BANK_LED_MODE(BANK13_LED_MODE)
          } else {
            #if BANK13_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK13BTN3P
            #else
              BANK13BTN3R
            #endif
          }
          break;
        case 4:
          if(state == PRESS){
            BANK13BTN4P
            BANK_LED_MODE(BANK13_LED_MODE)
          } else {
            #if BANK13_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK13BTN4P
            #else
              BANK13BTN4R
            #endif
          }
          break;
        case 5:
          if(state == PRESS){
            BANK13BTN5P
            BANK_LED_MODE(BANK13_LED_MODE)
          } else {
            #if BANK13_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK13BTN5P
            #else
              BANK13BTN5R
            #endif
          }
          break;
        case 6:
          if(state == PRESS){
            BANK13BTN6P
            BANK_LED_MODE(BANK13_LED_MODE)
          } else {
            #if BANK13_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK13BTN6P
            #else
              BANK13BTN6R
            #endif
          }
          break;
        case 7:
          if(state == PRESS){
            BANK13BTN7P
            BANK_LED_MODE(BANK13_LED_MODE)
          } else {
            #if BANK13_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK13BTN7P
            #else
              BANK13BTN7R
            #endif
          }
          break;
      }
      break;
    #endif
    #if BANKS > 14
    case 14:
      switch(i){
        case 0:
          if(state == PRESS){
            BANK14BTN0P
            BANK_LED_MODE(BANK14_LED_MODE)
          } else {
            #if BANK14_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK14BTN0P
            #else
              BANK14BTN0R
            #endif
          }
          break;
        case 1:
          if(state == PRESS){
            BANK14BTN1P
            BANK_LED_MODE(BANK14_LED_MODE)
          } else {
            #if BANK14_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK14BTN1P
            #else
              BANK14BTN1R
            #endif
          }
          break;
        case 2:
          if(state == PRESS){
            BANK14BTN2P
            BANK_LED_MODE(BANK14_LED_MODE)
          } else {
            #if BANK14_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK14BTN2P
            #else
              BANK14BTN2R
            #endif
          }
          break;
        case 3:
          if(state == PRESS){
            BANK14BTN3P
            BANK_LED_MODE(BANK14_LED_MODE)
          } else {
            #if BANK14_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK14BTN3P
            #else
              BANK14BTN3R
            #endif
          }
          break;
        case 4:
          if(state == PRESS){
            BANK14BTN4P
            BANK_LED_MODE(BANK14_LED_MODE)
          } else {
            #if BANK14_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK14BTN4P
            #else
              BANK14BTN4R
            #endif
          }
          break;
        case 5:
          if(state == PRESS){
            BANK14BTN5P
            BANK_LED_MODE(BANK14_LED_MODE)
          } else {
            #if BANK14_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK14BTN5P
            #else
              BANK14BTN5R
            #endif
          }
          break;
        case 6:
          if(state == PRESS){
            BANK14BTN6P
            BANK_LED_MODE(BANK14_LED_MODE)
          } else {
            #if BANK14_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK14BTN6P
            #else
              BANK14BTN6R
            #endif
          }
          break;
        case 7:
          if(state == PRESS){
            BANK14BTN7P
            BANK_LED_MODE(BANK14_LED_MODE)
          } else {
            #if BANK14_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK14BTN7P
            #else
              BANK14BTN7R
            #endif
          }
          break;
      }
      break;
    #endif
    #if BANKS > 15
    case 15:
      switch(i){
        case 0:
          if(state == PRESS){
            BANK15BTN0P
            BANK_LED_MODE(BANK15_LED_MODE)
          } else {
            #if BANK15_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK15BTN0P
            #else
              BANK15BTN0R
            #endif
          }
          break;
        case 1:
          if(state == PRESS){
            BANK15BTN1P
            BANK_LED_MODE(BANK15_LED_MODE)
          } else {
            #if BANK15_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK15BTN1P
            #else
              BANK15BTN1R
            #endif
          }
          break;
        case 2:
          if(state == PRESS){
            BANK15BTN2P
            BANK_LED_MODE(BANK15_LED_MODE)
          } else {
            #if BANK15_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK15BTN2P
            #else
              BANK15BTN2R
            #endif
          }
          break;
        case 3:
          if(state == PRESS){
            BANK15BTN3P
            BANK_LED_MODE(BANK15_LED_MODE)
          } else {
            #if BANK15_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK15BTN3P
            #else
              BANK15BTN3R
            #endif
          }
          break;
        case 4:
          if(state == PRESS){
            BANK15BTN4P
            BANK_LED_MODE(BANK15_LED_MODE)
          } else {
            #if BANK15_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK15BTN4P
            #else
              BANK15BTN4R
            #endif
          }
          break;
        case 5:
          if(state == PRESS){
            BANK15BTN5P
            BANK_LED_MODE(BANK15_LED_MODE)
          } else {
            #if BANK15_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK15BTN5P
            #else
              BANK15BTN5R
            #endif
          }
          break;
        case 6:
          if(state == PRESS){
            BANK15BTN6P
            BANK_LED_MODE(BANK15_LED_MODE)
          } else {
            #if BANK15_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK15BTN6P
            #else
              BANK15BTN6R
            #endif
          }
          break;
        case 7:
          if(state == PRESS){
            BANK15BTN7P
            BANK_LED_MODE(BANK15_LED_MODE)
          } else {
            #if BANK15_LED_MODE == LED_MOMENTARY
              ledState[i] = false;
              BANK15BTN7P
            #else
              BANK15BTN7R
            #endif
          }
          break;
      }
      break;
    #endif
  }
}
