#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>

const int WIDTH = 800;
const int HEIGHT = 600;
const int TILE_SIZE = 40;
const int BOARD_WIDTH = WIDTH / TILE_SIZE;
const int BOARD_HEIGHT = HEIGHT / TILE_SIZE;

// Directions
enum Direction { UP, DOWN, LEFT, RIGHT };

// Game entities
struct Entity {
    float x, y;
    float speed; // Speed for smooth movement
    Direction dir;
};

// Game state
Entity pacman;
Entity ghosts[4];
int score = 0;
int lives = 3;
bool board[BOARD_WIDTH][BOARD_HEIGHT];
bool pellets[BOARD_WIDTH][BOARD_HEIGHT];

// Function to initialize the game
void initGame() {
    pacman.x = TILE_SIZE + TILE_SIZE / 2; // Start position (centered in tile)
    pacman.y = TILE_SIZE + TILE_SIZE / 2;
    pacman.speed = 3.0f; // Increased speed for Pac-Man
    pacman.dir = RIGHT;

    // Initialize ghosts with different speeds and initial positions
    for (int i = 0; i < 4; ++i) {
        ghosts[i] = { (TILE_SIZE * (5 + i)), (TILE_SIZE * 5), 2.0f, LEFT }; // Ghosts speed
    }

    // Initialize the board with walls and paths
    for (int x = 0; x < BOARD_WIDTH; ++x) {
        for (int y = 0; y < BOARD_HEIGHT; ++y) {
            if ((x == 0 || x == BOARD_WIDTH - 1 || y == 0 || y == BOARD_HEIGHT - 1) ||
                ((x % 2 == 0) && (y % 2 == 0))) {
                board[x][y] = true; // Wall
            } else {
                board[x][y] = false; // Path
                pellets[x][y] = true; // Pellet
            }
        }
    }
}

// Function to draw a circle at given coordinates
void drawCircle(float x, float y, float radius, float r, float g, float b) {
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(r, g, b);
    glVertex2f(x, y); // Center of circle
    for (int i = 0; i <= 360; i++) {
        float angle = i * (3.14159f / 180);
        glVertex2f(x + cos(angle) * radius, y + sin(angle) * radius);
    }
    glEnd();
}

// Function to draw the maze
void drawMaze() {
    glColor3f(0.0f, 0.0f, 1.0f); // Blue color for walls
    glBegin(GL_LINES);
    for (int x = 0; x < BOARD_WIDTH; ++x) {
        for (int y = 0; y < BOARD_HEIGHT; ++y) {
            if (board[x][y]) {
                glVertex2f(x * TILE_SIZE, y * TILE_SIZE);
                glVertex2f(x * TILE_SIZE + TILE_SIZE, y * TILE_SIZE);
                glVertex2f(x * TILE_SIZE + TILE_SIZE, y * TILE_SIZE);
                glVertex2f(x * TILE_SIZE + TILE_SIZE, y * TILE_SIZE + TILE_SIZE);
                glVertex2f(x * TILE_SIZE + TILE_SIZE, y * TILE_SIZE + TILE_SIZE);
                glVertex2f(x * TILE_SIZE, y * TILE_SIZE + TILE_SIZE);
                glVertex2f(x * TILE_SIZE, y * TILE_SIZE + TILE_SIZE);
                glVertex2f(x * TILE_SIZE, y * TILE_SIZE);
            }
        }
    }
    glEnd();
}

// Function to draw the game entities
void drawEntities() {
    // Draw Pac-Man
    drawCircle(pacman.x, pacman.y, TILE_SIZE / 3.0f, 1.0f, 1.0f, 0.0f); // Yellow

    // Draw Ghosts
    for (int i = 0; i < 4; ++i) {
        drawCircle(ghosts[i].x, ghosts[i].y, TILE_SIZE / 3.0f,
                   i % 2 == 0 ? 1.0f : .5f,
                   i % 2 == 0 ? .5f : .75,
                   i % 2 == 0 ? .75 : .5); // Different colors for ghosts
    }

    // Draw Pellets
    for (int x = 0; x < BOARD_WIDTH; ++x) {
        for (int y = 0; y < BOARD_HEIGHT; ++y) {
            if (pellets[x][y]) {
                drawCircle(x * TILE_SIZE + TILE_SIZE / 2.0f,
                            y * TILE_SIZE + TILE_SIZE / 2.0f,
                            TILE_SIZE / 8.0f,
                            1.0f,
                            1.0f,
                            1.0f); // White pellet
            }
        }
    }
}

// Function to check if a position is valid for movement
bool canMove(float newX, float newY) {
    int gridX = static_cast<int>(newX / TILE_SIZE);
    int gridY = static_cast<int>(newY / TILE_SIZE);

    return !board[gridX][gridY]; // Return true if not a wall
}

