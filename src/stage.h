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
#include "render_entity.h"


// Generic Game Area
struct sStage {
    int x, y;
    int width, heigth;
    // To do: if the map gets to big, we can
    // subdivide it in different stages
    std::vector<sStage*> next_areas;

    std::vector<sRenderEntity*> render_elements;

    //std::vector<sColiders> collider_elements;
    
    sStage(int n_x, int n_y, int n_width, int n_heigh);

    void render_stage(Camera *camera);

    void add_instance(int type, Matrix44 model);

    int add_element(std::string mesh_name, 
        std::string text_name, 
        std::string shader_fs,
        std::string shader_vs,
        eColliderType col_type);
    int add_element(std::string elem_name);

    bool testStageCollisionsWith(Vector3 position, float radius, Vector3 &normal);
};

#endif