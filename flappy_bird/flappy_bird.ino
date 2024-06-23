#include <SdFat.h>
#include <UTFT.h>
#include <UTFT_SdRaw.h>
#include <URTouch.h>
#include <EEPROM.h>
#include <SPI.h>


// clock
extern uint8_t SmallFont[];
#define SD_CHIP_SELECT 53
SdFat sd;


UTFT    myGLCD(CTE32_R2, 38, 39, 40, 41); //Parameters should be adjusted to your Display/Schield model
URTouch  myTouch( 6, 5, 4, 3, 2);
UTFT_SdRaw myFiles(&myGLCD);

//==== Defining Fonts
extern uint8_t SmallFont[];
extern uint8_t BigFont[];
extern uint8_t SevenSegNumFont[];

int color = 0;
word colorlist[] = {VGA_WHITE, VGA_BLACK, VGA_RED, VGA_BLUE, VGA_GREEN, VGA_FUCHSIA, VGA_YELLOW, VGA_AQUA};
int  bsize = 4;


extern unsigned int bird01[0x41A]; // Bird Bitmap
extern unsigned int  bird02[0x41A]; // Bird Bitmap

int upper_Left, upper_Center, upper_Right, middle_Left, middle_Center, middle_Right, lower_Left, lower_Center, lower_Right; // THE VARIABLES FOR THE GAME BOARD
int turn, win_Case, score_1, score_2, numb_Turns;
extern unsigned int bird01[0x41A]; // Bird Bitmap
int x, y; // Variables for the coordinates where the display has been pressed
int x2, y2, x3, y3;
char currentPage, selectedUnit;
char stCurrent[20] = "";
int stCurrentLen = 0;
char stLast[20] = "";
char stpass[3] = "234";
// Floppy Bird
int xP = 319;
int yP = 100;
int yB = 50;
int movingRate = 3;
int fallRateInt = 0;
float fallRate = 0;
int score = 0;
int score1 = 0;
int lastSpeedUpScore = 0;
int highestScore;
boolean screenPressed = false;
boolean gameStarted = false;
int gameNumber = 0;
int imagenumber = 0;
int radius = 10;
boolean newPillar = true;
const int X_pin = A1; // analog pin connected to X output
const int Y_pin = A0;
const int button = 8;
int buttonState = 0;
int xvalue = 0;
int yvalue = 0;
      int xx = 0;
      int yy = 0;
       int avx;
      int avy;
int buzzer = 12;



void setup() {
  pinMode(button, INPUT);
  pinMode(buzzer, OUTPUT);
  digitalWrite(button, HIGH);
  pinMode (Y_pin, INPUT);
  pinMode (X_pin, INPUT);
  myGLCD.InitLCD();
  myGLCD.clrScr();
  myTouch.InitTouch();
  myTouch.setPrecision(PREC_MEDIUM);
  highestScore = EEPROM.read(0); // Read the highest score from the EEPROM
  myGLCD.clrScr();;

  
    if (!sd.begin(SD_CHIP_SELECT, SPI_FULL_SPEED))
  {
    Serial.println(F("card failed or not present"));// put your setup code here, to run once:
    Serial.println(F("Rettrying. . ."));
  }
  else
  {
//    mysd = 1;
    Serial.println(F("Card initialised."));
  }
     myFiles.load(0, 0, 320, 240, "img_1.RAW", 1, 0); 
  drawHomeScreen();
  myGLCD.setBrightness(16);                
  myGLCD.drawRoundRect (10, 10, 60, 36); // Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
      myGLCD.setFont(SmallFont); // Sets the font to big
      myGLCD.setBackColor(225, 0, 0); // Sets the background color of the area where the text will be printed to green, same as the button
      myGLCD.print("Back", LEFT, 10); // Prints the string// draws home screen

}

