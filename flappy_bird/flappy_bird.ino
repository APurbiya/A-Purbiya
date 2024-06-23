#include <UTFT.h>
#include <URTouch.h>
#include <EEPROM.h>

//==== Creating Objects
UTFT    myGLCD(CTE32_R2, 38, 39, 40, 41); //Parameters should be adjusted to your Display/Schield model
URTouch  myTouch( 6, 5, 4, 3, 2);

//==== Defining Fonts
extern uint8_t SmallFont[];
extern uint8_t BigFont[];
extern uint8_t SevenSegNumFont[];

extern unsigned int bird01[0x41A]; // Bird Bitmap

int x, y; // Variables for the coordinates where the display has been pressed
int x2, y2, x3, y3;
char currentPage, selectedUnit;
char stCurrent[20] = "";
int stCurrentLen = 0;
char stLast[20] = "";
// Floppy Bird
int xP = 319;
int yP = 100;
int yB = 50;
int movingRate = 3;
int fallRateInt = 0;
float fallRate = 0;
int score = 0;
int lastSpeedUpScore = 0;
int highestScore;
boolean screenPressed = false;
boolean gameStarted = false;
int gameNumber = 0;
int radius = 10;
boolean newPillar = true;

void setup() {
  // Initiate display
  myGLCD.InitLCD();
  myGLCD.clrScr();
  myTouch.InitTouch();
  myTouch.setPrecision(PREC_MEDIUM);

  highestScore = EEPROM.read(0); // Read the highest score from the EEPROM
  Serial.begin(19200);

 myGLCD.setBackColor(0, 0, 0); // Sets the background color of the area where the text will be printed to black
  myGLCD.setColor(255, 255, 255); // Sets color to white
  myGLCD.setFont(BigFont); // Sets font to big
  myGLCD.print("Arnav's Masterpiece", CENTER, 10); // Prints the string on the screen
  myGLCD.setColor(255, 0, 0); // Sets color to red
  myGLCD.drawLine(0, 32, 319, 32); // Draws the red line
  myGLCD.setColor(255, 255, 255); // Sets color to white
  myGLCD.setFont(SmallFont); // Sets the font to small
  myGLCD.print("You are about to enter the world of Arnav", CENTER, 41); // Prints the string
  myGLCD.setFont(BigFont);
  myGLCD.print("Loading. ", CENTER, 64); 
  delay(2000);
  myGLCD.clrScr();; 
  
 myGLCD.setBackColor(0, 0, 0); // Sets the background color of the area where the text will be printed to black
  myGLCD.setColor(255, 255, 255); // Sets color to white
  myGLCD.setFont(BigFont); // Sets font to big
  myGLCD.print("Arnav's Masterpiece", CENTER, 10); // Prints the string on the screen
  myGLCD.setColor(255, 0, 0); // Sets color to red
  myGLCD.drawLine(0, 32, 319, 32); // Draws the red line
  myGLCD.setColor(255, 255, 255); // Sets color to white
  myGLCD.setFont(SmallFont); // Sets the font to small
  myGLCD.print("You are about to enter the world of Arnav", CENTER, 41); // Prints the string
  myGLCD.setFont(BigFont);
  myGLCD.print("Loading.. ", CENTER, 64); 
  delay(2000);
  myGLCD.clrScr();;
  
 myGLCD.setBackColor(0, 0, 0); // Sets the background color of the area where the text will be printed to black
  myGLCD.setColor(255, 255, 255); // Sets color to white
  myGLCD.setFont(BigFont); // Sets font to big
  myGLCD.print("Arnav's Masterpiece", CENTER, 10); // Prints the string on the screen
  myGLCD.setColor(255, 0, 0); // Sets color to red
  myGLCD.drawLine(0, 32, 319, 32); // Draws the red line
  myGLCD.setColor(255, 255, 255); // Sets color to white
  myGLCD.setFont(SmallFont); // Sets the font to small
  myGLCD.print("You are about to enter the world of Arnav", CENTER, 41); // Prints the string
  myGLCD.setFont(BigFont);
  myGLCD.print("Loading.... ", CENTER, 64); 
  delay(2000);
  myGLCD.clrScr();;
  delay(2000);
  drawHomeScreen();
}

