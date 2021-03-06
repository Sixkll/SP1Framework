// This is the main file for the game logic and function
//
//
#include "game.h"
#include "Framework\console.h"
#include <iostream>
#include <iomanip>
#include <sstream>

double  g_dElapsedTime;
double  g_dDeltaTime;
double  enemyBounceTime;
double  PowerTime;
int     score;
int     playerLives;
int     ghostEaten;
bool    PwrdUp;


COORD previousPos; //previous position of player
COORD enemyPrevPos[4]; //previous position for enemy
WORD charColor = 0x2B;
WORD berryColor = 0xA1;

SKeyEvent g_skKeyEvent[K_COUNT];
SMouseEvent g_mouseEvent;

// Game specific variables here
SGameChar   g_sChar; //character variables
SGameChar   g_sBerry[30]; // Berry
SGameChar   g_sWall[2000]; // Wall
SGameChar   g_sPortal[2]; // portal/ teleporter
SGameChar   g_sEnemy[4];  //enmey
SGameChar   g_sPowerUp[4]; // pwrUp
EGAMESTATES g_eGameState = S_SPLASHSCREEN; // initial state

int map[25][80] = { 
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,1,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,1,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,1,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,1,0,0,1,
1,0,0,1,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,1,1,1,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,1,1,1,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,1,0,0,1,
1,0,0,1,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,1,0,0,1,
1,0,0,1,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,1,0,0,1,
1,0,0,1,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,
1,0,0,1,1,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,1,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,1,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1,
0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,
1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,1,1,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,1,1,0,0,1,
1,0,0,1,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,1,0,0,1,
1,0,0,1,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,1,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,1,0,0,1,
1,0,0,1,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,1,0,0,1,
1,0,0,1,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,1,0,0,1,
1,0,0,1,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,1,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,                    
};


// Console object
Console g_Console(80, 25, "SP1 Framework");

//--------------------------------------------------------------
// Purpose  : Initialisation function
//            Initialize variables, allocate memory, load data from file, etc. 
//            This is called once before entering into your main loop
// Input    : void
// Output   : void
//--------------------------------------------------------------
void init( void )
{
    /* initialize random seed: */
    srand(time(NULL));

    // Set precision for floating point output
    g_dElapsedTime = 0.0;    
    enemyBounceTime = 0;
    PowerTime = 0;
    // sets the initial state for the game
    g_eGameState = S_SPLASHSCREEN;

    g_sChar.m_cLocation.X = g_Console.getConsoleSize().X / 2;
    g_sChar.m_cLocation.Y = g_Console.getConsoleSize().Y / 2;
    g_sChar.m_bActive = true;
    g_sPortal[0, 1].m_bActive = true;
    PwrdUp = false;

    // sets the width, height and the font name to use in the console
    g_Console.setConsoleFont(0, 16, L"Consolas");

    //initialize wall
    //for (int i = 0; i < 100; i++)
    //{
    //    g_sWall[i].m_bActive = true;
    //    g_sWall[i].m_cLocation.X = rand() % 80;
    //    g_sWall[i].m_cLocation.Y = rand() % 25;
    //}

    //init enemy
    playerLives = 3;
    for (int i = 0; i < 4; i++)
    {
        g_sEnemy[i].m_bActive = true;
        g_sEnemy[i].m_cLocation.X = rand() % 80;
        g_sEnemy[i].m_cLocation.Y = rand() % 25;
    }

    //init bery
    for (int i = 0; i < 30; i++)
    {
        g_sBerry[i].m_bActive = true;
        g_sBerry[i].m_cLocation.X = rand() % 80;
        g_sBerry[i].m_cLocation.Y = rand() % 25;
    }

    //power ups
    for (int i = 0; i < 4; i++)
    {
        g_sPowerUp[i].m_bActive = true;
        g_sPowerUp[i].m_cLocation.X = rand() % 80;
        g_sPowerUp[i].m_cLocation.Y = rand() % 25;
    }

    int wallIdx = 0;

    for (int i = 0; i < 25; i++)
    {
        for (int j = 0; j < 80; j++)
        {
            if (map[i][j] == 1)
            {
                g_sWall[wallIdx].m_bActive = true;
                g_sWall[wallIdx].m_cLocation.X = j;
                g_sWall[wallIdx].m_cLocation.Y = i;
                wallIdx++;
            }
        }
    }

    // remember to set your keyboard handler, so that your functions can be notified of input events
    g_Console.setKeyboardHandler(keyboardHandler);
    g_Console.setMouseHandler(mouseHandler);
}

