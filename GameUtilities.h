#ifndef GAMEUTILITIES_H
#define GAMEUTILITIES_H

#include "raylib.h"
#include <vector>
#include "Enemy.h"
#include "GameSettings.h"

class GameUtilities{
public:
    static void DrawInfiniteGrid(float spacing, Vector3 centerPosition)
    {
        int lines = 100; // Number of lines to draw on each side

        for (int i = -lines; i <= lines; i++)
        {
            float x = centerPosition.x + i * spacing;
            float zStart = centerPosition.z - lines * spacing;
            float zEnd = centerPosition.z + lines * spacing;

            DrawLine3D(
                (Vector3){ x, centerPosition.y, zStart },
                (Vector3){ x, centerPosition.y, zEnd },
                LIGHTGRAY
            );

            float z = centerPosition.z + i * spacing;
            float xStart = centerPosition.x - lines * spacing;
            float xEnd = centerPosition.x + lines * spacing;

            DrawLine3D(
                (Vector3){ xStart, centerPosition.y, z },
                (Vector3){ xEnd, centerPosition.y, z },
                LIGHTGRAY
            );
        }
    }
    static void SpawnEnemy(const Vector3 playerPosition, std::vector<Enemy>& enemies)
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
        float spawnZ = playerPosition.z - ENEMY_SPAWN_DISTANCE;  // negative Z is forward

        Vector3 spawnPosition = { randomX, randomY, spawnZ };
        Vector3 enemySize = { ENEMY_SIZE, ENEMY_SIZE, ENEMY_SIZE };

        // Create a new enemy and add it to the list
        enemies.emplace_back(ENEMY_MODEL,ENEMY_TEXTURE,spawnPosition, enemySize);
    }

    static float CalculateDistance(const Vector3& a, const Vector3& b)
    {
        return Vector3Distance(a, b);
    }
};

#endif