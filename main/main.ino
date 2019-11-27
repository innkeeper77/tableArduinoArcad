// Code looseley derived initiall from https://learn.adafruit.com/neopixel-painter/test-neopixel-strip
// Dependencies: Adafruit_NeoPixel.h, available from the arduino IDE library manager
#include <Adafruit_NeoPixel.h> 
#define pinStrand 6
#define numberLED 200
#define maxBrightness 200

uint8_t displayArray [numberLED * 3]; // Byte array in format: LED0: R LED0: G LED0: B LED1: R and so on. Color values are 0 - 255
const int displayArraySize = (numberLED * 3);

Adafruit_NeoPixel lightStrand = Adafruit_NeoPixel(numberLED, pinStrand, NEO_RGB + NEO_KHZ400); //khz400 is for WS2811 lights

void setup() 
{
  Serial.begin(9600);
  Serial.println("Setup commencing");
  
  // Sample display array
  int j = 0;
  for(int i=0; i < numberLED; i++)
  {
    if (i%10 == 0)
    {
      displayArray[j] = 255;
      displayArray[j+1] = 0;
      displayArray[j+2] = 0;
    }
    if (i%10 == 1)
    {
      displayArray[j] = 255;
      displayArray[j+1] = 127;
      displayArray[j+2] = 0;
    }
    if (i%10 == 2)
    {
      displayArray[j] = 255;
      displayArray[j+1] = 255;
      displayArray[j+2] = 0;
    }
    if (i%10 == 3)
    {
      displayArray[j] = 0;
      displayArray[j+1] = 255;
      displayArray[j+2] = 0;
    }
    if (i%10 == 4)
    {
      displayArray[j] = 0;
      displayArray[j+1] = 0;
      displayArray[j+2] = 255;
    }
    if (i%10 == 5)
    {
      displayArray[j] = 75;
      displayArray[j+1] = 0;
      displayArray[j+2] = 130;
    }
    if (i%10 == 6)
    {
      displayArray[j] = 148;
      displayArray[j+1] = 0;
      displayArray[j+2] = 211;
    }
    if (i%10 == 7)
    {
      displayArray[j] = 123;
      displayArray[j+1] = 123;
      displayArray[j+2] = 123;
    }
    if (i%10 == 8)
    {
      displayArray[j] = 0;
      displayArray[j+1] = 0;
      displayArray[j+2] = 0;
    }
    if (i%10 == 9)
    {
      displayArray[j] = 255;
      displayArray[j+1] = 255;
      displayArray[j+2] = 255;
    }
    j = j + 3;
  }
  
  lightStrand.begin();
  lightStrand.setBrightness(maxBrightness);
  lightStrand.show();
}

void loop() 
{
  chase(lightStrand.Color(255, 0, 0)); // Red
  chase(lightStrand.Color(0, 255, 0)); // Green
  chase(lightStrand.Color(0, 0, 255)); // Blue
  outputArray(displayArray, numberLED);
  delay(5000);
}
 
 static void chase(uint32_t c) 
 {
  for(uint16_t i=0; i<lightStrand.numPixels()+4; i++) 
  {
    lightStrand.setPixelColor(i  , c); // Draw new pixel
    lightStrand.setPixelColor(i-4, 0); // Erase pixel a few steps back
    lightStrand.show();
    delay(2);
  }
}

void outputArray(uint8_t displayArray[], int displayArraySize)
{
  int j = 0;
  for(int i=0; i<displayArraySize; i++)
  {
    lightStrand.setPixelColor(i, (lightStrand.Color(displayArray[j], displayArray[j+1], displayArray[j+2])));
    ; // Color order: R,W,G,B.
    j = j + 3;
  }
  lightStrand.show();  
}
