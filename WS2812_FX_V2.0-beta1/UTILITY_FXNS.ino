//------------------------------------- UTILITY FXNS --------------------------------------

//---FIND INDEX OF HORIZONAL OPPOSITE LED
int horizontal_index(int i) {
  //-ONLY WORKS WITH INDEX < TOPINDEX
  if (i == BOTTOM_INDEX) return BOTTOM_INDEX;
  if (i == TOP_INDEX && EVENODD == 1) return TOP_INDEX + 1;
  if (i == TOP_INDEX && EVENODD == 0) return TOP_INDEX;
  return LED_COUNT - i;
}

//---FIND INDEX OF ANTIPODAL OPPOSITE LED
int antipodal_index(int i) {
  int iN = i + TOP_INDEX;
  if (i >= TOP_INDEX) iN = ( i + TOP_INDEX ) % LED_COUNT;
  return iN;
}

//---FIND ADJACENT INDEX CLOCKWISE
int adjacent_cw(int i) {
  int r;
  if (i < LED_COUNT - 1) r = i + 1;
  else r = 0;
  return r;
}

//---FIND ADJACENT INDEX COUNTER-CLOCKWISE
int adjacent_ccw(int i) {
  int r;
  if (i > 0) r = i - 1;
  else r = LED_COUNT - 1;
  return r;
}

void setAll(byte red, byte green, byte blue) {
  for (int i = 0; i < LED_COUNT; i++ ) {
    //leds[i] = CRGB(red, green, blue);
    leds[i].setRGB(red, green, blue);
  }
  //FastLED.show();
  LEDS.show();
}
