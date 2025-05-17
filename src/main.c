#include "raylib.h"
#include <stdbool.h>
#include <stdio.h>
#define MAX_BODY 300
#include <string.h> 
#include<time.h>
#include <stdlib.h>


// Declare for the compiler to recognize
typedef struct Snake Snake;
bool check(Vector2 a, Vector2 b);
bool elementInSnake(Vector2 position, Snake *snake);
Vector2 GetRandomCell(int widthCells, int heightCells);
void DrawItems(Texture2D background, Rectangle sourceRect, Rectangle destRect, Vector2 origin, float rotation, Color tint);
Vector2 GetRandomFoodPosition(int widthCells, int heightCells, Snake *snake);


// Declare Button struct
typedef struct {
    Rectangle rect;
    Color color;
    float radius;
    Color borderColor; 
    float borderThickness; 
} Button;

// Function to initialize the button
void init_button(Button* button, Rectangle rect, Color color, float radius, Color borderColor, float borderThickness) {
    button->rect = rect;     
    button->color = color;    
    button->radius = radius;  
    button->borderColor = borderColor; 
    button->borderThickness = borderThickness; 
}

// Function to check if the mouse is over the button
bool is_mouse_over_button(Button button) {
    return CheckCollisionPointRec(GetMousePosition(), button.rect);
}

// Declare snake food struct
typedef struct {
    Vector2 position;
} Food;


// Draw the food
void DrawFood(Food *food, float cellSize, int rectX, int rectY, int widthCells, int heightCells, Snake *snake) {
    // Check if the food position is out of bounds, and reset it if necessary
    if (food->position.x < 0 || food->position.y < 0 || 
        food->position.x >= widthCells || food->position.y >= heightCells || 
        elementInSnake(food->position, snake)) {
        food->position = GetRandomFoodPosition(widthCells, heightCells, snake);
    }

    // Calculate the position of the food
    float foodX = rectX + (food->position.x * cellSize);
    float foodY = rectY + (food->position.y * cellSize);

    // Draw the apple's body as a slightly larger red circle
    DrawCircle(foodX + cellSize / 2, foodY + cellSize / 2, cellSize / 3, RED);

    // Draw the apple's leaf as a slightly larger green oval
    float leafX = foodX + cellSize / 2 - 5; // Position the leaf above the apple
    float leafY = foodY + cellSize / 2 - 8; // Position the leaf above the apple
    DrawEllipse(leafX, leafY, 7, 4, DARKGREEN); // Draw the leaf as a slightly larger ellipse
}

// Function to get random cell to place the food
Vector2 GetRandomCell(int widthCells, int heightCells) {
    // Seed the random number generator using the current time
    srand ((unsigned) time (0) );

    // Generate a random coordinate within the width and height boundaries of the snake board
    float x = GetRandomValue(0, widthCells - 1);
    float y = GetRandomValue(0, heightCells - 1);
    return (Vector2){x, y};   // Return the random position
}

// Declare Position struct for the snake body position
typedef struct {
    Vector2 position;
} Position;

// Declare Snake struct
typedef struct Snake {
    Position body[MAX_BODY];
    int length;
    Vector2 direction;
    Color color; 
} Snake;

// Declare a struct to hold the button context
typedef struct {
    bool *option_flag;
    bool *gameRunning;
    bool *gameOver;
    Snake *mySnake;
    Food *food;
    int widthCells;
    int heightCells;
    Button *backToMenu;
    Button *option;
    Button *backButton;
    Button *start;
    Button *playAgain;
    Button *GreenSnake;
    Button *BlackSnake;
    Button *snakeColorButton;
    Color *snakeColor;
} ButtonContext;

// Function to get random food position
Vector2 GetRandomFoodPosition(int widthCells, int heightCells, Snake *snake) {
    Vector2 position;
    do {
        position = GetRandomCell(widthCells, heightCells);
    } while (elementInSnake(position, snake)); 
    return position;
}