void loop() {
       if (gameNumber < 1){
       drawHomeScreen();
       }

 
 myGLCD.drawRoundRect (10, 10, 60, 36); // Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
      myGLCD.setFont(SmallFont); // Sets the font to big
      myGLCD.setBackColor(225, 0, 0); // Sets the background color of the area where the text will be printed to green, same as the button
      myGLCD.print("Back", LEFT, 10); // Prints the string// draws home screen


  if (gameNumber != 0) {
    if (myTouch.dataAvailable()) {
      myTouch.read();
      x = myTouch.getX(); // X coordinate where the screen has been pressed
      y = myTouch.getY(); // Y coordinates where the screen has been pressed

      // If we press the Back Button
      myGLCD.drawRoundRect (10, 10, 60, 36); // Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
      myGLCD.setFont(SmallFont); // Sets the font to big
      myGLCD.setBackColor(225, 0, 0); // Sets the background color of the area where the text will be printed to green, same as the button
      myGLCD.print("Back", LEFT, 10); // Prints the string
      if ((x >= 10) && (x <= 60) && (y >= 10) && (y <= 36)) {
        drawFrame(10, 10, 60, 36);
        gameNumber = 0; // Indicates we are at home screen
        myGLCD.clrScr();;
        drawHomeScreen(); // Draws the home screen
      }
    }
  }

  if (gameNumber == 1) {
    Serial.println("Game 1...");
    if (myTouch.dataAvailable()) {
      Serial.println("Touch Started...");
      myTouch.read();
      x = myTouch.getX(); // X coordinate where the screen has been pressed
      y = myTouch.getY(); // Y coordinates where the screen has been pressed
      while (myTouch.dataAvailable()) {
        Serial.println("Touch continue...");
        myTouch.read();
        x3 = myTouch.getX(); // X coordinate where the screen has been pressed
        y3 = myTouch.getY(); // Y coordinates where the screen has been pressed
        if (x3 > 0 && y3 > 0) {
          x2 = x3;
          y2 = y3;

        }
      }



      Serial.println(x);
      Serial.println(y);
      Serial.println(x2);
      Serial.println(y2);

      if (x > x2 || y > y2) {
        drawRectangle(x2, y2, x, y);
      }
      else {
        drawRectangle(x, y, x2, y2);
      }
      myGLCD.setColor(255, 255, 255);
      myGLCD.drawRoundRect (10, 10, 60, 36);
      myGLCD.setFont(BigFont);
      myGLCD.setBackColor(100, 155, 203);
      myGLCD.print("<-", 18, 15);
      if ((x >= 10) && (x <= 60) && (y >= 10) && (y <= 36)) {
        drawFrame(10, 10, 60, 36);
        gameNumber = 0; // Indicates we are at home screen
        myGLCD.clrScr();;
        drawHomeScreen();
        // Draws the home screen
      }


      x = 0;
      y = 0;
      x2 = 0;
      y2 = 0;
      delay(2000);
      myGLCD.clrScr(); // Clears the screen
    }

  }
  else if (gameNumber == 2) {
    xP = xP - movingRate; // xP - x coordinate of the pilars; range: 319 - (-51)
    drawPilars(xP, yP); // Draws the pillars

    // yB - y coordinate of the bird which depends on value of the fallingRate variable
    yB += fallRateInt;
    fallRate = fallRate + 0.4; // Each inetration the fall rate increase so that we can the effect of acceleration/ gravity
    fallRateInt = int(fallRate);

    // Checks for collision
    if (yB >= 180 || yB <= 0) { // top and bottom
      gameOver();

    }
    if ((xP <= 85) && (xP >= 5) && (yB <= yP - 2)) { // upper pillar
      gameOver();

    }
    if ((xP <= 85) && (xP >= 5) && (yB >= yP + 60)) { // lower pillar
      gameOver();

    }

    // Draws the bird
    drawBird(yB);

    // After the pillar has passed through the screen
    if (xP <= -51) {
      xP = 319; // Resets xP to 319
      yP = rand() % 100 + 20; // Random number for the pillars height
      score++; // Increase score by one
    }
    //==== Controlling the bird
    if (myTouch.dataAvailable() && !screenPressed ) {
      fallRate = -6; // Setting the fallRate negative will make the bird jump
      screenPressed = true;
    }
        buttonState = digitalRead(button);
    if (buttonState == HIGH) {
     
    }
    else 
    {
fallRate = -6;      
    }
    
    // Doesn't allow holding the screen / you must tap it
//    else if ( !myTouch.dataAvailable() && screenPressed) {
//      screenPressed = false;
//    }

    // After each 3 points, increases the moving rate of the pillars
    if ((score - lastSpeedUpScore) == 1) {
      lastSpeedUpScore = score;
      movingRate++;
    }


    myGLCD.setColor(255, 255, 255);
    myGLCD.drawRoundRect (10, 10, 60, 36);
    myGLCD.setFont(BigFont);
    myGLCD.setBackColor(100, 155, 203);
    myGLCD.print("<-", 18, 15);
    if ((x >= 10) && (x <= 60) && (y >= 10) && (y <= 36)) {

      drawFrame(10, 10, 60, 36);
      gameNumber = 0; // Indicates we are at home screen
      myGLCD.clrScr();;
      drawHomeScreen();
    }
  }
  else if (gameNumber == 3) {
    drawGame2();


  }

  else if (gameNumber == 4) {

    xP = xP - movingRate; // xP - x coordinate of the pilars; range: 319 - (-51)
    drawPilars1(xP, yP); // Draws the pillars

    // yB - y coordinate of the bird which depends on value of the fallingRate variable
    yB += fallRateInt;
    fallRate = fallRate + 0.4; // Each inetration the fall rate increase so that we can the effect of acceleration/ gravity
    fallRateInt = int(fallRate);

    // Checks for collision
    if (yB >= 180 || yB <= 0) { // top and bottom
      gameOver();

    }
    if ((xP <= 85) && (xP >= 5) && (yB <= yP - 2)) { // upper pillar
      score++;

    }
    if ((xP <= 85) && (xP >= 5) && (yB >= yP + 60)) { // lower pillar
      gameOver();

    }

    // Draws the bird
    drawBird1(yB);

    // After the pillar has passed through the screen
    if (xP <= -51) {
      xP = 319; // Resets xP to 319
      yP = rand() % 100 + 20; // Random number for the pillars height
      score++; // Increase score by one
    }
    //==== Controlling the bird
if (myTouch.dataAvailable() && !screenPressed ) {
      fallRate = -6; // Setting the fallRate negative will make the bird jump
      screenPressed = true;
    }
        buttonState = digitalRead(button);
    if (buttonState == HIGH) {
     
    }
    else 
    {
fallRate = -6;      
    }
    
    // Doesn't allow holding the screen / you must tap it
//    else if ( !myTouch.dataAvailable() && screenPressed) {
//      screenPressed = false;
//    }

    // After each 3 points, increases the moving rate of the pillars
    if ((score - lastSpeedUpScore) == 10) {
      lastSpeedUpScore = score;
      movingRate++;
    }
    myGLCD.setColor(255, 255, 255);
    myGLCD.drawRoundRect (10, 10, 60, 36);
    myGLCD.setFont(BigFont);
    myGLCD.setBackColor(100, 155, 203);
    myGLCD.print("<-", 18, 15);
    if ((x >= 10) && (x <= 60) && (y >= 10) && (y <= 36)) {

      drawFrame(10, 10, 60, 36);
      gameNumber = 0; // Indicates we are at home screen
      myGLCD.clrScr();;
      drawHomeScreen();
    }
  }
   else if (gameNumber == 5) {
    drawgame5();
   }
    else if (gameNumber == 6) {
    drawgame6();
   }
   else if (gameNumber == 7){
    game7();
   
   }
   else if (gameNumber == 8){
   game8();
   
   }


   
   
  else {
    Serial.println("Game 0...");
    // If Touched One then gameNumber = 1;
    avx = analogRead(X_pin);
  avy = analogRead(Y_pin);
myGLCD.setColor(0,0,0);
myGLCD.drawCircle(xx, yy, 10);
myGLCD.fillCircle(xx, yy, 10);

      int c = 1;

      if (avy > 550) {
    xx = xx - 10;
  }
  if (avy < 450) {
    xx = xx + 10;
  }
    if (avx > 550) {
    yy = yy + 10;
  }
  if (avx < 450) {
    yy = yy - 10;
  }
    if (myTouch.dataAvailable()) {
        
      myTouch.read();
      x = myTouch.getX(); // X coordinate where the screen has been pressed
      y = myTouch.getY(); // Y coordinates where the screen has been pressed
      // If we press the Distance Sensor Button
      
      if ((x >= 5) && (x <= 165) && (y >= 90) && (y <= 130)) {
        //drawFrame(35, 90, 285, 130); // Custom Function -Highlighs the buttons when it's pressed
        gameNumber = 1; // Indicates that we are the first example

        myGLCD.clrScr(); // Clears the screen

      }
      if ((x >= 5) && (x <= 165) && (y >= 140) && (y <= 180)) {
        gameNumber = 2;
        myGLCD.clrScr();
        initiateGame(); // Initiate the game
      }

      if ((x >= 5) && (x <= 165) && (y >= 190) && (y <= 230)) {
        gameNumber = 3;
        myGLCD.clrScr();
      }
      if ((x >= 5) && (x <= 165) && (y >= 40) && (y <= 80)) {
        gameNumber = 4;
        myGLCD.clrScr();
      }
//(165, 40, 315, 80);
           if ((x >= 165) && (x <= 315) && (y >= 40) && (y <= 80)) {
        gameNumber = 5;
        myGLCD.clrScr();
      }
      //(165, 90, 315, 130)
             if ((x >= 165) && (x <= 315) && (y >= 90) && (y <= 130)) {
        gameNumber = 6;
        myGLCD.clrScr();
      }
      //(165, 140, 315, 180)
       if ((x >= 165) && (x <= 315) && (y >= 140) && (y <= 180)) {
        gameNumber = 7;
        myGLCD.clrScr();
      }
// 165, 190, 315, 230
        if ((x >= 165) && (x <= 315) && (y >= 190) && (y <= 230)) {
        gameNumber = 8;
        myGLCD.clrScr();
      }

    }
    buttonState = digitalRead(button);
        if (buttonState == LOW) {
    
    
    
 if ((xx >= 5) && (xx <= 155) && (yy >= 90) && (yy <= 130)) {
        //drawFrame(35, 90, 285, 130); // Custom Function -Highlighs the buttons when it's pressed
        
        gameNumber = 1; // Indicates that we are the first example
        
        myGLCD.clrScr(); // Clears the screen
        

      }
      if ((xx >= 5) && (xx <= 155) && (yy >= 140) && (yy <= 180)) {
        
        gameNumber = 2;
        myGLCD.clrScr();
        
        initiateGame(); // Initiate the game
      }

      if ((xx >= 5) && (xx <= 155) && (yy >= 190) && (yy <= 230)) {
        
        gameNumber = 3;
        myGLCD.clrScr();
        
      }
      if ((xx >= 5) && (xx <= 155) && (yy >= 40) && (yy <= 80)) {
        
        gameNumber = 4;
        myGLCD.clrScr();
        
      }
//(165, 40, 315, 80);
           if ((xx >= 175) && (xx <= 315) && (yy >= 40) && (yy <= 80)) {
            
        gameNumber = 5;
        myGLCD.clrScr();
        
      }
      //(165, 90, 315, 130)
             if ((xx >= 175) && (xx <= 315) && (yy >= 90) && (yy <= 130)) {
              
        gameNumber = 6;
        myGLCD.clrScr();

        
      }
      //(165, 140, 315, 180)
       if ((xx >= 175) && (xx <= 315) && (yy >= 140) && (yy <= 180)) {
        
        gameNumber = 7;
        myGLCD.clrScr();
       
      }
// 165, 190, 315, 230
        if ((xx >= 175) && (xx<= 315) && (yy >= 190) && (yy<= 230)) {
          
        gameNumber = 8;
        myGLCD.clrScr();
        
    }
    

    }
     

    
      
  

}
}



