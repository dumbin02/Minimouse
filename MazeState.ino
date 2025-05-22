// Maze dimensions and properties
#define MAZE_WIDTH 10
#define MAZE_HEIGHT 10
#define UNVISITED_DISTANCE 255

// Define arrays for maze state
uint8_t horizontalWalls[MAZE_HEIGHT - 1][MAZE_WIDTH] = {0}; // Wall between (r,c) and (r+1,c)
uint8_t verticalWalls[MAZE_HEIGHT][MAZE_WIDTH - 1] = {0};   // Wall between (r,c) and (r,c+1)
uint8_t manhattanDistances[MAZE_HEIGHT][MAZE_WIDTH] = {0};

// Struct MousePosition and enum MouseDirection are now defined in MouseMain.h

// Global variables for mouse state
MousePosition currentPosition;
MouseDirection currentDirection;

// Function prototypes for managing maze state are in MouseMain.h
// No need for local prototypes here if they are in MouseMain.h and MouseMain.h is effectively included.

// --- Circular Queue for Flood Fill ---
#define MAX_QUEUE_SIZE 100 // Adjust size as needed
MousePosition floodFillQueue[MAX_QUEUE_SIZE];
uint8_t queueStart = 0;
uint8_t queueEnd = 0;
uint8_t queueCount = 0;

// Function prototypes for queue operations (these are local to MazeState.ino)
void initQueue();
bool enqueue(MousePosition pos);
MousePosition dequeue();
bool isQueueEmpty();
bool isQueueFull();

// --- Queue Implementation ---
void initQueue() {
    queueStart = 0;
    queueEnd = 0;
    queueCount = 0;
}

bool isQueueEmpty() {
    return queueCount == 0;
}

bool isQueueFull() {
    return queueCount == MAX_QUEUE_SIZE;
}

bool enqueue(MousePosition pos) {
    if (isQueueFull()) {
        return false; 
    }
    floodFillQueue[queueEnd] = pos;
    queueEnd = (queueEnd + 1) % MAX_QUEUE_SIZE;
    queueCount++;
    return true;
}

MousePosition dequeue() {
    MousePosition pos = {(int8_t)-1, (int8_t)-1}; // Returns {-1,-1} if empty
    if (isQueueEmpty()) {
        return pos; 
    }
    pos = floodFillQueue[queueStart];
    queueStart = (queueStart + 1) % MAX_QUEUE_SIZE;
    queueCount--;
    return pos;
}

// --- Maze and Mouse Initialization ---
void initializeMazeAndMouse() {
    currentPosition.x = 0; 
    currentPosition.y = 0; 
    currentDirection = EAST; // EAST es para derecha, WEST es para izquierda
/*
Aqui es
--------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------

*/
    for (int8_t r = 0; r < MAZE_HEIGHT; r++) {
        for (int8_t c = 0; c < MAZE_WIDTH; c++) {
            manhattanDistances[r][c] = UNVISITED_DISTANCE;
        }
    }
    manhattanDistances[4][4] = 0;
    manhattanDistances[4][5] = 0;
    manhattanDistances[5][4] = 0;
    manhattanDistances[5][5] = 0;

    for (uint8_t r = 0; r < MAZE_HEIGHT - 1; r++) {
        for (uint8_t c = 0; c < MAZE_WIDTH; c++) {
            horizontalWalls[r][c] = 0;
        }
    }
    for (int8_t r = 0; r < MAZE_HEIGHT; r++) {
        for (int8_t c = 0; c < MAZE_WIDTH - 1; c++) {
            verticalWalls[r][c] = 0;
        }
    }
    initQueue();
}

// --- Flood Fill Algorithm ---
void runFloodFill() {
    for (int8_t r_idx = 0; r_idx < MAZE_HEIGHT; r_idx++) {
        for (int8_t c_idx = 0; c_idx < MAZE_WIDTH; c_idx++) {
            manhattanDistances[r_idx][c_idx] = UNVISITED_DISTANCE;
        }
    }
    initQueue();
    MousePosition targetCells[4] = {
        {4, 4}, {4, 5}, {5, 4}, {5, 5}
    };
    for (uint8_t i = 0; i < 4; i++) {
        manhattanDistances[targetCells[i].y][targetCells[i].x] = 0;
        enqueue(targetCells[i]); 
    }
    while (!isQueueEmpty()) {
        MousePosition current = dequeue();
        int8_t r = current.y;
        int8_t c = current.x;
        uint8_t distance = manhattanDistances[r][c];
        int8_t dy[] = {-1, 0, 1, 0};
        int8_t dx[] = {0, 1, 0, -1};
        for (uint8_t i = 0; i < 4; i++) {
            int8_t nr = r + dy[i];
            int8_t nc = c + dx[i];
            if (nr < 0 || nc < 0 || nr >= MAZE_HEIGHT || nc >= MAZE_WIDTH) {
                continue;
            }
            bool canMove = false;
            if (dy[i] == -1) { 
                if (horizontalWalls[r-1][c] == 0) canMove = true;
            } else if (dx[i] == 1) { 
                if (verticalWalls[r][c] == 0) canMove = true;
            } else if (dy[i] == 1) { 
                if (horizontalWalls[r][c] == 0) canMove = true;
            } else if (dx[i] == -1) { 
                if (verticalWalls[r][c-1] == 0) canMove = true;
            }
            if (canMove && manhattanDistances[nr][nc] == UNVISITED_DISTANCE) {
                manhattanDistances[nr][nc] = distance + 1;
                MousePosition neighbor = {nc, nr};
                enqueue(neighbor);
            }
        }
    }
}

