// Code looseley derived initiall from https://learn.adafruit.com/neopixel-painter/test-neopixel-strip
// Dependencies: Adafruit_NeoPixel.h, available from the arduino IDE library manager
#include <Adafruit_NeoPixel.h> 
#define pinStrand 6
#define numberLED 200

Adafruit_NeoPixel lightStrand = Adafruit_NeoPixel(numberLED, pinStrand, NEO_RGB + NEO_KHZ400); //khz400 is for WS2811 lights

void setup() 
{
  lightStrand.begin();
}

void loop() 
{
  chase(lightStrand.Color(255, 0, 0)); // Red
  chase(lightStrand.Color(0, 255, 0)); // Green
  chase(lightStrand.Color(0, 0, 255)); // Blue
 }
 
 static void chase(uint32_t c) 
 {
  for(uint16_t i=0; i<lightStrand.numPixels()+4; i++) 
  {
    lightStrand.setPixelColor(i  , c); // Draw new pixel
    lightStrand.setPixelColor(i-4, 0); // Erase pixel a few steps back
    lightStrand.show();
    delay(25);
  }
}