void drawHomeScreen() {
  gameNumber = 0;
  // Title
  //Serial.begin(9600);

myGLCD.setColor(0,255,0);
myGLCD.drawCircle(xx, yy, 10);
myGLCD.fillCircle(xx, yy, 10);

  myGLCD.setBackColor(225, 225, 225); // Sets the background color of the area where the text will be printed to white
  myGLCD.setColor(0, 0, 0); // Sets color to black


  //Prints the string on the screen
  myGLCD.setColor(255, 0, 0); // Sets color to red
  myGLCD.drawLine(0, 32, 319, 32); // Draws the red line
  myGLCD.setColor(0, 0, 0); // Sets color to black
  myGLCD.setFont(BigFont); // Sets the font to small
  myGLCD.print("Arnav.purbiya.com", CENTER, 15); // Prints the string

  // Button - coin bird
  myGLCD.setColor(225, 0, 0); // Sets green color
  myGLCD.fillRoundRect (5, 40, 155, 80); // Draws filled rounded rectangle
  myGLCD.setColor(255, 255, 102); // Sets color to white
  myGLCD.drawRoundRect (5, 40, 155, 80); // Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
  myGLCD.setFont(SmallFont); // Sets the font to big
  myGLCD.setBackColor(225, 0, 0); // Sets the background color of the area where the text will be printed to green, same as the button
  myGLCD.print("  chase coin", LEFT, 52); // Prints the string

  // Button - rectangle game
  myGLCD.setColor(225, 0, 0); // Sets green color
  myGLCD.fillRoundRect (5, 90, 155, 130); // Draws filled rounded rectangle
  myGLCD.setColor(255, 255, 102); // Sets color to white
  myGLCD.drawRoundRect (5, 90, 155, 130); // Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
  myGLCD.setFont(SmallFont); // Sets the font to big
  myGLCD.setBackColor(225, 0, 0); // Sets the background color of the area where the text will be printed to green, same as the button
  myGLCD.print("  Rectangle Game", LEFT, 102); // Prints the string

  // Button - flappy bird
  myGLCD.setColor(225, 0, 0);
  myGLCD.fillRoundRect (5, 140, 155, 180);
  myGLCD.setColor(255, 255, 102);
  myGLCD.drawRoundRect (5, 140, 155, 180);
  myGLCD.setFont(SmallFont);
  myGLCD.setBackColor(225, 0, 0);
  myGLCD.print("  Flappy Bird", LEFT, 152);

  // Button - tic tac toe
  myGLCD.setColor(225, 0, 0);
  myGLCD.fillRoundRect (5, 190, 155, 230);
  myGLCD.setColor(255, 255, 102);
  myGLCD.drawRoundRect (5, 190, 155, 230);
  myGLCD.setFont(SmallFont);
  myGLCD.setBackColor(225, 0, 0);
  myGLCD.print("  Tic Tac Toe",   LEFT, 202);

  // Button -  game 5
  myGLCD.setColor(225, 0, 0); // Sets green color
  myGLCD.fillRoundRect (165, 40, 315, 80); // Draws filled rounded rectangle
  myGLCD.setColor(255, 255, 102); // Sets color to white
  myGLCD.drawRoundRect (165, 40, 315, 80); // Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
  myGLCD.setFont(SmallFont); // Sets the font to big
  myGLCD.setBackColor(225, 0, 0); // Sets the background color of the area where the text will be printed to green, same as the button
  myGLCD.print("paint ", RIGHT, 52); // Prints the string

  // Button - game 6
  myGLCD.setColor(225, 0, 0); // Sets green color
  myGLCD.fillRoundRect (165, 90, 315, 130); // Draws filled rounded rectangle
  myGLCD.setColor(255, 255, 102); // Sets color to white
  myGLCD.drawRoundRect (165, 90, 315, 130); // Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
  myGLCD.setFont(SmallFont); // Sets the font to big
  myGLCD.setBackColor(225, 0, 0); // Sets the background color of the area where the text will be printed to green, same as the button
  myGLCD.print("draw  ", RIGHT, 102); // Prints the string

  // Button - game 7
  myGLCD.setColor(225, 0, 0);
  myGLCD.fillRoundRect (165, 140, 315, 180);
  myGLCD.setColor(255, 255, 102);
  myGLCD.drawRoundRect (165, 140, 315, 180);
  myGLCD.setFont(SmallFont);
  myGLCD.setBackColor(225, 0, 0);
  myGLCD.print("  MATH NOTES", RIGHT, 152);

  // Button - game 8
  myGLCD.setColor(225, 0, 0);
  myGLCD.fillRoundRect (165, 190, 315, 230);
  myGLCD.setColor(255, 255, 102);
  myGLCD.drawRoundRect (165, 190, 315, 230);
  myGLCD.setFont(SmallFont);
  myGLCD.setBackColor(225, 0, 0);
  myGLCD.print("  game 8",   RIGHT, 202);

 

}
void drawButtons()
{
  // Draw the upper row of buttons
  for (x = 0; x < 5; x++)
  {
    myGLCD.setColor(0, 0, 255);
    myGLCD.fillRoundRect (10 + (x * 60), 10, 60 + (x * 60), 60);
    myGLCD.setColor(255, 255, 255);
    myGLCD.drawRoundRect (10 + (x * 60), 10, 60 + (x * 60), 60);
    myGLCD.printNumI(x + 1, 27 + (x * 60), 27);
  }
  // Draw the center row of buttons
  for (x = 0; x < 5; x++)
  {
    myGLCD.setColor(0, 0, 255);
    myGLCD.fillRoundRect (10 + (x * 60), 70, 60 + (x * 60), 120);
    myGLCD.setColor(255, 255, 255);
    myGLCD.drawRoundRect (10 + (x * 60), 70, 60 + (x * 60), 120);
    if (x < 4)
      myGLCD.printNumI(x + 6, 27 + (x * 60), 87);
  }
  myGLCD.print("0", 267, 87);
  // Draw the lower row of buttons
  myGLCD.setColor(0, 0, 255);
  myGLCD.fillRoundRect (10, 130, 150, 180);
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect (10, 130, 150, 180);
  myGLCD.print("Clear", 40, 147);
  myGLCD.setColor(0, 0, 255);
  myGLCD.fillRoundRect (160, 130, 300, 180);
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect (160, 130, 300, 180);
  myGLCD.print("Enter", 190, 147);
  myGLCD.setBackColor (0, 0, 0);
}

