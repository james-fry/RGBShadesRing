//   Graphical effects to run on the RGB Shades LED array
//   Each function should have the following components:
//    * Must be declared void with no parameters or will break function pointer array
//    * Check effectInit, if false then init any required settings and set effectInit true
//    * Set effectDelay (the time in milliseconds until the next run of this effect)
//    * All animation should be controlled with counters and effectDelay, no delay() or loops
//    * Pixel data should be written using leds[XY(x,y)] to map coordinates to the RGB Shades layout

// Triple Sine Waves
void threeSine() {

  static byte sineOffset = 0; // counter for current position of sine waves

  // startup tasks
  if (effectInit == false) {
    Serial.println("Current Effect: threeSine");
    effectInit = true;
    effectDelay = 20;
  }

  // Draw one frame of the animation into the LED array
  for (byte x = 0; x < kMatrixWidth; x++) {
    for (int y = 0; y < kMatrixHeight; y++) {

      // Calculate "sine" waves with varying periods
      // sin8 is used for speed; cos8, quadwave8, or triwave8 would also work here
      byte sinDistanceR = qmul8(abs(y * (255 / kMatrixHeight) - sin8(sineOffset * 9 + x * 16)), 2);
      byte sinDistanceG = qmul8(abs(y * (255 / kMatrixHeight) - sin8(sineOffset * 10 + x * 16)), 2);
      byte sinDistanceB = qmul8(abs(y * (255 / kMatrixHeight) - sin8(sineOffset * 11 + x * 16)), 2);

      leds[XY(x, y)] = CRGB(255 - sinDistanceR, 255 - sinDistanceG, 255 - sinDistanceB);
    }
  }

  sineOffset++; // byte will wrap from 255 to 0, matching sin8 0-255 cycle

}


// RGB Plasma
void plasma() {

  static byte offset  = 0; // counter for radial color wave motion
  static int plasVector = 0; // counter for orbiting plasma center


  // startup tasks
  if (effectInit == false) {
    Serial.println("Current Effect: plasma");
    effectInit = true;
    effectDelay = 10;
  }

  // Calculate current center of plasma pattern (can be offscreen)
  int xOffset = cos8(plasVector / 256);
  int yOffset = sin8(plasVector / 256);

  // Draw one frame of the animation into the LED array
  for (int x = 0; x < kMatrixWidth; x++) {
    for (int y = 0; y < kMatrixHeight; y++) {
      byte color = sin8(sqrt(sq(((float)x - 7.5) * 10 + xOffset - 127) + sq(((float)y - 2) * 10 + yOffset - 127)) + offset);
      leds[XY(x, y)] = CHSV(color, 255, 255);
    }
  }

  offset++; // wraps at 255 for sin8
  plasVector += 16; // using an int for slower orbit (wraps at 65536)

}


// Scanning pattern left/right, uses global hue cycle
void rider() {

  static byte riderPos = 0;
  // startup tasks
  if (effectInit == false) {
    Serial.println("Current Effect: rider");
    effectInit = true;
    effectDelay = 5;
    riderPos = 0;
  }

  // Draw one frame of the animation into the LED array
  for (byte x = 0; x < kMatrixWidth; x++) {
    int brightness = abs(x * (256 / kMatrixWidth) - triwave8(riderPos) * 2 + 127) * 3;
    if (brightness > 255) brightness = 255;
    brightness = 255 - brightness;
    CRGB riderColor = CHSV(cycleHue, 255, brightness);
    for (byte y = 0; y < kMatrixHeight; y++) {
      leds[XY(x, y)] = riderColor;
    }
  }

  riderPos++; // byte wraps to 0 at 255, triwave8 is also 0-255 periodic

}


// Shimmering noise, uses global hue cycle
void glitter() {

  // startup tasks
  if (effectInit == false) {
    Serial.println("Current Effect: glitter");
    effectInit = true;
    effectDelay = 15;
  }

  // Draw one frame of the animation into the LED array
  for (int x = 0; x < kMatrixWidth; x++) {
    for (int y = 0; y < kMatrixHeight; y++) {
      leds[XY(x, y)] = CHSV(cycleHue, 255, random8(5) * 63);
    }
  }
}


