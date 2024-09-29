#include "raylib.h"
#include <vector>
#include "Plane.h"
#include "Enemy.h"
#include "GameUtilities.h"
#include "SimpleTerrain.h"


int main()
{
    // Initialization
    const int screenWidth = 1400;
    const int screenHeight = 1000;

    InitWindow(screenWidth, screenHeight, WINDOW_NAME);
    SetTargetFPS(60);
    SetConfigFlags(FLAG_MSAA_4X_HINT);

    // Create a player instance
    Plane player(PLAYER_OBJ, PLAYER_TEXTURE, { 0.0f, 0.0f, 0.0f });
    player.SetScale(0.06f);
    player.SetFlipped(true);

    //Enemy enemy(ENEMY_OBJ, ENEMY_TEXTURE,{ 0.0f, 0.0f, 0.0f }, 2.0f);

    // Camera setup
    Camera3D camera = { 0 };
    camera.position = (Vector3){ 0.0f, 5.0f, -15.0f }; // Initial position
    camera.target = player.GetPosition();
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 65.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    SimpleTerrain simpleTerrain;
    simpleTerrain.LoadChunks(player.GetPosition());

    // Main game loop
    while (!WindowShouldClose())
    {
        // Update
        float deltaTime = GetFrameTime();

      
        // Move the player forward first
        player.Move({ 0.0f, 0.0f, -1.0f }, 5.0f, deltaTime);

        // Update the player (apply input, rotation, etc.)
        player.Update(deltaTime);

        //update the terrain
        simpleTerrain.Update(player.GetPosition());

        // Update the camera to follow the player without smoothing
        Vector3 cameraOffset = { 0.0f, -5.0f, -15.0f };// Adjusted offset values
        float smoothFactor = 15.0f * deltaTime; // Increase smooth factor
        //camera.position = Vector3Lerp(Vector3Subtract(player.GetPosition(), cameraOffset), player.GetPosition(), smoothFactor);
        camera.position = Vector3Subtract(player.GetPosition(), cameraOffset);
        camera.target = player.GetPosition();

        // Update enemies
        //enemy.Update(deltaTime, player.GetPosition());

        // Draw
        BeginDrawing();
            ClearBackground(SKYBLUE);

            BeginMode3D(camera);

                // Draw Terrain
                simpleTerrain.Draw();

                // Draw the player
                player.Draw();
                //enemy.Draw();

            EndMode3D();

            // Draw UI elements...
            FlightInfo info = player.GetFlightInfo();
            DrawRectangle(5, 45, 250, 110, Fade(SKYBLUE, 0.5f));
            DrawRectangleLines(5, 45, 250, 110, BLUE);
            char buffer[128];
            sprintf(buffer, "Speed: %.2f units/s", info.speed);
            DrawText(buffer, 10, 50, 15, BLACK);
            sprintf(buffer, "Altitude: %f ", info.altitude);
            DrawText(buffer, 10, 70, 15, BLACK);
            sprintf(buffer, "Pitch: %f ", info.pitch);
            DrawText(buffer, 10, 90, 15, BLACK);
            sprintf(buffer, "Roll: %f ", info.roll);
            DrawText(buffer, 10, 110, 15, BLACK);
            sprintf(buffer, "Yaw: %f ", info.yaw);
            DrawText(buffer, 10, 130, 15, BLACK);

            sprintf(buffer, "Plane : %f, %f, %f", player.GetPosition().x,player.GetPosition().y,player.GetPosition().z);
            DrawText(buffer, 10, 160, 15, WHITE);

            sprintf(buffer, "Camera: %f, %f, %f", camera.position.x,camera.position.y,camera.position.z);
            DrawText(buffer, 10, 190, 15, WHITE);

            DrawFPS(10, 220);


        EndDrawing();
    }

    // De-initialization
    player.Unload();
    simpleTerrain.Unload();
    //enemy.Unload();
    CloseWindow();

    return 0;
}
