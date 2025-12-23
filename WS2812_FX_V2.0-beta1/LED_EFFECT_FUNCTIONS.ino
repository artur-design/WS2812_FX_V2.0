//------------------------LED EFFECT FUNCTIONS------------------------


void one_color_allHSV(byte ahue, byte asat, byte abri) {    //-SET ALL LEDS TO ONE COLOR (HSV)
  for (int i = 0; i < LED_COUNT; i++) {
    leds[i] = CHSV(ahue, asat, abri);
  }
  LEDS.show();
}

void rainbow_fade() {                         //- РЕЖИМ 2-FADE ALL LEDS THROUGH HSV RAINBOW
  one_color_allHSV(++ihue,thissat, 255);
  if (safeDelay(thisdelay)) return;
}

void rainbow_loop() {                        //- РЕЖИМ 3-LOOP HSV RAINBOW
  if ((ihue+=thisstep) < thisstep) ihue = 0;
  if (++idex == LED_COUNT) idex = 0;
  leds[idex] = CHSV(ihue, thissat, 255);
  LEDS.show();
  if (safeDelay(thisdelay)) return;
}

void random_burst() {                        //- РЕЖИМ 4-RANDOM INDEX/COLOR
  leds[random8(LED_COUNT)] = CHSV(random8(255), thissat, 255);
  LEDS.show();
  if (safeDelay(thisdelay)) return;
}

void color_bounce(byte col) {                        //-РЕЖИМ 5-BOUNCE COLOR (SINGLE LED)
  if (!bouncedirection) if (++idex == LED_COUNT) bouncedirection = 1;
  if (bouncedirection) if (--idex == 0) bouncedirection = 0;
  setAll(0, 0, 0);
  if (col == 1) leds[idex] = CHSV(++thue, thissat, 255);
  else if (col == 2) leds[idex] = CHSV(random8(7, 35), random8(150, 255), 255);
  else leds[idex] = CHSV(thishue, thissat, 255);
  LEDS.show();
  if (safeDelay(thisdelay)) return;
}

void ems_lightsONE(boolean col) {                    //-РЕЖИМ 7-EMERGENCY LIGHTS (TWO COLOR SINGLE LED)
  byte a, b;
  if (++idex == LED_COUNT) idex = 0;
  if (col) {
    a = ++thue;
    b = thue + 128;
  }
  else {
    a = thishue;
    b = (thishue + 160) % 255;
  }
  setAll(0, 0, 0);
  leds[idex] = CHSV(a, thissat, 255);
  leds[antipodal_index(idex)] = CHSV(b, thissat, 255);
  LEDS.show();
  if (safeDelay(thisdelay)) return;
}

void ems_lightsALL(boolean col) {                  //-РЕЖИМ 8-EMERGENCY LIGHTS (TWO COLOR SOLID)
  if (++idex == LED_COUNT) {
    idex = 0;
    thue++;
  }
  byte a, b;
  if (col) {
    a = thue;
    b = thue + 128;
  }
  else {
    a = thishue;
    b = (thishue + 160) % 255;
  }
  leds[idex] = CHSV(a, thissat, 255);
  leds[antipodal_index(idex)] = CHSV(b, thissat, 255);
  LEDS.show();
  if (safeDelay(thisdelay)) return;
}

void flicker() {                          //-РЕЖИМ 9-FLICKER EFFECT
  byte random_bright = random8(0, 255);
  byte random_bool = random8(0, random_bright);
  if (random_bool < 10) {
    one_color_allHSV(thishue, thissat, random_bright);
    if (safeDelay(random8(10, 100))) return;
  }
}

void pulse_one_color_all() {              //-РЕЖИМ 10-PULSE BRIGHTNESS ON ALL LEDS TO ONE COLOR
  if (!bouncedirection) if (++ibright == 255) bouncedirection = 1;
  if (bouncedirection) if (--ibright == 1) bouncedirection = 0;
  one_color_allHSV(thishue, thissat, ibright);
  if (safeDelay(thisdelay)) return;
}

void pulse_one_color_all_rev() {           //- РЕЖИМ 11-PULSE SATURATION ON ALL LEDS TO ONE COLOR
  if (!bouncedirection) if (++isat == 255) bouncedirection = 1;
  if (bouncedirection) if (--isat == 1) bouncedirection = 0;
  one_color_allHSV(thishue, isat, 255);
  if (safeDelay(thisdelay)) return;
}

void fade_vertical() {                    //-РЕЖИМ 12-FADE 'UP' THE LOOP
  if (++idex > TOP_INDEX) idex = 0;
  if ((ibright += thisstep) < thisstep) ibright = 0;
  leds[idex] = CHSV(thishue, thissat, ibright);
  leds[horizontal_index(idex)] = CHSV(thishue, thissat, ibright);
  LEDS.show();
  if (safeDelay(thisdelay)) return;
}