void updateStr(int val)
{
  if (stCurrentLen < 20)
  {
    stCurrent[stCurrentLen] = val;
    stCurrent[stCurrentLen + 1] = '\0';
    stCurrentLen++;
    myGLCD.setColor(0, 255, 0);
    myGLCD.print(stCurrent, LEFT, 224);
  }
  else
  {
    myGLCD.setColor(255, 0, 0);
    myGLCD.print("BUFFER FULL!", CENTER, 192);
    delay(500);
    myGLCD.print("            ", CENTER, 192);
    delay(500);
    myGLCD.print("BUFFER FULL!", CENTER, 192);
    delay(500);
    myGLCD.print("            ", CENTER, 192);
    myGLCD.setColor(0, 255, 0);
  }
}

// Draw a red frame while a button is touched
void waitForIt(int x1, int y1, int x2, int y2)
{
  myGLCD.setColor(255, 0, 0);
  myGLCD.drawRoundRect (x1, y1, x2, y2);
  while (myTouch.dataAvailable())
    myTouch.read();
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect (x1, y1, x2, y2);
}

void drawRectangle(int x1, int y1, int x2, int y2) {
  myGLCD.setColor(225, 128, 0);
  myGLCD.drawRoundRect (x1, y1, x2, y2);
  myGLCD.fillRect(x1, y1, x2, y2);
}
// ===== initiateGame - Custom Function
void initiateGame() {
  myGLCD.clrScr();
  // Blue background
  myGLCD.setColor(114, 198, 206);
  myGLCD.fillRect(0, 0, 319, 239);

 
  // Ground
  myGLCD.setColor(221, 216, 148);
  myGLCD.fillRect(0, 215, 319, 239);
  myGLCD.setColor(47, 175, 68);
  myGLCD.fillRect(0, 205, 319, 214);
  // Text
  myGLCD.setColor(0, 0, 0);
  myGLCD.setBackColor(221, 216, 148);
  myGLCD.setFont(BigFont);
  myGLCD.print("Score:", 5, 220);
  myGLCD.setFont(SmallFont);
  myGLCD.print("Arnav's Creations", 140, 220);
  myGLCD.setColor(0, 0, 0);
  myGLCD.setBackColor(114, 198, 206);
  myGLCD.print("Highest Score: ", 5, 5);
  myGLCD.printNumI(highestScore, 120, 6);
  myGLCD.print(">RESET<", 255, 5);
  myGLCD.drawLine(0, 23, 319, 23);
  myGLCD.print("TAP TO START", CENTER, 100);

  drawBird(yB); // Draws the bird

  // Wait until we tap the sreen
  while (!gameStarted) {
    if (myTouch.dataAvailable()) {
      myTouch.read();
      x = myTouch.getX();
      y = myTouch.getY();
      // Reset higest score
      if ((x >= 250) && (x <= 319) && (y >= 0) && (y <= 28)) {
        highestScore = 0;
        myGLCD.setColor(114, 198, 206);
        myGLCD.fillRect(120, 0, 150, 22);
        myGLCD.setColor(0, 0, 0);
        myGLCD.printNumI(highestScore, 120, 5);
      }
      if ((x >= 0) && (x <= 319) && (y >= 30) && (y <= 239)) {
        gameStarted = true;
        myGLCD.setColor(114, 198, 206);
        myGLCD.fillRect(0, 0, 319, 32);
      }
    }
  }
  // Clears the text "TAP TO START" before the game start
  myGLCD.setColor(114, 198, 206);
  myGLCD.fillRect(85, 100, 235, 116);

}
// ===== drawPlillars - Custom Function
void drawPilars(int x, int y) {

  if (x >= 270) {
    myGLCD.setColor(0, 200, 20);
    myGLCD.fillRect(318, 0, x, y - 1);
    myGLCD.setColor(0, 0, 0);
    myGLCD.drawRect(319, 0, x - 1, y);

    myGLCD.setColor(0, 200, 20);
    myGLCD.fillRect(318, y + 81, x, 203);
    myGLCD.setColor(0, 0, 0);
    myGLCD.drawRect(319, y + 80, x - 1, 204);
  }



  else if ( x <= 268) {
    // Draws blue rectangle right of the pillar
    myGLCD.setColor(114, 198, 206);
    myGLCD.fillRect(x + 51, 0, x + 60, y);
    // Draws the pillar
    myGLCD.setColor(225, 174, 0);
    myGLCD.fillRect(x + 49, 1, x + 1, y - 1);
    // Draws the black frame of the pillar
    myGLCD.setColor(0, 0, 0);
    myGLCD.drawRect(x + 50, 0, x, y);
    // Draws the blue rectangle left of the pillar
    myGLCD.setColor(114, 198, 206);
    myGLCD.fillRect(x - 1, 0, x - 3, y);

    // The bottom pillar
    myGLCD.setColor(114, 198, 206);
    myGLCD.fillRect(x + 51, y + 80, x + 60, 204);
    myGLCD.setColor(0, 200, 20);
    myGLCD.fillRect(x + 49, y + 81, x + 1, 203);
    myGLCD.setColor(0, 0, 0);
    myGLCD.drawRect(x + 50, y + 80, x, 204);
    myGLCD.setColor(114, 198, 206);
    myGLCD.fillRect(x - 1, y + 80, x - 3, 204);
  }
  // Draws the score
  myGLCD.setColor(0, 0, 0);
  myGLCD.setBackColor(221, 216, 148);
  myGLCD.setFont(BigFont);
  myGLCD.printNumI(score, 100, 220);
}