// Function to check if the food is inside the snake
bool elementInSnake(Vector2 position, Snake *snake) {
    for (int i = 0; i < snake->length; i++) {
        // Compare the food position with the snake body position
        if (check(snake->body[i].position, position)) {
            return true;
        }
    }
    return false;
}

// Function to draw the snake
void DrawSnake(Snake *snake, float cellSize, int rectX, int rectY, Color snakeColor) {
    for (int i = 0; i < snake->length; i++) {
        // Draw the snake body based on length
        DrawCircle(
            rectX + snake->body[i].position.x * cellSize + cellSize / 2,  // x position
            rectY + snake->body[i].position.y * cellSize + cellSize / 2,   // y position
            cellSize / 2,  // radius
            snakeColor   // color
        );
    }

    // snake eyes direction
    float eyeOffsetX = snake->direction.x * (cellSize / 20);
    float eyeOffsetY = snake->direction.y * (cellSize / 10);

    // snake head position
    Vector2 headPos = (Vector2){
        rectX + snake->body[0].position.x * cellSize + cellSize / 2,
        rectY + snake->body[0].position.y * cellSize + cellSize / 2
    };

    // Draw eyes 
    DrawCircle(headPos.x - cellSize / 4 + eyeOffsetX, headPos.y - cellSize / 4 + eyeOffsetY, cellSize / 8, WHITE);
    DrawCircle(headPos.x + cellSize / 4 + eyeOffsetX, headPos.y - cellSize / 4 + eyeOffsetY, cellSize / 8, WHITE);

    // Draw pupils 
    DrawCircle(headPos.x - cellSize / 4 + eyeOffsetX, headPos.y - cellSize / 4 + eyeOffsetY, cellSize / 16, BLACK);
    DrawCircle(headPos.x + cellSize / 4 + eyeOffsetX, headPos.y - cellSize / 4 + eyeOffsetY, cellSize / 16, BLACK);

    // mouth position 
    Vector2 mouthPos = (Vector2){
        headPos.x + snake->direction.x * (cellSize / 25), // Move forward 
        headPos.y + snake->direction.y * (cellSize / 16) + (cellSize / 16) // Move down
    };

    // Draw the mouth using a half-circle
    DrawCircleSector(
        mouthPos,               // Position
        cellSize / 4,          // Radius
        0, 180,               // mouth Angles 
        10,                  // Segments 
        RED                 // Color
    );
}

// Function to check if the snake hit itself
void checkwithSnake(Snake *snake, bool *gameOver, Vector2 newHeadPosition, int widthCells, int heightCells) {
    // Check if the new head position is within the boundaries
    if (newHeadPosition.x < 0 || newHeadPosition.x >= widthCells ||
        newHeadPosition.y < 0 || newHeadPosition.y >= heightCells) {
        *gameOver = true; // Set the game over flag to true
        return; // Do not move the snake if it hits the wall
    }

    // Check if the snake collides with itself
    for (int i = 1; i < snake->length; i++) {  // Start from index 1 to avoid checking the head against itself
        if (check(newHeadPosition, snake->body[i].position)) {
            *gameOver = true; // The snake hit its body
            return;
        }
    }
}

void MoveSnake(Snake *snake, bool *addSegment, bool *gameOver, int widthCells, int heightCells) {
    if (*gameOver) return; // Stop updating movement if the game is over
    
    // Calculate the new head position
    Vector2 newHeadPosition = snake->body[0].position;
    newHeadPosition.x += snake->direction.x;
    newHeadPosition.y += snake->direction.y;

    // Call checkwithSnake with correct arguments
    checkwithSnake(snake, gameOver, newHeadPosition, widthCells, heightCells);

    // If game is over, stop execution
    if (*gameOver) return;

    // If we need to add a new segment to the snake
    if (*addSegment) {
        // Shift the existing segments of the snake to make space for the new one
        for (int i = snake->length; i > 0; i--) {
            snake->body[i] = snake->body[i - 1];
        }
        snake->length++;
        *addSegment = false;
    } else {
        // Shift the segments to the next position
        for (int i = snake->length - 1; i > 0; i--) {
            snake->body[i] = snake->body[i - 1];
        }
    }
    
    // Move the head of the snake to the new position
    snake->body[0].position = newHeadPosition;
}

