#include "raylib.h"
#include <stdlib.h>
#include <time.h>

#define GRID_SIZE_X 50
#define GRID_SIZE_Y 30
#define GRID_SIZE_Z 20

Camera camera = { 0 };
int grid[GRID_SIZE_X][GRID_SIZE_Y][GRID_SIZE_Z];
int newGrid[GRID_SIZE_X][GRID_SIZE_Y][GRID_SIZE_Z];
float lastUpdateTime = 0.0f;


static void UpdateDrawFrame(void); 
static void InitializeGrid(void);
static int countNeighbors(int x, int y, int z);
static void UpdateGrid(void);


int main()
{

    const int screenWidth = 1500;
    const int screenHeight = 1000;

    InitWindow(screenWidth, screenHeight, "Life");

    camera.position = (Vector3){ 20.0f, 20.0f, 20.0f };
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    SetTargetFPS(60); 

    InitializeGrid();


    while (!WindowShouldClose()) 
    {
        UpdateDrawFrame();
    }


    CloseWindow(); 


    return 0;
}


static void InitializeGrid(void)
{
    srand(time(NULL)); 
    for (int x = 0; x < GRID_SIZE_X; x++) {
        for (int y = 0; y < GRID_SIZE_Y; y++) {
            for (int z = 0; z < GRID_SIZE_Z; z++) {
                grid[x][y][z] = rand() % 2; 
            }
        }
    }
}


static int countNeighbors(int x, int y, int z)
{
    int count = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            for (int k = -1; k <= 1; k++) {
                if (i == 0 && j == 0 && k == 0) continue; 
                int nx = x + i;
                int ny = y + j;
                int nz = z + k;
                if (nx >= 0 && ny >= 0 && nz >= 0 && nx < GRID_SIZE_X && ny < GRID_SIZE_Y && nz < GRID_SIZE_Z) {
                    count += grid[nx][ny][nz];
                }
            }
        }
    }
    return count;
}


static void UpdateGrid(void)
{
    for (int x = 0; x < GRID_SIZE_X; x++) {
        for (int y = 0; y < GRID_SIZE_Y; y++) {
            for (int z = 0; z < GRID_SIZE_Z; z++) {
                int neighbors = countNeighbors(x, y, z);
                if (grid[x][y][z] == 1) {
                    if (neighbors < 2 || neighbors > 5) {
                        newGrid[x][y][z] = 0;
                    } else {
                        newGrid[x][y][z] = 1;
                    }
                } else {
                    if (neighbors == 4) {
                        newGrid[x][y][z] = 1;
                    } else {
                        newGrid[x][y][z] = 0;
                    }
                }
            }
        }
    }


    for (int x = 0; x < GRID_SIZE_X; x++) {
        for (int y = 0; y < GRID_SIZE_Y; y++) {
            for (int z = 0; z < GRID_SIZE_Z; z++) {
                grid[x][y][z] = newGrid[x][y][z];
            }
        }
    }
}


static void UpdateDrawFrame(void)
{

    UpdateCamera(&camera, CAMERA_FIRST_PERSON);

    if (GetTime() - lastUpdateTime > 0.5f) { 
        UpdateGrid();
        lastUpdateTime = GetTime();
    }

    BeginDrawing();

        ClearBackground(BLACK);

        BeginMode3D(camera);

            for (int x = 0; x < GRID_SIZE_X; x++) {
                for (int y = 0; y < GRID_SIZE_Y; y++) {
                    for (int z = 0; z < GRID_SIZE_Z; z++) {
                        if (grid[x][y][z] == 1) {
                            Vector3 position = { (float)x, (float)y, (float)z };
                            DrawCube(position, 0.5f, 0.5f, 0.5f, GREEN);
                        }
                    }
                }
            }

            // DrawGrd(2000, 1.0f);

            for (int i = -200; i <= 200; i++)
            {
                DrawLine3D((Vector3){ i * 20, 0, -200 * 20 }, (Vector3){ i * 20, 0, 200 * 20 }, LIGHTGRAY);
                DrawLine3D((Vector3){ -200 * 20, 0, i * 20 }, (Vector3){ 200 * 20, 0, i * 20 }, LIGHTGRAY);

                DrawLine3D((Vector3){ i * 20, -200 * 20, 0 }, (Vector3){ i * 20, 200 * 20, 0 }, LIGHTGRAY);
                DrawLine3D((Vector3){ -200 * 20, i * 20, 0 }, (Vector3){ 200 * 20, i * 20, 0 }, LIGHTGRAY);

                DrawLine3D((Vector3){ 0, i * 20, -200 * 20 }, (Vector3){ 0, i * 20, 200 * 20 }, LIGHTGRAY);
                DrawLine3D((Vector3){ 0, -200 * 20, i * 20 }, (Vector3){ 0, 200 * 20, i * 20 }, LIGHTGRAY);
            }


        EndMode3D();

        DrawText("Life", 10, 40, 20, DARKGRAY);
        DrawFPS(60, 10);

    EndDrawing();
}
