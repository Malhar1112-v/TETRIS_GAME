# **TETRIS GAME**  
A classic Tetris implementation in C++ with terminal-based graphics. Features falling tetrominoes, line clearing, score tracking, and high-score saving.

---

## **Table of Contents**  
1. [Features](#features)  
2. [Game Controls](#game-controls)  
3. [Implementation Details](#implementation-details)  
4. [Functions Overview](#functions-overview)  
5. [Compilation & Execution](#compilation--execution)  

---

## **Features**  
**Classic Tetris Mechanics** – Rotate and move falling blocks to clear lines.  
**Next Piece Preview** – See the upcoming tetromino for better planning.  
**Score & High Score System** – Earn points and compete against your best.  
**Game Over Detection** – Ends when blocks reach the top.  
**Restart Option** – Play again after losing.  

---

## **Game Controls**  
| Key | Action |  
|-----|--------|  
| **A** | Move Left |  
| **D** | Move Right |  
| **S** | Soft Drop (Move Down Faster) |  
| **W** | Rotate Piece |  
| **Space** | Hard Drop (Instant Drop) |  
| **Z** | Quit / Restart |  
| **R** | Restart (After Game Over) |  
| **Q** | Quit (After Game Over) |  

---

## **Implementation Details**  

### **1. Game Board Structure**  
- A 2D grid (`BOARD_WIDTH × BOARD_HEIGHT`) tracks placed blocks.  
- Each cell is `true` (occupied) or `false` (empty).  

### **2. Tetromino Shapes**  
- Seven classic tetrominoes (`I, J, L, O, S, T, Z`) stored as `vector<vector<bool>>`.  
- **Rotation** is done by matrix transposition.  

### **3. Movement & Collision**  
- **Movement checks** prevent out-of-bounds or overlapping blocks.  
- **Hard Drop** instantly places the piece at the lowest possible position.  

### **4. Line Clearing & Scoring**  
- Full rows are cleared, and rows above shift down.  
- **Scoring:**  
  - 1 line = 100 points  
  - 2 lines = 200 points  
  - 3 lines = 300 points  
  - 4 lines (Tetris) = 800 points  

### **5. Game Loop**  
1. Spawn a new tetromino.
2. Check for collisions.
3. Process player input. 
4. Auto-drop the piece every 500ms.
5. Check for completed lines.
6. Game over if new piece collides immediately.

---

## **Functions Overview**  

### **1. Tetromino Class**  
| Function | Description |  
|----------|-------------|  
| `getShapeForType()` | Returns the shape matrix for a given tetromino type. |  
| `initialize()` | Sets a new tetromino at the top center of the board. |  

### **2. TetrisGame Class**  
| Function | Description |  
|----------|-------------|  
| `resetGame()` | Clears the board and resets score (but keeps high score). |  
| `getRandomTetrominoType()` | Returns a random tetromino type. |  
| `spawnNewPiece()` | Moves the "next" piece to current and generates a new one. |  
| `isCollision()` | Checks if the current piece overlaps with walls or placed blocks. |  
| `mergePieceToBoard()` | Locks the current piece into the board. |  
| `clearLines()` | Removes completed lines and updates score. |  
| `rotatePiece()` | Rotates the tetromino (if no collision). |  
| `hardDrop()` | Instantly drops the piece to the bottom. |  
| `drawBoard()` | Renders the game board, score, and next piece preview. |  
| `gameLoop()` | Main game loop handling input, movement, and updates. |  
| `showGameOverScreen()` | Displays final score and restart options. |  
| `run()` | Starts the game loop and manages game states. |  

---

## **Compilation & Execution**  

### **Prerequisites**  
- **Windows OS** (for `conio.h` and `windows.h`)  
- **G++** (or another C++ compiler)  

### **Steps**  
- Download code file from github repository.
- **Compile:**  
   ~~~bash
   g++ tetris.cpp -o tetris_game
   ~~~
- **Run:**  
   ~~~bash
   ./tetris_game
   ~~~

### **For Linux/Mac Users**  
- Replace `conio.h` with `ncurses` or similar libraries.  
- Modify `Sleep()` to `usleep()` for compatibility.  

---

### **Enjoy the Game!** 🎮  
Challenge yourself to beat your high score and master Tetris!