// Function to get the current time for controlling the timing of the snake's movement
bool EventTriggered(double interval, double *lastTime) {
    double currentTime = GetTime();
    if (currentTime - *lastTime >= interval) {
        *lastTime = currentTime;
        return true;
    }
    return false;
}

// Function to check if both vectors equal to each other
bool check(Vector2 a, Vector2 b) {
    return (a.x == b.x) && (a.y == b.y);
}

// Function to check if the snake is in the same position as the food
void snakeEatFood(Snake *snake, Food *food, bool *addSegment, Sound eatSound) {
    if (check(snake->body[0].position, food->position)) {
        // Set the food position to a new random position and set addSegment to true
        food->position = GetRandomFoodPosition(22, 15, snake);
        *addSegment = true;
        PlaySound(eatSound); // Play eating sound effect
    }
}

// Function to stop the Game
void GameOver(bool *gameRunning) {
    printf("Game Over\n");
    *gameRunning = false; // Stop the game
}

// Function to check if the snake hit the wall
void checkwithWalls(Snake *snake, int widthCells, int heightCells, bool *gameOver) {
    // Check if the snake hits the sides of the grid
    if (snake->body[0].position.x < 0 || snake->body[0].position.x >= widthCells ||
        snake->body[0].position.y < 0 || snake->body[0].position.y >= heightCells) {
        *gameOver = true;  // Set the game over flag to true
    }
}

// Function to reset the snake
void ResetSnake(Snake* snake) {
    // Set the snake's features to the initial values
    snake->direction = (Vector2){0.0f, -1.0f};
    snake->length = 2;

    snake->body[0].position.x = 10; // Head
    snake->body[0].position.y = 10;

    snake->body[1].position.x = 9;  // Body
    snake->body[1].position.y = 10;

    snake->body[2].position.x = 8;  // Body
    snake->body[2].position.y = 10;
}

// Function to handle button mouse-over and click actions
void HandleButtonMouseEvent(Button *button, ButtonContext *context) {
    static Button *activeSnakeButton = NULL; // Tracks the currently selected snake button

    // Reset the button color to default unless it's the currently active snake button
    if (button != activeSnakeButton) {
        button->color = (Color){255, 140, 0, 255}; // Default button color
    }

    // Check if the mouse is over the button and the button is not the snake color button
    // or if the snake color is already set to DARKGREEN and the current button is the GreenSnake button
    if ((is_mouse_over_button(*button) && button != context->snakeColorButton) || 
        (ColorIsEqual(*context->snakeColor, DARKGREEN) && button == context->GreenSnake)) {
        
        button->color = RED; // Change button color when mouse is over it

        // Check if the left mouse button is pressed
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            
            // Handle different button actions based on which button is pressed
            if (button == context->option) {
                *context->option_flag = true; // Set the option flag to true when the option button is pressed
            } else if (button == context->backButton) {
                // Stop the game and set flags for game over when back button is pressed
                *context->gameRunning = false;
                *context->option_flag = false;
                *context->gameOver = true;
            } else if (button == context->backToMenu) {
                // Stop the game and set game over when back to menu button is pressed
                *context->gameRunning = false;
                *context->gameOver = true;
            } else if (button == context->start || button == context->playAgain) {
                // Start or restart the game when the start/play again button is pressed
                *context->gameRunning = true;
                *context->gameOver = false;
                ResetSnake(context->mySnake); // Reset the snake to its initial state
                // Randomly position the food on the board
                context->food->position = GetRandomFoodPosition(context->widthCells, context->heightCells, context->mySnake);
            } 
            // Handle snake color buttons (GreenSnake, BlackSnake)
            else if (button == context->GreenSnake) {
                *context->snakeColor = DARKGREEN; // Set snake color to green
                activeSnakeButton = context->GreenSnake; // Set this button as the active one
            } else if (button == context->BlackSnake) {
                *context->snakeColor = BLACK; // Set snake color to black
                activeSnakeButton = context->BlackSnake; // Set this button as the active one
            }
        }
    }
}

