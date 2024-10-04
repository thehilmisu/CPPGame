#include "raylib.h"
#include <vector>
#include <memory>
#include "Plane.h"
#include "Enemy.h"
#include "GameUtilities.h"
#include "SimpleTerrain.h"
#include "Cloud.h"


int main()
{
    // Initialization
    const int screenWidth = 1080;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, WINDOW_NAME);
    SetTargetFPS(60);
    SetConfigFlags(FLAG_MSAA_4X_HINT);

    // Create a player instance
    Plane player(PLAYER_RAFALE_OBJ,"",{ 0.0f, 20.0f, 0.0f });
    player.SetScale(0.4f);
    player.SetFlipped(true);

    // float enemySpawnTimer = 0.0f;
    // std::vector<Enemy> enemies;

    float cloudSpawnTimer = 0.0f;
    std::vector<Cloud> clouds;


    // Camera setup
    Camera3D camera = { 0 };
    camera.position = (Vector3){ 0.0f, 25.0f, -15.0f }; // Initial position
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
        //player.Move({ 0.0f, 0.0f, -1.0f }, 5.0f, deltaTime);

        // Update the player (apply input, rotation, etc.)
        player.Update(deltaTime);

        //update the terrain
        simpleTerrain.Update(player.GetPosition());

        // Update the camera to follow the player without smoothing
        Vector3 cameraOffset = { 0.0f, -5.0f, -15.0f };
        camera.position = Vector3Subtract(player.GetPosition(), cameraOffset);
        camera.target = player.GetPosition();

         // Update enemy spawning
        // enemySpawnTimer += deltaTime;
        //
        // if (enemySpawnTimer >= ENEMY_SPAWN_INTERVAL)
        // {
        //     enemySpawnTimer = 0.0f;
        //     GameUtilities::SpawnEnemy(player.GetPosition(), enemies);
        // }

        cloudSpawnTimer += deltaTime;
        if(cloudSpawnTimer >+ 1.0f) {
            cloudSpawnTimer = 0.0f;
            Cloud cloud(player.GetPosition(), 1.0f);
            clouds.push_back(cloud);
        }

        // Draw
        BeginDrawing();
            ClearBackground(SKYBLUE);

            BeginMode3D(camera);

                // Draw Terrain
                simpleTerrain.Draw();

                // Draw the player
                player.Draw();
                
                // Draw enemies
                // for(auto& enemy : enemies)
                // {
                //     enemy.Update(deltaTime, player.GetPosition());
                //     enemy.Draw();
                // }

                // Draw clouds
                for(auto& cloud : clouds) {
                    cloud.Update(deltaTime);
                    cloud.Draw();
                }

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
    // for(auto& i : enemies)
    //     i.Unload();
    // enemies.clear();
    for(auto& cloud : clouds) {
        cloud.Unload();
    }
    clouds.clear();
    CloseWindow();

    return 0;
}
