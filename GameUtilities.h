#ifndef GAMEUTILITIES_H
#define GAMEUTILITIES_H

#include "raylib.h"
#include <vector>
#include <random>
#include "Enemy.h"
#include "GameSettings.h"

class GameUtilities{
public:
    static Quaternion RotateTowardsMousePosition(Vector3 position, Camera3D camera) {
        // Get the mouse position in screen coordinates
        Vector2 mousePos = GetMousePosition();

        // Calculate a ray from the camera to the mouse position in world space
        Ray mouseRay = GetMouseRay(mousePos, camera);

        // Calculate the direction vector from the object position to the mouse ray position
        Vector3 direction = Vector3Subtract(mouseRay.position, position);

        // Normalize the direction vector
        direction = Vector3Normalize(direction);

        // Calculate the up vector for the rotation
        Vector3 up = { 0.0f, 1.0f, 0.0f }; // Assuming Y is up in the world

        // Calculate the rotation axis using cross product
        Vector3 rotationAxis = Vector3CrossProduct(up, direction);

        // Calculate the angle to rotate (use dot product to get the angle)
        float angle = acosf(Vector3DotProduct(up, direction));

        // Generate the quaternion for rotation around the axis
        Quaternion rotation = QuaternionFromAxisAngle(rotationAxis, angle);

        return rotation;
    }
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
        enemies.emplace_back(spawnPosition, enemySize);
    }

    static float CalculateDistance(const Vector3& a, const Vector3& b)
    {
        return Vector3Distance(a, b);
    }

    static float PerlinNoise(float x, float z) {
        // Simple noise function (replace with a more complex algorithm if needed)
        return sinf(x * 0.1f) * cosf(z * 0.1f);
    }

    static float fractalNoise(float x, float z) {
        return PerlinNoise(x, z) * 10.0f + PerlinNoise(x * 2, z * 2) * 5.0f + PerlinNoise(x * 4, z * 4) * 2.5f;
    }

    static float GenerateRandomFloat(float min, float max) {
        // Ensure that min is less than max
        if (min > max) std::swap(min, max);

        // Static random engine and distribution to maintain state across calls
        static std::random_device rd;                           // Seed for the random number engine
        static std::mt19937 generator(rd());                    // Mersenne Twister engine
        std::uniform_real_distribution<float> distribution(min, max); // Distribution range

        return distribution(generator);
    }


};

#endif