//====== drawBird() - Custom Function
void drawBird(int y) {
  // Draws the bird - bitmap
//  myGLCD.drawBitmap (50, y, 35, 30, bird01);
  myGLCD.drawBitmap (50, y, 35, 30, bird02);
  
  // Draws blue rectangles above and below the bird in order to clear its previus state
  myGLCD.setColor(114, 198, 206);
  myGLCD.fillRoundRect(50, y, 85, y - 6);
  myGLCD.fillRoundRect(50, y + 30, 85, y + 36);
}
void drawBird1(int y) {
  // Draws the bird - bitmap
  myGLCD.drawBitmap (50, y, 35, 30, bird01);
 
  
  // Draws blue rectangles above and below the bird in order to clear its previus state
  myGLCD.setColor(114, 198, 206);
  myGLCD.fillRoundRect(50, y, 85, y - 6);
  myGLCD.fillRoundRect(50, y + 30, 85, y + 36);
}
//======== gameOver() - Custom Function
void gameOver() {
  delay(3000); // 1 second
  // Clears the screen and prints the text
  myGLCD.clrScr();
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(0, 0, 0);
  myGLCD.setFont(BigFont);
  myGLCD.print("GAME OVER", CENTER, 40);
  myGLCD.print("Score:", 100, 80);
  myGLCD.printNumI(score, 200, 80);
  myGLCD.print("Restarting...", CENTER, 120);
  myGLCD.setFont(SevenSegNumFont);
  myGLCD.printNumI(3, CENTER, 150);
  delay(150);
  myGLCD.printNumI(2, CENTER, 150);
  delay(150);
  myGLCD.printNumI(1, CENTER, 150);
  delay(150);
  myGLCD.clrScr();
  myGLCD.setColor(0, 0, 0);
  myGLCD.setBackColor(234, 255, 241);
  myGLCD.setFont(BigFont);
  myGLCD.print("Thanks For Playing", CENTER, 40);

  // Writes the highest score in the EEPROM
  if (score > highestScore) {
    highestScore = score;
    EEPROM.write(0, highestScore);
  }
  // Resets the variables to start position values
  xP = 319;
  yB = 50;
  fallRate = 0;
  score = 0;
  lastSpeedUpScore = 0;
  movingRate = 3;
  gameStarted = false;
  // Restart game
  initiateGame();
}
void drawFrame(int x1, int y1, int x2, int y2) {
  myGLCD.setColor(255, 0, 0);
  myGLCD.drawRoundRect (x1, y1, x2, y2);
  while (myTouch.dataAvailable())
    myTouch.read();
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect (x1, y1, x2, y2);
}




// ===== drawPlillars - Custom Function
void drawPilars1(int x, int y) {
  int cx = x + 25;
  int cy = y - 15;

  myGLCD.setColor(114, 198, 206);
  myGLCD.drawCircle(cx + 3, cy, radius );
  myGLCD.fillCircle(cx + 3, cy, radius);

  if (x >= 270) {

    myGLCD.setColor(225, 174, 0);
    myGLCD.setColor(0, 0, 0);
    //      myGLCD.drawRect(x-30, y-30, x+30 ,y+30);
    myGLCD.setColor(255, 255, 0);
    myGLCD.fillCircle(cx, cy, radius);
    myGLCD.drawCircle(cx, cy, radius );
    myGLCD.fillCircle(cx, cy, radius);
    //      myGLCD.drawsquare
    myGLCD.setColor(225, 174, 0);
    myGLCD.fillRect(318, y + 81, x, 203);
    myGLCD.setColor(0, 0, 0);
    //        myGLCD.fillCircle(255, 255, 0);
    myGLCD.drawRect(319, y + 80, x - 1, 204);
  }
  else if ( x <= 268) {
    // Draws blue rectangle right of the pillar
    myGLCD.setColor(114, 198, 206);
    myGLCD.fillRect(x + 51, 0, x + 60, y);
    myGLCD.drawCircle(cx, cy, radius );
    myGLCD.setColor(255, 255, 0);
    myGLCD.fillCircle(cx, cy, radius );
    myGLCD.drawCircle(cx, cy, radius );

    // Draws the pillar
    //      myGLCD.setColor(225, 174, 0);
    //      myGLCD.fillRect(x+49, 1, x+1, y-1);
    myGLCD.setColor(255, 255, 0);
    myGLCD.drawCircle(cx, cy, radius );
    myGLCD.fillCircle(cx, cy, radius );

    //      // Draws the black frame of the pillar
    myGLCD.setColor(0, 0, 0);
    //      myGLCD.drawRect(x+50, 0, x, y);
    // Draws the blue rectangle left of the pillar
    //      myGLCD.setColor(114, 198, 206);
    //      myGLCD.fillRect(x-1, 0, x-3, y);
    //      myGLCD.fillCircle(x, y, 30);
    myGLCD.setColor(255, 255, 0);
    myGLCD.drawCircle(cx, cy, radius );


    // The bottom pillar
    myGLCD.setColor(225, 0, 225);
    myGLCD.fillRect(x + 51, y + 80, x + 60, 204);
    myGLCD.setColor(0, 200, 20);
    myGLCD.fillRect(x + 49, y + 81, x + 1, 203);
    myGLCD.setColor(0, 0, 0);
    myGLCD.drawRect(x + 50, y + 80, x, 204);
    myGLCD.setColor(114, 198, 206);
    myGLCD.fillRect(x - 1, y + 80, x - 3, 204);
  }
  // Draws the score
  myGLCD.setColor(0, 0, 0);
  myGLCD.setBackColor(221, 216, 148);
  myGLCD.setFont(BigFont);
  myGLCD.printNumI(score, 100, 220);
}

