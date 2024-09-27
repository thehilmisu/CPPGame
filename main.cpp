#include "raylib.h"
#include <vector>
#include "Plane.h"
#include "Enemy.h"
#include "GameSettings.h"
#include "GameUtilities.h"


void SpawnEnemy(const Vector3 playerPosition, std::vector<Enemy>& enemies)
{
    if (enemies.size() >= ENEMY_MAX_COUNT)
    {
        // Don't spawn more than the maximum number of enemies
        return;
    }

    // Random X and Y within specified ranges
    float randomX = playerPosition.x + ((rand() % 1000 / 1000.0f) * ENEMY_SPAWN_RANGE_X * 2) - ENEMY_SPAWN_RANGE_X;
    float randomY = playerPosition.y + ((rand() % 1000 / 1000.0f) * ENEMY_SPAWN_RANGE_Y * 2) - ENEMY_SPAWN_RANGE_Y;

    // Z position ahead of the player
    float spawnZ = playerPosition.z - ENEMY_SPAWN_DISTANCE;  // Assuming negative Z is forward

    Vector3 spawnPosition = { randomX, randomY, spawnZ };
    Vector3 enemySize = { ENEMY_SIZE, ENEMY_SIZE, ENEMY_SIZE };

    // Create a new enemy and add it to the list
    enemies.emplace_back(spawnPosition, enemySize);
}

int main()
{
    // Initialization
    const int screenWidth = 1400;
    const int screenHeight = 1000;

    InitWindow(screenWidth, screenHeight, "Flight Mania");
    SetTargetFPS(60);

    // Create a player instance
    Plane player(PLAYER_OBJ, PLAYER_TEXTURE, { 0.0f, 0.0f, 0.0f });
    player.SetScale(0.06f);
    player.SetFlipped(true);

    std::vector<Enemy> enemies;
    float enemySpawnTimer = 0.0f;

    // Camera setup
    Camera3D camera = { 0 };
    camera.position = (Vector3){ 0.0f, 5.0f, -15.0f }; // Initial position
    camera.target = player.GetPosition();
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 65.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    // Main game loop
    while (!WindowShouldClose())
    {
        // Update
        float deltaTime = GetFrameTime();

        // Update the player
        player.Update(deltaTime);

        // Update the camera to follow the player without smoothing

        Vector3 cameraOffset = { 0.0f, 5.0f, 15.0f };// Adjusted offset values
        float smoothFactor = 15.0f * deltaTime / 5.0f; // Increase smooth factor
        camera.position = Vector3Lerp(camera.position, Vector3Add(player.GetPosition(), cameraOffset), smoothFactor);
        camera.target = player.GetPosition();


        // Update enemy spawning
        enemySpawnTimer += deltaTime;

        if (enemySpawnTimer >= ENEMY_SPAWN_INTERVAL)
        {
            enemySpawnTimer = 0.0f;
            SpawnEnemy(player.GetPosition(), enemies);
        }

        // Update enemies
        for (auto& enemy : enemies)
        {
            enemy.Update(deltaTime, player.GetPosition());
        }

        // Draw
        BeginDrawing();
            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

                // Draw the player
                player.Draw();
                //player.Move({0.0f,0.0f,-1.0f}, 5.0f, deltaTime);

                for (auto& enemy : enemies)
                {
                    enemy.Draw();
                }

                // Draw grid
                //DrawInfiniteGrid(1.0f, (Vector3){ player.GetPosition().x, 0.0f, player.GetPosition().z });
                DrawGrid(50, 10);

            EndMode3D();

            // Draw UI elements...

        EndDrawing();
    }

    // De-initialization
    player.Unload();
    CloseWindow();

    return 0;
}
