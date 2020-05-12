#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include <vector>
#include "framework.h"
#include "game_camera.h"
#include "mesh.h"
#include "area.h"

struct sGameScene {
    std::vector<sArea*> game_areas;
    int curr_area;

    sGameCamera scene_cam;

    Matrix44 player_model;
    Mesh *player_body;
    Texture *player_texture;
    Shader *player_shader;

    sGameScene(); 

    int create_area();
    void add_element_to_area();
    void render_scene();
};



#endif