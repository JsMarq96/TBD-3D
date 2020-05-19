/*
    By Juan S. Marquerie
*/
#ifndef _AREA_H_
#define _AREA_H_

#include <vector>
#include <string>
#include <iterator>
#include "framework.h"
#include "shader.h"
#include "texture.h"
#include "game_camera.h"
#include "mesh.h"

#define ITEMS_PER_AREA 100
#define DATA_DIR_LEN 40.f

// Generic Rendering type
struct sRenderItems {
    std::string mesh_id;
    std::string texture_id;
    std::string shader_vs_id;
    std::string shader_fs_id;

    //bool is_rigid;
    Matrix44 models[ITEMS_PER_AREA];

    int last_inserted_index;

    sRenderItems(std::string &shader_fs,
                 std::string &shader_vs,
                 std::string &mesh);

    sRenderItems() {
        shader_fs_id = "";
        shader_vs_id = "";
        mesh_id = "";
        last_inserted_index = -1;
    }

    bool add_element(Matrix44 &model);
};

enum AreaElements: uint8 {
    TREES_ID = 0,
    HOUSES_ID = 1
};

// Generic Game Area
struct sStage {
    int x, y;
    int width, heigth;
    std::vector<sStage*> next_areas;

    sRenderItems area_elements[2];

    std::vector<sRenderItems*> stage_elements;
    
    sStage(int n_x, int n_y, int n_width, int n_heigh);

    void render_stage(Camera *camera);
    void add_tree(Matrix44 tree_model);
    void add_house(Matrix44 house_model);

    void add_instance(int type, Matrix44 model);
    int add_element(std::string mesh_name, 
        std::string text_name, 
        std::string shader_fs,
        std::string shader_vs);
};

#endif