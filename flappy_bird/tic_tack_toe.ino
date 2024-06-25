
#include <UTFT.h>
#include <URTouch.h>

// Declare which fonts we will be using from the utft library
extern uint8_t SmallFont[];
extern uint8_t BigFont[];

// Initialize the display for the MEGA
UTFT myGLCD(CTE32_R2,38,39,40,41); // Remember to change the model parameter to suit your display module!
// Initialize touchscreen
URTouch myTouch( 6, 5, 4, 3, 2);

// DECLARE ALL GLOBAL VARIABLES
int x,y; // storage for x and y coordinate touch readings
int upper_Left, upper_Center, upper_Right, middle_Left, middle_Center, middle_Right, lower_Left, lower_Center, lower_Right; // THE VARIABLES FOR THE GAME BOARD
int turn, win_Case, score_1, score_2, numb_Turns;

 

 

//BEGIN SETUP
void setup()
{
Serial.begin(9600); // The serial monitor is only used for debugging, this is not necessaryn for the operation of the game
// Setup the LCD
myGLCD.InitLCD();
myTouch.InitTouch();
myTouch.setPrecision(PREC_MEDIUM);

// Clear the screen on start up to avoid errors
myGLCD.clrScr();
}

 
