// Code examples used in creation:
// https://learn.adafruit.com/neopixel-painter/test-neopixel-strip
// https://github.com/allenhuffman/LEDSign

// Setup: !
// Nintento SNES classic controller is connected to SDA/SCL/Power
// See below to modify code to fit your LED setup
// TODO: Implement code to work with other LED setups

// Dependencies: Adafruit_NeoPixel.h and NintendoExtensionCtrl.h, available from the arduino IDE library manager
#include <Adafruit_NeoPixel.h> 
#include <NintendoExtensionCtrl.h>

SNESMiniController snes;

// LED strip configuration ~~ Define here for how project is wired. Only works for rectangles!
#define pinStrand 6
#define maxBrightness 255
#define numberLED 200
#define ledPerRow 10
const int firstLedPosition = 4; // 1,2,3, or 4: cartesian quadrants (EG: 1 is upper right, 4 is bottom right)
const bool zigZag = 1; // 0: straight (false) or 1: zigzag (true)

// CALCULATED DEFINES AND VARIABLE INITIALIZATIONS
#define numberRows (numberLED/ledPerRow);
const int displayArraySize = (numberLED * 3);
uint8_t displayArray [numberLED * 3] = {0} ; // Byte array in format: LED0: R LED0: G LED0: B LED1: R and so on. Color values are 0 - 255
uint8_t displayArrayTestRainbow [numberLED * 3];
uint8_t displayArrayTestGreen [numberLED * 3] = {0};
bool colDirLeft = 0; // INITIAL column directon: false is right, true is left colDir: 1: right 2: left.
bool rowsDirDown = 0; // Rows direction: False = start from bottom and go up. True = start from top and go down

// Timing
unsigned long prevMillis = 0;
unsigned long currMillis = 0;
unsigned long prevMillisButton = 0;
unsigned long currMillisButton = 0;
bool withinPress = 0;
// Block Stacking Game data
#define ROWS 20
#define COLS 10
#define WHITE 33
#define BLACK 32
struct BLOCK
{
  int l_pos;
  int blockSize;
};

bool board[COLS][ROWS] = {0};
bool game_over = false;
bool active = false;
bool left = true;
int curr_row = 0;
unsigned long wait_timer = 0;
unsigned long TIME_THRESH = 450;
BLOCK active_block;
int block_size = 3;
int miss_counter = 0;
bool win = false;


Adafruit_NeoPixel lightStrand = Adafruit_NeoPixel(numberLED, pinStrand, NEO_RGB + NEO_KHZ400); //khz400 is for WS2811 lights

void setup() 
{
  // Begin immediately to set lights to off as soon as possible- default for my strand is every LED full brightness..
  lightStrand.begin();
  lightStrand.setBrightness(maxBrightness);
  lightStrand.show();
  
  Serial.begin(9600);
  Serial.println("Setup commencing");

  snes.begin();

  while (!snes.connect()) 
  {
    Serial.println("Nintendo Controller not detected! Retry in 1 second");
    delay(1000);
  }
  
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

  // ~~~~~~~~~~~~~~ The following setup code is non critical, and is for testing purposes ~~~~~~~~~
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
  // ~~~~~~~~~~~~~~ END testing in setup ~~~~~~~~~

  // Reset games:
  resetBlockStackingGame();
}

// ~~~~ Global variables here. It's fine, it's an arduino! ~~~~~

int gameChoice = 1;
bool buttonPressInProcess = 0;
bool displayOn = 0;

// Controller button state bools
boolean padUp = 0;
boolean padDown = 0;
boolean padLeft = 0;
boolean padRight = 0;
boolean aButton = 0;
boolean bButton = 0;
boolean xButton = 0;
boolean yButton = 0;
boolean startButton = 0;
boolean selectButton = 0;
boolean lButton = 0;
boolean rButton = 0;

