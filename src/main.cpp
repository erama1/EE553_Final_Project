#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <vector>
#include <ctime>

using namespace std;

// Constants
const int SCREEN_WIDTH = 865;
const int SCREEN_HEIGHT = 675;
const int BLOCK_SIZE = 32;
const int NUM_BLOCKS_X = SCREEN_WIDTH / BLOCK_SIZE;
const int NUM_BLOCKS_Y = SCREEN_HEIGHT / BLOCK_SIZE;

// Enumeration to represent game objects
enum GameObjectType {
    GameObjectType_None,
    GameObjectType_Pacman,
    GameObjectType_Ghost,
    GameObjectType_Dot,
    GameObjectType_Powerup
};

// Forward declarations
class GameObject;
class Pacman;
class Ghost;
class Dot;
class Powerup;
class Game;

// Class to represent game objects
class GameObject {
public:
    int x, y;
    bool alive;
    bool wall;
    GameObjectType type;

    GameObject(int x, int y, GameObjectType type = GameObjectType_None, bool wall = false) {
        this->x = x;
        this->y = y;
        this->alive = true;
        this->wall = wall;
        this->type = type;
    }

    virtual void update(Game* game) {};

    virtual ~GameObject() {}

};


// Pacman class to represent the player-controlled Pacman object
class Pacman : public GameObject {
public:
    Pacman(int x_, int y_) : GameObject(x_, y_, GameObjectType_Pacman) {}

    void move(int dx, int dy) {
        x += dx;
        y += dy;
    }

    virtual ~Pacman() {}

};

// Ghost class to represent the enemy Ghost objects
class Ghost : public GameObject {
public:
    int dx, dy;
    int counter;
    int changeDirectionCounter;  // Add this line

    Ghost(int x_, int y_) : GameObject(x_, y_, GameObjectType_Ghost) {
        dx = 1;
        dy = 1;
        counter = 0;  // Initialize counter to 0
        changeDirectionCounter = 0;  // Initialize changeDirectionCounter to 0
    }

    void update(Game* game) override;

    virtual ~Ghost() {}

};


// Dot class to represent the small dots that Pacman eats
class Dot : public GameObject {
public:
    Dot(int x_, int y_) : GameObject(x_, y_, GameObjectType_Dot) {}

    virtual ~Dot() {}

};

// Powerup class to represent the powerup items that Pacman eats
class Powerup : public GameObject {
public:
    Powerup(int x_, int y_) : GameObject(x_, y_, GameObjectType_Powerup) {}

    virtual ~Powerup() {}

};

// Game class to manage game data and logic
class Game {
public:
    // SDL variables
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture_pacman;
    SDL_Texture* texture_ghost;
    SDL_Texture* texture_dot;
    SDL_Texture* texture_powerup;
    SDL_Texture* texture_wall;

    // Game variables
    Pacman* pacman;
    vector<Ghost*> ghosts;
    vector<Dot*> dots;
    vector<Powerup*> powerups;
    vector<GameObject*> walls;
    int ghostSpeed;

    Game() {
        // Initialize SDL
        SDL_Init(SDL_INIT_VIDEO);

        // Initialize ghost speed
        ghostSpeed = 1;

        // Create window and renderer
        window = SDL_CreateWindow("Pacman", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
        renderer = SDL_CreateRenderer(window, -1, 0);

        // Load textures
        
        SDL_Surface* surface_pacman = IMG_Load("src/pacman.png");
        SDL_Surface* surface_ghost = IMG_Load("src/ghost.png");
        SDL_Surface* surface_dot = IMG_Load("src/dot.png");
        SDL_Surface* surface_powerup = IMG_Load("src/powerup.png");
        SDL_Surface* surface_wall = IMG_Load("src/wall.png");
        texture_pacman = SDL_CreateTextureFromSurface(renderer, surface_pacman);
        texture_ghost = SDL_CreateTextureFromSurface(renderer, surface_ghost);
        texture_dot = SDL_CreateTextureFromSurface(renderer, surface_dot);
        texture_powerup = SDL_CreateTextureFromSurface(renderer, surface_powerup);
        texture_wall = SDL_CreateTextureFromSurface(renderer, surface_wall);
        SDL_FreeSurface(surface_pacman);
        SDL_FreeSurface(surface_ghost);
        SDL_FreeSurface(surface_dot);
        SDL_FreeSurface(surface_powerup);
        SDL_FreeSurface(surface_wall);

        pacman = new Pacman(0, 0);

        // Create game objects
        createMaze();
    }

    void update() {
    // Handle events
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            // Quit the game
            exit(0);
            break;
        case SDL_KEYDOWN:
            // Handle key presses
            int newX, newY;
            switch (event.key.keysym.sym) {
            case SDLK_UP:
                newX = pacman->x;
                newY = pacman->y - BLOCK_SIZE;
                break;
            case SDLK_DOWN:
                newX = pacman->x;
                newY = pacman->y + BLOCK_SIZE;
                break;
            case SDLK_LEFT:
                newX = pacman->x - BLOCK_SIZE;
                newY = pacman->y;
                break;
            case SDLK_RIGHT:
                newX = pacman->x + BLOCK_SIZE;
                newY = pacman->y;
                break;
            default:
                newX = pacman->x;
                newY = pacman->y;
                break;
            }

            // Check if move is valid
            bool canMove = true;
            for (auto wall : walls) {
                if (newX == wall->x && newY == wall->y) {
                    canMove = false;
                    break;
                }
            }

            if (canMove) {
                pacman->move(newX - pacman->x, newY - pacman->y);

                // Check collisions with dots and powerups
                for (auto dot : dots) {
                    if (pacman->x == dot->x && pacman->y == dot->y) {
                        dot->alive = false;
                    }
                }

                for (auto powerup : powerups) {
                    if (pacman->x == powerup->x && pacman->y == powerup->y) {
                        powerup->alive = false;
                    }
                }
            }
            break;
        }
    }

