/*
    By Juan S. Marquerie
*/
#ifndef _AREA_H_
#define _AREA_H_

#include <vector>
#include <string>
#include "framework.h"
#include "shader.h"

#define ITEMS_PER_AREA 100

// Generic Rendering type
struct sRenderItems {
    std::string mesh_id;
    std::string texture_id;

    char *shader_vs_id;
    char *shader_fs_id;

    //bool is_rigid;
    Matrix44 models[ITEMS_PER_AREA];

    int last_inserted_index;

    sRenderItems() {
        shader_fs_id = "", shader_vs_id = "";
        mesh_id = "", last_inserted_index = -1;
    }
};

// Generic Game Area
struct sArea {
    int x, y;
    int width, heigth;
    std::vector<sArea*> next_areas;

    sRenderItems trees;
    sRenderItems houses;

    sArea(int n_x, int n_y) {
        x= n_x, y = n_y;
    }

    void render_area(Camera &cam);
    void add_tree(Matrix44 &tree_model);
    void add_house(Matrix44 &house_model);
};

void sArea::render_area(Camera &cam) {
    Shader *curr_shader = Shader::Get(shader_vs_id, shader_fs_id);

    curr_shader->enable();



    curr_shader->disable();
}

#endif