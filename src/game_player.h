#ifndef _GAME_CAMERA_H_
#define _GAME_CAMERA_H_

#include "framework.h"
#include "camera.h"
#include "texture.h"
#include "shader.h"
#include "mesh.h"

enum CamType : uint8 {
    THIRD_PERSON = 0,
    FIRST_PERSON,
    MENU // Todo: far future
};

struct sPlayer {
    Vector3 position;
    Vector3 speed;
    Vector3 rotation;
    Matrix44 model;

    CamType cam_mode;

    Texture *texture;
    Shader *shader;
    Mesh *meshes[3];

    sPlayer(Vector3 start_pos);
    sPlayer();
    
    void get_camera(Camera *cam);
    void render(Camera *cam);
};


#endif