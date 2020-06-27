/*
    By Juan S. Marquerie
*/
#ifndef _AREA_H_
#define _AREA_H_

#include <vector>
#include <string>
#include <iterator>
#include "extra/pathfinders.h"
#include "particles/bullet_particle.h"
#include "framework.h"
#include "shader.h"
#include "texture.h"
#include "game_player.h"
#include "mesh.h"
#include "enviorment_entity.h"
#include "enemy_entity.h"
#include "consum_entity.h"
#include "game_map.h"

// Generic Game Area
struct sStage {
    int x, y;
    int width, heigth;

    eConsumType has_found_elem;

    std::string floor_img_dir;
    // To do: if the map gets to big, we can
    // subdivide it in different stages
    //std::vector<sStage*> next_areas;

    std::vector<sEnviormentEntity*> render_elements;
    
    sEnemyEntity enemys;

    sConsumableEntities cons;

    Vector3 light_pos;

    // Map for AI navigation
    sGameMap map;
    
    sStage(int n_x, int n_y, int n_width, int n_heigh);

    void render_stage(Camera *camara, bool double_light = false, Vector3 sec_light = Vector3());

    void update_stage(float elapsed_time, sBulletEntity &bullet_controller, Vector3 player_position);

    void add_enviorment_instance(int type, Matrix44 model);

    void add_enemy_entity(Vector3 position);

    void print_map() {
        map.print_map();
    };

    int add_element(std::string mesh_name, 
        std::string text_name, 
        std::string shader_fs,
        std::string shader_vs,
        bool is_instanced);
    int add_element(std::string elem_name);

    bool testStageCollisionsWith(Vector3 position, float radius, Vector3 &coll_pos, Vector3 &normal);

    bool testStageParticleCollisions(Vector3* origins, Vector3* direction, int part_num, float lenght);
};

#endif