    for (int i = 0; i < ghosts.size(); i++) {
    ghosts[i]->update(this);
}

    // ... (rest of the update function)
        // Clear the screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw game objects
        drawGameObject(pacman, texture_pacman);

        for (int i = 0; i < dots.size(); i++) {
            drawGameObject(dots[i], texture_dot);
        }
        for (int i = 0; i < powerups.size(); i++) {
            drawGameObject(powerups[i], texture_powerup);
        }

        for (int i = 0; i < ghosts.size(); i++) {
            drawGameObject(ghosts[i], texture_ghost);
        }
        for (int i = 0; i < walls.size(); i++) {
            drawGameObject(walls[i], texture_wall);
        }

        // Update the screen
        SDL_RenderPresent(renderer);
    }
    
    void createMaze() {
    const char maze[21][28] = {
        "###########################",
        "#o..........P#...........o#",
        "#.####.#####.#.####.#####.#",
        "#.........................#",
        "#.####.#.#########.#.####.#",
        "#......#.....#.....#......#",
        "######.#####.#.#####.######",
        "     #.#           #.#     ",
        "######.#           #.######",
        "[     .             .     ]",
        "######.#           #.######",
        "     #.#           #.#     ",
        "######.#..#######..#.######",
        "#............#............#",
        "#.####.#####.#.#####.####.#",
        "#O...#.......<.......#...O#",
        "####.#.#.#########.#.#.####",
        "#......#.....#.....#......#",
        "#.####.#####.#.#####.####.#",
        "#o.......................o#",
        "###########################"
    };

    for (int y = 0; y < NUM_BLOCKS_Y; ++y) {
        for (int x = 0; x < NUM_BLOCKS_X; ++x) {
            char c = maze[y][x];
            int xPos = x * BLOCK_SIZE;
            int yPos = y * BLOCK_SIZE;

            if (c == '#') {
                walls.push_back(new GameObject(xPos, yPos, GameObjectType_None, true));

            } else if (c == '.') {
                dots.push_back(new Dot(xPos, yPos));
            } else if (c == 'o') {
                powerups.push_back(new Powerup(xPos, yPos));
            } else if (c == 'P') {
                pacman = new Pacman(xPos, yPos);
            } else if (c == '<') {
                ghosts.push_back(new Ghost(xPos, yPos));
            }
        }
    }
}

    bool isWall(int x, int y) {
    int row = y / BLOCK_SIZE;
    int col = x / BLOCK_SIZE;

    if (row < 0 || row >= NUM_BLOCKS_Y || col < 0 || col >= NUM_BLOCKS_X) {
        return true; // Treat out-of-bounds as walls
    }

    for (GameObject* wall : walls) {
        if (wall->x == x && wall->y == y) {
            return true;
        }
    }

    return false;
}

    void drawGameObject(GameObject* gameObject, SDL_Texture* texture) {
        if (gameObject->alive) {
            SDL_Rect rect = { gameObject->x, gameObject->y, BLOCK_SIZE, BLOCK_SIZE };
            SDL_RenderCopy(renderer, texture, NULL, &rect);
        }
    }

    void run() {
        while (true) {
            update();
            SDL_Delay(16);
        }
    }

    ~Game() {
        // Clean up game objects
        delete pacman;
        for (int i = 0; i < ghosts.size(); i++) {
            delete ghosts[i];
        }
        for (int i = 0; i < dots.size(); i++) {
            delete dots[i];
        }
        for (int i = 0; i < powerups.size(); i++) {
            delete powerups[i];
        }


        // Clean up SDL
        SDL_DestroyTexture(texture_pacman);
        SDL_DestroyTexture(texture_ghost);
        SDL_DestroyTexture(texture_dot);
        SDL_DestroyTexture(texture_powerup);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_DestroyTexture(texture_wall);

        SDL_Quit();
    }

    bool checkCollisionWithWalls(int x, int y) {
        for (GameObject* wall : walls) {
            if (x == wall->x && y == wall->y) {
                return true;
            }
        }
        return false;
    }
};

// Define the Ghost::update function after Game class
void Ghost::update(Game* game) {
    counter++;

    if (counter % game->ghostSpeed == 0) {
        // Check if the ghost is in the middle of a block
        if (x % BLOCK_SIZE == 0 && y % BLOCK_SIZE == 0) {
            // Try to move the ghost in the current direction
            int newX = x + dx * BLOCK_SIZE;
            int newY = y + dy * BLOCK_SIZE;

            // If the ghost hits a wall, change direction
            if (game->isWall(newX, newY)) {
                // Find a new direction that doesn't lead to a wall
                while (true) {
                    int newDirection = rand() % 4;

                    switch (newDirection) {
                        case 0: dx = -1; dy = 0; break;
                        case 1: dx = 1; dy = 0; break;
                        case 2: dx = 0; dy = -1; break;
                        case 3: dx = 0; dy = 1; break;
                    }

                    newX = x + dx * BLOCK_SIZE;
                    newY = y + dy * BLOCK_SIZE;

                    if (!game->isWall(newX, newY)) {
                        break;
                    }
                }
            }
        }

        // If the ghost is not in the middle of a block or not hitting a wall, continue moving in the current direction
        x += dx;
        y += dy;
    }
}

// Main function
int main(int argc, char* argv[]) {

    Game game;
    game.run();
    return 0;
}

