#ifndef PORTSPINS
#define PORTSPINS

#define LARGE_FLASH true
#define SOFTPWM
#ifdef IS_RAMPS_13
  #define SERVO0_PIN        -1 // RAMPS_13 // Will conflict with BTN_EN2 on LCD_I2C_VIKI
#else
  #define SERVO0_PIN       -1
#endif
#define SERVO1_PIN          -1
#define SERVO2_PIN          -1
#define SERVO3_PIN          -1

#define X_STEP_PIN         1
#define X_DIR_PIN          2
#define X_ENABLE_PIN       3

#define X_MIN_PIN          -1

#ifndef X_MAX_PIN
	#define X_MAX_PIN      -1
#endif

#define Y_STEP_PIN         4
#define Y_DIR_PIN          5
#define Y_ENABLE_PIN       6
#define Y_MIN_PIN          -1
#define Y_MAX_PIN          -1

#define Z_STEP_PIN         7
#define Z_DIR_PIN          8
#define Z_ENABLE_PIN       9
#define Z_MIN_PIN          -1
#define Z_MAX_PIN          -1

#define E0_STEP_PIN        10
#define E0_DIR_PIN         11
#define E0_ENABLE_PIN      12

#define E1_STEP_PIN        13
#define E1_DIR_PIN         14
#define E1_ENABLE_PIN      15

#define SDPOWER            -1
#define SDSS               -1
#define LED_PIN            -1

#define FILWIDTH_PIN        -1 // ANALOG NUMBERING

#if ENABLED(Z_MIN_PROBE_ENDSTOP)
  // Define a pin to use as the signal pin on stm32 for the Z_PROBE endstop.
  #define Z_MIN_PROBE_PIN  -1
#endif

#if ENABLED(FILAMENT_RUNOUT_SENSOR)
  // define digital pin 4 for the filament runout sensor. Use the RAMPS 1.4 digital input 4 on the servos connector
  #define FILRUNOUT_PIN     -1
#endif


  #define FAN_PIN           17 // IO pin. Buffer needed


#define PS_ON_PIN          -1

#if ENABLED(REPRAP_DISCOUNT_SMART_CONTROLLER) || ENABLED(G3D_PANEL)
  #define KILL_PIN         41
#endif

  #define HEATER_0_PIN     16   // EXTRUDER 1


#if MB(RAMPS_14_SF) || MB(RAMPS_13_SF) || ENABLED(IS_RAMPS_EFB)
  #define HEATER_1_PIN     -1
#else
  #define HEATER_1_PIN      17   // EXTRUDER 2 (FAN On Sprinter)
#endif

#define HEATER_2_PIN      18

#define TEMP_0_PIN         24   // ANALOG NUMBERING
#define TEMP_1_PIN         25   // ANALOG NUMBERING
#define TEMP_2_PIN         -1   // ANALOG NUMBERING

#if MB(RAMPS_14_EFF) || MB(RAMPS_14_EEF) || MB(RAMPS_14_SF) || MB(RAMPS_13_EFF) || MB(RAMPS_13_EEF) || MB(RAMPS_13_SF)
  #define HEATER_BED_PIN   -1    // NO BED
#else
  #define HEATER_BED_PIN    -1   // BED
#endif

#define TEMP_BED_PIN         -1   // ANALOG NUMBERING

#if ENABLED(Z_PROBE_SLED)
  #define SLED_PIN           -1
#endif