// create border on the buttons
Rectangle GetBorderRectangle(Button start) {
    return (Rectangle){
        start.rect.x - start.borderThickness, 
        start.rect.y - start.borderThickness, 
        start.rect.width + 2 * start.borderThickness, 
        start.rect.height + 2 * start.borderThickness
    };
}

// Function to draw the button with a label
void DrawButtonWithLabel(Button *btn, const char *buttonText) {
    // Check if the button text is "Snake Color" to draw it differently (no border and rounded corners)
    if (strcmp(buttonText, "Snake Color") == 0) {
        int borderThickness = 2;  // Adjust this value to increase or decrease the border size
        DrawRectangle(btn->rect.x - borderThickness, btn->rect.y - borderThickness, 
                      btn->rect.width + 2 * borderThickness, btn->rect.height + 2 * borderThickness, WHITE); // Larger white border
        DrawRectangle(btn->rect.x, btn->rect.y, btn->rect.width, btn->rect.height, btn->color); // Button color inside the border
    } else {
        // Draw a button with rounded corners and a border
        Rectangle borderRect = GetBorderRectangle(*btn); // Assuming this function returns the border area
        // Draw the border with rounded corners
        DrawRectangleRounded(borderRect, btn->radius, 10, btn->borderColor);  // Border with rounded corners
        // Draw the button background with rounded corners
        DrawRectangleRounded(btn->rect, btn->radius, 10, btn->color);  // Button with rounded corners
    }

    // Set the position of the label and render it on the button
    int textX = btn->rect.x + (btn->rect.width - MeasureText(buttonText, 30)) / 2;
    int textY = btn->rect.y + (btn->rect.height - 30) / 2;
    DrawText(buttonText, textX, textY, 30, WHITE);
}

// Function to draw the board pattern
void board_pattern(int rectY, int rectX, int rectHeight, int rectWidth, int cellSize, Color color) {
    // Loop through each cell in the rectangle based on the cell size
    for (int y = rectY; y < rectY + rectHeight; y += cellSize) {
        for (int x = rectX; x < rectX + rectWidth; x += cellSize) {
            if ((x / cellSize + y / cellSize) % 2 == 0) {
                 // If even, draw a rectangle with the ORANGE color
                DrawRectangle(x, y, cellSize, cellSize, ORANGE);
            } else {
                 // If odd, draw a rectangle with the ORANGE color
                DrawRectangle(x, y, cellSize, cellSize, color);
            }
        }
    }
}

// Function to handle snake mechanics, including movement and loss conditions
void snake_rules(Snake* mySnake, Food* food, bool* gameOver, double* lastTime, bool* addSegment, int widthCells, int heightCells, Sound eatSound) {
    // Slow down snake movement relative to frame rate
    if (EventTriggered(0.15, lastTime)) {
        MoveSnake(mySnake, addSegment, gameOver, widthCells, heightCells); // Pass gameOver flag
        snakeEatFood(mySnake, food, addSegment, eatSound);
        checkwithWalls(mySnake, widthCells, heightCells, gameOver); // Check if the snake hits the wall
    }

    // Check if the arrow keys are pressed to change the snake's direction
    if (IsKeyPressed(KEY_UP) && mySnake->direction.y == 0) {
        mySnake->direction = (Vector2){0.0f, -1.0f};
    } else if (IsKeyPressed(KEY_DOWN) && mySnake->direction.y == 0) {
        mySnake->direction = (Vector2){0.0f, 1.0f};
    } else if (IsKeyPressed(KEY_LEFT) && mySnake->direction.x == 0) {
        mySnake->direction = (Vector2){-1.0f, 0.0f};
    } else if (IsKeyPressed(KEY_RIGHT) && mySnake->direction.x == 0) {
        mySnake->direction = (Vector2){1.0f, 0.0f};
    }
}

