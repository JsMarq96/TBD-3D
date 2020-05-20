#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include <vector>
#include "stage_parser.h"
#include "framework.h"
#include "game_camera.h"
#include "mesh.h"
#include "stage.h"
#include "input.h"

#define PRESS_W 0b00001
#define PRESS_A 0b00010
#define PRESS_S 0b00100
#define PRESS_D 0b01000

#define CHAR_SPEED 3
#define CHAR_ROT_SPEED 0.0005f

struct sGameScene {
    std::vector<sStage*> scene_stages;
    int curr_area;

    sGameCamera scene_cam;

    Matrix44 player_model;
    Mesh *player_body;
    Texture *player_texture;
    Shader *player_shader;

    sGameScene(); 

    void render_scene();
    void update_scene(float elapsed_time, uint8 pressed_keys);
};



#endif