#if ENABLED(ULTRA_LCD)

  #if ENABLED(NEWPANEL)
    #if ENABLED(PANEL_ONE)
      #define LCD_PINS_RS 40
      #define LCD_PINS_ENABLE 42
      #define LCD_PINS_D4 65
      #define LCD_PINS_D5 66
      #define LCD_PINS_D6 44
      #define LCD_PINS_D7 64
    #else
      #define LCD_PINS_RS 16
      #define LCD_PINS_ENABLE 17
      #define LCD_PINS_D4 23
      #define LCD_PINS_D5 25
      #define LCD_PINS_D6 27
      #define LCD_PINS_D7 29
    #endif

    #if ENABLED(REPRAP_DISCOUNT_SMART_CONTROLLER)
      #define BEEPER_PIN 37

      #define BTN_EN1 31
      #define BTN_EN2 33
      #define BTN_ENC 35

      #define SD_DETECT_PIN 49
    #elif ENABLED(LCD_I2C_PANELOLU2)
      #define BTN_EN1 47  // reverse if the encoder turns the wrong way.
      #define BTN_EN2 43
      #define BTN_ENC 32
      #define LCD_SDSS 53
      #define SD_DETECT_PIN -1
      #define KILL_PIN 41
    #elif ENABLED(LCD_I2C_VIKI)
      #define BTN_EN1 22  // reverse if the encoder turns the wrong way.
      #define BTN_EN2 7   // http://files.panucatt.com/datasheets/viki_wiring_diagram.pdf
                          // tells about 40/42.
                          // 22/7 are unused on RAMPS_14. 22 is unused and 7 the SERVO0_PIN on RAMPS_13.
      #define BTN_ENC -1
      #define LCD_SDSS 53
      #define SD_DETECT_PIN 49
    #elif ENABLED(ELB_FULL_GRAPHIC_CONTROLLER)
      #define BTN_EN1 35  // reverse if the encoder turns the wrong way.
      #define BTN_EN2 37
      #define BTN_ENC 31
      #define SD_DETECT_PIN 49
      #define LCD_SDSS 53
      #define KILL_PIN 41
      #define BEEPER_PIN 23
      #define DOGLCD_CS 29
      #define DOGLCD_A0 27
      #define LCD_PIN_BL 33
    #elif ENABLED(MINIPANEL)
      #define BEEPER_PIN 42
      // Pins for DOGM SPI LCD Support
      #define DOGLCD_A0  44
      #define DOGLCD_CS  66
      #define LCD_PIN_BL 65 // backlight LED on A11/D65
      #define SDSS   53

      #define KILL_PIN 64
      // GLCD features
      //#define LCD_CONTRAST 190
      // Uncomment screen orientation
      //#define LCD_SCREEN_ROT_90
      //#define LCD_SCREEN_ROT_180
      //#define LCD_SCREEN_ROT_270
      //The encoder and click button
      #define BTN_EN1 40
      #define BTN_EN2 63
      #define BTN_ENC 59  //the click switch
      //not connected to a pin
      #define SD_DETECT_PIN 49

    #else

      #define BEEPER_PIN 33  // Beeper on AUX-4

      // buttons are directly attached using AUX-2
      #if ENABLED(REPRAPWORLD_KEYPAD)
        #define BTN_EN1 64 // encoder
        #define BTN_EN2 59 // encoder
        #define BTN_ENC 63 // enter button
        #define SHIFT_OUT 40 // shift register
        #define SHIFT_CLK 44 // shift register
        #define SHIFT_LD 42 // shift register
      #elif ENABLED(PANEL_ONE)
        #define BTN_EN1 59 // AUX2 PIN 3
        #define BTN_EN2 63 // AUX2 PIN 4
        #define BTN_ENC 49 // AUX3 PIN 7
      #else
        #define BTN_EN1 37
        #define BTN_EN2 35
        #define BTN_ENC 31  // the click
      #endif

      #if ENABLED(G3D_PANEL)
        #define SD_DETECT_PIN 49
      #else
        //        #define SD_DETECT_PIN -1  // Ramps doesn't use this
      #endif

    #endif
  #else // !NEWPANEL (Old-style panel with shift register)

    #define BEEPER_PIN 33   // No Beeper added

    // Buttons are attached to a shift register
    // Not wired yet
    //#define SHIFT_CLK 38
    //#define SHIFT_LD 42
    //#define SHIFT_OUT 40
    //#define SHIFT_EN 17

    #define LCD_PINS_RS 16
    #define LCD_PINS_ENABLE 17
    #define LCD_PINS_D4 23
    #define LCD_PINS_D5 25
    #define LCD_PINS_D6 27
    #define LCD_PINS_D7 29

  #endif // !NEWPANEL

#endif // ULTRA_LCD

// SPI for Max6675 or Max31855 Thermocouple
#if DISABLED(SDSUPPORT)
  #define MAX6675_SS       66 // Do not use pin 53 if there is even the remote possibility of using Display/SD card
#else
  #define MAX6675_SS       66 // Do not use pin 49 as this is tied to the switch inside the SD card socket to detect if there is an SD card present
#endif

#if DISABLED(SDSUPPORT)
  // these pins are defined in the SD library if building with SD support
  #define SCK_PIN          52
  #define MISO_PIN         50
  #define MOSI_PIN         51
#endif

#ifndef KILL_PIN
  //  #define KILL_PIN         -1
#endif

#endif //PORTSPINS
