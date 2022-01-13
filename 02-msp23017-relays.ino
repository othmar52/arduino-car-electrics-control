

void setupRelays() {
  // init relays
  for (int i = 0; i < NUM_RELAYS; i++) {
    mcpr->pinMode(RELAY_PINS[i], OUTPUT);
    mcpr->pullUp(RELAY_PINS[i], OUTPUT);
    mcpr->digitalWrite(RELAY_PINS[i], HIGH);
  }
}

void setRelayOnForPin(uint8_t mcpPinNumber) {
  uint8_t pinIndex = getIndexForMcpRelayPin(mcpPinNumber);
  mcpr->digitalWrite(mcpPinNumber, LOW);
  RELAY_STATE[pinIndex] = true;
}

void setRelayOffForPin(uint8_t mcpPinNumber) {
  uint8_t pinIndex = getIndexForMcpRelayPin(mcpPinNumber);
  mcpr->digitalWrite(mcpPinNumber, HIGH);
  RELAY_STATE[pinIndex] = false;
}

void setRelayToggleForPin(uint8_t mcpPinNumber) {
  uint8_t pinIndex = getIndexForMcpRelayPin(mcpPinNumber);
  mcpr->digitalWrite(mcpPinNumber, RELAY_STATE[pinIndex]);
  RELAY_STATE[pinIndex] = !RELAY_STATE[pinIndex];
}

bool isRelayOn(uint8_t pinNumber) {
  return RELAY_STATE[getIndexForMcpRelayPin(pinNumber)];
}

bool isRelayOff(uint8_t pinNumber) {
  return !isRelayOn(pinNumber);
}

uint8_t getIndexForMcpRelayPin(uint8_t pinNumber) {
  for (uint8_t i = 0; i < NUM_RELAYS; i++) {
    if(RELAY_PINS[i] == pinNumber) {
      return i;
    }
  }
  //Serial.println("we should never reach this line");
  return 0;
}