void random_red() {                       //QUICK 'N DIRTY RANDOMIZE TO GET CELL AUTOMATA STARTED
  for (int i = 0; i < LED_COUNT; i++ ) {
    if (random8(255) > 127) leds[i].setRGB (255, 0, 0);
    else leds[i] = (0, 0, 0);
  }
  LEDS.show();
}

void rule30() {                          //-РЕЖИМ 13-1D CELLULAR AUTOMATA - RULE 30 (RED FOR NOW)
  if (!bouncedirection) {
    random_red();
    bouncedirection = 1;
  }
  CRGB temp[1] = leds[LED_COUNT - 1];
  for (int i = 0; i < LED_COUNT; i++ ) {
    if (temp[0] != leds[adjacent_cw(i)] && leds[i] == (0, 0, 0)) {
      temp[0] = leds[i];
      leds[i].setRGB (255, 0, 0);
    }
    else if (temp[0] == (0, 0, 0) && leds[i] != (0, 0, 0)) {
      temp[0] = leds[i];
      leds[i].setRGB (255, 0, 0);
    }
    else {
      temp[0] = leds[i];
      leds[i].setRGB(0, 0, 0);
    }
// 111 -> 101
// 110 -> 100
// 101 -> 101
// 100 -> 110
// 011 -> 011
// 010 -> 010
// 001 -> 011
// 000 -> 000
  }
  LEDS.show();
  if (safeDelay(thisdelay)) return;
}

void random_march() {                   //-РЕЖИМ 14-RANDOM MARCH CCW
  for (int i = LED_COUNT - 1; i > 0; i--) {
    leds[i] = leds[adjacent_ccw(i)];
  }
  leds[0] = CHSV(random8(0, 255), 255, 255);
  LEDS.show();
  if (safeDelay(thisdelay)) return;
}

void rwb_march() {                    //-РЕЖИМ 15-R,W,B MARCH CCW
  if (++idex > 2) idex = 0;
  for (int i = LED_COUNT - 1; i > 0; i-- ) {
    leds[i] = leds[adjacent_ccw(i)];
  }
  switch (idex) {
    case 0: leds[0].setRGB (255, 0, 0); break;
    case 1: leds[0].setRGB (255, 255, 255); break;
    case 2: leds[0].setRGB (0, 0, 255); break;
  }
  LEDS.show();
  if (safeDelay(thisdelay)) return;
}

void radiation() {                   //-РЕЖИМ 16-SORT OF RADIATION SYMBOLISH-
  for (byte i = 0; i < LED_COUNT / 6; i++ ) {    //-HACKY, I KNOW...
    if ((tcount += .01) > 3.14) {
      thishue++;
      tcount = 0.0;
    }
    ibright = sin(tcount) * 255;
    leds[(LED_COUNT / 12 * 11 + i) % LED_COUNT] = CHSV(thishue, thissat, ibright);
    leds[LED_COUNT / 12 * 3 + i] = CHSV(thishue, thissat, ibright);
    leds[LED_COUNT / 12 * 7 + i] = CHSV(thishue, thissat, ibright);
  }
  LEDS.show();
  if (safeDelay(thisdelay)) return;
}

void color_loop_vardelay(boolean col, byte sdvig) {                    //-РЕЖИМ 17-COLOR LOOP (SINGLE LED) w/ VARIABLE DELAY
  byte a;
  if (col) a = ++thue;
  else a = thishue;
  if (++idex == LED_COUNT) {
    idex = 0;
    a += sdvig;
    thue = a;
    thishue = a;
  }
  int di = abs(TOP_INDEX - idex);
  int t = constrain((10 / di) * 10, 10, 500);
  setAll(0, 0, 0);
  leds[idex] = CHSV(a, thissat, 255);
  LEDS.show();
  if (safeDelay(t)) return;
}

void white_temps() {                            //-РЕЖИМ 18-SHOW A SAMPLE OF BLACK BODY RADIATION COLOR TEMPERATURES
  byte N9 = LED_COUNT / 9;
  for (byte i = 0; i < N9; i++ ) {
    leds[i].setRGB (255, 147, 41);              //-CANDLE - 1900
    leds[i + N9].setRGB (255, 197, 143);        //-40W TUNG - 2600
    leds[i + (N9 * 2)].setRGB (255, 214, 170);  //-100W TUNG - 2850
    leds[i + (N9 * 3)].setRGB (255, 241, 224);  //-HALOGEN - 3200
    leds[i + (N9 * 4)].setRGB (255, 250, 244);  //-CARBON ARC - 5200
    leds[i + (N9 * 5)].setRGB (255, 255, 251);  //-HIGH NOON SUN - 5400
    leds[i + (N9 * 6)].setRGB (255, 255, 255);  //-DIRECT SUN - 6000
    leds[i + (N9 * 7)].setRGB (201, 226, 255);  //-OVERCAST SKY - 7000
    leds[i + (N9 * 8)].setRGB (64, 156, 255);   //-CLEAR BLUE SKY - 20000
  }
  LEDS.show();
//  delay(100);
}

