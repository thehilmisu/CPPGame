#include "raylib.h"
#include "Plane.h"

#define     PLAYER_OBJ      "assets/plane.obj"
#define     PLAYER_TEXTURE  "assets/plane_diffuse.png"
#define     ENEMY_OBJ       ""
#define     ENEMY_TEXTURE   ""


int main()
{
    // Initialization
    const int screenWidth = 1080;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "3D Game with LookAt Function");
    SetTargetFPS(60);

    // Create a player instance
    Plane player(PLAYER_OBJ, PLAYER_TEXTURE, { 0.0f, 0.0f, 0.0f });
    player.SetScale(0.06f); 

    // Camera setup
    Camera3D camera = { 0 };
    camera.position = (Vector3){ 0.0f, 5.0f, 10.0f };
    camera.target = player.GetPosition();
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 65.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    // Cube (target) position
    Vector3 cubePosition = { 5.0f, 0.0f, 0.0f };

    // Main game loop
    while (!WindowShouldClose())
    {
        // Update
        float deltaTime = GetFrameTime();

        // Move the cube with arrow keys
        // float cubeSpeed = 5.0f * deltaTime;
        // if (IsKeyDown(KEY_LEFT)) cubePosition.x -= cubeSpeed;
        // if (IsKeyDown(KEY_RIGHT)) cubePosition.x += cubeSpeed;
        // if (IsKeyDown(KEY_UP)) cubePosition.z -= cubeSpeed;
        // if (IsKeyDown(KEY_DOWN)) cubePosition.z += cubeSpeed;
        // if (IsKeyDown(KEY_W)) cubePosition.y += cubeSpeed;
        // if (IsKeyDown(KEY_S)) cubePosition.y -= cubeSpeed;

        // // Make the player look at the cube
        // player.LookAt(cubePosition);

        // Update the player (if any additional updates are needed)
        player.Update(deltaTime);

        // Draw
        BeginDrawing();
            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

                // Draw the player
                player.Draw();

                // Draw the cube (target)
                DrawCube(cubePosition, 1.0f, 1.0f, 1.0f, RED);
                DrawCubeWires(cubePosition, 1.0f, 1.0f, 1.0f, MAROON);

                // Draw grid
                DrawGrid(100, 1.0f);

            EndMode3D();

            DrawText("Use arrow keys to move the cube (target).", 10, 10, 20, DARKGRAY);
            DrawText("Player is looking at the cube.", 10, 35, 20, DARKGRAY);
            DrawText("Use PAGE UP/DOWN to move cube vertically.", 10, 60, 20, DARKGRAY);

        EndDrawing();
    }

    // De-initialization
    CloseWindow();

    return 0;
}
