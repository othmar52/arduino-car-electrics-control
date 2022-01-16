
void handleTurnSignalButton(int holdTime, uint8_t directionPin, uint8_t otherDirectionPin) {
  turnSignalTargetPin = directionPin;
  if (holdTime < TURN_SIGNAL_LISTEN_SHORT) {
    if (isRelayOn(turnSignalTargetPin)) {
      Serial.println("turn signal relais OFF");
      setRelayOffForPin(turnSignalTargetPin);
      turnSignalLoopUntil = 0;
      return;
    }
    // ensure other turn signal is off
    setRelayOffForPin(otherDirectionPin);

    Serial.print("turn ");
    Serial.print((directionPin == MCP_PIN_RELAY_TURN_LEFT) ? "left" : "right");
    Serial.print(" signal relais for ");
    Serial.print(TURN_SIGNAL_STOP_AFTER);
    Serial.println(" milliseconds");
    turnSignalLoopUntil = millis() + TURN_SIGNAL_STOP_AFTER;
    return;
  }
  // ensure other turn signal is off
  setRelayOffForPin(otherDirectionPin);
  
  Serial.print("turn ");
  Serial.print((directionPin == MCP_PIN_RELAY_TURN_LEFT) ? "left" : "right");
  Serial.println(" signal relais ON forever");
  setRelayOnForPin(directionPin);
}


void turnSignalLoop() {
  if (turnSignalLoopUntil == 0) {
    return;
  }
  if(millis() > turnSignalLoopUntil) {
    turnSignalLoopUntil = 0;
    setRelayOffForPin(turnSignalTargetPin);
    return;
  }
  if(isRelayOff(turnSignalTargetPin)) {
    setRelayOnForPin(turnSignalTargetPin);
  }
}
