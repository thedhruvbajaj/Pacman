# 🕹️ Pac-Man in C++ (OpenGL)  

A custom implementation of the **classic Pac-Man game** built using **C++ and OpenGL**. This project showcases game development concepts such as **pathfinding, rendering, and collision detection**, along with efficient use of **data structures and algorithms**.  

## 🚀 Features  
- 🎮 **Smooth Player Controls** – Move Pac-Man in all four directions.  
- 👻 **AI-Controlled Ghosts** – Ghosts follow Pac-Man using a **greedy algorithm**.  
- 🍒 **Collectible Pellets & Power-ups** – Eat pellets to score points.  
- 🔥 **Dynamic Rendering** – Graphics rendered using **OpenGL**.  

## 🛠️ Technologies Used  
- **C++** – Core game logic and data structures.  
- **OpenGL** – Rendering and graphics.  
- **GLUT** – Window management and user input.  
- **Data Structures & Algorithms** – Used for pathfinding and game mechanics.  

## 🎮 How to Play  
1. **Run the game** (build instructions below).  
2. **Use arrow keys** to navigate Pac-Man.  
3. **Avoid ghosts** and collect all pellets.  

## 🔧 Installation & Compilation  
### **Requirements**  
- C++ Compiler (g++)  
- OpenGL & GLUT libraries  

### **Build & Run**  
```sh
g++ pacman.cpp -o pacman -lGL -lGLU -lglut  
./pacman  
