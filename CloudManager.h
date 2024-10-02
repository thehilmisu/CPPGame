//
// Created by developer on 10/2/24.
//

#ifndef CLOUDMANAGER_H
#define CLOUDMANAGER_H

#include "raylib.h"
#include "Cloud.h"

class CloudManager {
  public:
    CloudManager();
    ~CloudManager() = default;
    void GenerateCloud(float deltaTime, const Vector3 playerPosition);

};


#endif //CLOUDMANAGER_H