void sin_bright_wave() {        //-РЕЖИМ 19-BRIGHTNESS SINE WAVE
  for (int i = 0; i < LED_COUNT; i++ ) {
    if ((tcount += .1) > 3.14) tcount = 0.0;
//    ibright = sin(tcount) * 255;
    leds[i] = CHSV(thishue, thissat, sin(tcount) * 255);
    LEDS.show();
    if (safeDelay(thisdelay)) return;
  }
}

void pop_horizontal(boolean col, byte sdvig) {        //-РЕЖИМ 20-POP FROM LEFT TO RIGHT UP THE RING
  int ix;
  byte a;
  if (col) a = ++thue;
  else a = thishue;
  if (!bouncedirection) {
    bouncedirection = 1;
    ix = idex;
  }
  else {
    bouncedirection = 0;
    ix = horizontal_index(idex);
    if (++idex > TOP_INDEX) {
      idex = 0;
      a += sdvig;
      thue = a;
      thishue = a;
    }
  }
  setAll(0, 0, 0);
  leds[ix] = CHSV(a, thissat, 255);
  LEDS.show();
  if (safeDelay(thisdelay)) return;
}

void quad_bright_curve() {      //-РЕЖИМ 21-QUADRATIC BRIGHTNESS CURVER
  int ax;
  for (int x = 0; x < LED_COUNT; x++ ) {
    if (x <= TOP_INDEX) ax = x;
    else if (x > TOP_INDEX) ax = LED_COUNT - x;
    byte a = 1; byte b = 1; byte c = 0;
    int iquad = -(ax * ax * a) + (ax * b) + c; //-ax2+bx+c
    int hquad = -(TOP_INDEX * TOP_INDEX * a) + (TOP_INDEX * b) + c;
    ibright = byte((float(iquad) / float(hquad)) * 255);
    leds[x] = CHSV(thishue, thissat, ibright);
  }
  LEDS.show();
  if (safeDelay(thisdelay)) return;
}

void flame() {                                    //-РЕЖИМ 22-FLAMEISH EFFECT
//  byte idelay = random8(0, 35);
  float hmin = 0.1; float hmax = 45.0;
  float hdif = hmax - hmin;
  byte randtemp = random8(0, 3);
  float hinc = (hdif / float(TOP_INDEX)) + randtemp;
  byte ihue = hmin;
  for (int i = 0; i <= TOP_INDEX; i++ ) {
    leds[i] = CHSV((ihue += hinc), thissat, 255);
    leds[horizontal_index(i)] = CHSV(ihue, thissat, 255);
    leds[TOP_INDEX].setRGB (255, 255, 255);
    LEDS.show();
    if (safeDelay(random8(0, 35))) return;
  }
}

void rainbow_vertical() {                        //-m23-RAINBOW 'UP' THE LOOP
  if (++idex > TOP_INDEX) idex = 0;
  if ((ihue += thisstep) < thisstep) ihue = 0;
  leds[idex] = CHSV(ihue, thissat, 255);
  leds[horizontal_index(idex)] = CHSV(ihue, thissat, 255);
  LEDS.show();
  if (safeDelay(thisdelay)) return;
}

void pacman() {                                  //-m24-REALLY TERRIBLE PACMAN CHOMPING EFFECT
  byte s = LED_COUNT / 4;
  byte a;
  for (byte j = 0; j < 6; j++) {
    for (int i = 0; i < LED_COUNT; i++ ) {
      leds[i].setRGB(255, 255, 0);
    }
    if (j == 1 || j == 5) a = 0;
    if (j == 2 || j == 4) a = 1;
    if (j == 3) a = 2;
    if(j != 0) {
      for (byte x = (s - a); x < (s + a + 1); x++) {
        leds[x] = (0, 0, 0);
      }
    }
    LEDS.show();
    if (safeDelay(thisdelay)) return;
  }
}

void random_color_pop() {                         //-m25-RANDOM COLOR POP
  setAll(0, 0, 0);
  leds[random(LED_COUNT)] = CHSV(random8(255), thissat, 255);
  LEDS.show();
  if (safeDelay(thisdelay)) return;
}