// Function to handle game end
void gameEnd(int boxX, int boxY, int boxWidth, int boxHeight, bool* gameOver, bool* gameRunning, Snake* mySnake, Food* food, int widthCells, int heightCells, Button* backToMenu, Button* playAgain) {
    // Create a window to render the message and display the score
    if (*gameOver) {
        DrawRectangle(boxX, boxY, boxWidth, boxHeight, (Color){0, 0, 0, 180});
        DrawRectangleLines(boxX, boxY, boxWidth, boxHeight, WHITE);
        DrawText("GAME OVER", boxX + 100, boxY + 40, 30, RED);
        DrawText(TextFormat("your score : %d", mySnake->length-2 ), boxX + 120, boxY + 95, 20, WHITE);

        // Draw the "Menu" button with border
        DrawButtonWithLabel(backToMenu, "Menu");   
        DrawButtonWithLabel(playAgain, "Restart"); 

        // Handle mouse events for the buttons
        ButtonContext context = {
            .gameRunning = gameRunning,
            .gameOver = gameOver,
            .mySnake = mySnake,
            .food = food,
            .widthCells = widthCells,
            .heightCells = heightCells,
            .backToMenu = backToMenu,
            .playAgain = playAgain
        };
        HandleButtonMouseEvent(playAgain, &context);
        HandleButtonMouseEvent(backToMenu, &context);
    }
}

// draw the imgaes like background and food
void DrawItems(Texture2D background, Rectangle sourceRect, Rectangle destRect, Vector2 origin, float rotation, Color tint) {
    // using elements to draw the image based on it
    DrawTexturePro(
        background, 
        sourceRect, 
        destRect, 
        origin, 
        rotation, 
        tint
    );
}

