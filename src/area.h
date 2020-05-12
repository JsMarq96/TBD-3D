/*
    By Juan S. Marquerie
*/
#ifndef _AREA_H_
#define _AREA_H_

#include <vector>
#include <string>
#include "framework.h"
#include "shader.h"
#include "texture.h"
#include "game_camera.h"
#include "mesh.h"

#define ITEMS_PER_AREA 100

// Generic Rendering type
struct sRenderItems {
    const char* mesh_id;
    const char* texture_id;

    const char *shader_vs_id;
    const char *shader_fs_id;

    //bool is_rigid;
    Matrix44 models[ITEMS_PER_AREA];

    int last_inserted_index;

    sRenderItems(const char* shader_fs,
                 const char* shader_vs,
                 const char* mesh);

    sRenderItems() {
        shader_fs_id = "", shader_vs_id = "";
        mesh_id = "", last_inserted_index = -1;
    }

    bool add_element(Matrix44 &model);
};

enum AreaElements: uint8 {
    TREES_ID = 0,
    HOUSES_ID = 1
};

// Generic Game Area
struct sArea {
    int x, y;
    int width, heigth;
    std::vector<sArea*> next_areas;

    sRenderItems area_elements[2];

    sArea(int n_x, int n_y, int n_width, int n_heigh);

    void render_area(sGameCamera &cam);
    void add_tree(Matrix44 &tree_model);
    void add_house(Matrix44 &house_model);
};

#endif