void ems_lightsSTROBE(byte a, byte b) {                  //-m26-EMERGENCY LIGHTS (STROBE LEFT/RIGHT)
  for (byte x = 0 ; x < 5; x++ ) {
    for (int i = 0 ; i < TOP_INDEX; i++ ) {
      leds[i] = CHSV(a, thissat, 255);
    }
    LEDS.show(); if (safeDelay(thisdelay)) return;
    setAll(0, 0, 0);
    if (safeDelay(thisdelay)) return;
  }
  for (byte x = 0 ; x < 5; x++ ) {
    for (int i = TOP_INDEX ; i < LED_COUNT; i++ ) {
      leds[i] = CHSV(b, thissat, 255);
    }
    LEDS.show(); if (safeDelay(thisdelay)) return;
    setAll(0, 0, 0);
    if (safeDelay(thisdelay)) return;
  }
}

void rgb_propeller(boolean col, byte a, byte b, byte c) {                           //-m27-RGB PROPELLER
  if (++idex == LED_COUNT) idex = 0;
  if (col) thishue++;
  for (byte i = 0; i < LED_COUNT; i++ ) {
    if (i < LED_COUNT / 3) leds[(i + idex) % LED_COUNT] = CHSV(a, thissat, 255);
    else if (i < LED_COUNT / 3 * 2)leds[(i + idex) % LED_COUNT] = CHSV(b, thissat, 255);
    else leds[(i + idex) % LED_COUNT] = CHSV(c, thissat, 255);
  }
  LEDS.show();
  if (safeDelay(thisdelay)) return;
}

void kitt() {                                     //-m28-KNIGHT INDUSTIES 2000
  int rand = random(0, TOP_INDEX);
  for (int i = 0; i < rand; i++ ) {
    leds[TOP_INDEX + i] = CHSV(thishue, thissat, 255);
    leds[TOP_INDEX - i] = CHSV(thishue, thissat, 255);
    LEDS.show();
    if (safeDelay(thisdelay / rand)) return;
  }
  for (int i = rand; i > 0; i-- ) {
    leds[TOP_INDEX + i] = CHSV(thishue, thissat, 0);
    leds[TOP_INDEX - i] = CHSV(thishue, thissat, 0);
    LEDS.show();
    if (safeDelay(thisdelay / rand)) return;
  }
}

void matrix(boolean col) {                                   //-m29-ONE LINE MATRIX
  byte a;
  for (int i = LED_COUNT - 1; i > 0; i--) {
    leds[i] = leds[adjacent_ccw(i)];
  }
  if (random8(0, 100) > 90) {
    if (col) a = ++thue;
    else a = thishue;
    leds[0] = CHSV(a, thissat, 255);
  }
  else leds[0] = CHSV(a, thissat, 0);
  LEDS.show();
  if (safeDelay(thisdelay)) return;
}

void strip_march_cw() {                        //-m30-MARCH STRIP CW
  for (int i = 0; i < LED_COUNT; i++ ) {
    leds[i] = leds[adjacent_cw(i)];
  }
  LEDS.show();
  if (safeDelay(thisdelay)) return;
}

void strip_march_ccw() {                        //-m31-MARCH STRIP CCW
  for (int i = LED_COUNT - 1; i >= 0; i--) {
    leds[i] = leds[adjacent_ccw(i)];
  }
  LEDS.show();
  if (safeDelay(thisdelay)) return;
}

void new_rainbow_loop() {                      //-m88-RAINBOW FADE FROM FAST_SPI2
  fill_rainbow( leds, LED_COUNT, --ihue );
  LEDS.show();
  if (safeDelay(thisdelay)) return;
}

