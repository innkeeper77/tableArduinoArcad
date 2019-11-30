// Code examples used in creation:
// https://learn.adafruit.com/neopixel-painter/test-neopixel-strip
// https://github.com/allenhuffman/LEDSign

// Dependencies: Adafruit_NeoPixel.h, available from the arduino IDE library manager
#include <Adafruit_NeoPixel.h> 
#define pinStrand 6
#define numberLED 200
#define maxBrightness 255

// LED strip configuration ~~ Define here for how project is wired. Only works for rectangles!
#define numberLED 200
#define ledPerRow 10
const int firstLedPosition = 4; //1,2,3, or 4: cartesian quadrants (EG: 1 is upper right, 4 is bottom right)
const bool zigZag = 1; // 0: straight (false) or 1: zigzag (true)

// CALCULATED DEFINES AND VARIABLE INITIALIZATIONS
#define numberRows (numberLED/ledPerRow);
const int displayArraySize = (numberLED * 3);
uint8_t displayArray [numberLED * 3] = {0} ; // Byte array in format: LED0: R LED0: G LED0: B LED1: R and so on. Color values are 0 - 255
uint8_t displayArrayTestRainbow [numberLED * 3];
uint8_t displayArrayTestGreen [numberLED * 3] = {0};
bool colDirLeft = 0; // INITIAL column directon: false is right, true is left colDir: 1: right 2: left.
bool rowsDirDown = 0; // Rows direction: False = start from bottom and go up. True = start from top and go down

Adafruit_NeoPixel lightStrand = Adafruit_NeoPixel(numberLED, pinStrand, NEO_RGB + NEO_KHZ400); //khz400 is for WS2811 lights

void setup() 
{
  // Begin immediately to set lights to off as soon as possible- default for my strand is every LED full brightness..
  lightStrand.begin();
  lightStrand.setBrightness(maxBrightness);
  lightStrand.show();
  
  Serial.begin(9600);
  Serial.println("Setup commencing");
  
  // Determine additional needed parameters from user selected defines
  // Determine needed directions from layout, change bools if needed
  if (firstLedPosition == 1 || firstLedPosition == 2)
  {
    rowsDirDown = 1;
  }
  if (firstLedPosition == 2 || firstLedPosition == 4)
  {
    colDirLeft = 1;
  }

  
  // Create sample display array test
  int j = 0;
  for(int i=0; i < numberLED; i++)
  {
    if (i%10 == 0)
    {
      displayArrayTestRainbow[j] = 255;
      displayArrayTestRainbow[j+1] = 0;
      displayArrayTestRainbow[j+2] = 0;
    }
    if (i%10 == 1)
    {
      displayArrayTestRainbow[j] = 255;
      displayArrayTestRainbow[j+1] = 127;
      displayArrayTestRainbow[j+2] = 0;
    }
    if (i%10 == 2)
    {
      displayArrayTestRainbow[j] = 255;
      displayArrayTestRainbow[j+1] = 255;
      displayArrayTestRainbow[j+2] = 0;
    }
    if (i%10 == 3)
    {
      displayArrayTestRainbow[j] = 0;
      displayArrayTestRainbow[j+1] = 255;
      displayArrayTestRainbow[j+2] = 0;
    }
    if (i%10 == 4)
    {
      displayArrayTestRainbow[j] = 0;
      displayArrayTestRainbow[j+1] = 0;
      displayArrayTestRainbow[j+2] = 255;
    }
    if (i%10 == 5)
    {
      displayArrayTestRainbow[j] = 75;
      displayArrayTestRainbow[j+1] = 0;
      displayArrayTestRainbow[j+2] = 130;
    }
    if (i%10 == 6)
    {
      displayArrayTestRainbow[j] = 148;
      displayArrayTestRainbow[j+1] = 0;
      displayArrayTestRainbow[j+2] = 211;
    }
    if (i%10 == 7)
    {
      displayArrayTestRainbow[j] = 123;
      displayArrayTestRainbow[j+1] = 123;
      displayArrayTestRainbow[j+2] = 123;
    }
    if (i%10 == 8)
    {
      displayArrayTestRainbow[j] = 0;
      displayArrayTestRainbow[j+1] = 0;
      displayArrayTestRainbow[j+2] = 0;
    }
    if (i%10 == 9)
    {
      displayArrayTestRainbow[j] = 255;
      displayArrayTestRainbow[j+1] = 255;
      displayArrayTestRainbow[j+2] = 255;
    }
    j = j + 3;
  }

  for(int i=0; i < numberLED; i++)
  {
    displayArrayTestGreen[j+1] = 255;
    j = j + 3;
  }
}

void loop() 
{
  chase(lightStrand.Color(255, 0, 0)); // Red
  chase(lightStrand.Color(0, 255, 0)); // Green
  chase(lightStrand.Color(0, 0, 255)); // Blue
  outputArray(displayArrayTestRainbow, numberLED);
  delay(1000);
  outputArray(displayArray, numberLED);
  delay(1000);
  outputArray(displayArrayTestGreen, numberLED);
  delay(1000);
  outputArray(displayArrayTestRainbow, numberLED);
  delay(1000);
  outputArray(displayArray, numberLED);
}

 static void chase(uint32_t c) 
 {
  for(uint16_t i=0; i<lightStrand.numPixels()+4; i++) 
  {
    lightStrand.setPixelColor(i  , c); // Draw new pixel
    lightStrand.setPixelColor(i-4, 0); // Erase pixel a few steps back
    lightStrand.show();
  }
}

void outputArray(uint8_t displayArray[], int displayArraySize)
{
  // TODO: Fix hardcoded pixel orientation- not set up to accept other orientations

  // Array format is top left > top right, down 1 row left > right, etc.
  // For loop: 0 - end, 
  // If rows start at top, start from the top. Else, bottom:
    // In each for loop, if zigzag, every other time switch order printed
    // choose once, starting loop, based on where first LED is placed:

  if (zigZag == 1) // In order to avoid a check each time through the loop, lines of code are sacrificied to only check it once!
  {
    Serial.println("Zigzag = 1");
    // Setup when initial "zig" happens.
    bool zigZagNow = 0;
    if (colDirLeft == 1) zigZagNow = 1;
    Serial.print("Initial zigZagNow = ");
    Serial.print(zigZagNow);
    Serial.print("\n");
    if (rowsDirDown == 1) // Top to bottom LEDs
    {
      //TODO: Implement
    }
    else // Zig zag, bottom to top
    {
      int j = 0;
      for(int i=0; i<displayArraySize; i += ledPerRow)
      {
        if (zigZagNow == 1)
        {
          
          // Display inverted row
          for(int k=(ledPerRow); k>0; k--)
          {
            lightStrand.setPixelColor((i+k-1), (lightStrand.Color(displayArray[j], displayArray[j+1], displayArray[j+2])));
            j = j + 3;
          }
        }
        else
        {
          // Display regular row
          for(int k=0; k<ledPerRow; k++)
          {
            lightStrand.setPixelColor((i+k), (lightStrand.Color(displayArray[j], displayArray[j+1], displayArray[j+2])));
            j = j + 3;
          }
        }
        zigZagNow = !zigZagNow;
      }
    }
  }
  else // NO zigzag
  {
    // TODO: Implement
  }

  
  lightStrand.show();  
}
