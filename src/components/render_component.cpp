#include "render_component.h"

// Initialize the Render compoenet
sRenderComponent::sRenderComponent(std::string &shader_vs,
                 std::string &shader_fs,
                 std::string &mesh) {
    shader_fs_id = shader_vs;
    shader_vs_id = shader_fs;
    mesh_id = mesh;

    last_inserted_index = -1;
}

bool sRenderComponent::add_element(Matrix44 &new_model) {
    if (last_inserted_index+1 == ITEMS_PER_AREA)
        return false;
    last_inserted_index++;
    
    models[last_inserted_index] = new_model;
    return true;
}

void sRenderComponent::render(Camera *camera) {
    Shader *curr_shader = Shader::Get(shader_vs_id.c_str(), shader_fs_id.c_str());
        Mesh *mesh = Mesh::Get(mesh_id.c_str());
        curr_shader->enable();
        
        curr_shader->setUniform("u_color", Vector4(1, 1, 1, 1));
        curr_shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
        //curr_shader->setUniform("u_texture", Texture::Get(area_elements[j].texture_id.c_str()));

        mesh->enableBuffers(curr_shader);

        for (int i = 0; i < last_inserted_index; i++) {
            curr_shader->setUniform("u_model", models[i]);
            mesh->drawCall(GL_TRIANGLES, 1, 0);
        }

        mesh->disableBuffers(curr_shader);
        curr_shader->disable();
}

bool sRenderComponent::testCollisionsWith(Vector3 position, float radius, Vector3 &normal) {
    Mesh *mesh = Mesh::Get(mesh_id.c_str());
    Vector3 collision_vector;
    for (int i = 0; i < last_inserted_index; i++) {
        if (mesh->testSphereCollision(models[i], position, radius, collision_vector, normal)) {
            return true;
        }
    }
    
    return false;
}