void drawGame2() {

  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect (10, 10, 60, 36);
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(100, 155, 203);
  myGLCD.print("<-", 18, 15);
  if ((x >= 10) && (x <= 60) && (y >= 10) && (y <= 36)) {

    drawFrame(10, 10, 60, 36);
    gameNumber = 0; // Indicates we are at home screen
    myGLCD.clrScr();;
    drawHomeScreen();

  }
  if ((numb_Turns == 0) || (numb_Turns == 2) || (numb_Turns == 4) || (numb_Turns == 6) || (numb_Turns == 8))
  {
    turn = 1; // player one turn
  }
  else if ((numb_Turns == 1) || (numb_Turns == 3) || (numb_Turns == 5) || (numb_Turns == 7))
  {
    turn = 2; //player two turn
  }

  myGLCD.setFont(SmallFont);// set font to small to print text

  // PRINT THE GAME BOARD – WHITE LINES
  myGLCD.setColor(255, 255, 255); // set the color to white
  myGLCD.fillRect(0, 60, 190, 65); // TOP HORIZONTAL LINE
  myGLCD.fillRect(0, 125, 190, 130); // BOTTOM HORIZONTAL LINE
  myGLCD.fillRect(60, 0, 65, 190); // LEFT VERTICAL LINE
  myGLCD.fillRect(125, 0, 130, 190); // RIGHT VERTICAL LINE

  // THIS WILL CHANGE ONCE GAME IS IMPLEMENTED
  myGLCD.setColor(255, 255, 0); // SET THE FONT COLOR TO YELLOW
  myGLCD.print(" Player, it’s your Turn!", LEFT, 210); //MESSAGE FOR PLAYER ONE TURN
  myGLCD.setColor(255, 255, 0); // SET THE FONT TO RED
  myGLCD.print(" SCORE ", RIGHT, 15);
  myGLCD.setColor(255, 0, 0);
  myGLCD.print(" Player 1: 0 ", RIGHT, 28);
  myGLCD.setColor(255, 0, 0);
  myGLCD.print(" Player 2: 0 ", RIGHT, 41);
  //

  // DRAW RESET AND INFO BUTTONS
  myGLCD.setFont(BigFont);
  myGLCD.setColor(255, 255, 255); //set color to white
  myGLCD.drawRoundRect(210, 70, 310, 115); // RESET BUTTON
  myGLCD.print("Reset", 220, 85);
  myGLCD.drawRoundRect(210, 135, 310, 180); // INFO BUTTON
  myGLCD.print("Info", 225, 150);

  // DECLARE ALL TOUCH AREAS

  if (myTouch.dataAvailable())
  {
    myTouch.read();
    x = myTouch.getX();
    y = myTouch.getY();

    // UL:
    if ((y >= 0) && (y <= 50)) // upper row of tic tac toe board
    {
      if ((x >= 0) && (x <= 50)) //Left Space
      {
        if (upper_Left == 0)// check to see if spot has been taken yet
        {
          delay(10);
          if (turn == 1)
          {
            myGLCD.print("X", 20, 20);
            numb_Turns++;
            upper_Left = 1;
          }
          if (turn == 2)
          {
            myGLCD.print("O", 20, 20);
            numb_Turns++;
            upper_Left = 2;
          }
        }
      }
    }

    //UC:

    if ((y >= 0) && (y <= 50)) // upper row of tic tac toe board
    {
      if ((x >= 80) && (x <= 110)) // Center Space
      {
        if (upper_Center == 0)// check to see if spot has been taken yet
        {
          delay(10);
          if (turn == 1)
          {
            myGLCD.print("X", 85, 20);
            numb_Turns++;
            upper_Center = 1;
          }
          if (turn == 2)
          {
            myGLCD.print("O", 85, 20);
            numb_Turns++;
            upper_Center = 2;
          }
        }
      }
    }

    //UR:

    if ((y >= 0) && (y <= 50)) // upper row of tic tac toe board
    {
      if ((x >= 130) && (x <= 180)) // Right Space
      {
        if (upper_Right == 0)// check to see if spot has been taken yet
        {
          delay(10);
          if (turn == 1)
          {
            myGLCD.print("X", 150, 20);
            numb_Turns++;
            upper_Right = 1;
          }
          if (turn == 2)
          {
            myGLCD.print("O", 150, 20);
            numb_Turns++;
            upper_Right = 2;
          }
        }
      }
    }

    //ML:

    if ((y >= 70) && (y <= 115)) // Middle row of tic tac toe board
    {
      if ((x >= 0) && (x <= 50)) //Left Space
      {
        if (middle_Left == 0)// check to see if spot has been taken yet
        {
          delay(10);
          if (turn == 1)
          {
            myGLCD.print("X", 20, 85);
            numb_Turns++;
            middle_Left = 1;
          }
          if (turn == 2)
          {
            myGLCD.print("O", 20, 85);
            numb_Turns++;
            middle_Left = 2;
          }
        }
      }
    }

    //MC:

    if ((y >= 70) && (y <= 115)) // Middle row of tic tac toe board
    {
      if ((x >= 80) && (x <= 110)) //Center Space
      {
        if (middle_Center == 0)// check to see if spot has been taken yet
        {
          delay(10);
          if (turn == 1)
          {
            myGLCD.print("X", 85, 85);
            numb_Turns++;
            middle_Center = 1;
          }
          if (turn == 2)
          {
            myGLCD.print("O", 85, 85);
            numb_Turns++;
            middle_Center = 2;
          }
        }
      }
    }

    //MR:

    if ((y >= 70) && (y <= 115)) // Middle row of tic tac toe board
    {
      if ((x >= 130) && (x <= 180)) //Center Space
      {
        if (middle_Right == 0)// check to see if spot has been taken yet
        {
          delay(10);
          if (turn == 1)
          {
            myGLCD.print("X", 150, 85);
            numb_Turns++;
            middle_Right = 1;
          }
          if (turn == 2)
          {
            myGLCD.print("O", 150, 85);
            numb_Turns++;
            middle_Right = 2;
          }
        }
      }
    }

    //LL:

    if ((y >= 130) && (y <= 180)) // Lower row of tic tac toe board
    {
      if ((x >= 0) && (x <= 50)) //Left Space
      {
        if (lower_Left == 0)// check to see if spot has been taken yet
        {
          delay(10);
          if (turn == 1)
          {
            myGLCD.print("X", 20, 150);
            numb_Turns++;
            lower_Left = 1;
          }
          if (turn == 2)
          {
            myGLCD.print("O", 20, 150);
            numb_Turns++;
            lower_Left = 2;
          }
        }
      }
    }

    //LC:

    if ((y >= 130) && (y <= 180)) // Lower row of tic tac toe board
    {
      if ((x >= 80) && (x <= 110)) //Center Space
      {
        if (lower_Center == 0)// check to see if spot has been taken yet
        {
          delay(10);
          if (turn == 1)
          {
            myGLCD.print("X", 85, 150);
            numb_Turns++;
            lower_Center = 1;
          }
          if (turn == 2)
          {
            myGLCD.print("O", 85, 150);
            numb_Turns++;
            lower_Center = 2;
          }
        }
      }
    }

    //LR:

    if ((y >= 130) && (y <= 180)) // Lower row of tic tac toe board
    {
      if ((x >= 130) && (x <= 180)) //Center Space
      {
        if (lower_Right == 0)// check to see if spot has been taken yet
        {
          delay(10);
          if (turn == 1)
          {
            myGLCD.print("X", 150, 150);
            numb_Turns++;
            lower_Right = 1;
          }
          if (turn == 2)
          {
            myGLCD.print("O", 150, 150);
            numb_Turns++;
            lower_Right = 2;
          }
        }
      }
    }

    //Game Info:
    // First, check to make sure that the game has not started so no x’s or o’s are lost when the screen rewrites itself.
    //If the game has started the info button will become inactive
    if ((upper_Left == 0) && (upper_Center == 0) && (upper_Right == 0) && (middle_Left == 0) && (middle_Center == 0) && (middle_Right == 0) && (lower_Left == 0) && (lower_Center == 0) && (lower_Right == 0))
    {

      if ((y >= 135) && (y <= 180))
      {
        if ((x >= 210) && (y <= 310))
        {
          delay(10);
          myGLCD.clrScr();
          delay(500);
          myGLCD.setColor(0, 255, 0);
          myGLCD.print("TIC TAC TOE", CENTER, 50);
          myGLCD.setFont(SmallFont);
          myGLCD.setColor(0, 0, 255);
          myGLCD.print("McVaney", CENTER, 80);
          myGLCD.print("(2014)", CENTER, 95);
          delay(5000);
          myGLCD.clrScr();
        }
      }
    }

    // CHECK WIN CASES
    //Player One Win
    if (((upper_Left == 1) && (upper_Center == 1) && (upper_Right == 1)) || ((middle_Left == 1) && (middle_Center == 1) && (middle_Right == 1)) || ((lower_Left == 1) && (lower_Center == 1) && (lower_Right == 1)) || ((upper_Left == 1) && (middle_Left == 1) && (lower_Left == 1)) || ((upper_Center == 1) && (middle_Center == 1) && (lower_Center == 1)) || ((upper_Right == 1) && (middle_Right == 1) && (lower_Right == 1)) || ((upper_Left == 1) && (middle_Center == 1) && ( lower_Right == 1)) || ((upper_Right == 1) && (middle_Center == 1) && (lower_Left == 1)))
    {
      myGLCD.setColor(0, 255, 0);
      myGLCD.print("Player One Wins!!", CENTER, 65);

      delay(1500);
      upper_Left = 0;
      upper_Center = 0;
      upper_Right = 0;
      middle_Left = 0;
      middle_Center = 0;
      middle_Right = 0;
      lower_Left = 0;
      lower_Center = 0;
      lower_Right = 0; // THE VARIABLES FOR THE GAME BOARD
      turn = 0;
      score_1++;
      numb_Turns = 0;
      myGLCD.clrScr();
    }
    //Player Two Win
    if (((upper_Left == 2) && (upper_Center == 2) && (upper_Right == 2)) || ((middle_Left == 2) && (middle_Center == 2) && (middle_Right == 2)) || ((lower_Left == 2) && (lower_Center == 2) && (lower_Right == 2)) || ((upper_Left == 2) && (middle_Left == 2) && (lower_Left == 2)) || ((upper_Center == 2) && (middle_Center == 2) && (lower_Center == 2)) || ((upper_Right == 2) && (middle_Right == 2) && (lower_Right == 2)) || ((upper_Left == 2) && (middle_Center == 2) && ( lower_Right == 2)) || ((upper_Right == 2) && (middle_Center == 2) && (lower_Left == 2)))
    {
      myGLCD.setColor(0, 255, 0);
      myGLCD.print("Player Two Wins!!", CENTER, 65);

      delay(1500);
      upper_Left = 0;
      upper_Center = 0;
      upper_Right = 0;
      middle_Left = 0;
      middle_Center = 0;
      middle_Right = 0;
      lower_Left = 0;
      lower_Center = 0;
      lower_Right = 0; // THE VARIABLES FOR THE GAME BOARD
      turn = 0;
      score_2++;
      numb_Turns = 0;
      myGLCD.clrScr();
    }

    // TIE GAME
    if (numb_Turns == 9)
    {

      myGLCD.setColor(0, 255, 0);
      myGLCD.print("TIE GAME!", CENTER, 65);

      delay(1500);
      upper_Left = 0;
      upper_Center = 0;
      upper_Right = 0;
      middle_Left = 0;
      middle_Center = 0;
      middle_Right = 0;
      lower_Left = 0;
      lower_Center = 0;
      lower_Right = 0; // THE VARIABLES FOR THE GAME BOARD
      turn = 0;
      win_Case = 0;
      numb_Turns = 0;
      myGLCD.clrScr();
    }
  }

  delay (100);

}
void drawColorMarkerAndBrushSize(int col)
{
  myGLCD.setColor(VGA_BLACK);
  myGLCD.fillRect(25, 0, 31, 239);
  myGLCD.fillRect(myGLCD.getDisplayXSize()-31, 161, myGLCD.getDisplayXSize()-1, 191);
  myGLCD.setColor(VGA_WHITE);
  myGLCD.drawPixel(25, (col*30)+15);
  for (int i=1; i<7; i++)
    myGLCD.drawLine(25+i, ((col*30)+15)-i, 25+i, ((col*30)+15)+i);
  
  if (color==1)
    myGLCD.setColor(VGA_WHITE);
  else
    myGLCD.setColor(colorlist[col]);
  if (bsize==1)
    myGLCD.drawPixel(myGLCD.getDisplayXSize()-15, 177);
  else
    myGLCD.fillCircle(myGLCD.getDisplayXSize()-15, 177, bsize);
    
  myGLCD.setColor(colorlist[col]);
}

