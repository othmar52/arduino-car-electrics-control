
void loopWiperModes() {
  currentWiperMode++;
  if(currentWiperMode > WIPER_MODE_FAST) {
    currentWiperMode = WIPER_MODE_OFF;
  }
  Serial.print("current wiper mode is now ");
  Serial.println(currentWiperMode);
  // TODO set resistance of digital potentiometer X9C
  // https://www.digikey.at/de/products/detail/renesas-electronics-america-inc/X9C102PIZ/1068170
}


void wiperLoop() {
  if (currentWiperMode == WIPER_MODE_OFF) {
    if (isRelayOn(MCP_PIN_RELAY_WIPER)) {
      setRelayOffForPin(MCP_PIN_RELAY_WIPER);
    }
    return;
  }
  if (isRelayOff(MCP_PIN_RELAY_WIPER)) {
    setRelayOnForPin(MCP_PIN_RELAY_WIPER);
  }
}


void fountainSolutionLoop() {
  if (fountainLoopUntil == 0) {
    return;
  }
  if(millis() < fountainLoopUntil && isRelayOff(MCP_PIN_RELAY_FOUNTAIN_SOLUTION)) {
    setRelayOnForPin(MCP_PIN_RELAY_FOUNTAIN_SOLUTION);
  }
  if(millis() > fountainLoopUntil) {
    setRelayOffForPin(MCP_PIN_RELAY_FOUNTAIN_SOLUTION);
    fountainLoopUntil = 0;
  }
}
