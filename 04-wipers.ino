

void setupDigiPot() {
  digiPot.begin(DIGIPOT_CSPIN, DIGIPOT_INCPIN, DIGIPOT_UDPIN);
  digiPot.setPotMin(false);
}

void loopWiperModes() {
  currentWiperMode++;
  if(currentWiperMode > WIPER_MODE_FAST) {
    currentWiperMode = WIPER_MODE_OFF;
  }
  Serial.print("current wiper mode is now ");
  Serial.println(currentWiperMode);

  // set resistance of digital potentiometer X9C
  // https://www.digikey.at/de/products/detail/renesas-electronics-america-inc/X9C102PIZ/1068170
  // thanks to https://www.youtube.com/watch?v=KqBruMXQjmA

  // TODO check which values to set
  // digiPot.setPotMin(false); ->   33 Ohm
  // digiPot.setPot(18, false); -> 203 Ohm
  // digiPot.setPot(20, false); -> 220 Ohm
  // digiPot.setPot(25, false); -> 268 Ohm
  // digiPot.setPot(50, false); -> 503 Ohm
  // digiPot.setPot(75, false); -> 740 Ohm
  // digiPot.setPotMax(false); ->  960 Ohm

  if (currentWiperMode == WIPER_MODE_OFF) {    digiPot.setPot(0, false); }
  if (currentWiperMode == WIPER_MODE_SLOW) {   digiPot.setPotMin(false); }
  if (currentWiperMode == WIPER_MODE_MEDIUM) { digiPot.setPotMax(false); }
  if (currentWiperMode == WIPER_MODE_FAST) {   digiPot.setPot(18, false); }
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
