/*
 * https://github.com/othmar52/arduino-car-electrics-control
 */

#include "X9C.h"
#include <Bounce2mcp.h>

//#include <Wire.h>
#include "Adafruit_MCP23017.h"

Adafruit_MCP23017 mcp1;
Adafruit_MCP23017* mcpr = &mcp1;

#define NUM_BUTTONS 4
#define NUM_RELAYS 8

/*
                   DIGITAL POTENTIOMETER
                        X9C102 (1K)
                         TOP VIEW
                   ┌────────U────────┐
                   │ ●               │
    Arduino D5 <───│1 INC       Vcc 8│──> Arduino 5V
                   │                 │
    Arduino D6 <───│2 U/D        CS 7│──> Arduino D7
                   │                 │
             * <───│3 VH/RH   VL/RL 6│──>
                   │                 │
   Arduino GND <───│4 Vss     VW/RW 5│──> *
                   │                 │
                   └─────────────────┘

      *Between pins 3 and 5 we have our dynamic resistor
*/

/*
      7   6   5   4   3   2   1   0          3V  GND GND GND
      |   |   |   |   |   |   |   |   |   |   |   |   |   |
    ---------------------------------------------------------
    |                                                       |
    Ↄ                       MCP23017                        |
    |                                                       |
    ---------------------------------------------------------
      |   |   |   |   |   |   |   |   |   |   |   |   |   |
      8   9  10  11  12  13  14  15  3V  GND      A5  A4
*/

// pin numbers of MCP23017 for pushbuttons
#define MCP_PIN_BTN_HEAD_LEFT 8
#define MCP_PIN_BTN_HEAD_RIGHT 9
#define MCP_PIN_BTN_WRIST_LEFT 10
#define MCP_PIN_BTN_WRIST_RIGHT 11

// pin numbers of MCP23017 for relays
#define MCP_PIN_RELAY_TURN_LEFT 0
#define MCP_PIN_RELAY_TURN_RIGHT 1
#define MCP_PIN_RELAY_BEAM_HEADLIGHTS 3
#define MCP_PIN_RELAY_FLASH_LIGHT 2
#define MCP_PIN_RELAY_HAZARD_FLASHER 4
#define MCP_PIN_RELAY_WIPER 5
#define MCP_PIN_RELAY_FOUNTAIN_SOLUTION 6
#define MCP_PIN_RELAY_REAR_FOG_LAMP 7

#define DIGIPOT_CSPIN 7
#define DIGIPOT_INCPIN 5
#define DIGIPOT_UDPIN 6

#define WIPER_MODE_OFF 0
#define WIPER_MODE_SLOW 1
#define WIPER_MODE_MEDIUM 2
#define WIPER_MODE_FAST 3

#define WIPER_MODE_TMP1 1
#define WIPER_MODE_TMP2 2
#define WIPER_MODE_TMP3 3
#define WIPER_MODE_TMP4 4
#define WIPER_MODE_TMP5 5
#define WIPER_MODE_TMP6 6
#define WIPER_MODE_TMP7 7
#define WIPER_MODE_TMP8 8
#define WIPER_MODE_TMP9 9
#define WIPER_MODE_TMP10 10

#define TURN_SIGNAL_LISTEN_SHORT 300
#define TURN_SIGNAL_LISTEN_LONG 3000
#define TURN_SIGNAL_STOP_AFTER 5000

#define FOUNTAIN_SOLUTION_DURATION 2000
#define HAZARD_FLASHER_TOGGLE_DURATION 1000

#define FLASH_LIGHT_CYCLES 4
#define FLASH_LIGHT_ON 200
#define FLASH_LIGHT_OFF 100

#define HEAD_LEFT_BUTTON_LISTEN_SHORT 500
#define HEAD_RIGHT_BUTTON_LISTEN_SHORT 400

// Pins, used by buttons
const uint8_t BUTTON_PINS[NUM_BUTTONS] = {
  MCP_PIN_BTN_HEAD_LEFT,
  MCP_PIN_BTN_HEAD_RIGHT,
  MCP_PIN_BTN_WRIST_LEFT,
  MCP_PIN_BTN_WRIST_RIGHT
};

// Pins, used by relays
const uint8_t RELAY_PINS[NUM_RELAYS] = {
  MCP_PIN_RELAY_TURN_LEFT,
  MCP_PIN_RELAY_TURN_RIGHT,
  MCP_PIN_RELAY_BEAM_HEADLIGHTS,
  MCP_PIN_RELAY_FLASH_LIGHT,
  MCP_PIN_RELAY_HAZARD_FLASHER,
  MCP_PIN_RELAY_WIPER,
  MCP_PIN_RELAY_FOUNTAIN_SOLUTION,
  MCP_PIN_RELAY_REAR_FOG_LAMP
};

// Helper to track push duration for each button
unsigned long BUTTON_LASTDOWN[NUM_BUTTONS] = {0, 0, 0, 0};
unsigned long RELAY_STATE[NUM_RELAYS] = {false, false, false, false, false, false, false, false};

unsigned long flashLightLoopStart = 0;
unsigned long turnSignalLoopUntil = 0;
unsigned long fountainLoopUntil = 0;
unsigned long hazardFlasherLoopUntil = 0;

uint8_t turnSignalTargetPin;
uint8_t currentWiperMode = WIPER_MODE_OFF;

// Button debounce wrappers
BounceMcp * buttons = new BounceMcp[NUM_BUTTONS];

X9C digiPot;

void setup() {
  Serial.begin(115200);
  // setup MCP23017
  mcp1.begin(0);
  setupButtons();
  setupRelays();
  setupDigiPot();
}

void loop() {
  loopButtons();
  flashLightLoop();
  turnSignalLoop();
  wiperLoop();
  fountainSolutionLoop();
  hazardFlasherLoop();
}
