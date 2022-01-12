#include <Bounce2mcp.h>

//#include <Wire.h>
#include "Adafruit_MCP23017.h"

Adafruit_MCP23017 mcp1;
Adafruit_MCP23017* mcpr = &mcp1;


#define NUM_BUTTONS 3

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
#define MCP_PIN_BTN_1 8
#define MCP_PIN_BTN_2 9
#define MCP_PIN_BTN_3 10

#define MCP_PIN_RELAY_1 7

// Pins, used by buttons
const uint8_t BUTTON_PINS[NUM_BUTTONS] = {
  MCP_PIN_BTN_1,
  MCP_PIN_BTN_2,
  MCP_PIN_BTN_3
};

// Helper to track push duration for each button
unsigned long BUTTON_LASTDOWN[NUM_BUTTONS] = {0, 0, 0};

// Button debounce wrappers
BounceMcp * buttons = new BounceMcp[NUM_BUTTONS];

bool relayPinState = false;

void setupButtons() {

  // init push buttons
  for (int i = 0; i < NUM_BUTTONS; i++) {
    mcpr->pinMode(BUTTON_PINS[i], INPUT);
    mcpr->pullUp(BUTTON_PINS[i], HIGH);
    mcpr->pinMode(MCP_PIN_RELAY_1, OUTPUT);
    mcpr->pullUp(MCP_PIN_RELAY_1, HIGH);
    mcpr->digitalWrite(MCP_PIN_RELAY_1, HIGH);
    buttons[i].attach(*mcpr, BUTTON_PINS[i], 5);
  }

}


void loopButtons() {

  for (int i = 0; i < NUM_BUTTONS; i++)  {
    // Updating all debouncers
    buttons[i].update();
    if ( buttons[i].fell() ) {
      BUTTON_LASTDOWN[i] = millis();
    }

    if ( buttons[i].rose() ) {
      // When button was released...
      handleButtonRelease(i, millis() - BUTTON_LASTDOWN[i]);
    }
  }
}

void handleButtonRelease(uint8_t eIdx, int holdTime) { 
  Serial.print(eIdx);
  Serial.print(" push ");
  Serial.println(holdTime);
  mcpr->digitalWrite(MCP_PIN_RELAY_1, relayPinState);
  relayPinState = !relayPinState;
}

void setup() {
  Serial.begin(115200);
  pinMode(1, INPUT);
  pinMode(2, INPUT);
  mcp1.begin(0);

  setupButtons();
}
void loop() {
  loopButtons();
}