void loop() {
  if (gameNumber != 0) {
    if (myTouch.dataAvailable()) {
      myTouch.read();
      x = myTouch.getX(); // X coordinate where the screen has been pressed
      y = myTouch.getY(); // Y coordinates where the screen has been pressed

      // If we press the Back Button
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
        //        Serial.println(x2);
        //        Serial.println(y2);
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
      if ((x >= 10) && (x <= 60) && (y >= 10) && (y <= 36)) {
        drawFrame(10, 10, 60, 36);
        gameNumber = 0; // Indicates we are at home screen
        myGLCD.clrScr();;
        drawHomeScreen(); // Draws the home screen
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
    if (myTouch.dataAvailable() && !screenPressed) {
      fallRate = -6; // Setting the fallRate negative will make the bird jump
      screenPressed = true;
    }
    // Doesn't allow holding the screen / you must tap it
    else if ( !myTouch.dataAvailable() && screenPressed) {
      screenPressed = false;
    }

    // After each one points, increases the moving rate of the pillars
    if ((score - lastSpeedUpScore) == 1) {
      lastSpeedUpScore = score;
      movingRate++;
    }
  }
  else if (gameNumber == 3) {
       xP=xP-movingRate; // xP - x coordinate of the pilars; range: 319 - (-51)   
    drawPilars1(xP, yP); // Draws the pillars 

    
    // yB - y coordinate of the bird which depends on value of the fallingRate variable
    yB+=fallRateInt; 
    fallRate=fallRate+0.4; // Each inetration the fall rate increase so that we can the effect of acceleration/ gravity
    fallRateInt= int(fallRate);
    
    // Checks for collision
    if(yB>=180 || yB<=0){ // top and bottom
      gameOver();
    }
//    if((xP<=85) && (xP>=5) && (yB<=yP-2)){ // upper pillar
//      gameOver();
//    }
    
    if ( newPillar && (xP > 80) && (xP < 115) && (yB > yP-15-10) && (yB < yP-15+10) ) {
      score+=10;
      newPillar = false;
        Serial.println("coordinate xP, yP, yB = ");
        Serial.println(xP);
        Serial.println(yP);
        Serial.println(yB);
    }
    if((xP<=85) && (xP>=5) && (yB>=yP+60)){ // lower pillar
      gameOver();
    }
    
    // Draws the bird
    drawBird(yB);

    // After the pillar has passed through the screen
    if (xP<=-51){
      xP=319; // Resets xP to 319
      yP = rand() % 100+20; // Random number for the pillars height
      score++; // Increase score by one
      newPillar = true;
    }
    //==== Controlling the bird
    if (myTouch.dataAvailable()&& !screenPressed) {
       fallRate=-6; // Setting the fallRate negative will make the bird jump
       screenPressed = true;
    }
    // Doesn't allow holding the screen / you must tap it
    else if ( !myTouch.dataAvailable() && screenPressed){
      screenPressed = false;
    }
    
    // After each five points, increases the moving rate of the pillars
    if ((score - lastSpeedUpScore) == 5) {
      lastSpeedUpScore = score;
      movingRate++;
    }
  }
  else {
    Serial.println("Game 0...");
    // If Touched One then gameNumber = 1;
    if (myTouch.dataAvailable()) {
      myTouch.read();
      x = myTouch.getX(); // X coordinate where the screen has been pressed
      y = myTouch.getY(); // Y coordinates where the screen has been pressed
      // If we press the Distance Sensor Button
      if ((x >= 35) && (x <= 285) && (y >= 90) && (y <= 130)) {
        //drawFrame(35, 90, 285, 130); // Custom Function -Highlighs the buttons when it's pressed
        gameNumber = 1; // Indicates that we are the first example

        myGLCD.clrScr(); // Clears the screen
        //drawDistanceSensor(); // It is called only once, because in the next iteration of the loop, this above if statement will be false so this funtion won't be called. This function will draw the graphics of the first example.

      }
      if ((x >= 35) && (x <= 285) && (y >= 140) && (y <= 180)) {
        gameNumber = 2;
        myGLCD.clrScr();
        initiateGame(); // Initiate the game
      }
    }
  }

}







void drawHomeScreen() {
  // Title
  myGLCD.setBackColor(0, 0, 0); // Sets the background color of the area where the text will be printed to black
  myGLCD.setColor(255, 255, 255); // Sets color to white
  myGLCD.setFont(BigFont); // Sets font to big
  myGLCD.print("Arnav's Masterpiece", CENTER, 10); // Prints the string on the screen
  myGLCD.setColor(255, 0, 0); // Sets color to red
  myGLCD.drawLine(0, 32, 319, 32); // Draws the red line
  myGLCD.setColor(255, 255, 255); // Sets color to white
  myGLCD.setFont(SmallFont); // Sets the font to small
  myGLCD.print("Arnav.purbiya.com", CENTER, 41); // Prints the string
  myGLCD.setFont(BigFont);
  myGLCD.print("Home Screen ", CENTER, 64);

  // Button - Distance Sensor
  myGLCD.setColor(16, 167, 103); // Sets green color
  myGLCD.fillRoundRect (35, 90, 285, 130); // Draws filled rounded rectangle
  myGLCD.setColor(255, 255, 255); // Sets color to white
  myGLCD.drawRoundRect (35, 90, 285, 130); // Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
  myGLCD.setFont(BigFont); // Sets the font to big
  myGLCD.setBackColor(16, 167, 103); // Sets the background color of the area where the text will be printed to green, same as the button
  myGLCD.print("Rectangle Game", CENTER, 102); // Prints the string

  // Button - RGB LED Control
  myGLCD.setColor(16, 167, 103);
  myGLCD.fillRoundRect (35, 140, 285, 180);
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect (35, 140, 285, 180);
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(16, 167, 103);
  myGLCD.print("Flappy Bird", CENTER, 152);

  // Button - Birduino
  myGLCD.setColor(16, 167, 103);
  myGLCD.fillRoundRect (35, 190, 285, 230);
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect (35, 190, 285, 230);
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(16, 167, 103);
  myGLCD.print("Thankyou",   CENTER, 202);
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
  myGLCD.setColor(0, 0, 255);
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

void drawGame2() {
}
void drawGame3() {
}
