#ifndef GAMEUTILITIES_H
#define GAMEUTILITIES_H

#include "raylib.h"

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
};

#endif