//--------------------------------------------------------------
// Purpose  : Reset before exiting the program
//            Do your clean up of memory here
//            This is called once just before the game exits
// Input    : Void
// Output   : void
//--------------------------------------------------------------
void shutdown( void )
{
    // Reset to white text on black background
    colour(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);

    g_Console.clearBuffer();
}

//--------------------------------------------------------------
// Purpose  : Get all the console input events
//            This function sets up the keyboard and mouse input from the console.
//            We will need to reset all the keyboard status, because no events will be sent if no keys are pressed.
//
//            Remember to set the handlers for keyboard and mouse events.
//            The function prototype of the handler is a function that takes in a const reference to the event struct
//            and returns nothing. 
//            void pfKeyboardHandler(const KEY_EVENT_RECORD&);
//            void pfMouseHandlerconst MOUSE_EVENT_RECORD&);
// Input    : Void
// Output   : void
//--------------------------------------------------------------
void getInput( void )
{
    // resets all the keyboard events
    memset(g_skKeyEvent, 0, K_COUNT * sizeof(*g_skKeyEvent));
    // then call the console to detect input from user
    g_Console.readConsoleInput();    
}

//--------------------------------------------------------------
// Purpose  : This is the handler for the keyboard input. Whenever there is a keyboard event, this function will be called.
//            Ideally, you should pass the key event to a specific function to handle the event.
//            This is because in some states, some keys would be disabled. Hence, to reduce your code complexity, 
//            it would be wise to split your keyboard input handlers separately.
//            
//            The KEY_EVENT_RECORD struct has more attributes that you can use, if you are adventurous enough.
//
//            In this case, we are not handling any keyboard event in the Splashscreen state
//            
// Input    : const KEY_EVENT_RECORD& keyboardEvent - reference to a key event struct
// Output   : void
//--------------------------------------------------------------
void keyboardHandler(const KEY_EVENT_RECORD& keyboardEvent)
{    
    switch (g_eGameState)
    {
    case S_SPLASHSCREEN: // don't handle anything for the splash screen
        break;
    case S_GAME: 
    case S_LOSE:
    case S_WIN:

        gameplayKBHandler(keyboardEvent); // handle gameplay keyboard event 
        break;
    }
}

//--------------------------------------------------------------
// Purpose  : This is the handler for the mouse input. Whenever there is a mouse event, this function will be called.
//            Ideally, you should pass the key event to a specific function to handle the event.
//            This is because in some states, some keys would be disabled. Hence, to reduce your code complexity, 
//            it would be wise to split your keyboard input handlers separately.
//            
//            For the mouse event, if the mouse is not moved, no event will be sent, hence you should not reset the mouse status.
//            However, if the mouse goes out of the window, you would not be able to know either. 
//
//            The MOUSE_EVENT_RECORD struct has more attributes that you can use, if you are adventurous enough.
//
//            In this case, we are not handling any mouse event in the Splashscreen state
//            
// Input    : const MOUSE_EVENT_RECORD& mouseEvent - reference to a mouse event struct
// Output   : void
//--------------------------------------------------------------
void mouseHandler(const MOUSE_EVENT_RECORD& mouseEvent)
{    
    switch (g_eGameState)
    {
    case S_SPLASHSCREEN: // don't handle anything for the splash screen
        break;
    case S_GAME: gameplayMouseHandler(mouseEvent); // handle gameplay mouse event
        break;
    }
}