void loop() 
{
  //delay(2);
  // General Data
  bool controllerReadSuccess = snes.update();  // Get new data from the controller
  if (!controllerReadSuccess) // Error condition, no controller
  {
    Serial.println("Controller disconnected! Arduino delaying until retry");
    delay(1000);
  }
  else // Normal
  {
    // Read the controller dpad (Up/Down/Left/Right) dpadUp() etc
    padUp = snes.dpadUp();
    padDown = snes.dpadDown();
    padLeft = snes.dpadLeft();
    padRight = snes.dpadRight();
    // read the controller buttons (A/B/X/Y, Start/Select, L/R) snes.buttonA() etc
    aButton = snes.buttonA();
    bButton = snes.buttonB();
    xButton = snes.buttonX();
    yButton = snes.buttonY();
    startButton = snes.buttonStart();
    selectButton = snes.buttonSelect();
    lButton = snes.buttonZL();
    rButton = snes.buttonZR();
  }


  //Serial.println(game_over);
  //Serial.println(miss_counter);
  //Serial.println();
  
  // Block Stacking Game Logic
  if (gameChoice == 1)
  {
    if (controllerFree())
    {
      withinPress = 0;
    }
   {
   if (active == false)
    {
      for (int i = active_block.l_pos; i < active_block.l_pos+active_block.blockSize; ++i)
      {
        board[i][curr_row] = 1;
      }
      miss_counter = 0;
      active = true;
      prevMillis = millis();
      print_board(board, displayArray);
    }
    else
    {
      if (wait_timer >= TIME_THRESH)
      {
        if (left)
        {
          if (active_block.l_pos + active_block.blockSize > COLS - 1)
          {
            left = false;
          }
          else
          {
            board[active_block.l_pos][curr_row] = 0;
            ++active_block.l_pos;
            for (int i = active_block.l_pos; i < active_block.l_pos + active_block.blockSize; ++i)
            {
              board[i][curr_row] = 1;
            }           
          }
        }
        else
        {
          if (active_block.l_pos <= 0)
          {
            left = true;
          }
          else
          {
            board[active_block.l_pos+active_block.blockSize-1][curr_row] = 0;
            --active_block.l_pos;
            for (int i = active_block.l_pos; i < active_block.l_pos + active_block.blockSize; ++i)
            {
              board[i][curr_row] = 1;
            }
          }
        }
        print_board(board, displayArray);
        wait_timer = 0;
        prevMillis = millis();
      }
      else
      {
        //key poll here
        if (buttonPressed())
        {
          if (curr_row == 0)
          {
            ++curr_row;
            active_block.l_pos = 4;
            active_block.blockSize = block_size;
            active = false;
          }
          else
          {
            if (left)
            {
              for (int i = active_block.l_pos; i < active_block.l_pos + active_block.blockSize; ++i)
              {
                if (board[i][curr_row - 1] == (char)BLACK)
                {
                  board[i][curr_row] = (char)BLACK;
                  ++miss_counter;
                }
              }
              if (miss_counter >= block_size)
              {
                game_over = true;
              }
            }
            else
            {
              for (int i = active_block.l_pos; i < active_block.l_pos + active_block.blockSize; ++i)
              {
                if (board[i][curr_row - 1] == (char)BLACK)
                {
                  board[i][curr_row] = (char)BLACK;
                  ++miss_counter;
                }
              }
              if (miss_counter >= block_size)
              {
                game_over = true;
              }
            }
            ++curr_row;
            if (curr_row > 6)
            {
              block_size = 2;
              TIME_THRESH = 175;
            }
            else if (curr_row > 12)
            {
              block_size = 1;
              TIME_THRESH = 100;
            }
            else if (curr_row == 18)
            {
              game_over = true;
              win = true;
            }
            active_block.l_pos = 4;
            active_block.blockSize = block_size;
            active = false;
          }
        }
        //Polling stuff here
        currMillis = millis();
        wait_timer = currMillis - prevMillis;
      }
    }
  }
  if (win && game_over)
  {
    //cout << "You Won!" << endl;
    outputArray(displayArrayTestRainbow, 200);
    delay(3000);
    resetBlockStackingGame();
  }
  else if (game_over)
  {
    //cout << "You Lost." << endl;
    chase(lightStrand.Color(255, 0, 0));
    resetBlockStackingGame();
  }
  }
}

// ~~~~~~~~ General functions ~~~~~~~~
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

  if (zigZag == 1) // In order to avoid a check each time through the loop, some code is duplicated here
  {
    // Setup when initial "zig" happens.
    bool zigZagNow = 0;
    if (colDirLeft == 1) zigZagNow = 1;
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

// ~~~~~~~ Block Stacking Game Functions
void print_board(bool board[COLS][ROWS], uint8_t displayArray[])
{
  int arrayCounter = 0;
  for (int i = ROWS-1; i >= 0; --i)
  {
    for (int j = 0; j < COLS; ++j)
    {
      //cout << board[j][i];
      //Serial.print(board[j][i]);
      
      if (board[j][i] == 0)
      {
      displayArray[arrayCounter] = 0;
      displayArray[arrayCounter + 1] = 0;
      displayArray[arrayCounter + 2] = 0;
      }
      else if (board[j][i] == 1)
      {
      displayArray[arrayCounter] = 0;
      displayArray[arrayCounter + 1] = 110;
      displayArray[arrayCounter + 2] = 0;
      }
      arrayCounter = (arrayCounter + 3);
    }
    //Serial.print("\n");
  }
  outputArray(displayArray, 200);
}

void resetBlockStackingGame()
{
  for (int i = 0; i < ROWS; ++i)
  {
    for (int j = 0; j < COLS; ++j)
    {
      board[j][i] = 0;
    }
  }
  game_over = false;
  active = false;
  left = true;
  curr_row = 0;
  wait_timer = 0;
  active_block.l_pos = 0;
  active_block.blockSize = 3;
}

bool buttonPressed()
{
  
  if (aButton == 1 || bButton == 1 || xButton == 1 || yButton == 1 || lButton == 1 || rButton == 1)
  {
    if (!withinPress)
    {
      withinPress = 1;
      Serial.println("START PRESS");
      return 1;
    }
  }
  return 0;
}

bool controllerFree()
{
  
  if (aButton == 0 && bButton == 0 && xButton == 0 && yButton == 0 && lButton == 0 && rButton == 0)
  {
    return 1;
  }
  return 0;
}
