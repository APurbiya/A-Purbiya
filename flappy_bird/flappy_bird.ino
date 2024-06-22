 #include <UTFT.h> 
#include <URTouch.h>
#include <EEPROM.h>

//==== Creating Objects
UTFT    myGLCD(CTE32_R2, 38, 39, 40, 41); 
URTouch  myTouch( 6, 5, 4, 3, 2);

//==== Defining Fonts
extern uint8_t SmallFont[];
extern uint8_t BigFont[];
extern uint8_t SevenSegNumFont[];

extern unsigned int bird01[0x41A]; // Bird Bitmap

int x, y; // Variables for the coordinates where the display has been pressed

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

void setup() {
  // Initiate display
  myGLCD.InitLCD();
  myGLCD.clrScr();
  myTouch.InitTouch();
  myTouch.setPrecision(PREC_MEDIUM);
  
  highestScore = EEPROM.read(0); // Read the highest score from the EEPROM
  
  initiateGame(); // Initiate the game
}

void loop() {
    xP=xP-movingRate; // xP - x coordinate of the pilars; range: 319 - (-51)   
    //drawPilars(xP, yP); // Draws the pillars 
    
    // yB - y coordinate of the bird which depends on value of the fallingRate variable
    yB+=fallRateInt; 
    fallRate=fallRate+0.4; // Each inetration the fall rate increase so that we can the effect of acceleration/ gravity
    fallRateInt= int(fallRate);
    
    // Checks for collision
    if(yB>=180 || yB<=0){ // top and bottom
      //gameOver();
    }
    if((xP<=85) && (xP>=5) && (yB<=yP-2)){ // upper pillar
      //gameOver();
    }
    if((xP<=85) && (xP>=5) && (yB>=yP+60)){ // lower pillar
      //gameOver();
    }
    
    // Draws the bird
    drawBird(yB);

    // After the pillar has passed through the screen
    if (xP<=-51){
      xP=319; // Resets xP to 319
      yP = rand() % 100+20; // Random number for the pillars height
      score++; // Increase score by one
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

// ===== initiateGame - Custom Function
void initiateGame() {
  myGLCD.clrScr();
  // Blue background
  myGLCD.setColor(114, 198, 206);
  myGLCD.fillRect(0,0,319,239);
  // Ground
  myGLCD.setColor(221,216,148);
  myGLCD.fillRect(0, 215, 319, 239);
  myGLCD.setColor(47,175,68);
  myGLCD.fillRect(0, 205, 319, 214);
  // Text
  myGLCD.setColor(0, 0, 0);
  myGLCD.setBackColor(221, 216, 148);
  myGLCD.setFont(BigFont);
  myGLCD.print("Score:",5,220);
  myGLCD.setFont(SmallFont);
  myGLCD.print("Arnav Purbiya", 140, 220); 
  myGLCD.setColor(0, 0, 0);
  myGLCD.setBackColor(114, 198, 206);
  myGLCD.print("Highest Score: ",5,5);
  myGLCD.printNumI(highestScore, 120, 6);
  myGLCD.print(">RESET<",255,5);
  myGLCD.drawLine(0,23,319,23);
  myGLCD.print("TAP TO START",CENTER,100);
  
  drawBird(yB); // Draws the bird
  
  // Wait until we tap the sreen
  while (!gameStarted) {
    if (myTouch.dataAvailable()) {
    myTouch.read();
    x=myTouch.getX();
    y=myTouch.getY();        
    // Reset higest score
    if ((x>=250) && (x<=319) &&(y>=0) && (y<=28)) {
    highestScore = 0;
    myGLCD.setColor(114, 198, 206);
    myGLCD.fillRect(120, 0, 150, 22);
    myGLCD.setColor(0, 0, 0);
    myGLCD.printNumI(highestScore, 120, 5);
    } 
    if ((x>=0) && (x<=319) &&(y>=30) && (y<=239)) {
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
void drawBird(int y) {
  // Draws the bird - bitmap
  myGLCD.drawBitmap (50, y, 35, 30, bird01);
  // Draws blue rectangles above and below the bird in order to clear its previus state
  myGLCD.setColor(114, 198, 206);
  myGLCD.fillRoundRect(50,y,85,y-6);
  myGLCD.fillRoundRect(50,y+30,85,y+36);
}