//--------------------------------------------------------------
// Purpose  : This is the keyboard handler in the game state. Whenever there is a keyboard event in the game state, this function will be called.
//            
//            Add more keys to the enum in game.h if you need to detect more keys
//            To get other VK key defines, right click on the VK define (e.g. VK_UP) and choose "Go To Definition" 
//            For Alphanumeric keys, the values are their ascii values (uppercase).
// Input    : const KEY_EVENT_RECORD& keyboardEvent
// Output   : void
//--------------------------------------------------------------
void gameplayKBHandler(const KEY_EVENT_RECORD& keyboardEvent)
{
    // here, we map the key to our enums
    EKEYS key = K_COUNT;
    switch (keyboardEvent.wVirtualKeyCode)
    {
    case VK_UP: key = K_UP; break;
    case VK_DOWN: key = K_DOWN; break;
    case VK_LEFT: key = K_LEFT; break; 
    case VK_RIGHT: key = K_RIGHT; break; 
    case VK_SPACE: key = K_SPACE; break;
    case VK_ESCAPE: key = K_ESCAPE; break; 
    }
    // a key pressed event would be one with bKeyDown == true
    // a key released event would be one with bKeyDown == false
    // if no key is pressed, no event would be fired.
    // so we are tracking if a key is either pressed, or released
    if (key != K_COUNT)
    {
        g_skKeyEvent[key].keyDown = keyboardEvent.bKeyDown;
        g_skKeyEvent[key].keyReleased = !keyboardEvent.bKeyDown;
    }    
}

//--------------------------------------------------------------
// Purpose  : This is the mouse handler in the game state. Whenever there is a mouse event in the game state, this function will be called.
//            
//            If mouse clicks are detected, the corresponding bit for that mouse button will be set.
//            mouse wheel, 
//            
// Input    : const KEY_EVENT_RECORD& keyboardEvent
// Output   : void
//--------------------------------------------------------------
void gameplayMouseHandler(const MOUSE_EVENT_RECORD& mouseEvent)
{
    if (mouseEvent.dwEventFlags & MOUSE_MOVED) // update the mouse position if there are no events
    {
        g_mouseEvent.mousePosition = mouseEvent.dwMousePosition;
    }
    g_mouseEvent.buttonState = mouseEvent.dwButtonState;
    g_mouseEvent.eventFlags = mouseEvent.dwEventFlags;
}

//--------------------------------------------------------------
// Purpose  : Update function
//            This is the update function
//            double dt - This is the amount of time in seconds since the previous call was made
//
//            Game logic should be done here.
//            Such as collision checks, determining the position of your game characters, status updates, etc
//            If there are any calls to write to the console here, then you are doing it wrong.
//
//            If your game has multiple states, you should determine the current state, and call the relevant function here.
//
// Input    : dt = deltatime
// Output   : void
//--------------------------------------------------------------
void update(double dt)
{
    // get the delta time
    g_dElapsedTime += dt;
    g_dDeltaTime = dt;

    switch (g_eGameState)
    {
        case S_SPLASHSCREEN : splashScreenWait(); // game logic for the splash screen
            break;
        case S_GAME: updateGame(); // gameplay logic when we are in the game
            break;
        case S_WIN:
            processUserInput();
            break;
        case S_LOSE: 
            processUserInput();
            break;
        //case S_LOSE: renderGameOver();
        //    if (g_skKeyEvent[K_SPACE].keyDown)
        //    { // reset game
        //        g_eGameState = S_SPLASHSCREEN;
        //    }
        //    break;
        //case S_WIN: renderWin();
        //    if (g_skKeyEvent[K_SPACE].keyDown)
        //    { // reset game
        //        g_eGameState = S_SPLASHSCREEN;
        //    }
            break;
    }
}


void splashScreenWait()    // waits for time to pass in splash screen
{
    if (g_dElapsedTime > 3.0) // wait for 3 seconds to switch to game mode, else do nothing
        g_eGameState = S_GAME;
}

void updateGame()       // gameplay logic
{
    processUserInput(); // checks if you should change states or do something else with the game, e.g. pause, exit
    moveCharacter();    // moves the character, collision detection, physics, etc
                        // sound can be played here too.
    updateBerry();
    updateEnemy();
    updatePwrUp();
    updatePortal();
}