void drawgame5(){

    myGLCD.clrScr();
  myGLCD.setFont(BigFont);

  myTouch.InitTouch();
  myTouch.setPrecision(PREC_HI);
  
  myGLCD.setColor(VGA_WHITE);
  myGLCD.drawLine(32,0,32,myGLCD.getDisplayYSize()-1);
  myGLCD.drawLine(myGLCD.getDisplayXSize()-32,0,myGLCD.getDisplayXSize()-32,myGLCD.getDisplayYSize()-1);
  myGLCD.print("C", myGLCD.getDisplayXSize()-24, 8);
  myGLCD.print("L", myGLCD.getDisplayXSize()-24, 24);
  myGLCD.print("E", myGLCD.getDisplayXSize()-24, 40);
  myGLCD.print("A", myGLCD.getDisplayXSize()-24, 56);
  myGLCD.print("R", myGLCD.getDisplayXSize()-24, 72);
  myGLCD.print("+", myGLCD.getDisplayXSize()-24, 136);
  myGLCD.print("-", myGLCD.getDisplayXSize()-24, 200);
  myGLCD.fillRect(myGLCD.getDisplayXSize()-32,96,myGLCD.getDisplayXSize()-1,128);
  myGLCD.drawLine(myGLCD.getDisplayXSize()-32,160,myGLCD.getDisplayXSize()-1,160);
  myGLCD.drawLine(myGLCD.getDisplayXSize()-32,192,myGLCD.getDisplayXSize()-1,192);
  myGLCD.drawLine(myGLCD.getDisplayXSize()-32,224,myGLCD.getDisplayXSize()-1,224);
  for (int i=0; i<8; i++)
  {
    myGLCD.setColor(colorlist[i]);
    myGLCD.fillRect(0, (i*30), 24, (((i+1)*30)-1));
  }
  drawColorMarkerAndBrushSize(color);

  long x, y;
  
  while (myTouch.dataAvailable() == true)
  {
    myTouch.read();
    x = myTouch.getX();
    y = myTouch.getY();
    if ((x!=-1) and (y!=-1))
    {
      if (x>(31+bsize) and (x<myGLCD.getDisplayXSize()-(31+bsize)))
      {
        if (bsize==1)
          myGLCD.drawPixel(x, y);
        else
          myGLCD.fillCircle(x, y, bsize);
      }
      else
      {
        if (x<(30+bsize))
        {
          if (y<240)
          {
            color = y / 30;
            drawColorMarkerAndBrushSize(color);
            while (myTouch.dataAvailable()) {};
            delay(50);
         }
        }
        else
        {
          if (y<96)
          {
            myGLCD.setColor(VGA_BLACK);
            myGLCD.fillRect(33, 0, myGLCD.getDisplayXSize()-33, myGLCD.getDisplayYSize()-1);
            myGLCD.setColor(colorlist[color]);
          }
          if ((y>128) and (y<160))
          {
            if (bsize<7)
            {
              bsize++;
              drawColorMarkerAndBrushSize(color);
              while (myTouch.dataAvailable()) {};
              delay(50);
            }
          }
          if ((y>160) and (y<192))
          {
              bsize=4;
              drawColorMarkerAndBrushSize(color);
              while (myTouch.dataAvailable()) {};
              delay(50);
          }
          if ((y>192) and (y<224))
          {
            if (bsize>1)
            {
              bsize--;
              drawColorMarkerAndBrushSize(color);
              while (myTouch.dataAvailable()) {};
              delay(50);
            }
          }
        }
      }
    }
  }
            // If we press the Back Button
      myGLCD.drawRoundRect (10, 10, 60, 36); // Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
      myGLCD.setFont(SmallFont); // Sets the font to big
      myGLCD.setBackColor(225, 0, 0); // Sets the background color of the area where the text will be printed to green, same as the button
      myGLCD.print("Back", CENTER, 10); // Prints the string
      if ((x >= 10) && (x <= 60) && (y >= 10) && (y <= 36)) {
        drawFrame(10, 10, 60, 36);
        gameNumber = 0; // Indicates we are at home screen
        myGLCD.clrScr();;
        drawHomeScreen(); // Draws the home screen
      } 
}
void drawgame6(){
    long x, y;
//  xx = 0;
//  yy = 0;
      
  while (true)
  {
    myGLCD.setColor(247, 255, 0);
    myTouch.read();
    x = myTouch.getX();
    y = myTouch.getY();
    if ((x!=-1) and (y!=-1))
    {
      myGLCD.drawPixel (x, y);
    }
  
      avx = analogRead(X_pin);
  avy = analogRead(Y_pin);


      int c = 1;

      if (avy > 550) {
    xx = xx - 1;
  }
  if (avy < 450) {
    xx = xx + 1;
  }
    if (avx > 550) {
    yy = yy + 1;
  }
  if (avx < 450) {
    yy = yy - 1;
  }
      myGLCD.drawPixel (xx, yy);
      
    
    
          // If we press the Back Button
      myGLCD.drawRoundRect (10, 10, 60, 36); // Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
      myGLCD.setFont(SmallFont); // Sets the font to big
      myGLCD.setBackColor(225, 0, 0); // Sets the background color of the area where the text will be printed to green, same as the button
      myGLCD.print("Back", CENTER, 10); // Prints the string
      if ((x >= 10) && (x <= 60) && (y >= 10) && (y <= 36)) {
        break;
        drawFrame(10, 10, 60, 36);
        gameNumber = 0; // Indicates we are at home screen
        myGLCD.clrScr();;
        xx = 0;
        yy = 0;
        drawHomeScreen(); // Draws the home screen
      }
}
  
}

