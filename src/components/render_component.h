#ifndef _RENDER_COMPONENT_H_
#define _RENDER_COMPONENT_H_

#include <string>
#include "../framework.h"
#include "../shader.h"
#include "../mesh.h"
#include "../texture.h"
#include "../camera.h"

#define ITEMS_PER_AREA 100
#define DATA_DIR_LEN 40.f


// Generic Rendering type
struct sRenderComponent {
    std::string mesh_id;
    std::string texture_id;
    std::string shader_vs_id;
    std::string shader_fs_id;

    Matrix44 models[ITEMS_PER_AREA];

    int last_inserted_index;

    sRenderComponent(std::string &shader_fs,
                 std::string &shader_vs,
                 std::string &mesh);

    sRenderComponent() {
        shader_fs_id = "";
        shader_vs_id = "";
        mesh_id = "";
        last_inserted_index = -1;
    }

    bool add_element(Matrix44 &model);
    void render(Camera *camara);
    bool testCollisionsWith(Vector3 position, float radius, Vector3 &normal);
    //bool testCollisions();
};

#endif