void moveCharacter()
{    
    if (ghostEaten == 4)
    {
        g_eGameState = S_WIN;
    }
    // Updating the location of the character based on the key release
    // providing a beep sound whenver we shift the character
    previousPos.X = g_sChar.m_cLocation.X;
    previousPos.Y = g_sChar.m_cLocation.Y;

    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.Y > 0)
    {
        //Beep(1440, 30);
        g_sChar.m_cLocation.Y--;       
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X > 0)
    {
        //Beep(1440, 30);
        g_sChar.m_cLocation.X--;        
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.Y < g_Console.getConsoleSize().Y - 1)
    {
        //Beep(1440, 30);
        g_sChar.m_cLocation.Y++;        
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X < g_Console.getConsoleSize().X - 1)
    {
        //Beep(1440, 30);
        g_sChar.m_cLocation.X++;
    }
    if (g_skKeyEvent[K_SPACE].keyReleased)
    {
        g_sChar.m_bActive = !g_sChar.m_bActive;        
    }

    for (int i = 0; i < 2000; ++i)
    {
        if (g_sWall[i].m_cLocation.X == g_sChar.m_cLocation.X && g_sWall[i].m_cLocation.Y == g_sChar.m_cLocation.Y)
        {
            g_sChar.m_cLocation.X = previousPos.X;
            g_sChar.m_cLocation.Y = previousPos.Y;
            return;
        }
    }
   
}
void processUserInput()
{
    // quits the game if player hits the escape key
    if (g_skKeyEvent[K_ESCAPE].keyReleased)
        g_bQuitGame = true;
    if (g_skKeyEvent[K_SPACE].keyReleased)
    {
        switch (g_eGameState)
        {
        case S_WIN:
            reset();
            break;
        case S_LOSE:
            reset();
            break;
        }
    }
}

//--------------------------------------------------------------
// Purpose  : Render function is to update the console screen
//            At this point, you should know exactly what to draw onto the screen.
//            Just draw it!
//            To get an idea of the values for colours, look at console.h and the URL listed there
// Input    : void
// Output   : void
//--------------------------------------------------------------
void render()
{
    clearScreen();      // clears the current screen and draw from scratch 
    switch (g_eGameState)
    {
    case S_SPLASHSCREEN: renderSplashScreen();
        break;
    case S_GAME: renderGame();
        break;
    case S_LOSE: renderGameOver();
        break;
    case S_WIN: renderWin();
        break;
    }
    renderFramerate();      // renders debug information, frame rate, elapsed time, etc
    renderInputEvents();    // renders status of input events
    renderToScreen();       // dump the contents of the buffer to the screen, one frame worth of game
}

void clearScreen()
{
    // Clears the buffer with this colour attribute
    g_Console.clearBuffer(0x1F);
}

void renderToScreen()
{
    // Writes the buffer to the console, hence you will see what you have written
    g_Console.flushBufferToConsole();
}

void renderSplashScreen()  // renders the splash screen
{
    COORD c = g_Console.getConsoleSize();
    c.Y /= 3;
    c.X = c.X / 2 - 9;
    g_Console.writeToBuffer(c, "A game in 3 seconds", 0x03);
    c.Y += 1;
    c.X = g_Console.getConsoleSize().X / 2 - 20;
    g_Console.writeToBuffer(c, "Press <Space> to change character colour", 0x09);
    c.Y += 1;
    c.X = g_Console.getConsoleSize().X / 2 - 9;
    g_Console.writeToBuffer(c, "Press 'Esc' to quit", 0x09);
}

void renderGame()
{
    renderMap();        // renders the map to the buffer first
    renderBerry();      // renders the berries(30)
    renderPortal();
    renderWall();
    renderEnemy();
    renderPowerUp();
    renderCharacter();  // renders the character into the buffer
}

void renderMap()
{
    //// Set up sample colours, and output shadings
    //const WORD colors[] = {
    //    0x1A, 0x2B, 0x3C, 0x4D, 0x5E, 0x6F,
    //    0xA1, 0xB2, 0xC3, 0xD4, 0xE5, 0xF6
    //};

    //COORD c;
    //for (int i = 0; i < 12; ++i)
    //{
    //    c.X = 5 * i;
    //    c.Y = i + 1;
    //    colour(colors[i]);
    //    g_Console.writeToBuffer(c, " ����", colors[i]);
    //}
}

void renderCharacter()
{
    // Draw the location of the character
    if (PwrdUp == false)
    {
        if (g_sChar.m_bActive)
        {
            charColor = 0x6B;
        }
        g_Console.writeToBuffer(g_sChar.m_cLocation, (char)1, charColor);
    }
    if (PwrdUp == true)
    {
        if (g_sChar.m_bActive)
        {
            charColor = 0x4B;
        }
        g_Console.writeToBuffer(g_sChar.m_cLocation, (char)1, charColor);
    }
}