void demo_modeA() {
  byte r = 10;
  thisdelay = 20; thisstep = 10; thishue = 0; thissat = 255;
  setAll(255, 255, 255); if (safeDelay(1200)) return;
  for (int i = 0; i < r * 25; i++) {
    rainbow_fade();
  }
  for (int i = 0; i < r * 20; i++) {
    rainbow_loop();
  }
  for (int i = 0; i < r * 20; i++) {
    random_burst();
  }
  for (int i = 0; i < r * 12; i++) {
    color_bounce(0);
  }
  thisdelay = 40;
//  for (int i = 0; i < r * 12; i++) {
//    color_bounceFADE(0);
//  }
  for (int i = 0; i < r * 6; i++) {
    ems_lightsONE(0);
  }
  for (int i = 0; i < r * 5; i++) {
    ems_lightsALL(0);
  }
  thishue = 160; thissat = 50;
  for (int i = 0; i < r * 40; i++) {
    flicker();
  }
  setAll(0, 0, 0);
  thisdelay = 15; thishue = 0; thissat = 255;
  for (int i = 0; i < r * 50; i++) {
    pulse_one_color_all();
  }
  for (int i = 0; i < r * 40; i++) {
    pulse_one_color_all_rev();
  }
  thisdelay = 60; thishue = 180;
  for (int i = 0; i < r * 5; i++) {
    fade_vertical();
  }
  random_red();
  thisdelay = 100;
  for (int i = 0; i < r * 5; i++) {
    rule30();
  }
  thisdelay = 40;
  for (int i = 0; i < r * 8; i++) {
    random_march();
  }
  thisdelay = 80;
  for (int i = 0; i < r * 5; i++) {
    rwb_march();
  }
  setAll(0, 0, 0);
  thisdelay = 60; thishue = 95;
  for (int i = 0; i < r * 15; i++) {
    radiation();
  }
  for (int i = 0; i < r * 15; i++) {
    color_loop_vardelay(0, 0);
  }
  for (int i = 0; i < r * 5; i++) {
    white_temps();
  }
  thisdelay = 35; thishue = 180;
  for (int i = 0; i < r; i++) {
    sin_bright_wave();
  }
  thisdelay = 100; thishue = 0;
  for (int i = 0; i < r * 5; i++) {
    pop_horizontal(0, 0);
  }
  thisdelay = 100; thishue = 180;
  for (int i = 0; i < r * 4; i++) {
    quad_bright_curve();
  }
  setAll(0, 0, 0);
  for (int i = 0; i < r * 3; i++) {
    flame();
  }
  thisdelay = 50;
  for (int i = 0; i < r * 10; i++) {
    pacman();
  }
  thisdelay = 50; thisstep = 15;
  for (int i = 0; i < r * 12; i++) {
    rainbow_vertical();
  }
  thisdelay = 100;
  for (int i = 0; i < r * 3; i++) {
    strip_march_ccw();
  }
  for (int i = 0; i < r * 3; i++) {
    strip_march_cw();
  }
  demo_modeB();
  thisdelay = 5;
  for (int i = 0; i < r * 120; i++) {
    new_rainbow_loop();
  }
  setAll(255, 0, 0); if (safeDelay(1200)) return;
  setAll(0, 255, 0); if (safeDelay(1200)) return;
  setAll(0, 0, 255); if (safeDelay(1200)) return;
  setAll(255, 255, 0); if (safeDelay(1200)) return;
  setAll(0, 255, 255); if (safeDelay(1200)) return;
  setAll(255, 0, 255); if (safeDelay(1200)) return;
}

void demo_modeB() {
  int r = 10;
  setAll(0, 0, 0);
  thisdelay = 35;
  for (int i = 0; i < r * 10; i++) {
    random_color_pop();
  }
  for (int i = 0; i < r / 2; i++) {
    ems_lightsSTROBE(0, 160 % 255);
  }
  thisdelay = 50;
  for (int i = 0; i < r * 10; i++) {
    rgb_propeller(0, 0, 80 % 255, 160 % 255);
  }
  setAll(0, 0, 0);
  thisdelay = 100; thishue = 0;
  for (int i = 0; i < r * 3; i++) {
    kitt();
  }
  setAll(0, 0, 0);
  thisdelay = 30; thishue = 95;
  for (int i = 0; i < r * 25; i++) {
    matrix(0);
  }
  setAll(0, 0, 0);
}

//-----------------------------плавное заполнение цветом-----------------------------------------
void colorWipe(byte red, byte green, byte blue, int SpeedDelay) {
  for (uint16_t i = 0; i < LED_COUNT; i++) {
    leds[i].setRGB (red, green, blue);
    LEDS.show();
    if (safeDelay(SpeedDelay)) return;
  }
}
//-----------------------------------бегающие светодиоды-----------------------------------


//---------------------------------линейный огонь-------------------------------------
void Fire(byte Cooling, byte Sparking, int SpeedDelay) {
  static byte heat[LED_COUNT];
  byte cooldown;

  // Step 1.  Cool down every cell a little
  for ( int i = 0; i < LED_COUNT; i++) {
    cooldown = random8(((Cooling * 10) / LED_COUNT) + 2);
    if (cooldown > heat[i]) heat[i] = 0;
    else heat[i] = heat[i] - cooldown;
  }
  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for ( int k = LED_COUNT - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
  }
  // Step 3.  Randomly ignite new 'sparks' near the bottom
  if ( random8(255) < Sparking ) {
    byte y = random8(7);
    heat[y] = heat[y] + random8(160, 255);
    //heat[y] = random(160,255);
  }
  // Step 4.  Convert heat to LED colors
  for ( int j = 0; j < LED_COUNT; j++) {
    setPixelHeatColor(j, heat[j] );
  }
  LEDS.show();
  if (safeDelay(SpeedDelay)) return;
}

