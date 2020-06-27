#include "enviorment_entity.h"

// Initialize the Render compoenet
sEnviormentEntity::sEnviormentEntity(std::string &shader_vs,
                 std::string &shader_fs,
                 std::string &mesh,
                 std::string &text_id,
                 bool is_instanced) {
    shader_fs_id = shader_vs;
    shader_vs_id = shader_fs;
    texture_id = text_id;
    mesh_id = mesh;
    fl_instancing = is_instanced;

    last_inserted_index = -1;
}

bool sEnviormentEntity::add_element(Matrix44 &new_model) {
    if (last_inserted_index+1 == ITEMS_PER_AREA)
        return false;
    last_inserted_index++;
    if (fl_instancing) {
        float angle = rand() * 180;
        //new_model.rotate(angle, Vector3(0,1,0));
    }
    models[last_inserted_index] = new_model;
    return true;
}

void sEnviormentEntity::render(Camera *camera, bool double_light, Vector3 sec_light) {
    Shader *curr_shader = Shader::Get(shader_vs_id.c_str(), shader_fs_id.c_str());
    Mesh *mesh = Mesh::Get(mesh_id.c_str());
    curr_shader->enable();
        
    curr_shader->setUniform("u_color", Vector4(1, 1, 1, 1));
    curr_shader->setUniform("u_texture", Texture::Get(texture_id.c_str()));
    curr_shader->setUniform("u_light_pos", Vector3(250, 120, 250));
    curr_shader->setUniform("u_camera_pos", camera->eye);
    curr_shader->setUniform("u_double_light", double_light);
    curr_shader->setUniform("u_second_light_pos", sec_light);
    curr_shader->setUniform("u_viewprojection", camera->viewprojection_matrix);

    // If the entity is rednered via instancing
    if (fl_instancing) {
        mesh->renderInstanced(GL_TRIANGLES, models, last_inserted_index);
        curr_shader->disable();
        return;
    }

    mesh->enableBuffers(curr_shader);

    for (int i = 0; i <= last_inserted_index; i++) {
        curr_shader->setUniform("u_model", models[i]);
        mesh->render(GL_TRIANGLES);
    }

    mesh->disableBuffers(curr_shader);
    curr_shader->disable();
}

// Todo: generalize
bool ciliderCollider(Vector3 pos1, float radius1, Vector3 pos2, float radius2, Vector3 &coll_pos, Vector3 &normal_coll) {
    pos1.y = 0.;
    // Distance vector between the 2 centers
    Vector3 diff = pos1 - pos2;
    float inter_center_distance = diff.length();

    if (float tmp = (inter_center_distance - radius1 - radius2) <= 0) {
        // Get collision vector
        coll_pos.x = ((pos1.x + radius2) + (pos2.x * radius1)) / (radius1 + radius2);
        coll_pos.y = ((pos1.y + radius2) + (pos2.y * radius1)) / (radius1 + radius2);
        // Vector from the collision point to the center of the o
        normal_coll = (pos1 - pos2).normalize();
        return true;
    }
    return false;
}

bool sEnviormentEntity::testCollisionsWith(Vector3 position, float adv_radius, Vector3 &coll_pos, Vector3 &normal) {
    Mesh *mesh = Mesh::Get(mesh_id.c_str());
    for (int i = 0; i <= last_inserted_index; i++) {
        bool res;
        Vector3 mesh_center;
        switch(col_type) {
            case MESH:
                res = mesh->testSphereCollision(models[i], position, adv_radius, coll_pos, normal);
                break;
            case CILINDER:
                mesh_center = Vector3(models[i].m[12], 0, models[i].m[14]);
                res = ciliderCollider(position, adv_radius, mesh_center, radius, coll_pos, normal);
                break;
        }
        if (res) {
            return true;
        }
    }
    
    return false;
}