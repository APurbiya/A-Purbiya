#include <UTFT.h>
#include <URTouch.h>

// Declare which fonts we will be using from the utft library
extern uint8_t SmallFont[];
extern uint8_t BigFont[];

// Initialize the display for the MEGA
UTFT myGLCD(CTE32_R2, 38, 39, 40, 41); // Remember to change the model parameter to suit your display module!
// Initialize touchscreen
URTouch myTouch(6, 5, 4, 3, 2);

// DECLARE ALL GLOBAL VARIABLES
int x, y; // storage for x and y coordinate touch readings
int upper_Left, upper_Center, upper_Right, middle_Left, middle_Center, middle_Right, lower_Left, lower_Center, lower_Right; // THE VARIABLES FOR THE GAME BOARD
int turn, win_Case, score_1, score_2, numb_Turns;

// BEGIN SETUP
void setup() {
    Serial.begin(9600); // The serial monitor is only used for debugging, this is not necessary for the operation of the game
    // Setup the LCD
    myGLCD.InitLCD();
    myTouch.InitTouch();
    myTouch.setPrecision(PREC_MEDIUM);

    // Clear the screen on start up to avoid errors
    myGLCD.clrScr();
}

// BEGIN MAIN LOOP
void loop() {
    // The following if statement will check to see whose turn it is.
    // if number of turns is = 0,2,4,6,8, then it is player one’s turn.
    // if number of turns is = 1,3,5,7, then it is player two’s turn.
    if ((numb_Turns == 0) || (numb_Turns == 2) || (numb_Turns == 4) || (numb_Turns == 6) || (numb_Turns == 8)) {
        turn = 1; // player one turn
    } else if ((numb_Turns == 1) || (numb_Turns == 3) || (numb_Turns == 5) || (numb_Turns == 7)) {
        turn = 2; // player two turn
    }

    myGLCD.setFont(SmallFont); // set font to small to print text

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

    // DRAW RESET AND INFO BUTTONS
    myGLCD.setFont(BigFont);
    myGLCD.setColor(255, 255, 255); //set color to white
    myGLCD.drawRoundRect(210, 70, 310, 115); // RESET BUTTON
    myGLCD.print("Reset", 220, 85);
    myGLCD.drawRoundRect(210, 135, 310, 180); // INFO BUTTON
    myGLCD.print("Info", 225, 150);

    // DECLARE ALL TOUCH AREAS
    if (myTouch.dataAvailable()) {
        myTouch.read();
        x = myTouch.getX();
        y = myTouch.getY();

        // UL:
        if ((y >= 0) && (y <= 50)) { // upper row of tic tac toe board
            if ((x >= 0) && (x <= 50)) { // Left Space
                if (upper_Left == 0) { // check to see if spot has been taken yet
                    delay(10);
                    if (turn == 1) {
                        myGLCD.print("X", 20, 20);
                        numb_Turns++;
                        upper_Left = 1;
                    }
                    if (turn == 2) {
                        myGLCD.print("O", 20, 20);
                        numb_Turns++;
                        upper_Left = 2;
                    }
                }
            }
        }

        // UC:
        if ((y >= 0) && (y <= 50)) { // upper row of tic tac toe board
            if ((x >= 80) && (x <= 110)) { // Center Space
                if (upper_Center == 0) { // check to see if spot has been taken yet
                    delay(10);
                    if (turn == 1) {
                        myGLCD.print("X", 85, 20);
                        numb_Turns++;
                        upper_Center = 1;
                    }
                    if (turn == 2) {
                        myGLCD.print("O", 85, 20);
                        numb_Turns++;
                        upper_Center = 2;
                    }
                }
            }
        }

        // UR:
        if ((y >= 0) && (y <= 50)) { // upper row of tic tac toe board
            if ((x >= 130) && (x <= 180)) { // Right Space
                if (upper_Right == 0) { // check to see if spot has been taken yet
                    delay(10);
                    if (turn == 1) {
                        myGLCD.print("X", 150, 20);
                        numb_Turns++;
                        upper_Right = 1;
                    }
                    if (turn == 2) {
                        myGLCD.print("O", 150, 20);
                        numb_Turns++;
                        upper_Right = 2;
                    }
                }
            }
        }

        // ML:
        if ((y >= 70) && (y <= 115)) { // Middle row of tic tac toe board
            if ((x >= 0) && (x <= 50)) { // Left Space
                if (middle_Left == 0) { // check to see if spot has been taken yet
                    delay(10);
                    if (turn == 1) {
                        myGLCD.print("X", 20, 85);
                        numb_Turns++;
                        middle_Left = 1;
                    }
                    if (turn == 2) {
                        myGLCD.print("O", 20, 85);
                        numb_Turns++;
                        middle_Left = 2;
                    }
                }
            }
        }

        // MC:
        if ((y >= 70) && (y <= 115)) { // Middle row of tic tac toe board
            if ((x >= 80) && (x <= 110)) { // Center Space
                if (middle_Center == 0) { // check to see if spot has been taken yet
                    delay(10);
                    if (turn == 1) {
                        myGLCD.print("X", 85, 85);
                        numb_Turns++;
                        middle_Center = 1;
                    }
                    if (turn == 2) {
                        myGLCD.print("O", 85, 85);
                        numb_Turns++;
                        middle_Center = 2;
                    }
                }
            }
        }

        // MR:
        if ((y >= 70) && (y <= 115)) { // Middle row of tic tac toe board
            if ((x >= 130) && (x <= 180)) { // Center Space
                if (middle_Right == 0) { // check to see if spot has been taken yet
                    delay(10);
                    if (turn == 1) {
                        myGLCD.print("X", 150, 85);
                        numb_Turns++;
                        middle_Right = 1;
                    }
                    if (turn == 2) {
                        myGLCD.print("O", 150, 85);
                        numb_Turns++;
                        middle_Right = 2;
                    }
                }
            }
        }

        // LL:
        if ((y >= 130) && (y <= 180)) { // Lower row of tic tac toe board
            if ((x >= 0) && (x <= 50)) { // Left Space
                if (lower_Left == 0) { // check to see if spot has been taken yet
                    delay(10);
                    if (turn == 1) {
                        myGLCD.print("X", 20, 150);
                        numb_Turns++;
                        lower_Left = 1;
                    }
                    if (turn == 2) {
                        myGLCD.print("O", 20, 150);
                        numb_Turns++;
                        lower_Left = 2;
                    }
                }
            }
        }

        // LC:
        if ((y >= 130) && (y <= 180)) { // Lower row of tic tac toe board
            if ((x >= 80) && (x <= 110)) { // Center Space
                if (lower_Center == 0) { // check to see if spot has been taken yet
                    delay(10);
                    if (turn == 1) {
                        myGLCD.print("X", 85, 150);
                        numb_Turns++;
                        lower_Center = 1;
                    }
                    if (turn == 2) {
                        myGLCD.print("O", 85, 150);
                        numb_Turns++;
                        lower_Center = 2;
                    }
                }
            }
        }

        // LR:
        if ((y >= 130) && (y <= 180)) { // Lower row of tic tac toe board
            if ((x >= 130) && (x <= 180)) { // Center Space
                if (lower_Right == 0) { // check to see if spot has been taken yet
                    delay(10);
                    if (turn == 1) {
                        myGLCD.print("X", 150, 150);
                        numb_Turns++;
                        lower_Right = 1;
                    }
                    if (turn == 2) {
                        myGLCD.print("O", 150, 150);
                        numb_Turns++;
                        lower_Right = 2;
                    }
                }
            }
        }

        // Reset: resets all game variables, including score, and clears the LCD to start a new game session.
        // if ((y >= 70) && (y <= 115)) {
        //     if ((x >= 280) && (x <= 300)) {
        //         delay(10);
        //         upper_Left = 0;
        //         upper_Center = 0;
        //         upper_Right = 0;
        //         middle_Left = 0;
        //         middle_Center = 0;
        //         middle_Right = 0;
        //         lower_Left = 0;
        //         lower_Center = 0;
        //         lower_Right = 0; // THE VARIABLES FOR THE GAME BOARD
        //         turn = 0;
        //         win_Case = 0;
        //         score_1 = 0;
        //         score_2 = 0;
        //         numb_Turns = 0;
        //         myGLCD.clrScr();
        //     }
        // }

        // Game Info:
        // First, check to make sure that the game has not started so no x’s or o’s are lost when the screen rewrites itself.
        // If the game has started the info button will become inactive
        if ((upper_Left == 0) && (upper_Center == 0) && (upper_Right == 0) && (middle_Left == 0) && (middle_Center == 0) && (middle_Right == 0) && (lower_Left == 0) && (lower_Center == 0) && (lower_Right == 0)) {
            if ((y >= 135) && (y <= 180)) {
                if ((x >= 210) && (y <= 310)) {
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
        // Player One Win
        if (((upper_Left == 1) && (upper_Center == 1) && (upper_Right == 1)) ||
            ((middle_Left == 1) && (middle_Center == 1) && (middle_Right == 1)) ||
            ((lower_Left == 1) && (lower_Center == 1) && (lower_Right == 1)) ||
            ((upper_Left == 1) && (middle_Left == 1) && (lower_Left == 1)) ||
            ((upper_Center == 1) && (middle_Center == 1) && (lower_Center == 1)) ||
            ((upper_Right == 1) && (middle_Right == 1) && (lower_Right == 1)) ||
            ((upper_Left == 1) && (middle_Center == 1) && (lower_Right == 1)) ||
            ((upper_Right == 1) && (middle_Center == 1) && (lower_Left == 1))) {
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
        // Player Two Win
        if (((upper_Left == 2) && (upper_Center == 2) && (upper_Right == 2)) ||
            ((middle_Left == 2) && (middle_Center == 2) && (middle_Right == 2)) ||
            ((lower_Left == 2) && (lower_Center == 2) && (lower_Right == 2)) ||
            ((upper_Left == 2) && (middle_Left == 2) && (lower_Left == 2)) ||
            ((upper_Center == 2) && (middle_Center == 2) && (lower_Center == 2)) ||
            ((upper_Right == 2) && (middle_Right == 2) && (lower_Right == 2)) ||
            ((upper_Left == 2) && (middle_Center == 2) && (lower_Right == 2)) ||
            ((upper_Right == 2) && (middle_Center == 2) && (lower_Left == 2))) {
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
        if (numb_Turns == 9) {
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

    delay(100);
}