// Gently sparkling and fading LEDs
void sparkle() {
  // startup tasks
  if (effectInit == false) {
    Serial.println("Current Effect: sparkle");
    effectInit = true;
    effectDelay = 100;
  }

  // Draw one frame of the animation into the LED array
  byte pos = random(LAST_VISIBLE_LED);
  if (pos < LAST_VISIBLE_LED){
    leds[pos-1] = CHSV(cycleHue, 0, 100);
    leds[pos] = CHSV(cycleHue, 0, 255);
    leds[pos+1] = CHSV(cycleHue, 0, 100);
  }
  if (pos == LAST_VISIBLE_LED){
    leds[pos-1] = CHSV(cycleHue, 0, 100);
    leds[pos] = CHSV(cycleHue, 0, 255);
    leds[0] = CHSV(cycleHue, 0, 100);
  }
}

/*
// Morse code
// - .... .. ... / .. ... / .- / -- . ... ... .- --. . / .. -. / -- --- .-. ... . / -.-. --- -.. .
void morseCode() {
  static char morse[] = ".... .. ... / .. ... / .- / -- . ... ... .- --. . / .. -. / -- --- .-. ... . / -.-. --- -.. .";
  static byte framebuffer[LAST_VISIBLE_LED] = {0};
  static int stingpos = 0;
  // startup tasks
  if (effectInit == false) {
    Serial.println("Current Effect: morseCode");
    effectInit = true;
    effectDelay = 20;
  }

  // Draw one frame of the animation into the LED array

  leds[random(LAST_VISIBLE_LED)] = CHSV(cycleHue, 0, 255);
}
*/

// Dot spins round the ring (use for 60 led ring)
void dotSpinner() {
  static byte dotPos = 0;
  if (effectInit == false) {
    Serial.println("Current Effect: dotSpinner");
    effectInit = true;
    effectDelay = 5;
    dotPos = 0;
    //blank=0;
  }
  effectDelay = map(analogRead(0), 0, 1023, 0, 100);
  // Draw one frame of the animation into the LED array
  //if (blank=0) {
    leds[dotPos] = CHSV(cycleHue, 255, 255);
    leds[dotPos-1] = CHSV(cycleHue, 255, 0);
    //Serial.println("Dotpos: " + String(dotPos));
    if (dotPos==0) {
      leds[LAST_VISIBLE_LED-1] = CHSV(cycleHue, 255, 0);
    }
    else {
      leds[dotPos-1] = CHSV(cycleHue, 255, 0);
    }

    if (++dotPos >= LAST_VISIBLE_LED){
      dotPos = 0;
    }

}


void ringFlasher() {
  static int analog = analogRead(0);
  static byte brightbyte = 0;

  // startup tasks
  if (effectInit == false) {
    Serial.println("Current Effect: ringFlasher");
    effectInit = true;
    effectDelay = 0;
  }

  // Draw one frame of the animation into the LED array
    analog = analogRead(1);
    brightbyte = map(analog, 0, 1023, 0, 255);
    for (int x = 0; x < LAST_VISIBLE_LED; x++) {
      leds[x] = CHSV(cycleHue, 255, brightbyte);
    }

}


// Fills saturated colors into the array from alternating directions
void colorFill() {

  static byte currentColor = 0;
  static byte currentRow = 0;
  static byte currentDirection = 0;

  // startup tasks
  if (effectInit == false) {
    Serial.println("Current Effect: colorFill");
    effectInit = true;
    effectDelay = 45;
    currentColor = 0;
    currentRow = 0;
    currentDirection = 0;
    currentPalette = RainbowColors_p;
  }

  // test a bitmask to fill up or down when currentDirection is 0 or 2 (0b00 or 0b10)
  if (!(currentDirection & 1)) {
    effectDelay = 45; // slower since vertical has fewer pixels
    for (byte x = 0; x < kMatrixWidth; x++) {
      byte y = currentRow;
      if (currentDirection == 2) y = kMatrixHeight - 1 - currentRow;
      leds[XY(x, y)] = currentPalette[currentColor];
    }
  }

  // test a bitmask to fill left or right when currentDirection is 1 or 3 (0b01 or 0b11)
  if (currentDirection & 1) {
    effectDelay = 20; // faster since horizontal has more pixels
    for (byte y = 0; y < kMatrixHeight; y++) {
      byte x = currentRow;
      if (currentDirection == 3) x = kMatrixWidth - 1 - currentRow;
      leds[XY(x, y)] = currentPalette[currentColor];
    }
  }

  currentRow++;

  // detect when a fill is complete, change color and direction
  if ((!(currentDirection & 1) && currentRow >= kMatrixHeight) || ((currentDirection & 1) && currentRow >= kMatrixWidth)) {
    currentRow = 0;
    currentColor += random8(3, 6);
    if (currentColor > 15) currentColor -= 16;
    currentDirection++;
    if (currentDirection > 3) currentDirection = 0;
    effectDelay = 300; // wait a little bit longer after completing a fill
  }


}