void setPixelHeatColor (int Pixel, byte temperature) {
  // Scale 'heat' down from 0-255 to 0-191
  byte t192 = round((temperature / 255.0) * 191);

  // calculate ramp up from
  byte heatramp = t192 & 0x3F; // 0..63
  heatramp <<= 2; // scale up to 0..252

  // figure out which third of the spectrum we're in:
  if ( t192 > 0x80) leds[Pixel].setRGB (255, 255, heatramp);                    // hottest
  else if ( t192 > 0x40 ) leds[Pixel].setRGB (255, heatramp, 0);            // middle
  else leds[Pixel].setRGB (heatramp, 0, 0);                              // coolest
}

//-------------------------------newKITT---------------------------------------
void NewKITT(boolean col, boolean col2, byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay, byte mode[], byte x) {
  for (byte j = 0; j < x; j++) {
    switch (mode[j]) {
      case 1: 
        for (int i = LED_COUNT - EyeSize - 2; i > 0; i--) {
          lr(col, col2, red, green, blue, EyeSize, i);
          LEDS.show();
          if (safeDelay(SpeedDelay)) return;
        }
        break;
      case 2:
        for (int i = 0; i < LED_COUNT - EyeSize - 2; i++) {
          lr(col, col2, red, green, blue, EyeSize, i);
          LEDS.show();
          if (safeDelay(SpeedDelay)) return;
        }
        break;
      case 3:
        for (int i = 0; i <= ((LED_COUNT - EyeSize) / 2); i++) {
          thue++;
          lr(col, col2, red, green, blue, EyeSize, i);
          oc(col, col2, red, green, blue, EyeSize, i, SpeedDelay);
        }
        break;
      case 4:
        for (int i = ((LED_COUNT - EyeSize) / 2); i >= 0; i--) {
          lr(col, col2, red, green, blue, EyeSize, i);
          oc(col, col2, red, green, blue, EyeSize, i, SpeedDelay);
        }
        break;
    }
    if (safeDelay(ReturnDelay)) return;
  }
}

void lrocut(int i, bool x, bool col, byte a, byte green, byte blue) {
  if(x) {
    if (col) leds[i] = CHSV(a, green, blue / 10);
    else leds[i].setRGB (a / 10, green / 10, blue / 10);
  }
  else {
    if (col) leds[i] = CHSV(a, green, blue);
    else leds[i].setRGB (a, green, blue);
  }
}

void lr(boolean col, boolean col2, byte red, byte green, byte blue, int EyeSize, int i) {
  byte a;
  if (col2) a = ++thue;
  else a = red;
  setAll(0, 0, 0);
  lrocut(i, 1, col, a, green, blue);
  for (int j = 1; j <= EyeSize; j++) {
    lrocut(i + j, 0, col, a, green, blue);
  }
  lrocut(EyeSize + 1 + i, 1, col, a, green, blue);
}

void oc(boolean col, boolean col2, byte red, byte green, byte blue, int EyeSize, int i, int SpeedDelay) {
  byte a;
  if (col2) a = thue;
  else a = red;
  lrocut(LED_COUNT - i - 1, 1, col, a, green, blue);
    for (int j = 1; j <= EyeSize; j++) {
      lrocut(LED_COUNT - j - i - 1, 0, col, a, green, blue);
    }
    lrocut(LED_COUNT - EyeSize - 2 - i, 1, col, a, green, blue);
    LEDS.show();
    if (safeDelay(SpeedDelay)) return;
}

//-------------------------------newKITT---------------------------------------
void rainbowCycle(int SpeedDelay) {
  byte *c;

  for (int j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
    for (int i = 0; i < LED_COUNT; i++) {
      c = Wheel(((i * 256 / LED_COUNT) + j) & 255);
      leds[i].setRGB (*c, *(c + 1), *(c + 2));
    }
    LEDS.show();
    if (safeDelay(SpeedDelay)) return;
  }
}

byte * Wheel(byte WheelPos) {
  static byte c[3];

  if (WheelPos < 85) {
    c[0] = WheelPos * 3;
    c[1] = 255 - WheelPos * 3;
    c[2] = 0;
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    c[0] = 255 - WheelPos * 3;
    c[1] = 0;
    c[2] = WheelPos * 3;
  } else {
    WheelPos -= 170;
    c[0] = 0;
    c[1] = WheelPos * 3;
    c[2] = 255 - WheelPos * 3;
  }
  return c;
}

