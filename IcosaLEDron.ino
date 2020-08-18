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

/*
  Uses an Adafruit Adafruit Feather nRF52840 Sense, with onboard sensors: https://learn.adafruit.com/adafruit-feather-sense/overview
  
  ST Micro series 9-DoF motion - LSM6DS33 Accel/Gyro + LIS3MDL magnetometer
  APDS9960 Proximity, Light, Color, and Gesture Sensor
  PDM Microphone sound sensor
  SHT Humidity
  BMP280 temperature and barometric pressure/altitude

  In Arduino, select board: Adafruit Feather nRF52840 Express
*/

#include <FastLED.h>
#include "Sensors.h"
#include "GradientPalettes.h"

#define DATA_PIN 5
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS 20

#define MILLI_AMPS 500
#define FRAMES_PER_SECOND 120

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

CRGB leds[NUM_LEDS];

uint8_t speed = 16;
uint8_t hue = 0;
uint8_t secondsPerPalette = 10;
uint8_t secondsPerPattern = 10;
uint8_t currentPatternNumber = 0; // Index number of which pattern is current

const float Pi = 3.14159;

#include "Map.h"
#include "Patterns.h"

// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList patterns = {
  complementaryRollPitchHSV,
  //  accelHSV,
  //  xRainbow,
  //  xyRainbow,
  //  yRainbow,
  //  yzRainbow,
  //  zRainbow,
  //  xzRainbow,
  //  angleRainbow,
  //  xPalette,
  //  xyPalette,
  //  yPalette,
  //  yzPalette,
  //  zPalette,
  //  xzPalette,
  //  paletteShift,
  //  rainbow,
  //  rainbowWithGlitter,
  //  confetti,
  //  sinelon,
  //  juggle,
  //  bpm
};

void setup(void) {
  Serial.begin(115200);

  //  delay(1000);

  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setDither(true);
  FastLED.setBrightness(128);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, MILLI_AMPS);
  leds[0] = CRGB::Red;
  FastLED.show();

  // initialize the sensors
  //  bmp280.begin();
  //  lis3mdl.begin_I2C();
  lsm6ds33.begin_I2C();
  //  sht30.begin();

  calibrateIMU(250, 250);
}

void loop(void) {
  // change to a new cpt-city gradient palette
  EVERY_N_SECONDS( secondsPerPalette ) {
    currentPaletteIndex = addmod8( currentPaletteIndex, 1, gradientPaletteCount);
    targetPalette = gradientPalettes[ currentPaletteIndex ];
  }

  EVERY_N_MILLISECONDS(40) {
    nblendPaletteTowardPalette( currentPalette, targetPalette, 8); // slowly blend the current palette to the next
    hue++;  // slowly cycle the "base color" through the rainbow
  }

  if (readIMU()) {
    long currentTime = micros();
    lastInterval = currentTime - lastTime; // expecting this to be ~104Hz +- 4%
    lastTime = currentTime;

    doImuCalculations();
    printCalculations();
  }

  // Call the current pattern function once, updating the 'leds' array
  patterns[currentPatternNumber]();

  FastLED.show();

  EVERY_N_SECONDS( secondsPerPattern ) {
    nextPattern(); // change patterns periodically
    //    Serial.println(currentPatternNumber);
  }
}

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  currentPatternNumber = (currentPatternNumber + 1) % ARRAY_SIZE(patterns);
}