void renderFramerate()
{
    COORD c;
    // displays the framerate
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(3);
    ss << 1.0 / g_dDeltaTime << "fps";
    c.X = g_Console.getConsoleSize().X - 9;
    c.Y = 0;
    g_Console.writeToBuffer(c, ss.str());

    // displays the current berry score
    ss.str("");
    ss << "score: " << score;
    c.X = g_Console.getConsoleSize().X - 66;
    c.Y = 0;
    g_Console.writeToBuffer(c, ss.str());

    // displays the elapsed time
    ss.str("");
    ss << g_dElapsedTime << "secs";
    c.X = 0;
    c.Y = 0;
    g_Console.writeToBuffer(c, ss.str());

    //displays current number of lives
    ss.str("");
    ss << "lives left: " << playerLives;
    c.X = g_Console.getConsoleSize().X - 55;
    c.Y = 0;
    g_Console.writeToBuffer(c, ss.str());

    ss.str("");
    ss << "PowerUpTime: " << PowerTime;
    c.X = g_Console.getConsoleSize().X - 40;
    c.Y = 0;
    g_Console.writeToBuffer(c, ss.str());
}

// this is an example of how you would use the input events
void renderInputEvents()
{
    // keyboard events
    COORD startPos = {50, 2};
    std::ostringstream ss;
    std::string key;
    //for (int i = 0; i < K_COUNT; ++i)
    //{
    //    ss.str("");
    //    switch (i)
    //    {
    //    case K_UP: key = "UP";
    //        break;
    //    case K_DOWN: key = "DOWN";
    //        break;
    //    case K_LEFT: key = "LEFT";
    //        break;
    //    case K_RIGHT: key = "RIGHT";
    //        break;
    //    case K_SPACE: key = "SPACE";
    //        break;
    //    default: continue;
    //    }
    //    if (g_skKeyEvent[i].keyDown)
    //        ss << key << " pressed";
    //    else if (g_skKeyEvent[i].keyReleased)
    //        ss << key << " released";
    //    else
    //        ss << key << " not pressed";

    //    COORD c = { startPos.X, startPos.Y + i };
    //    g_Console.writeToBuffer(c, ss.str(), 0x17);
    //}

    // mouse events    
    ss.str("");
    ss << "(" << g_mouseEvent.mousePosition.X << ", " << g_mouseEvent.mousePosition.Y << ")";
    g_Console.writeToBuffer(g_mouseEvent.mousePosition, ss.str(), 0x59);
    ss.str("");
    switch (g_mouseEvent.eventFlags)
    {
    case 0:
        if (g_mouseEvent.buttonState == FROM_LEFT_1ST_BUTTON_PRESSED)
        {
            ss.str("Left Button Pressed");
            g_Console.writeToBuffer(g_mouseEvent.mousePosition.X, g_mouseEvent.mousePosition.Y + 1, ss.str(), 0x59);
        }
        else if (g_mouseEvent.buttonState == RIGHTMOST_BUTTON_PRESSED)
        {
            ss.str("Right Button Pressed");
            g_Console.writeToBuffer(g_mouseEvent.mousePosition.X, g_mouseEvent.mousePosition.Y + 2, ss.str(), 0x59);
        }
        else
        {
            ss.str("Some Button Pressed");
            g_Console.writeToBuffer(g_mouseEvent.mousePosition.X, g_mouseEvent.mousePosition.Y + 3, ss.str(), 0x59);
        }
        break;
    case DOUBLE_CLICK:
        ss.str("Double Clicked");
        g_Console.writeToBuffer(g_mouseEvent.mousePosition.X, g_mouseEvent.mousePosition.Y + 4, ss.str(), 0x59);
        break;        
    case MOUSE_WHEELED:
        if (g_mouseEvent.buttonState & 0xFF000000)
            ss.str("Mouse wheeled down");
        else
            ss.str("Mouse wheeled up");
        g_Console.writeToBuffer(g_mouseEvent.mousePosition.X, g_mouseEvent.mousePosition.Y + 5, ss.str(), 0x59);
        break;
    default:        
        break;
    }
    
}