//-------------------------------TwinkleRandom---------------------------------------
void TwinkleRandom(int Count, int SpeedDelay, boolean OnlyOne) {
  setAll(0, 0, 0);
  for (int i = 0; i < Count; i++) {
    leds[random(LED_COUNT)].setRGB (random8(), random8(), random8());
    LEDS.show();
    if (safeDelay(SpeedDelay)) return;
    if (OnlyOne) setAll(0, 0, 0);
  }
  if (safeDelay(SpeedDelay)) return;
}

//-------------------------------RunningLights---------------------------------------
void RunningLights(byte red, byte green, byte blue, int WaveDelay) {
  int Position = 0;

  for (int i = 0; i < LED_COUNT * 2; i++)
  {
    Position++; // = 0; //Position + Rate;
    for (int i = 0; i < LED_COUNT; i++) {
      // sine wave, 3 offset waves make a rainbow!
      //float level = sin(i+Position) * 127 + 128;
      //setPixel(i,level,0,0);
      //float level = sin(i+Position) * 127 + 128;
      leds[i].setRGB (((sin(i + Position) * 127 + 128) / 255)*red,
               ((sin(i + Position) * 127 + 128) / 255)*green,
               ((sin(i + Position) * 127 + 128) / 255)*blue);
    }

    LEDS.show();
    if (safeDelay(WaveDelay)) return;
  }
}

//-------------------------------Sparkle---------------------------------------
void Sparkle(byte red, byte green, byte blue, int SpeedDelay) {
  leds[random(LED_COUNT)].setRGB (red, green, blue);
  LEDS.show();
  if (safeDelay(SpeedDelay)) return;
  setAll(0, 0, 0);
}

//-------------------------------SnowSparkle---------------------------------------
void SnowSparkle(byte red, byte green, byte blue, int SparkleDelay, int SpeedDelay) {
  setAll(red, green, blue);
  if (safeDelay(SpeedDelay)) return;
  leds[random(LED_COUNT)].setRGB (255, 255, 255);
  LEDS.show();
  if (safeDelay(SparkleDelay)) return;
}

//-------------------------------theaterChase---------------------------------------
void theaterChase(byte red, byte green, byte blue, int SpeedDelay) {
    for (byte q = 0; q < 3; q++) {
      for (int i = 0; i < LED_COUNT; i = i + 3) {
        leds[i + q].setRGB (red, green, blue);
      }
      LEDS.show();
      if (safeDelay(SpeedDelay)) return;
      for (int i = 0; i < LED_COUNT; i++) {
        leds[i] = (0, 0, 0);
      }
    }
}

//-------------------------------theaterChaseRainbow---------------------------------------
void theaterChaseRainbow(int SpeedDelay) {
  byte *c;

  for (byte j = 0; j < 256; j++) {   // cycle all 256 colors in the wheel
    for (byte q = 0; q < 3; q++) {
      for (int i = 0; i < LED_COUNT; i = i + 3) {
        c = Wheel( (i + j) % 255);
        leds[i + q].setRGB (*c, *(c + 1), *(c + 2));
      }
      LEDS.show();
      if (safeDelay(SpeedDelay)) return;
      for (int i = 0; i < LED_COUNT; i++) {
        leds[i] = (0, 0, 0);
      }
    }
  }
}

//-------------------------------Strobe---------------------------------------
void Strobe(byte red, byte green, byte blue, int StrobeCount, int FlashDelay, int EndPause) {
  for (int j = 0; j < StrobeCount; j++) {
    setAll(red, green, blue);
    if (safeDelay(FlashDelay)) return;
    setAll(0, 0, 0);
    if (safeDelay(FlashDelay)) return;
  }
  if (safeDelay(EndPause)) return;
}

//-------------------------------BouncingColoredBalls---------------------------------------
void BouncingColoredBalls(boolean col, byte BallCount, byte colors[][3]) {
  float Gravity = -1.81;
  int StartHeight = 3;

  float Height[BallCount];
  float ImpactVelocityStart = sqrt( -2 * Gravity * StartHeight );
  float ImpactVelocity[BallCount];
  float TimeSinceLastBounce[BallCount];
  int   Position[BallCount];
  long  ClockTimeSinceLastBounce[BallCount];
  float Dampening[BallCount];

  for (byte i = 0 ; i < BallCount ; i++) {
    ClockTimeSinceLastBounce[i] = millis();
    Height[i] = StartHeight;
    Position[i] = 0;
    ImpactVelocity[i] = ImpactVelocityStart;
    TimeSinceLastBounce[i] = 0;
    Dampening[i] = 0.90 - float(i) / pow(BallCount, 2);
  }

  while (true) {
    if (changeFlag) {
      changeFlag = false;
      return;
    }
    for (byte i = 0 ; i < BallCount ; i++) {
      TimeSinceLastBounce[i] =  millis() - ClockTimeSinceLastBounce[i];
      Height[i] = 0.5 * Gravity * pow( TimeSinceLastBounce[i] / 1000 , 2.0 ) + ImpactVelocity[i] * TimeSinceLastBounce[i] / 1000;

      if ( Height[i] < 0 ) {
        Height[i] = 0;
        ImpactVelocity[i] = Dampening[i] * ImpactVelocity[i];
        ClockTimeSinceLastBounce[i] = millis();

        if ( ImpactVelocity[i] < 0.01 ) ImpactVelocity[i] = ImpactVelocityStart;
      }
      Position[i] = round( Height[i] * (LED_COUNT - 1) / StartHeight);
    }

    for (byte i = 0 ; i < BallCount ; i++) {
      if (col == 1) leds[Position[i]] = CHSV(colors[i][0], colors[i][1], colors[i][2]);
      else leds[Position[i]].setRGB (colors[i][0], colors[i][1], colors[i][2]);
    }
    LEDS.show();
    setAll(0, 0, 0);
  }
}