// Emulate 3D anaglyph glasses
void threeDee() {

  // startup tasks
  if (effectInit == false) {
    Serial.println("Current Effect: threeDee");
    effectInit = true;
    effectDelay = 50;
  }

  for (byte x = 0; x < kMatrixWidth; x++) {
    for (byte y = 0; y < kMatrixHeight; y++) {
      if (x < 7) {
        leds[XY(x, y)] = CRGB::Blue;
      } else if (x > 8) {
        leds[XY(x, y)] = CRGB::Red;
      } else {
        leds[XY(x, y)] = CRGB::Black;
      }
    }
  }

  leds[XY(6, 0)] = CRGB::Black;
  leds[XY(9, 0)] = CRGB::Black;

}

// Random pixels scroll sideways, uses current hue
#define rainDir 0
void sideRain() {

  // startup tasks
  if (effectInit == false) {
    Serial.println("Current Effect: sideRain");
    effectInit = true;
    effectDelay = 30;
  }

  scrollArray(rainDir);
  byte randPixel = random8(kMatrixHeight);
  for (byte y = 0; y < kMatrixHeight; y++) leds[XY((kMatrixWidth - 1) * rainDir, y)] = CRGB::Black;
  leds[XY((kMatrixWidth - 1)*rainDir, randPixel)] = CHSV(cycleHue, 255, 255);

}

// Pixels with random locations and random colors selected from a palette
// Use with the fadeAll function to allow old pixels to decay
void confetti() {

  // startup tasks
  if (effectInit == false) {
    Serial.println("Current Effect: confetti");
    effectInit = true;
    effectDelay = 10;
    selectRandomPalette();
  }

  // scatter random colored pixels at several random coordinates
  for (byte i = 0; i < 4; i++) {
    leds[XY(random16(kMatrixWidth), random16(kMatrixHeight))] = ColorFromPalette(currentPalette, random16(255), 255); //CHSV(random16(255), 255, 255);
    random16_add_entropy(1);
  }

}


// Draw slanting bars scrolling across the array, uses current hue
void slantBars() {

  static byte slantPos = 0;

  // startup tasks
  if (effectInit == false) {
    Serial.println("Current Effect: slantBars");
    effectInit = true;
    effectDelay = 5;
  }

  for (byte x = 0; x < kMatrixWidth; x++) {
    for (byte y = 0; y < kMatrixHeight; y++) {
      leds[XY(x, y)] = CHSV(cycleHue, 255, quadwave8(x * 32 + y * 32 + slantPos));
    }
  }

  slantPos -= 4;

}


#define NORMAL 0
#define RAINBOW 1
#define charSpacing 2
// Scroll a text string
void scrollText(byte message, byte style, CRGB fgColor, CRGB bgColor) {


  static byte currentMessageChar = 0;
  static byte currentCharColumn = 0;
  static byte paletteCycle = 0;
  static CRGB currentColor;
  static byte bitBuffer[16] = {0};
  static byte bitBufferPointer = 0;

  // startup tasks
  if (effectInit == false) {
    Serial.println("Current Effect: scrollText");
    effectInit = true;
    effectDelay = 35;
    currentMessageChar = 0;
    currentCharColumn = 0;
    selectFlashString(message);
    loadCharBuffer(loadStringChar(message, currentMessageChar));
    currentPalette = RainbowColors_p;
    for (byte i = 0; i < kMatrixWidth; i++) bitBuffer[i] = 0;
  }

  paletteCycle += 15;

  if (currentCharColumn < 5) { // characters are 5 pixels wide
    bitBuffer[(bitBufferPointer + kMatrixWidth - 1) % kMatrixWidth] = charBuffer[currentCharColumn]; // character
  } else {
    bitBuffer[(bitBufferPointer + kMatrixWidth - 1) % kMatrixWidth] = 0; // space
  }

  CRGB pixelColor;
  for (byte x = 0; x < kMatrixWidth; x++) {
    for (byte y = 0; y < 5; y++) { // characters are 5 pixels tall
      if (bitRead(bitBuffer[(bitBufferPointer + x) % kMatrixWidth], y) == 1) {
        if (style == RAINBOW) {
          pixelColor = ColorFromPalette(currentPalette, paletteCycle+y*16, 255);
        } else {
          pixelColor = fgColor;
        }
      } else {
        pixelColor = bgColor;
      }
      leds[XY(x, y)] = pixelColor;
    }
  }

  currentCharColumn++;
  if (currentCharColumn > (4 + charSpacing)) {
    currentCharColumn = 0;
    currentMessageChar++;
    char nextChar = loadStringChar(message, currentMessageChar);
    if (nextChar == 0) { // null character at end of strong
      currentMessageChar = 0;
      nextChar = loadStringChar(message, currentMessageChar);
    }
    loadCharBuffer(nextChar);
  }

  bitBufferPointer++;
  if (bitBufferPointer > 15) bitBufferPointer = 0;

}