void updateBerry()
{
    for (int i = 0; i < 30; i++)
    {
        if (g_sChar.m_cLocation.X == g_sBerry[i].m_cLocation.X && g_sChar.m_cLocation.Y == g_sBerry[i].m_cLocation.Y)
        {
            if (g_sBerry[i].m_bActive == true)
            {
                g_sBerry[i].m_bActive = false;
                score += 10;
            }
        }
    }
    if (score >= 110)
    {
        g_eGameState = S_WIN;
    }

}

void renderBerry()
{
    for (int i = 0; i < 30; i++)
    {
        if (g_sBerry[i].m_bActive == true)
        {
            g_Console.writeToBuffer(g_sBerry[i].m_cLocation, (char)8, berryColor);
        }
    }
}

void renderWall()
{
    for (int i = 0; i < 2000; i++)
    {

        if (g_sWall[i].m_bActive == true)
        {
            g_Console.writeToBuffer(g_sWall[i].m_cLocation, "#", 0x03);
        }
    }
}

void updatePortal()
{
        if (g_sChar.m_cLocation.X == g_sPortal[0].m_cLocation.X && g_sChar.m_cLocation.Y == g_sPortal[0].m_cLocation.Y)
        {
            g_sChar.m_cLocation.X = 78;
            g_sChar.m_cLocation.Y = 12;
        }
        if (g_sChar.m_cLocation.X == g_sPortal[1].m_cLocation.X && g_sChar.m_cLocation.Y == g_sPortal[1].m_cLocation.Y)
        {
            g_sChar.m_cLocation.X = 1;
            g_sChar.m_cLocation.Y = 12;
        }
}

void renderPortal()
{
    g_sPortal[0].m_cLocation.X = 0;
    g_sPortal[0].m_cLocation.Y = 12;
    g_sPortal[1].m_cLocation.X = 79;
    g_sPortal[1].m_cLocation.Y = 12;

    g_Console.writeToBuffer(g_sPortal[0].m_cLocation, "@", 0x04);
    g_Console.writeToBuffer(g_sPortal[1].m_cLocation, "@", 0x04);
}

