//
// Created by developer on 10/2/24.
//

#include "CloudManager.h"

CloudManager::CloudManager(){

}

void CloudManager::GenerateCloud(float deltaTime, const Vector3 playerPosition){
    Cloud cloud("assets/Clouds/Cloud_Polygon_Blender_1.obj","cloud_diffuse.png",{0.0f, 20.0f, -20.0f},{10.0f,1.0f,1.0f});
    cloud.Draw();

}