void scrollTextZero() {
  scrollText(0, NORMAL, CRGB::Red, CRGB::Black);
}

void scrollTextOne() {
  scrollText(1, RAINBOW, 0, CRGB::Black);
}

void scrollTextTwo() {
  scrollText(2, NORMAL, CRGB::Green, CRGB(0,0,8));
}



#define analyzerFadeFactor 5
#define analyzerScaleFactor 1.5
#define analyzerPaletteFactor 2
void drawAnalyzer() {

  // startup tasks
  if (effectInit == false) {
    Serial.println("Current Effect: drawAnalyzer");
    effectInit = true;
    effectDelay = 10;
    selectRandomAudioPalette();
  }

  CRGB pixelColor;

  const float yScale = 255.0 / kMatrixHeight;

  for (byte x = 0; x < kMatrixWidth / 2; x++) {
    byte newX = x;
    int freqVal;
    if (x < 2) {
      newX = 0;
      freqVal = spectrumDecay[newX] / 2;
    } else {
      newX = x - 1;
      freqVal = spectrumDecay[newX];
    }

    for (byte y = 0; y < kMatrixHeight; y++) {
      if (x > 6) {
        pixelColor = ColorFromPalette(currentPalette, 0, 0);
      } else {
        int senseValue = freqVal / analyzerScaleFactor - yScale * (kMatrixHeight - 1 - y);
        int pixelBrightness = senseValue * analyzerFadeFactor;
        if (pixelBrightness > 255) pixelBrightness = 255;
        if (pixelBrightness < 0) pixelBrightness = 0;

        int pixelPaletteIndex = senseValue / analyzerPaletteFactor - 15;
        if (pixelPaletteIndex > 240) pixelPaletteIndex = 240;
        if (pixelPaletteIndex < 0) pixelPaletteIndex = 0;

        pixelColor = ColorFromPalette(currentPalette, pixelPaletteIndex, pixelBrightness);
      }
      leds[XY(x, y)] = pixelColor;
      leds[XY(kMatrixWidth - x - 1, y)] = pixelColor;
    }
  }


}

#define VUFadeFactor 5
#define VUScaleFactor 2.0
#define VUPaletteFactor 1.5
void drawVU() {
  // startup tasks


  if (effectInit == false) {
    Serial.println("Current Effect: drawVU");
    effectInit = true;
    effectDelay = 10;
    selectRandomAudioPalette();
  }

  CRGB pixelColor;

  const float xScale = 255.0 / (kMatrixWidth / 2);
  float specCombo = (spectrumDecay[0] + spectrumDecay[1] + spectrumDecay[2] + spectrumDecay[3]) / 4.0;

  for (byte x = 0; x < kMatrixWidth / 2; x++) {
    int senseValue = specCombo / VUScaleFactor - xScale * x;
    int pixelBrightness = senseValue * VUFadeFactor;
    if (pixelBrightness > 255) pixelBrightness = 255;
    if (pixelBrightness < 0) pixelBrightness = 0;

    int pixelPaletteIndex = senseValue / VUPaletteFactor - 15;
    if (pixelPaletteIndex > 240) pixelPaletteIndex = 240;
    if (pixelPaletteIndex < 0) pixelPaletteIndex = 0;

    pixelColor = ColorFromPalette(currentPalette, pixelPaletteIndex, pixelBrightness);

    for (byte y = 0; y < kMatrixHeight; y++) {
      leds[XY(x, y)] = pixelColor;
      leds[XY(kMatrixWidth - x - 1, y)] = pixelColor;
    }
  }


}

/*
void RGBpulse() {

  // startup tasks
  if (effectInit == false) {
    Serial.println("Current Effect: RGBpulse");
    effectInit = true;
    effectDelay = 1;
  }

  static byte RGBcycle = 0;

  fadeAll(1);

  if (beatDetect()) {

    switch (RGBcycle) {
      case 0:
        fillAll(CRGB::Red);
        break;
      case 1:
        fillAll(CRGB::Lime);
        break;
      case 2:
        fillAll(CRGB::Blue);
        break;
    }

    RGBcycle++;
    if (RGBcycle > 2) RGBcycle = 0;
  }

}
*/