void game7(){



  
   myFiles.load(0, 0, 320, 240, "img_2.RAW", 1, 0);



  
   myFiles.load(0, 0, 320, 240, "img_3.RAW", 1, 0);


   myFiles.load(0, 0, 320, 240, "img_4.RAW", 1, 0);



   myFiles.load(0, 0, 320, 240, "img_5.RAW", 1, 0);


   myFiles.load(0, 0, 320, 240, "img_6.RAW", 1, 0);



        gameNumber = 0;
        imagenumber == 0;// Indicates we are at home screen
        myGLCD.clrScr();;  
        drawHomeScreen();


  
}



void game8 (){
   myTouch.read();
    x = myTouch.getX();
    y = myTouch.getY();
  int xx = 120;
  int yy = 180;
  int avx;
  int avy;
  int c = 1;
 
      myGLCD.drawRoundRect (10, 10, 60, 36); // Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
      myGLCD.setFont(SmallFont); // Sets the font to big
      myGLCD.setBackColor(225, 0, 0); // Sets the background color of the area where the text will be printed to green, same as the button
      myGLCD.print("Back", CENTER, 10); // Prints the string 
      drawFrame(10, 10, 60, 36);
myGLCD.drawCircle(120, 180, 10);
myGLCD.setColor(255, 255, 225);
myGLCD.fillRect(0, 200, 319, 239);
myGLCD.setBackColor(225, 255, 255);
myGLCD.setColor(0, 0, 0);

myGLCD.setColor(0, 0, 255);
myGLCD.drawCircle(250, 150, 5);

while(c<10){
  avx = analogRead(X_pin);
  avy = analogRead(Y_pin);

myGLCD.setColor(0,0,0);
myGLCD.drawCircle(xx, yy, 10);
myGLCD.fillCircle(xx, yy, 10);

myGLCD.print("Score", LEFT, 210);
  if (avy > 550) {
    xx--;
  }
  if (avy < 450) {
    xx++;
  }
    if (avx > 550) {
    yy ++;
  }
  if (avx < 450) {
    yy--;
  }


myGLCD.setColor(0,255,0);
myGLCD.drawCircle(xx, yy, 10);
myGLCD.fillCircle(xx, yy, 10);

buttonState = digitalRead(button);
    if (buttonState == HIGH) {
     
    }
    else 
    {
        gameNumber = 0; // Indicates we are at home screen
        myGLCD.clrScr();;
        drawHomeScreen(); // Draws the home screen  
        break;   
    }

      


}     
       
  

    
}