// Function to get the next position based on current position and direction
MousePosition getNextPosition(MousePosition current, MouseDirection dir) {
    MousePosition next = current;
    switch (dir) {
        case NORTH: next.y--; break;
        case EAST:  next.x++; break;
        case SOUTH: next.y++; break;
        case WEST:  next.x--; break;
    }
    return next;
}

// Function to get the new direction after turning left or right
MouseDirection getNextDirection(bool turnRight) {
    if (turnRight) {
        return (MouseDirection)((currentDirection + 1) % 4);
    } else {
        return (MouseDirection)((currentDirection + 3) % 4);
    }
}

bool canMoveForward() {
    switch (currentDirection) {
        case NORTH:
            if (currentPosition.y == 0) return false;
            return horizontalWalls[currentPosition.y - 1][currentPosition.x] == 0;
        case EAST:
            if (currentPosition.x == MAZE_WIDTH - 1) return false;
            return verticalWalls[currentPosition.y][currentPosition.x] == 0;
        case SOUTH:
            if (currentPosition.y == MAZE_HEIGHT - 1) return false;
            return horizontalWalls[currentPosition.y][currentPosition.x] == 0;
        case WEST:
            if (currentPosition.x == 0) return false;
            return verticalWalls[currentPosition.y][currentPosition.x - 1] == 0;
    }
    return false;
}



// --- Wall Management Function ---
void addWallsAtCurrentPosition(bool leftWall, bool frontWall, bool rightWall) {
    MouseDirection leftDir = (MouseDirection)((currentDirection + 3) % 4);
    MouseDirection frontDir = currentDirection;
    MouseDirection rightDir = (MouseDirection)((currentDirection + 1) % 4);

    // Handle Left Wall
    if (leftDir == NORTH && currentPosition.y > 0) {
        horizontalWalls[currentPosition.y-1][currentPosition.x] = leftWall ? 1 : 0;
    } else if (leftDir == EAST && currentPosition.x < MAZE_WIDTH-1) {
        verticalWalls[currentPosition.y][currentPosition.x] = leftWall ? 1 : 0;
    } else if (leftDir == SOUTH && currentPosition.y < MAZE_HEIGHT-1) {
        horizontalWalls[currentPosition.y][currentPosition.x] = leftWall ? 1 : 0;
    } else if (leftDir == WEST && currentPosition.x > 0) {
        verticalWalls[currentPosition.y][currentPosition.x-1] = leftWall ? 1 : 0;
    }

    // Handle Front Wall
    if (frontDir == NORTH && currentPosition.y > 0) {
        horizontalWalls[currentPosition.y-1][currentPosition.x] = frontWall ? 1 : 0;
    } else if (frontDir == EAST && currentPosition.x < MAZE_WIDTH-1) {
        verticalWalls[currentPosition.y][currentPosition.x] = frontWall ? 1 : 0;
    } else if (frontDir == SOUTH && currentPosition.y < MAZE_HEIGHT-1) {
        horizontalWalls[currentPosition.y][currentPosition.x] = frontWall ? 1 : 0;
    } else if (frontDir == WEST && currentPosition.x > 0) {
        verticalWalls[currentPosition.y][currentPosition.x-1] = frontWall ? 1 : 0;
    }

    // Handle Right Wall
    if (rightDir == NORTH && currentPosition.y > 0) {
        horizontalWalls[currentPosition.y-1][currentPosition.x] = rightWall ? 1 : 0;
    } else if (rightDir == EAST && currentPosition.x < MAZE_WIDTH-1) {
        verticalWalls[currentPosition.y][currentPosition.x] = rightWall ? 1 : 0;
    } else if (rightDir == SOUTH && currentPosition.y < MAZE_HEIGHT-1) {
        horizontalWalls[currentPosition.y][currentPosition.x] = rightWall ? 1 : 0;
    } else if (rightDir == WEST && currentPosition.x > 0) {
        verticalWalls[currentPosition.y][currentPosition.x-1] = rightWall ? 1 : 0;
    }
} 
// Static variables declared outside to save processing power
static MousePosition forwardPos;
static MousePosition rightPos;
static MousePosition leftPos;
static uint8_t currentValue;
static uint8_t forwardValue;
static uint8_t rightValue;
static uint8_t leftValue;

