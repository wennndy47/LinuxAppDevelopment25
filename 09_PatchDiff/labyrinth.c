#include <time.h>
#include <stdlib.h>
#include <stdio.h>


#define SIZE 6
#define GRID_SIZE (SIZE * 2 + 1)


void genMaze(
    char maze[GRID_SIZE][GRID_SIZE], 
    int visited[SIZE][SIZE], 
    int x, 
    int y
) {
    int dx[] = {0, 1, 0, -1};
    int dy[] = {-1, 0, 1, 0};
    visited[y][x] = 1;
    int directions[4] = {0, 1, 2, 3};
    for (int i = 0; i < 4; i++) {
        int j = rand() % 4;
        int temp = directions[i];
        directions[i] = directions[j];
        directions[j] = temp;
    }

    for (int i = 0; i < 4; i++) {
        int dir = directions[i];
        int nx = x + dx[dir];
        int ny = y + dy[dir];
        
        if ((nx >= 0 && nx < SIZE && ny >= 0 && ny < SIZE) && !visited[ny][nx]) {
            int wallX = x * 2 + 1 + dx[dir];
            int wallY = y * 2 + 1 + dy[dir];
            maze[wallY][wallX] = '.';
            genMaze(maze, visited, nx, ny);
        }
    }
}

void addWalls(char maze[GRID_SIZE][GRID_SIZE], int currentWalls) {
    int totalCells = GRID_SIZE * GRID_SIZE;
    while (currentWalls < totalCells * 15 / 100) {
        int i = rand() % (GRID_SIZE - 2) + 1;
        int j = rand() % (GRID_SIZE - 2) + 1;
        if (maze[i][j] == '.' && (i % 2 == 0 || j % 2 == 0)) {
            int isCritical = 0;
            maze[i][j] = '#';
            int wallCount = 0;
            for (int di = -1; di <= 1; di++) {
                for (int dj = -1; dj <= 1; dj++) {
                    if ((di == 0 || dj == 0) && !(di == 0 && dj == 0)) {
                        int ni = i + di, nj = j + dj;
                        if (ni >= 0 && ni < GRID_SIZE && nj >= 0 && nj < GRID_SIZE) {
                            if (maze[ni][nj] == '#') wallCount++;
                        }
                    }
                }
            }
            if (wallCount < 3) {
                currentWalls++;
            } else {
                maze[i][j] = '.';
            }
        }
    }
}

int main() {
    srand(time(NULL));
    char maze[GRID_SIZE][GRID_SIZE];
    int visited[SIZE][SIZE] = {0};
    
    for (int i = 0; i < GRID_SIZE; i++) 
        for (int j = 0; j < GRID_SIZE; j++)
            if (i % 2 == 0 || j % 2 == 0)
                maze[i][j] = '#';
            else
                maze[i][j] = '.';
    
    genMaze(maze, visited, 0, 0);
    int totalCells = GRID_SIZE * GRID_SIZE;
    int currentWalls = 0;
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            if (maze[i][j] == '#') {
                currentWalls++;
            }
        }
    }
    if (currentWalls < totalCells * 15 / 100)
        addWalls(maze, currentWalls);

    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            printf("%c", maze[i][j]);
        }
        printf("\n");
    }
    return 0;
}