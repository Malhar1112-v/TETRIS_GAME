#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>
#include <conio.h>
#include <windows.h>

using namespace std;

enum class TetrominoType { I, J, L, O, S, T, Z };

class Tetromino {
public:
    TetrominoType type;
    int x, y;
    vector<vector<bool>> shape;

    static vector<vector<bool>> getShapeForType(TetrominoType type) {
        switch (type) {
            case TetrominoType::I: return {{true, true, true, true}};
            case TetrominoType::J: return {{true, false, false}, {true, true, true}};
            case TetrominoType::L: return {{false, false, true}, {true, true, true}};
            case TetrominoType::O: return {{true, true}, {true, true}};
            case TetrominoType::S: return {{false, true, true}, {true, true, false}};
            case TetrominoType::T: return {{false, true, false}, {true, true, true}};
            case TetrominoType::Z: return {{true, true, false}, {false, true, true}};
            default: return {};
        }
    }

    void initialize(TetrominoType newType, int boardWidth) {
        type = newType;
        shape = getShapeForType(type);
        x = boardWidth / 2 - shape[0].size() / 2;
        y = 0;
    }
};

class TetrisGame {
private:
    const int BOARD_WIDTH = 10;
    const int BOARD_HEIGHT = 20;
    const int NEXT_PIECE_DISPLAY_WIDTH = 6;
    const int NEXT_PIECE_DISPLAY_HEIGHT = 4;
    int highscore = 0;
    vector<vector<bool>> board;
    Tetromino currentPiece;
    Tetromino nextPiece;
    int score;

public:
    TetrisGame() : board(BOARD_HEIGHT, vector<bool>(BOARD_WIDTH, false)), score(0) {
        srand(static_cast<unsigned>(time(nullptr)));
    }

    void resetGame() {
        board = vector<vector<bool>>(BOARD_HEIGHT, vector<bool>(BOARD_WIDTH, false));
        if (score>highscore) 
        {
            highscore=score;
        }
        score=0;
        nextPiece.initialize(getRandomTetrominoType(), BOARD_WIDTH);
        spawnNewPiece();
    }

    TetrominoType getRandomTetrominoType() {
        return static_cast<TetrominoType>(rand() % 7);
    }

    void spawnNewPiece() {
        currentPiece = nextPiece;
        nextPiece.initialize(getRandomTetrominoType(), BOARD_WIDTH);
    }

