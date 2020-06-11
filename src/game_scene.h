#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include <vector>
#include "stage_parser.h"
#include "framework.h"
#include "game_player.h"
#include "mesh.h"
#include "stage.h"
#include "input.h"
#include "particles/bullet_particle.h"

#define PRESS_W 0b00001
#define PRESS_A 0b00010
#define PRESS_S 0b00100
#define PRESS_D 0b01000

#define CHAR_SPEED 60.f
#define CHAR_POINTED_SPEED 0.2f
#define CHAR_ROT_SPEED 0.0005f
#define CHAR_ROT_POINT_SPEED 0.00005f

struct sGameScene {
    std::vector<sStage*> scene_stages;
    int curr_area;

    sPlayer player;
    sBulletEntity bullets;

    bool prev_mouse_press = false;

    sGameScene(); 

    void render_scene();
    void update_scene(float elapsed_time, uint8 pressed_keys);
};



#endif