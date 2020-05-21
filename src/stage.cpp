#include "stage.h"

// Todo: frustum coling
void sStage::render_stage(Camera *camera) {
    std::vector<sRenderComponent*>::iterator it;
    // Render elements
    for (it = render_elements.begin(); it < render_elements.end(); it++) {
        sRenderComponent* curr_item = *it;
        curr_item->render(camera);
    }
}

void sStage::add_instance(int type, Matrix44 model) {
    render_elements[type]->add_element(model);
}

int sStage::add_element(std::string mesh_name, std::string text_name, std::string shader_fs, std::string shader_vs) {
    sRenderComponent* new_elem = new sRenderComponent(shader_fs, shader_vs, mesh_name);

    render_elements.insert(render_elements.end(), new_elem);
    
    return render_elements.size()-1;
}

int sStage::add_element(std::string elem_name) {
    int index = -1;
    std::cout << elem_name << std::endl;
    if (elem_name == "tree.obj") {
        index = add_element("data/meshes/tree.obj","data/textures","data/shaders/flat.fs", "data/shaders/basic.vs");
    } else if (elem_name == "house.obj") {
        index = add_element("data/meshes/house.obj","data/textures","data/shaders/flat.fs", "data/shaders/basic.vs");
    }

    return index;
}

sStage::sStage(int n_x, int n_y, int n_width, int n_heigh) {
    x= n_x, y = n_y;
    width = n_width, heigth = n_heigh;
}