void updateEnemy()
{
    if (playerLives <= 0)
    {
        g_eGameState = S_LOSE;
    }
    enemyBounceTime += g_dDeltaTime; // every 1 second..
    if (enemyBounceTime >= 1)
    {
        enemyBounceTime = 0;
        if (PwrdUp == false)
        {
            for (int i = 0; i < 4; i++)
            {
                if (g_sChar.m_cLocation.X == g_sEnemy[i].m_cLocation.X && g_sChar.m_cLocation.Y == g_sEnemy[i].m_cLocation.Y)
                {
                    playerLives -= 1;
                    g_sChar.m_cLocation.X = 39;
                    g_sChar.m_cLocation.Y = 12;
                }
            }

            // 0 - left, 1 - up, 2 - right, 3 - down
            for (int i = 0; i < 3; i++)
            {
                enemyPrevPos[i].X = g_sEnemy[i].m_cLocation.X;
                enemyPrevPos[i].Y = g_sEnemy[i].m_cLocation.Y;
                int EnemyDir = rand() % 4;
                if (EnemyDir == 0) // left
                {
                    g_sEnemy[i].m_cLocation.X -= 1;
                }
                if (EnemyDir == 1) // up
                {
                    g_sEnemy[i].m_cLocation.Y -= 1;
                }
                if (EnemyDir == 2) // right
                {
                    g_sEnemy[i].m_cLocation.X += 1;
                }
                if (EnemyDir == 3) // down
                {
                    g_sEnemy[i].m_cLocation.Y += 1;
                }
                for (int j = 0; j < 2000; ++j)
                {
                    if (g_sWall[j].m_cLocation.X == g_sEnemy[i].m_cLocation.X && g_sWall[j].m_cLocation.Y == g_sEnemy[i].m_cLocation.Y)
                    {
                        g_sEnemy[i].m_cLocation.X = enemyPrevPos[i].X;
                        g_sEnemy[i].m_cLocation.Y = enemyPrevPos[i].Y;
                        break;
                    }
                }
            }//end of for loop

            // follows the player
            //enemyPrevPos[3].X = g_sEnemy[3].m_cLocation.X;
            //enemyPrevPos[3].Y = g_sEnemy[3].m_cLocation.Y;
            if (g_sEnemy[3].m_cLocation.X > g_sChar.m_cLocation.X && map[g_sEnemy[3].m_cLocation.Y][g_sEnemy[3].m_cLocation.X - 1] != 1) // move left
            {
                g_sEnemy[3].m_cLocation.X -= 1;
            }
            else if (g_sEnemy[3].m_cLocation.X < g_sChar.m_cLocation.X && map[g_sEnemy[3].m_cLocation.Y][g_sEnemy[3].m_cLocation.X + 1] != 1) // move right
            {
                g_sEnemy[3].m_cLocation.X += 1;
            }
            else if (g_sEnemy[3].m_cLocation.Y > g_sChar.m_cLocation.Y && map[g_sEnemy[3].m_cLocation.Y - 1][g_sEnemy[3].m_cLocation.X] != 1) // move up
            {
                g_sEnemy[3].m_cLocation.Y -= 1;
            }
            else if (g_sEnemy[3].m_cLocation.Y < g_sChar.m_cLocation.Y && map[g_sEnemy[3].m_cLocation.Y + 1][g_sEnemy[3].m_cLocation.X] != 1) // move down
            {
                g_sEnemy[3].m_cLocation.Y += 1;
            }
            //for (int j = 0; j < 2000; ++j)
            //{
            //    if (g_sWall[j].m_cLocation.X == g_sEnemy[3].m_cLocation.X && g_sWall[j].m_cLocation.Y == g_sEnemy[3].m_cLocation.Y)
            //    {
            //        g_sEnemy[3].m_cLocation.X = enemyPrevPos[3].X;
            //        g_sEnemy[3].m_cLocation.Y = enemyPrevPos[3].Y;
            //        return;
            //    }
            //}
            //}
        }
        else //if (PwrdUp == true)
        {
            for (int i = 0; i < 4; i++)
            {
                //enemyPrevPos[i].X = g_sEnemy[i].m_cLocation.X;
                //enemyPrevPos[i].Y = g_sEnemy[i].m_cLocation.Y;
                if (g_sEnemy[i].m_cLocation.X < g_sChar.m_cLocation.X && map[g_sEnemy[i].m_cLocation.Y][g_sEnemy[i].m_cLocation.X - 1] != 1) // move left
                {
                    g_sEnemy[i].m_cLocation.X -= 1;
                }
                else if (g_sEnemy[i].m_cLocation.X > g_sChar.m_cLocation.X && map[g_sEnemy[i].m_cLocation.Y][g_sEnemy[i].m_cLocation.X + 1] != 1) // move right
                {
                    g_sEnemy[i].m_cLocation.X += 1;
                }
                else if (g_sEnemy[i].m_cLocation.Y < g_sChar.m_cLocation.Y && map[g_sEnemy[i].m_cLocation.Y - 1][g_sEnemy[i].m_cLocation.X] != 1) // move up
                {
                    g_sEnemy[i].m_cLocation.Y -= 1;
                }
                else if (g_sEnemy[i].m_cLocation.Y > g_sChar.m_cLocation.Y && map[g_sEnemy[i].m_cLocation.Y + 1][g_sEnemy[i].m_cLocation.X] != 1) // move down
                {
                    g_sEnemy[i].m_cLocation.Y += 1;
                }
                //for (int j = 0; j < 2000; ++j)
                //{
                //    if (g_sWall[j].m_cLocation.X == g_sEnemy[i].m_cLocation.X && g_sWall[j].m_cLocation.Y == g_sEnemy[i].m_cLocation.Y)
                //    {
                //        g_sEnemy[i].m_cLocation.X = enemyPrevPos[i].X;
                //        g_sEnemy[i].m_cLocation.Y = enemyPrevPos[i].Y;
                //        break;
                //    }
                //}
            }
        }
    }
}

