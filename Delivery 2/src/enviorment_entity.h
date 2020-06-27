#ifndef _RENDER_COMPONENT_H_
#define _RENDER_COMPONENT_H_

#include <string>
#include "framework.h"
#include "shader.h"
#include "mesh.h"
#include "texture.h"
#include "camera.h"

#define ITEMS_PER_AREA 220
#define DATA_DIR_LEN 40.f

enum eColliderType {
    MESH = 0,
    CILINDER = 1,
    BOX = 2
};

// Generic Rendering type
struct sEnviormentEntity {
    std::string mesh_id;
    std::string texture_id;
    std::string shader_vs_id;
    std::string shader_fs_id;
    bool fl_instancing;

    eColliderType col_type;
    float radius;

    Matrix44 models[ITEMS_PER_AREA];

    int last_inserted_index;

    sEnviormentEntity(std::string &shader_fs,
                 std::string &shader_vs,
                 std::string &mesh,
                 std::string &text_id,
                 bool is_instanced);

    sEnviormentEntity() {
        shader_fs_id = "";
        shader_vs_id = "";
        mesh_id = "";
        last_inserted_index = -1;
        col_type = MESH;
    }

    bool add_element(Matrix44 &model);
    void render(Camera *camara, bool double_light, Vector3 sec_light);
    bool testCollisionsWith(Vector3 position, float radius, Vector3 &coll_pos, Vector3 &normal);

    void set_collider_as_cilinder(float c_radius) { col_type = CILINDER; radius = c_radius; };
    void set_collider_as_mesh() { col_type = MESH; };
    //bool testCollisions();
};

#endif