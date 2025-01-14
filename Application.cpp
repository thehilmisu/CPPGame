#include "raylib.h"

#include "Application.h"
#include <raymath.h>
#include <vector>
#include <memory>
#include "Plane.h"
#include "Enemy.h"
#include "GameUtilities.h"
#include "SimpleTerrain.h"
#include "Cloud.h"
#include "Tree.h"
#include "ResourceManager.h"
#include "Missile.h"
#include "StaticTerrain.h"

Application::Application()
{
}

void Application::Run()
{

    // Initialization
    const int screenWidth = GetScreenWidth() / 4;
    const int screenHeight = GetScreenHeight() / 4;

    InitWindow(screenWidth, screenHeight, WINDOW_NAME);
    SetTargetFPS(60);
    SetConfigFlags(FLAG_MSAA_4X_HINT);

    // Load all the resources
    ResourceManager::LoadResources();
	
    // Create a player instance
    Plane player({ 0.0f, 20.0f, 0.0f });
    player.SetScale(0.2f);
    player.SetFlipped(true);

    float enemySpawnTimer = 0.0f;
    std::vector<Enemy> enemies;

    float cloudSpawnTimer = 0.0f;
    std::vector<Cloud> clouds;

    float treeSpawnTimer = 0.0f;
    std::vector<Tree> trees;

    // Camera setup
    Camera3D camera = { 0 };
    camera.position = (Vector3){ 0.0f, 35.0f, -5.0f }; // Initial position
    camera.target = player.GetPosition();
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    float randomHeightCounter = 0.0f;
    SimpleTerrain simpleTerrain;

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
        randomHeightCounter += deltaTime;
        if (randomHeightCounter > GameUtilities::GenerateRandomFloat(3.0f, 10.0f )) {
            randomHeightCounter = 0.0f;
            simpleTerrain.SetHeightMeshMap((Vector3){16.0f, GameUtilities::GenerateRandomFloat(0.1f, 2.0f), 16.0f});
        }
        simpleTerrain.Update(player.GetPosition());

        // Update the camera to follow the player without smoothing
        Vector3 cameraOffset = { 0.0f, -35.0f, -1.0f };
        camera.position = Vector3Subtract(player.GetPosition(), cameraOffset);
        camera.target = Vector3Subtract(player.GetPosition(), {0.0f, 0.0f, 7.0f});

        // Update enemy spawning
        // enemySpawnTimer += deltaTime;
        //
        // if (enemySpawnTimer >= ENEMY_SPAWN_INTERVAL)
        // {
        //     enemySpawnTimer = 0.0f;
        //     GameUtilities::SpawnEnemy(player.GetPosition(), enemies);
        // }

        cloudSpawnTimer += deltaTime;
        if(cloudSpawnTimer >= 0.7f) {
            cloudSpawnTimer = 0.0f;
            Cloud cloud(player.GetPosition(), 3.0f);
            clouds.push_back(cloud);
        }

        treeSpawnTimer += deltaTime;
        if(treeSpawnTimer >= 0.7f) {
            treeSpawnTimer = 0.0f;
            Tree tree(player.GetPosition(), 1.0f);
			trees.push_back(tree);
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
                for(auto& enemy : enemies)
                {
                    enemy.Update(deltaTime, player.GetPosition());
                    enemy.Draw();
                }

                // Draw clouds
                for(auto& cloud : clouds) {
                    cloud.Update(deltaTime);
                    cloud.Draw();
                }

                for(auto& tree : trees) {
                    tree.Update(deltaTime);
                    tree.Draw();
                }

            EndMode3D();

            // Draw UI elements...
            auto [pitch, yaw, roll, altitude, speed] = player.GetFlightInfo();

            DrawRectangle(5, 45, 250, 110, Fade(GREEN, 0.5f));
            DrawRectangleLines(5, 45, 250, 110, DARKGREEN);
            char buffer[128];
            sprintf(buffer, "Speed: %.2f units/s", speed);
            DrawText(buffer, 10, 50, 15, BLACK);
            sprintf(buffer, "Altitude: %f ", altitude);
            DrawText(buffer, 10, 70, 15, BLACK);
            sprintf(buffer, "Pitch: %f ", pitch);
            DrawText(buffer, 10, 90, 15, BLACK);
            sprintf(buffer, "Roll: %f ", roll);
            DrawText(buffer, 10, 110, 15, BLACK);
            sprintf(buffer, "Yaw: %f ", yaw);
            DrawText(buffer, 10, 130, 15, BLACK);

            sprintf(buffer, "Plane : %f, %f, %f", player.GetPosition().x,player.GetPosition().y,player.GetPosition().z);
            DrawText(buffer, 10, 160, 15, WHITE);

            sprintf(buffer, "Camera: %f, %f, %f", camera.position.x,camera.position.y,camera.position.z);
            DrawText(buffer, 10, 190, 15, WHITE);

            DrawFPS(10, 220);


        EndDrawing();
    }

    // De-initialization
    simpleTerrain.Unload();
    
    enemies.clear();

    clouds.clear();

    trees.clear();

    ResourceManager::UnloadResources();
    CloseWindow();

}
