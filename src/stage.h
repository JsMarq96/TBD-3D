/*
    By Juan S. Marquerie
*/
#ifndef _AREA_H_
#define _AREA_H_

#include <vector>
#include <string>
#include <iterator>
#include "extra/pathfinders.h"
#include "framework.h"
#include "shader.h"
#include "texture.h"
#include "game_player.h"
#include "mesh.h"
#include "enviorment_entity.h"
#include "game_map.h"

// Generic Game Area
struct sStage {
    int x, y;
    int width, heigth;

    std::string floor_img_dir;
    // To do: if the map gets to big, we can
    // subdivide it in different stages
    //std::vector<sStage*> next_areas;

    std::vector<sEnviormentEntity*> render_elements;

    Vector3 light_pos;

    // Map for AI navigation
    uint8 *map;
    
    sStage(int n_x, int n_y, int n_width, int n_heigh);

    void render_stage(Camera *camera);

    void add_instance(int type, Matrix44 model);

    void print_map() {
        std::cout << "====== MAP ======" << std::endl;
        for (int i = 0; i < width/2; i++) {
            for (int j = 0; j < heigth/2; j++) {
                std::cout << " " << std::to_string(map[(i * width/2) + j]) << " ";
            }
            std::cout << std::endl;
        }
        std::cout << "=================" << std::endl;
    };

    int add_element(std::string mesh_name, 
        std::string text_name, 
        std::string shader_fs,
        std::string shader_vs,
        bool is_instanced);
    int add_element(std::string elem_name);

    bool testStageCollisionsWith(Vector3 position, float radius, Vector3 &coll_pos, Vector3 &normal);
};

#endif