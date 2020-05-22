#include "render_entity.h"

// Initialize the Render compoenet
sRenderEntity::sRenderEntity(std::string &shader_vs,
                 std::string &shader_fs,
                 std::string &mesh,
                 std::string &text_id,
                 eColliderType type,
                 bool is_instanced) {
    shader_fs_id = shader_vs;
    shader_vs_id = shader_fs;
    texture_id = text_id;
    mesh_id = mesh;
    col_type = type;
    fl_instancing = is_instanced;

    last_inserted_index = -1;
}

bool sRenderEntity::add_element(Matrix44 &new_model) {
    if (last_inserted_index+1 == ITEMS_PER_AREA)
        return false;
    last_inserted_index++;
    
    models[last_inserted_index] = new_model;
    return true;
}

void sRenderEntity::render(Camera *camera) {
    Shader *curr_shader = Shader::Get(shader_vs_id.c_str(), shader_fs_id.c_str());
    Mesh *mesh = Mesh::Get(mesh_id.c_str());
    curr_shader->enable();
        
    curr_shader->setUniform("u_color", Vector4(1, 1, 1, 1));
    curr_shader->setUniform("u_texture", Texture::Get(texture_id.c_str()));
    curr_shader->setUniform("light_pos", Vector3(250, 120, 250));
    curr_shader->setUniform("camera_pos", camera->eye);
    curr_shader->setUniform("u_viewprojection", camera->viewprojection_matrix);

    // If the entity is rednered via instancing
    if (fl_instancing) {
        mesh->renderInstanced(GL_TRIANGLES, models, last_inserted_index-1);
        curr_shader->disable();
        return;
    }


    mesh->enableBuffers(curr_shader);

    for (int i = 0; i <= last_inserted_index; i++) {
        curr_shader->setUniform("u_model", models[i]);
        mesh->drawCall(GL_TRIANGLES, 1, 0);
    }

    mesh->disableBuffers(curr_shader);
    curr_shader->disable();
}

// Todo: generalize
bool ciliderCollider(Vector3 pos1, float radius1, Vector3 pos2, float radius2, Vector3 &coll_pos, Vector3 &normal_coll) {
    pos1.y = 0.;
    Vector3 diff = pos2 - pos1;
    float inter_center_distance = diff.length();
    if (float tmp = (inter_center_distance - radius1 - radius2) <= 0) {
        coll_pos = pos2 + (diff.normalize() * radius2);
        normal_coll = (coll_pos - pos2).normalize();
        return true;
    }
    return false;
}

bool sRenderEntity::testCollisionsWith(Vector3 position, float radius, Vector3 &coll_pos, Vector3 &normal) {
    Mesh *mesh = Mesh::Get(mesh_id.c_str());
    for (int i = 0; i <= last_inserted_index; i++) {
        bool res;
        Vector3 mesh_center;
        switch(col_type) {
            case MESH:
                res = mesh->testSphereCollision(models[i], position, radius, coll_pos, normal);
                break;
            case CILINDER:
                mesh_center = Vector3(models[i].m[12], 0, models[i].m[14]);
                res = ciliderCollider(position, radius, mesh_center, radius, coll_pos, normal);
                break;
        }
        if (res) {
            return true;
        }
    }
    
    return false;
}