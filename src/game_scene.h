#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include <vector>
#include "framework.h"
#include "game_camera.h"
#include "mesh.h"
#include "area.h"
#include "input.h"

#define PRESS_W 0b00001
#define PRESS_A 0b00010
#define PRESS_S 0b00100
#define PRESS_D 0b01000

#define CHAR_SPEED 10

struct sGameScene {
    std::vector<sArea*> game_areas;
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