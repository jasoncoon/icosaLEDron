/*
   IcosaLEDron: https://github.com/jasoncoon/IcosaLEDron
   Copyright (C) 2020 Jason Coon
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

float mapf(float x, float in_min, float in_max, uint8_t out_min, uint8_t out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void accelHSV() {
  uint8_t x = mapf(accelX, -10.0, 10.0, 0, 255);
  uint8_t y = mapf(accelY, -10.0, 10.0, 0, 255);
  //  uint8_t z = mapf(accelZ, -10.0, 10.0, 0, 255);

  EVERY_N_MILLIS(100) {
    Serial.print(x);
    Serial.print(" ");
    Serial.print(y);
    //    Serial.print(" ");
    //    Serial.print(z);
    Serial.println(" ");
  }

  CHSV color = CHSV(x, 255, y);

  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    leds[i] = color;
  }
}

void complementaryRollPitchHSV() {
  uint8_t roll = mapf(complementaryRoll + 180.0, 0, 360.0, 0, 255);
  uint8_t pitch = mapf(complementaryPitch + 180.0, 0, 360.0, 0, 255);

//  EVERY_N_MILLIS(100) {
//    Serial.print(x);
//    Serial.print(" ");
//    Serial.print(y);
//    Serial.println(" ");
//  }

  CHSV color = CHSV(roll, 255, pitch);

  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    leds[i] = color;
  }
}

void paletteShift()
{
  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette(currentPalette, i + hue, 255, LINEARBLEND);
  }
}

void rainbow()
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, hue, 7);
}

void addGlitter( fract8 chanceOfGlitter)
{
  if ( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void rainbowWithGlitter()
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}

void confetti()
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( hue + random8(64), 200, 255);
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16( 13, 0, NUM_LEDS - 1 );
  leds[pos] += CHSV( hue, 255, 192);
}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for ( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, hue + (i * 2), beat - hue + (i * 10));
  }
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for ( int i = 0; i < 8; i++) {
    leds[beatsin16( i + 7, 0, NUM_LEDS - 1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}
