#ifndef _GAME_CAMERA_H_
#define _GAME_CAMERA_H_

#include "framework.h"
#include "camera.h"
#include "texture.h"
#include "shader.h"
#include "mesh.h"
#include "input.h"

#define CHAR_SPEED 60.f
#define CHAR_SLOW_SPEED 25.f
#define CHAR_POINTED_SPEED 0.2f
#define CHAR_ROT_SPEED 0.0005f
#define CHAR_ROT_POINT_SPEED 0.00005f

enum CamType : uint8 {
    THIRD_PERSON = 0,
    FIRST_PERSON = 1,
};

struct sPlayer {
    Vector3 direction;
    Vector3 position;
    Vector3 speed;
    Vector3 rotation;
    Matrix44 model;

    CamType cam_mode;

    float shoot_anim;
    bool has_shot_on_frame;

    Texture *texture[2];
    Shader *shader;
    Mesh *meshes[2];

    float charecter_speed[2] = { CHAR_SPEED, CHAR_SLOW_SPEED };

    sPlayer(Vector3 start_pos);
    sPlayer();
    
    void get_camera(Camera *cam);
    void render(Camera *cam);
    void render_camera_fog(Camera *cam);
    void update(float elapsed_time);
    void shoot_animation();

    bool has_shoot(Vector3 &light_position) {
        light_position = position;
        light_position.y += 2.f;
        return shoot_anim > 0.55;
    }
};


#endif