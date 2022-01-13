 
void setupButtons() {
  // init push buttons
  for (int i = 0; i < NUM_BUTTONS; i++) {
    mcpr->pinMode(BUTTON_PINS[i], INPUT);
    mcpr->pullUp(BUTTON_PINS[i], HIGH);
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
      handleButtonRelease(BUTTON_PINS[i], millis() - BUTTON_LASTDOWN[i]);
    }
  }
}

void handleButtonRelease(uint8_t btnIdx, int holdTime) { 
  //Serial.print(btnIdx); Serial.print(" push "); Serial.println(holdTime);

  if (btnIdx == MCP_PIN_BTN_HEAD_LEFT) {
    return handleButtonHeadLeft(holdTime);
  }
  if (btnIdx == MCP_PIN_BTN_HEAD_RIGHT) {
    return handleButtonHeadRight(holdTime);
  }
  if (btnIdx == MCP_PIN_BTN_WRIST_LEFT) {
    return handleButtonWristLeft(holdTime);
  }
  if (btnIdx == MCP_PIN_BTN_WRIST_RIGHT) {
    return handleButtonWristRight(holdTime);
  }
}
