#include "area.h"

sRenderItems::sRenderItems(std::string &shader_vs,
                 std::string &shader_fs,
                 std::string &mesh) {
/*sRenderItems::sRenderItems(const char* shader_vs,
                 const char* shader_fs,
                 const char* mesh) {
    shader_fs_id = new char[DATA_DIR_LEN];
    shader_vs_id = new char[DATA_DIR_LEN];
    mesh_id = new char[DATA_DIR_LEN];
    
    strcpy((char*) &shader_vs_id[0], shader_vs);
    strcpy((char*) &shader_fs_id[0], shader_fs);
    strcpy((char*) &mesh_id[0], mesh);*/

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
void sArea::render_area(Camera *camera) {
    // Render elements
    for (int j = 0; j < 2; j++) {
        // Load shader
        Shader *curr_shader = Shader::Get(area_elements[j].shader_vs_id.c_str(), area_elements[j].shader_fs_id.c_str());
        Mesh *mesh = Mesh::Get(area_elements[j].mesh_id.c_str());
        curr_shader->enable();
        
        curr_shader->setUniform("u_color", Vector4(1, 1, 1, 1));
        curr_shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
        //curr_shader->setUniform("u_texture", Texture::Get(area_elements[j].texture_id.c_str()));

        mesh->enableBuffers(curr_shader);

        for (int i = 0; i < area_elements[j].last_inserted_index; i++) {
            curr_shader->setUniform("u_model", area_elements[j].models[i]);
            mesh->drawCall(GL_TRIANGLES, 1, 0);
        }

        mesh->disableBuffers(curr_shader);
        curr_shader->disable();
    }
}

void sArea::add_tree(Matrix44 tree_model) {
    area_elements[TREES_ID].add_element(tree_model);
}

void sArea::add_house(Matrix44 house_model) {
    area_elements[HOUSES_ID].add_element(house_model);
}

sArea::sArea(int n_x, int n_y, int n_width, int n_heigh) {
    x= n_x, y = n_y;
    width = n_width, heigth = n_heigh;

    std::string vs_shader = "data/shaders/basic.vs", fs_shader = "data/shaders/flat.fs", tree_mesh = "data/meshes/tree.obj";

    area_elements[TREES_ID] = sRenderItems(fs_shader, vs_shader, tree_mesh);
    area_elements[HOUSES_ID] =  sRenderItems(fs_shader, vs_shader, tree_mesh);
}