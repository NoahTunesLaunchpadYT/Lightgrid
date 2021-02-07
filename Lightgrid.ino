/*
 *     Lightgrid.ino by NoahTunes Launchpad
 *     ----------------------------------
 *     USB MIDI to WS2812B by mat1jaczczcyyyyy
 *     ----------------------------------
 *    
 */

/*  
 * LED Strip definition
 * --------------------
 */

const uint16_t _nLED = 300;  // Number of LEDs
const uint16_t BRIGHTNESS = 100; //Brightness of Lightgrid
const byte _dLED = 1;   // Data pin

#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel _LED = Adafruit_NeoPixel(_nLED, _dLED, NEO_GRB + NEO_KHZ800);

/*
 * Color Palette
 * Generate with retinaConverter.py (Retina 2.0+ Palette)
 * ------------------------------------------------------
 */

const uint16_t _r[128] = {0, 16, 32, 63, 63, 63, 32, 16, 63, 63, 32, 16, 63, 63, 32, 16, 33, 20, 10, 5, 18, 0, 0, 0, 18, 0, 0, 0, 18, 0, 0, 0, 18, 0, 0, 0, 18, 0, 0, 0, 18, 0, 0, 0, 11, 0, 0, 0, 26, 11, 6, 3, 63, 63, 32, 16, 63, 63, 32, 16, 63, 37, 29, 8, 0, 0, 0, 0, 0, 4, 31, 7, 63, 46, 43, 24, 3, 0, 0, 0, 6, 22, 43, 10, 63, 33, 28, 0, 14, 21, 13, 22, 12, 26, 52, 63, 63, 45, 35, 32, 14, 0, 3, 5, 5, 25, 32, 54, 53, 63, 39, 25, 5, 54, 31, 38, 35, 15, 28, 55, 39, 13, 6, 1, 45, 15, 44, 18};
const uint16_t _g[128] = {0, 16, 32, 63, 15, 0, 0, 0, 46, 15, 8, 4, 43, 63, 32, 16, 63, 63, 32, 16, 63, 63, 32, 16, 63, 63, 32, 16, 63, 63, 32, 16, 63, 63, 32, 16, 48, 41, 21, 11, 33, 21, 11, 6, 9, 0, 0, 0, 13, 0, 0, 0, 15, 0, 0, 0, 16, 0, 0, 0, 3, 13, 20, 13, 14, 18, 5, 0, 17, 0, 31, 7, 0, 63, 58, 63, 34, 63, 41, 10, 0, 0, 6, 4, 12, 55, 63, 63, 63, 63, 63, 34, 20, 20, 7, 0, 17, 41, 63, 22, 10, 18, 19, 5, 7, 14, 0, 16, 18, 47, 55, 44, 5, 52, 58, 37, 25, 15, 28, 63, 0, 0, 51, 16, 43, 12, 20, 5};
const uint16_t _b[128] = {0, 16, 32, 63, 15, 0, 0, 0, 26, 0, 0, 0, 11, 0, 0, 0, 12, 0, 0, 0, 18, 0, 0, 0, 23, 6, 3, 1, 22, 21, 11, 6, 45, 37, 18, 9, 63, 63, 32, 16, 63, 63, 32, 16, 63, 63, 32, 16, 62, 63, 32, 16, 63, 63, 32, 16, 27, 20, 10, 5, 0, 0, 0, 1, 0, 6, 27, 63, 19, 50, 31, 7, 0, 11, 1, 2, 0, 23, 63, 63, 63, 63, 30, 0, 0, 1, 5, 0, 9, 27, 50, 63, 48, 57, 63, 22, 0, 0, 0, 1, 0, 3, 8, 10, 22, 6, 0, 10, 4, 9, 11, 3, 11, 26, 34, 63, 63, 15, 28, 63, 0, 0, 0, 0, 0, 0, 0, 0};

/*
 * MIDI handler
 * ------------
 */

const byte _pStart = 0;  // First note in array
bool update = false;
int counter = 0;
int dif = 36;

void noteOn(byte ch, byte p, byte v) {
  _LED.setPixelColor(p + ((ch-1) * 100 - dif) - _pStart, _r[v], _g[v], _b[v]);
  update = true;
}

void noteOff(byte ch, byte p, byte v) {
  _LED.setPixelColor(p + ((ch-1) * 100 - dif) - _pStart, 0, 0, 0);
  update = true;
}

void sysEx(byte *data, unsigned int length) {
  if (length == 6) {
    //_LED.setPixelColor(*(data+1) - _pStart, *(data+2), *(data+3), *(data+4));
    update = true;
  }
}

/*
 * Initialize LED Strip, Startup Animation "meteorRain", and USB MIDI
 * ---------------------------------
 */

void setup() {
  randomSeed(analogRead(14));
  _LED.begin();
  _LED.setBrightness(BRIGHTNESS);
  _LED.clear();
  int r = random(0, 255);
  int g = random(0, 255 - r);
  int b = 255 - r - g; 

  meteorRain(r, g, b, 20, 60, true, 1, 1);
  
  usbMIDI.setHandleNoteOn(noteOn);
  usbMIDI.setHandleNoteOff(noteOff);
  usbMIDI.setHandleSystemExclusive(sysEx);
} 

/* 
 * Read MIDI and update Strip
 * --------------------------
 */
void loop() {
  while (usbMIDI.read());
  if (update) {
    _LED.show();
    counter ++; 
    Serial.println(counter);
    update = false;
  }
}

void meteorRain(byte red, byte green, byte blue, byte meteorSize, byte meteorTrailDecay, boolean meteorRandomDecay, int SpeedDelay, int stepSize) {  
  _LED.clear();
 
  for(int i = 0; i < (_nLED+_nLED); i += stepSize) {
   
   
    // fade brightness all LEDs one step
    for(int j=0; j<_nLED; j++) {
      if( (!meteorRandomDecay) || (random(10)>5) ) {
        fadeToBlack(j, meteorTrailDecay );        
      }
    }
    
   
    // draw meteor
    for(int j = 0; j < meteorSize; j++) {
      if( ( i-j < _nLED) && (i-j>=0) ) {
        _LED.setPixelColor(i-j, red, green, blue);
      }
    }
   
    _LED.show();
    delay(SpeedDelay);
  }
}


void fadeToBlack(int ledNo, byte fadeValue) {
 #ifdef ADAFRUIT_NEOPIXEL_H
    // NeoPixel
    uint32_t oldColor;
    uint8_t r, g, b;
    int value;
   
    oldColor = _LED.getPixelColor(ledNo);
    r = (oldColor & 0x00ff0000UL) >> 16;
    g = (oldColor & 0x0000ff00UL) >> 8;
    b = (oldColor & 0x000000ffUL);

    r=(r<=10)? 0 : (int) r-(r*fadeValue/256);
    g=(g<=10)? 0 : (int) g-(g*fadeValue/256);
    b=(b<=10)? 0 : (int) b-(b*fadeValue/256);
   
    _LED.setPixelColor(ledNo, r,g,b);
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   leds[ledNo].fadeToBlackBy( fadeValue );
 #endif  
}
