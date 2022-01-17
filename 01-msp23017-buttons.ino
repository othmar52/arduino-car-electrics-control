 
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
      BUTTON_EXCEEDS_LONGPRESS[i] = false;
      BUTTON_ISPRESSED[i] = true;
    }
    // some actions are triggered after x seconds during hold and stopped on release
    if (BUTTON_ISPRESSED[i] == true
    && BUTTON_EXCEEDS_LONGPRESS[i] == false
    && reachedLongestInterval(BUTTON_PINS[i], millis() - BUTTON_LASTDOWN[i]) == true) {
      handleButtonReachedLongestInterval(BUTTON_PINS[i]);
      BUTTON_EXCEEDS_LONGPRESS[i] = true;
    }

    if ( buttons[i].rose() ) {
      // When button was released...
      handleButtonRelease(BUTTON_PINS[i], millis() - BUTTON_LASTDOWN[i]);
      BUTTON_EXCEEDS_LONGPRESS[i] = false;
      BUTTON_ISPRESSED[i] = false;
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

void handleButtonReachedLongestInterval(uint8_t btnIdx) {
  if (btnIdx == MCP_PIN_BTN_HEAD_LEFT) {
    return handleButtonHeadLeftEnterLongpress();
  }
  if (btnIdx == MCP_PIN_BTN_HEAD_RIGHT) {
    return handleButtonHeadRightEnterLongpress();
  }
  if (btnIdx == MCP_PIN_BTN_WRIST_LEFT) {
    return handleButtonWristLeftEnterLongpress();
  }
  if (btnIdx == MCP_PIN_BTN_WRIST_RIGHT) {
    return handleButtonWristRightEnterLongpress();
  }
}