void renderEnemy()
{
    if (PwrdUp == false)
    {
        for (int i = 0; i < 3; i++)
        {
            if (g_sEnemy[i].m_bActive == true)
            {
                g_Console.writeToBuffer(g_sEnemy[i].m_cLocation, "E", 0x5B);
            }
        }
        if (g_sEnemy[3].m_bActive == true)
        {
            g_Console.writeToBuffer(g_sEnemy[3].m_cLocation, "E", 0x4A);
        }
    }
    if (PwrdUp == true)
    {
        for (int i = 0; i < 4; i++)
        {
            if (g_sEnemy[i].m_bActive == true)
            {
                g_Console.writeToBuffer(g_sEnemy[i].m_cLocation, "!", 0x6D);
            }
        }
    }
}

void renderPowerUp()
{
    for (int i = 0; i < 4; i++)
    {
        if (g_sPowerUp[i].m_bActive == true)
        {
            g_Console.writeToBuffer(g_sPowerUp[i].m_cLocation, "P", 0x3C);
        }
    }
}

void updatePwrUp()
{
    PowerTime += g_dDeltaTime;
    for (int i = 0; i < 4; i++)
    {
        if (g_sChar.m_cLocation.X == g_sPowerUp[i].m_cLocation.X && g_sChar.m_cLocation.Y == g_sPowerUp[i].m_cLocation.Y)
        {
            PowerTime = 0;
            PwrdUp = true;
            g_sPowerUp[i].m_bActive = false;

        }
    }
    if (PowerTime >= 15)
    {
        PwrdUp = false;
    }

    if (PwrdUp == true)
    {
        for (int i = 0; i < 4; i++)
        {
            if (g_sChar.m_cLocation.X == g_sEnemy[i].m_cLocation.X && g_sChar.m_cLocation.Y == g_sEnemy[i].m_cLocation.Y)
            {
                if (g_sEnemy[i].m_bActive == true)
                {
                    g_sEnemy[i].m_bActive = false;
                    ghostEaten += 1;
                }
            }
        }
    }
}

void renderGameOver() // Game Over screen
{
    COORD c = g_Console.getConsoleSize();
    //if (playerLives <= 0)
    //{
        //g_eGameState = S_LOSE;
        std::ostringstream ss;
        c.Y /= 3;
        c.X = c.X / 2 - 5;
        g_Console.writeToBuffer(c, "Game Over", 0x04);
        c.Y += 1;
        c.X = g_Console.getConsoleSize().X / 2 - 7;
        ss << "You got a score of " << score;
        g_Console.writeToBuffer(c, ss.str(), 0x04);
        c.Y += 1;
        c.X = g_Console.getConsoleSize().X / 2 - 9;
        g_Console.writeToBuffer(c, "Press Space to restart", 0x04);
    //}
}

void renderWin()
{
    COORD c = g_Console.getConsoleSize();
    //if (score >= 110)
    //{
        //g_eGameState = S_WIN;
        std::ostringstream ss;
        c.Y /= 3;
        c.X = c.X / 2 - 5;
        g_Console.writeToBuffer(c, "You Win", 0x04);
        c.Y += 1;
        c.X = g_Console.getConsoleSize().X / 2 - 7;
        ss << "You got a score of " << score * playerLives; // player gets a higher score depending on their lives left + time they took
        g_Console.writeToBuffer(c, ss.str(), 0x04);
        c.Y += 1;
        c.X = g_Console.getConsoleSize().X / 2 - 9;
        g_Console.writeToBuffer(c, "Press Space to restart", 0x04);
 //   }
}

void reset()
{
    g_eGameState = S_SPLASHSCREEN;
    g_sChar.m_cLocation.X = 39;
    g_sChar.m_cLocation.Y = 12;
    score = 0;
    ghostEaten = 0;
    playerLives = 3;
    PwrdUp = false;
    PowerTime = 0;
    for (int i = 0; i < 4; i++)
    {
        g_sEnemy[i].m_bActive = true;
        g_sEnemy[i].m_cLocation.X = rand() % 80;
        g_sEnemy[i].m_cLocation.Y = rand() % 25;
    }
    for (int i = 0; i < 30; i++)
    {
        g_sBerry[i].m_bActive = true;
        g_sBerry[i].m_cLocation.X = rand() % 80;
        g_sBerry[i].m_cLocation.Y = rand() % 25;
    }
}