// Helper function to check if position is within maze bounds
bool isValidPosition(MousePosition pos) {
    return pos.x >= 0 && pos.x < MAZE_WIDTH && 
           pos.y >= 0 && pos.y < MAZE_HEIGHT;
}

// Returns movement direction: 0=Forward, 1=Right, 2=Left, 3=Turn Around, 4=Stuck
uint8_t getNextBestMove() {
    // Check if current position is valid
    if (currentPosition.x < 0 || currentPosition.x >= MAZE_WIDTH ||
        currentPosition.y < 0 || currentPosition.y >= MAZE_HEIGHT) {
        return 4 ; // Default to stuck if position invalid
    }

    // Get current cell's value
    currentValue = manhattanDistances[currentPosition.y][currentPosition.x];
    
    // If sensors detect walls in front, to the left, and to the right,
    // AND the current cell is marked as unvisited, then turn around.
    if (GlobalFrontWall && GlobalLeftWall && GlobalRightWall && (currentValue != UNVISITED_DISTANCE)) {
        return 3; // Turn around
    }

    // Initialize potential move values
    forwardValue = UNVISITED_DISTANCE; 
    rightValue = UNVISITED_DISTANCE;
    leftValue = UNVISITED_DISTANCE;

    // Check forward cell
    forwardPos = currentPosition;
    switch(currentDirection) {
        case NORTH: forwardPos.y--; break;
        case EAST:  forwardPos.x++; break;
        case SOUTH: forwardPos.y++; break;
        case WEST:  forwardPos.x--; break;
    }
    if (isValidPosition(forwardPos) && !GlobalFrontWall) {
        forwardValue = manhattanDistances[forwardPos.y][forwardPos.x];
    }

    // Check right cell
    rightPos = currentPosition;
    switch((MouseDirection)((currentDirection + 1) % 4)) {
        case NORTH: rightPos.y--; break;
        case EAST:  rightPos.x++; break;
        case SOUTH: rightPos.y++; break;
        case WEST:  rightPos.x--; break;
    }
    if (isValidPosition(rightPos) && !GlobalRightWall) {
        rightValue = manhattanDistances[rightPos.y][rightPos.x];
    }

    // Check left cell
    leftPos = currentPosition;
    switch((MouseDirection)((currentDirection + 3) % 4)) {
        case NORTH: leftPos.y--; break;
        case EAST:  leftPos.x++; break;
        case SOUTH: leftPos.y++; break;
        case WEST:  leftPos.x--; break;
    }
    if (isValidPosition(leftPos) && !GlobalLeftWall) {
        leftValue = manhattanDistances[leftPos.y][leftPos.x];
    }

    // Check if we're stuck (all adjacent cells are unreachable)
    if (forwardValue == UNVISITED_DISTANCE && 
        rightValue == UNVISITED_DISTANCE && 
        leftValue == UNVISITED_DISTANCE) {
        return 4; // We're stuck!
    }

    // Determine the best move
    uint8_t bestAction = 0; // Default to Turn Around
    uint8_t minFoundValue = currentValue; // We are looking for a value strictly less than this.

    // Priority 1: Forward
    if (forwardValue < minFoundValue) {
        minFoundValue = forwardValue;
        bestAction = 0; // Go Forward
    }

    // Priority 2: Right
    if (rightValue < minFoundValue) {
        minFoundValue = rightValue;
        bestAction = 1; // Turn Right
    } else if (rightValue == minFoundValue) {
        if (bestAction != 0) { 
            bestAction = 1; // Turn Right (preferred over Left or initial Turn Around)
        }
    }

    // Priority 3: Left
    if (leftValue < minFoundValue) {
        minFoundValue = leftValue;
        bestAction = 2; // Turn Left
    } else if (leftValue == minFoundValue) {
        if (bestAction != 0 && bestAction != 1) {
            bestAction = 2; // Turn Left (preferred over initial Turn Around)
        }
    }
    
    // Final decision: if we haven't found a move that leads to a cell with
    // a value strictly less than the current cell's value, then turn around.
    if (minFoundValue >= currentValue) {
        return 4; // Turn Around
    }

    return bestAction;
}