// Function to update positions of entities with smooth movement and edge collision detection
void updateEntities() {
    float moveSpeedPacman = pacman.speed;

    // Move Pac-Man based on direction with smooth movement and edge collision detection
    float newPacmanX = pacman.x;
    float newPacmanY = pacman.y;

    if (pacman.dir == UP) newPacmanY -= moveSpeedPacman;
    else if (pacman.dir == DOWN) newPacmanY += moveSpeedPacman;
    else if (pacman.dir == LEFT) newPacmanX -= moveSpeedPacman;
    else if (pacman.dir == RIGHT) newPacmanX += moveSpeedPacman;

   // Check if Pac-Man can move to the new position
   if (canMove(newPacmanX, newPacmanY)) {
       pacman.x = newPacmanX;
       pacman.y = newPacmanY;
   }

   // Check pellet collection
   int px = static_cast<int>(pacman.x / TILE_SIZE);
   int py = static_cast<int>(pacman.y / TILE_SIZE);
    
   if (pellets[px][py]) {
       pellets[px][py] = false; // Remove pellet from board
       score += 10; // Increase score
       std::cout << "Score: " << score << std::endl; // Display score in console.
   }

   // Move Ghosts towards Pac-Man using greedy algorithm with smooth movement and edge collision detection.
   for (int i = 0; i < 4; ++i) {
       float ghostXPrev = ghosts[i].x;
       float ghostYPrev = ghosts[i].y;

       // Calculate direction towards Pac-Man with smooth movement and wall collision detection.
       if (ghosts[i].x < pacman.x && canMove(ghosts[i].x + ghosts[i].speed , ghosts[i].y)) {
           ghosts[i].x += ghosts[i].speed;
       } else if (ghosts[i].x > pacman.x && canMove(ghosts[i].x - ghosts[i].speed , ghosts[i].y)) {
           ghosts[i].x -= ghosts[i].speed;
       }

       if (ghosts[i].y < pacman.y && canMove(ghosts[i].x , ghosts[i].y + ghosts[i].speed)) {
           ghosts[i].y += ghosts[i].speed;
       } else if (ghosts[i].y > pacman.y && canMove(ghosts[i].x , ghosts[i].y - ghosts[i].speed)) {
           ghosts[i].y -= ghosts[i].speed;
       }

       // Check collision with Pac-Man after moving.
       if (fabs(pacman.x - ghosts[i].x) < (TILE_SIZE /3.0 ) && fabs(pacman.y - ghosts->y) < (TILE_SIZE /3.0 )) {
           lives--;
           std::cout << "Lives left: " << lives << std::endl;

           if(lives <= 0) { 
              std::cout << "Game Over! Final Score: " << score << std::endl ;
              glfwSetWindowShouldClose(glfwGetCurrentContext(), GLFW_TRUE); 
           } else{
              pacman.x= TILE_SIZE + TILE_SIZE/2 ; 
              pacman.y= TILE_SIZE+ TILE_SIZE/2 ; 
           }
       }
   }
}
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
   if(action == GLFW_PRESS || action == GLFW_REPEAT){
       if(key == GLFW_KEY_UP){
           pacman.dir= UP ;
       } else if(key== GLFW_KEY_DOWN){
           pacman.dir= DOWN ;
       } else if(key== GLFW_KEY_LEFT){
           pacman.dir= LEFT ;
       } else if(key== GLFW_KEY_RIGHT){
           pacman.dir= RIGHT ;
       }
   }
}

int main() {
   srand(static_cast<unsigned int>(time(nullptr)));

   // Initialize GLFW
   if (!glfwInit()) {
       std::cerr << "Failed to initialize GLFW" << std::endl;
       return -1;
   }

   // Create a windowed mode window and its OpenGL context
   GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT,"Pac-Man Game", NULL,NULL);
   if (!window) {
       glfwTerminate();
       std::cerr << "Failed to create GLFW window" << std::endl ; 
       return -1 ;
   }

   glfwMakeContextCurrent(window);

   glewExperimental= GL_TRUE ; 
   GLenum err= glewInit();

   if(err != GLEW_OK){
      std::cerr << "Failed to initialize GLEW: " << glewGetErrorString(err)<<std::endl ;
      return-1 ; 
   }

   glfwSetKeyCallback(window,keyCallback);

   initGame();

   while(!glfwWindowShouldClose(window)){
      glClear(GL_COLOR_BUFFER_BIT);

      glViewport(0 ,0 ,WIDTH ,HEIGHT );
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      glOrtho(0 ,WIDTH ,HEIGHT ,0 ,-1 ,1 ); 

      updateEntities();

      drawMaze(); 
      drawEntities();

      glfwSwapBuffers(window);

      glfwPollEvents();

      std::this_thread::sleep_for(std::chrono::milliseconds(50)); 
   }

   glfwDestroyWindow(window);
   glfwTerminate();

return-1 ; 
}