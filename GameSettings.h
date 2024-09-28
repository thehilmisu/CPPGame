#ifndef GAMESETTINGS_H
#define GAMESETTINGS_H

#include "raylib.h"

#define     PLAYER_OBJ      "assets/plane.obj"
#define     PLAYER_TEXTURE  "assets/plane_diffuse.png"
#define     MIN_ROLL_PITCH_VALUE    -50.0f
#define     MAX_ROLL_PITCH_VALUE    +50.0f
#define     ROLL_PITCH_CONSTANT      60.0f



#define     INITIAL_PLAYER_POSITION     
#define     CAMERA_POSITION


#define ENEMY_MODEL         "assets/cube.obj"
#define ENEMY_TEXTURE       "assets/cube_diffuse.png"
#define ENEMY_SPAWN_DISTANCE 50.0f
#define ENEMY_SPAWN_INTERVAL 2.0f
#define ENEMY_MAX_COUNT 20
#define ENEMY_SIZE 2.0f
#define ENEMY_SPAWN_RANGE_X 20.0f
#define ENEMY_SPAWN_RANGE_Y 10.0f



class GameSettings{
public:
    
};

#endif