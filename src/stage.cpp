#include "stage.h"

sRenderItems::sRenderItems(std::string &shader_vs,
                 std::string &shader_fs,
                 std::string &mesh) {
    shader_fs_id = shader_vs;
    shader_vs_id = shader_fs;
    mesh_id = mesh;

    last_inserted_index = -1;
}

bool sRenderItems::add_element(Matrix44 &new_model) {
    if (last_inserted_index+1 == ITEMS_PER_AREA)
        return false;
    last_inserted_index++;
    models[last_inserted_index] = new_model;
    return true;
}

// Todo: frustum coling
void sStage::render_area(Camera *camera) {
    std::vector<sRenderItems*>::iterator it;
    // Render elements
    for (it = stage_elements.begin(); it < stage_elements.end(); it++) {
        // Load shader
        sRenderItems* curr_item = *it;
        Shader *curr_shader = Shader::Get(curr_item->shader_vs_id.c_str(), curr_item->shader_fs_id.c_str());
        Mesh *mesh = Mesh::Get(curr_item->mesh_id.c_str());
        curr_shader->enable();
        
        curr_shader->setUniform("u_color", Vector4(1, 1, 1, 1));
        curr_shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
        //curr_shader->setUniform("u_texture", Texture::Get(area_elements[j].texture_id.c_str()));

        mesh->enableBuffers(curr_shader);

        for (int i = 0; i < curr_item->last_inserted_index; i++) {
            curr_shader->setUniform("u_model", curr_item->models[i]);
            mesh->drawCall(GL_TRIANGLES, 1, 0);
        }

        mesh->disableBuffers(curr_shader);
        curr_shader->disable();
    }
}

void sStage::add_tree(Matrix44 tree_model) {
    area_elements[TREES_ID].add_element(tree_model);
}

void sStage::add_house(Matrix44 house_model) {
    area_elements[HOUSES_ID].add_element(house_model);
}

void sStage::add_instance(int type, Matrix44 model) {
    stage_elements[type]->add_element(model);
}

int sStage::add_element(std::string mesh_name, std::string text_name, std::string shader_fs, std::string shader_vs) {
    sRenderItems* new_elem = new sRenderItems(shader_fs, shader_vs, mesh_name);

    stage_elements.push_back(new_elem);
    
    return stage_elements.size() - 1;
}

sStage::sStage(int n_x, int n_y, int n_width, int n_heigh) {
    x= n_x, y = n_y;
    width = n_width, heigth = n_heigh;

    std::string vs_shader = "data/shaders/basic.vs", fs_shader = "data/shaders/flat.fs", tree_mesh = "data/meshes/tree.obj";

    area_elements[TREES_ID] = sRenderItems(fs_shader, vs_shader, tree_mesh);
    area_elements[HOUSES_ID] =  sRenderItems(fs_shader, vs_shader, tree_mesh);
}