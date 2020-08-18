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

// These coordinates were mapped while holding the IcosaLEDron with the face with the first LED and USB port flat on top
// with the USB port towards the back, and the point of that triangle face pointing forward.
// Looking down on the IcosaLEDron, the origin is at the bottom left corner, with x increasing from the left towards the right,
// y increasing from the front to the back, and z increasing from the bottom towards the top.

const uint8_t coordsX[NUM_LEDS] = { 128, 43, 85, 170, 213, 255, 213, 128, 43, 0, 0, 43, 128, 213, 255, 213, 170, 85, 43, 128 };
const uint8_t coordsY[NUM_LEDS] = { 170, 85, 0, 0, 85, 170, 255, 255, 255, 170, 85, 0, 0, 0, 85, 170, 255, 255, 170, 85 };
const uint8_t coordsZ[NUM_LEDS] = { 255, 204, 153, 153, 204, 153, 153, 204, 153, 153, 102, 102, 51, 51, 102, 51, 102, 102, 51, 0 };
const uint8_t  angles[NUM_LEDS] =  { 255, 28, 85, 142, 198, 198, 227, 255, 0, 28, 57, 85, 113, 142, 170, 170, 227, 0, 28, 113 };

uint8_t hues = 1;

void xRainbow() {
  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    uint8_t x = coordsX[i];

    leds[i] = CHSV(beat8(speed) - (x * hues), 255, 255);
  }
}

void xyRainbow() {
  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    uint8_t x = coordsX[i];
    uint8_t y = coordsY[i];

    leds[i] = CHSV(beat8(speed) - ((x + y) * hues), 255, 255);
  }
}

void yRainbow() {
  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    uint8_t y = coordsY[i];

    leds[i] = CHSV(beat8(speed) - (y * hues), 255, 255);
  }
}

void yzRainbow() {
  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    uint8_t y = coordsY[i];
    uint8_t z = coordsZ[i];

    leds[i] = CHSV(beat8(speed) - ((y + z) * hues), 255, 255);
  }
}

void zRainbow() {
  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    uint8_t z = coordsY[i];

    leds[i] = CHSV(beat8(speed) - (z * hues), 255, 255);
  }
}

void xzRainbow() {
  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    uint8_t x = coordsX[i];
    uint8_t z = coordsZ[i];

    leds[i] = CHSV(beat8(speed) - ((x + z) * hues), 255, 255);
  }
}

void xyzRainbow() {
  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    uint8_t x = coordsX[i];
    uint8_t y = coordsY[i];
    uint8_t z = coordsZ[i];

    leds[i] = CHSV(beat8(speed) - ((x + y + z) * hues), 255, 255);
  }
}

void angleRainbow() {
  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    uint8_t a = angles[i];

    leds[i] = CHSV(beat8(speed) - (a * hues), 255, 255);
  }
}

void xPalette() {
  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    uint8_t x = coordsX[i];

    leds[i] = ColorFromPalette(currentPalette, beat8(speed) - (x * hues));
  }
}

void xyPalette() {
  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    uint8_t x = coordsX[i];
    uint8_t y = coordsY[i];

    leds[i] = ColorFromPalette(currentPalette, beat8(speed) - ((x + y) * hues));
  }
}

void yPalette() {
  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    uint8_t y = coordsY[i];

    leds[i] = ColorFromPalette(currentPalette, beat8(speed) - (y * hues));
  }
}

void yzPalette() {
  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    uint8_t y = coordsY[i];
    uint8_t z = coordsZ[i];

    leds[i] = ColorFromPalette(currentPalette, beat8(speed) - ((y + z) * hues));
  }
}

void zPalette() {
  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    uint8_t z = coordsZ[i];

    leds[i] = ColorFromPalette(currentPalette, beat8(speed) - (z * hues));
  }
}

void xzPalette() {
  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    uint8_t x = coordsX[i];
    uint8_t z = coordsZ[i];

    leds[i] = ColorFromPalette(currentPalette, beat8(speed) - ((x + z) * hues));
  }
}

void anglePalette() {
  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    uint8_t a = angles[i];

    leds[i] = ColorFromPalette(currentPalette, beat8(speed) - (a * hues));
  }
}