    bool isCollision(const Tetromino& piece) {
        for (size_t y = 0; y < piece.shape.size(); ++y) {
            for (size_t x = 0; x < piece.shape[y].size(); ++x) {
                if (piece.shape[y][x]) {
                    int boardX = piece.x + x;
                    int boardY = piece.y + y;
                    if (boardX < 0 || boardX >= BOARD_WIDTH || boardY >= BOARD_HEIGHT || 
                        (boardY >= 0 && board[boardY][boardX])) {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    void mergePieceToBoard() {
        for (size_t y = 0; y < currentPiece.shape.size(); ++y) {
            for (size_t x = 0; x < currentPiece.shape[y].size(); ++x) {
                if (currentPiece.shape[y][x]) {
                    board[currentPiece.y + y][currentPiece.x + x] = true;
                }
            }
        }
    }

    void clearLines() {
        int linesCleared = 0;
        for (int y = BOARD_HEIGHT - 1; y >= 0; --y) {
            bool lineFull = true;
            for (int x = 0; x < BOARD_WIDTH; ++x) {
                if (!board[y][x]) {
                    lineFull = false;
                    break;
                }
            }
            if (lineFull) {
                ++linesCleared;
                for (int yy = y; yy > 0; --yy) {
                    board[yy] = board[yy - 1];  
                }
                board[0] = vector<bool>(BOARD_WIDTH, false);  
                ++y; 
            }
        }
        score += linesCleared * 100;
    }

    void rotatePiece() {
        vector<vector<bool>> rotated(currentPiece.shape[0].size(), 
            vector<bool>(currentPiece.shape.size()));

        for (size_t y = 0; y < currentPiece.shape.size(); ++y) {
            for (size_t x = 0; x < currentPiece.shape[y].size(); ++x) {
                rotated[x][currentPiece.shape.size() - 1 - y] = currentPiece.shape[y][x];
            }
        }

        auto originalShape = currentPiece.shape;
        currentPiece.shape = rotated;
        if (isCollision(currentPiece)) {
            currentPiece.shape = originalShape;
        }
    }

    void hardDrop() {
        while (true) {
            ++currentPiece.y;
            if (isCollision(currentPiece)) {
                --currentPiece.y;
                mergePieceToBoard();
                clearLines();
                spawnNewPiece();
                break;
            }
        }
    }

    void drawBoard() {
        cout << "\033[H";  
        cout << "=== TETRIS ===  " << "\n" << "Score: " << score << " || High Score: " << highscore <<"\n";
        cout << "Controls: W-Rotate, A-Left, S-Down, D-Right, Space-Hard Drop, Z-Quit or Restart\n\n";

        for (int y = 0; y < max(BOARD_HEIGHT, NEXT_PIECE_DISPLAY_HEIGHT + 2); ++y) {
            if (y < BOARD_HEIGHT) {
                cout << "|";
                for (int x = 0; x < BOARD_WIDTH; ++x) {
                    bool isCurrent = false;
                    for (size_t pieceY = 0; pieceY < currentPiece.shape.size(); ++pieceY) {
                        for (size_t pieceX = 0; pieceX < currentPiece.shape[pieceY].size(); ++pieceX) {
                            if (currentPiece.shape[pieceY][pieceX] && 
                                x == currentPiece.x + pieceX && 
                                y == currentPiece.y + pieceY) {
                                isCurrent = true;
                            }
                        }
                    }
                    cout << (isCurrent ? "[]" : (board[y][x] ? "[]" : "  "));
                }
                cout << "|";
            } else {
                cout << string(BOARD_WIDTH * 2 + 2, ' ');
            }

            if (y == 0) {
                cout << "  Next:";
            } else if (y <= NEXT_PIECE_DISPLAY_HEIGHT) {
                cout << "  ";
                for (int x = 0; x < NEXT_PIECE_DISPLAY_WIDTH; ++x) {
                    bool isPiece = false;
                    if (y - 1 < nextPiece.shape.size() && x < nextPiece.shape[0].size()) {
                        isPiece = nextPiece.shape[y - 1][x];
                    }
                    cout << (isPiece ? "[]" : "  ");
                }
            }
            cout << "\n";
        }
        cout << " " << string(BOARD_WIDTH * 2, '-') << "\n";
    }

    void gameLoop() {
        auto lastFallTime = chrono::steady_clock::now();
        
        while (true) {
            drawBoard();

            if (_kbhit()) {
                char key = _getch();
                switch (key) {
                    case 'a': --currentPiece.x; if (isCollision(currentPiece)) ++currentPiece.x; break;
                    case 'd': ++currentPiece.x; if (isCollision(currentPiece)) --currentPiece.x; break;
                    case 's':
                        ++currentPiece.y;
                        if (isCollision(currentPiece)) {
                            --currentPiece.y;
                            mergePieceToBoard();
                            clearLines();
                            spawnNewPiece();
                            if (isCollision(currentPiece)) {
                                return; // Game over
                            }
                        }
                        break;
                    case 'w': rotatePiece(); break;
                    case ' ': hardDrop(); break;
                    case 'z': return;
                }
            }

            auto currentTime = chrono::steady_clock::now();
            if (chrono::duration_cast<chrono::milliseconds>(currentTime - lastFallTime).count() > 500) {
                ++currentPiece.y;
                if (isCollision(currentPiece)) {
                    --currentPiece.y;
                    mergePieceToBoard();
                    clearLines();
                    spawnNewPiece();
                    if (isCollision(currentPiece)) {
                        return; // Game over
                    }
                }
                lastFallTime = currentTime;
            }

            Sleep(50);
        }
    }

    void showGameOverScreen() {
        system("cls");
        cout << "GAME OVER!\n";
        cout << "Final Score: " << score << "\n";
        cout << "Press R to restart or Q to quit\n";
        
        while (true) {
            if (_kbhit()) {
                char key = tolower(_getch());
                if (key == 'r') {
                    break;
                } else if (key == 'q') {
                    exit(0);
                }
            }
            Sleep(100);
        }
    }

    void run() {
        while (true) {
            resetGame();
            gameLoop();
            showGameOverScreen();
        }
    }
};

int main() {
    TetrisGame game;
    game.run();
    return 0;
}