int main() {
    // Initialization game variables

    // width and height for game screen
    const int screenWidth = 800;
    const int screenHeight = 600;

    // size of each cell along with Number of cells in horizontal and vertical
    const int cellSize = 33;    
    const int widthCells = 18;   
    const int heightCells = 15;  

    // The total width and height of the board
    int rectWidth = widthCells * cellSize;   
    int rectHeight = heightCells * cellSize;  

    // The position of x and y axis of board
    int rectX = (screenWidth - rectWidth) / 2;
    int rectY = (screenHeight - rectHeight) / 2 + 20;  

    // Vertical offset to move the start button down
    int verticalOffset = 150;  

    // size and position of game over window
    int boxWidth = 400;
    int boxHeight = 250;
    int boxX = (screenWidth - boxWidth) / 2;
    int boxY = (screenHeight - boxHeight) / 2;

    // restart and return menu buttons size and gap
    int buttonWidth = 270;
    int buttonHeight = 50;           
    int gap = -60;    

    // Calculate the total width of both buttons and the gap
    int totalButtonWidth = (buttonWidth * 2) + gap;

    // Calculate the X positions to center the buttons within the box
    int playAgainX = boxX + (boxWidth - totalButtonWidth) + 200 / 2;  // Center the buttons horizontally
    int backToMenuX = playAgainX + buttonWidth + gap;           // Position second button next to the first with a gap

    // Vertically center buttons within the box
    int buttonY = boxY + (boxHeight - buttonHeight) / 2 + 50;

    bool addSegment = false;   //  Flag track whether a segment should be added 
    bool gameRunning = false;   // Flag  if the game is currently running
    bool gameOver = false;     // Flag indicating if the game is over
    bool option_flag = false;     // Flag indicating if the game is over

    double lastTime = 0;       // Stores game updates to control elements based on game frame

    // Set vertical offsets for each button (to move them down)
    float snakeTileBtn_offset = 150;  
    float GreenSnake_btn = 30;   
    float BlackSnake_btn = 70;   
    float backButton_btn = 170; 

    // snake color
    Color snakeColor = DARKGREEN;
    // Define a color
    Color color = {255, 140, 0, 255};

    // Declare struct
    Snake mySnake;
    Food food;
    
    // Declare buttons
    Button start;
    Button options;
    Button playAgain;
    Button backToMenu;

    // Options menu buttons
    Button snakeColorButton;
    Button GreenSnake;
    Button BlackSnake;
    Button backButton;

    // Initialize game window and audio device for sound effects
    InitWindow(screenWidth, screenHeight, "Snake game");
    InitAudioDevice(); // Initialize audio device
    
    // Load sound effects
    Sound eatSound = LoadSound("resources/eat.wav"); // Sound when snake eats food
    Music backgroundMusic = LoadMusicStream("resources/background.mp3"); // Background music
    
    // Set background music volume and loop it
    SetMusicVolume(backgroundMusic, 0.5f);
    PlayMusicStream(backgroundMusic);

    // define images 
    Texture2D background = LoadTexture("resources/snake2.png");
    

    // Define a random position for food
    food.position = GetRandomFoodPosition(widthCells, heightCells, &mySnake);
    
    // game frames
    SetTargetFPS(60);
    
    // Reset the snake when the game restarts
    ResetSnake(&mySnake); 

    // Declare a function to draw images, such as the background
    void DrawItems(Texture2D background, Rectangle sourceRect, Rectangle destRect, Vector2 origin, float rotation, Color tint);


    // Initialize the buttons
    init_button(&playAgain, (Rectangle){
        playAgainX,     // X position
        buttonY,        // Y position
        buttonWidth - 120,    // Button width
        buttonHeight    // Button height
    }, WHITE, 20.0f, WHITE, 2.5f);

    init_button(&backToMenu, (Rectangle){
        backToMenuX,    // X position
        buttonY,        // Y position 
        buttonWidth - 120,    // Button width
        buttonHeight    // Button height
    }, WHITE, 20.0f, WHITE, 2.5f);

    init_button(&start, (Rectangle){
        screenWidth / 2 - buttonWidth / 2,         // Center horizontally
        screenHeight / 2 - 70 / 2 + verticalOffset - 45, // Move up vertically by reducing verticalOffset by 20
        buttonWidth,                               // Button width
        70                                         // Button height
    }, WHITE, 20.0f, WHITE, 2.5f);
    
    init_button(&options, (Rectangle){
        screenWidth / 2 - buttonWidth / 2,         // Center horizontally
        screenHeight / 2 - 70 / 2 + verticalOffset + 100 - 45, // Move up vertically by reducing verticalOffset + 100 by 20
        buttonWidth,                               // Button width
        70                                         // Button height
    }, WHITE, 20.0f, WHITE, 2.5f);
    

    // Initialize options menu buttons
    init_button(&snakeColorButton, (Rectangle){
        screenWidth / 2 - buttonWidth / 2,         // Center horizontally
        screenHeight / 2 - 70 / 2 - snakeTileBtn_offset, // Use verticalOffset1
        buttonWidth,                               // Button width
        70                                         // Button height
    }, WHITE, 20.0f, WHITE, 2.5f);

    init_button(&GreenSnake, (Rectangle){
        screenWidth / 2 - buttonWidth / 2,         // Center horizontally
        screenHeight / 2 - 70 / 2 - GreenSnake_btn, // Use verticalOffset2
        buttonWidth,                               // Button width
        70                                         // Button height
    }, WHITE, 20.0f, WHITE, 2.5f);

    init_button(&BlackSnake, (Rectangle){
        screenWidth / 2 - buttonWidth / 2,         // Center horizontally
        screenHeight / 2 - 70 / 2 + BlackSnake_btn, // Use verticalOffset3
        buttonWidth,                               // Button width
        70                                         // Button height
    }, WHITE, 20.0f, WHITE, 2.5f);

    init_button(&backButton, (Rectangle){
        screenWidth / 2 - buttonWidth / 2,         // Center horizontally
        screenHeight / 2 - 70 / 2 + backButton_btn, // Use verticalOffset4
        buttonWidth,                               // Button width
        70                                         // Button height
    }, WHITE, 20.0f, WHITE, 2.5f);

    // Create a ButtonContext instance
    ButtonContext context = {
        .option_flag = &option_flag,
        .gameRunning = &gameRunning,
        .gameOver = &gameOver,
        .mySnake = &mySnake,
        .food = &food,
        .widthCells = widthCells,
        .heightCells = heightCells,
        .backToMenu = &backToMenu,
        .option = &options,
        .backButton = &backButton,
        .start = &start,
        .playAgain = &playAgain,
        .GreenSnake = &GreenSnake,
        .BlackSnake = &BlackSnake,
        .snakeColorButton = &snakeColorButton,
        .snakeColor = &snakeColor
    };
    
    // Game starter
    while (!WindowShouldClose()) {
        // Update background music stream
        UpdateMusicStream(backgroundMusic);
        
        // Check if the game is not running
        if (option_flag) {
            // Options menu
            BeginDrawing();
            ClearBackground(DARKGREEN);

            // Draw options menu buttons
            DrawButtonWithLabel(&snakeColorButton, "Snake Color");
            DrawButtonWithLabel(&GreenSnake, "Green");
            DrawButtonWithLabel(&BlackSnake, "Black");
            DrawButtonWithLabel(&backButton, "Back");

            // Process button press events and handle corresponding actions
            HandleButtonMouseEvent(&snakeColorButton, &context);
            HandleButtonMouseEvent(&GreenSnake, &context);
            HandleButtonMouseEvent(&BlackSnake, &context);
            HandleButtonMouseEvent(&backButton, &context);

            EndDrawing();
        }
        else if (!gameRunning) {
            DrawItems(
                background,
                (Rectangle){0, 0, background.width, background.height},
                (Rectangle){0, 0, GetScreenWidth(), GetScreenHeight()},
                (Vector2){0, 0},
                0.0f,
                WHITE
            );
        
            // Handle mouse interactions
            HandleButtonMouseEvent(&start, &context);
            HandleButtonMouseEvent(&options, &context);
        
            // Draw buttons using the reusable function
            DrawButtonWithLabel(&start, "Play");
            DrawButtonWithLabel(&options, "Options");
            
            EndDrawing();
        }
        // Check if the game is running
        else if (gameRunning) {
            // Check if the event is triggered to move the snake by call the function
            snake_rules(&mySnake, &food, &gameOver, &lastTime, &addSegment, widthCells, heightCells, eatSound);

            // Draw the game
            BeginDrawing();
            ClearBackground(DARKGREEN);

            // Draw the board's outer border
            DrawRectangleRounded((Rectangle){rectX - 2, rectY - 2, rectWidth + 4, rectHeight + 4}, 0.0f, 20, BLACK);
            DrawRectangleRounded((Rectangle){rectX, rectY, rectWidth, rectHeight}, 0.0f, 20, color);
            
            // function draw the board pattern
            board_pattern(rectY, rectX, rectHeight, rectWidth, cellSize, color);

            // functions check if snake collides with itself or the wall
            checkwithWalls(&mySnake, widthCells, heightCells, &gameOver);
    

            // functions draw the snake and its food
            DrawFood(&food, cellSize, rectX, rectY, widthCells, heightCells, &mySnake);
            DrawSnake(&mySnake, cellSize, rectX, rectY, snakeColor);

            // Draw the score based on the snake's length subtracted from 2
            DrawText(TextFormat("%d", mySnake.length - 2), rectX + 580, rectY - 45, 27, WHITE);
            DrawText(TextFormat("Snake Game" , mySnake.length - 2), rectX , rectY - 45, 27, WHITE);
            
            // function handle game over 
            gameEnd(boxX, boxY, boxWidth, boxHeight, &gameOver, &gameRunning, &mySnake, &food, widthCells, heightCells, &backToMenu, &playAgain);
            
            // End drawing the current frame
            EndDrawing();
        }
    }
    
    // Unload resources and close the game window
    UnloadTexture(background);
    UnloadSound(eatSound);
    UnloadMusicStream(backgroundMusic);
    CloseAudioDevice();
    CloseWindow();
    
    return 0;
}