boolean safeDelay(int delTime) {
  uint32_t thisTime = millis();
  while (millis() - thisTime <= delTime) {
    if (changeFlag) {
      changeFlag = false;
      return true;
    }
  }
  return false;
}

void sparkles(byte sparkel_duration, byte sparkel_amount, byte sparkel_spread) {
  static uint8_t sparkle_pixel;
  EVERY_N_MILLISECONDS_I( timingObj, 1) {
    timingObj.setPeriod(sparkel_duration);
    leds[sparkle_pixel] = (0, 0, 0);
    uint8_t previous_pixel = sparkle_pixel;
    while (previous_pixel == sparkle_pixel) {  // pixel can't repeat
      sparkle_pixel = random8(LED_COUNT);
    }
    if (random8(100) < sparkel_amount) {
      //leds[sparkle_pixel] = CRGB(random8(), random8(), random8());
      leds[sparkle_pixel] = CHSV(random8(), random8(20,200), random8(50,255));
    }
  }
  EVERY_N_MILLISECONDS(10) {
    //fadeToBlackBy(leds, NUM_LEDS, 1);  // fade out a bit over time
    blur1d(leds, LED_COUNT, sparkel_spread);  // spreads and fades out color over time
  }
}

void gradient() {
  CHSV hsv( random8(), random8(150, 255), random8(150, 255) );
  CRGB rgb;
  hsv2rgb_rainbow(hsv, rgb);
  byte na = rgb.r;
  byte nb = rgb.g;
  byte nc = rgb.b;

  for (int j = LED_COUNT; j > 0; j--) {
    byte a = map(j - 1, 0, LED_COUNT, na, ta);
    byte b = map(j - 1, 0, LED_COUNT, nb, tb);
    byte c = map(j - 1, 0, LED_COUNT, nc, tc);
    for (int i = LED_COUNT - 1; i > 0; i--) {
      leds[i] = leds[adjacent_ccw(i)];
    }
    leds[0].setRGB(a, b, c);
    FastLED.show();
    if (safeDelay(thisdelay)) return;
  }
  ta = na;
  tb = nb;
  tc = nc;
}

void matrix2(boolean col) {                                   //-m29-ONE LINE MATRIX
  byte rand = random8(0, 100);
  byte a;
  for (byte j = 8; j > 0; j--) {
    for (int i = LED_COUNT - 1; i > 0; i--) {
      leds[i] = leds[adjacent_ccw(i)];
    }
    if (rand > 80) {
      if (col) a = ++thue;
      else a = thishue;
      leds[0] = CHSV(a, thissat, (j - 1) * 32);
    }
    else leds[0] = CHSV(a, thissat, 0);
    LEDS.show();
    if (safeDelay(thisdelay)) return;
  }
}

//void kadr() {
//  idex = 0;
//  for (byte i = 0; i < LED_COUNT / 5 + 1; i++) {
//    tcount = 0.0;
//    byte rand = random8(5, 15);
//    data[i] = random(1);
//    for(int i = 0; i < rand; i++){
//      tcount = tcount + 3.14 / rand;
//      ibright = sin(tcount) * 255;
//      leds[idex] = CHSV(150, 255, ibright);
//      idex++;
//      if (safeDelay(thisdelay)) return;
//      if (idex == LED_COUNT) {
//        idex = 0;
//        continue;
//      }
//    }
//    if (idex == 0) continue;
//  }
//}

//boolean data[LED_COUNT / 5 + 1];
//
//void volna() {
//  if (!boucederiction) {
//    kadr();
//    boncederiction = 1;
//  }
//  tcount = 0.0;
//  for(byte i = 0; i < LED_COUNT / 5 + 1; i++) {
//    if (data[i])
//  }
//}
