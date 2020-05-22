#include "stage.h"

// Todo: frustum coling
void sStage::render_stage(Camera *camera) {
    std::vector<sRenderEntity*>::iterator it;
    // Render elements
    for (it = render_elements.begin(); it < render_elements.end(); it++) {
        sRenderEntity* curr_item = *it;
        curr_item->render(camera);
    }
}

void sStage::add_instance(int type, Matrix44 model) {
    render_elements[type]->add_element(model);
}

int sStage::add_element(std::string mesh_name, std::string text_name, std::string shader_fs, std::string shader_vs, eColliderType col_type) {
    sRenderEntity* new_elem = new sRenderEntity(shader_fs, shader_vs, mesh_name, text_name, col_type);

    render_elements.insert(render_elements.end(), new_elem);
    
    return render_elements.size()-1;
}

int sStage::add_element(std::string elem_name) {
    int index = -1;
    std::cout << elem_name << std::endl;
    if (elem_name == "tree.obj") {
        index = add_element("data/meshes/tree.obj","data/textures/tree.png","data/shaders/phong.ps", "data/shaders/basic.vs", CILINDER);
    } else if (elem_name == "house.obj") {
        index = add_element("data/house_f2.obj","data/textures/tree.png","data/shaders/flat.fs", "data/shaders/basic.vs", MESH);
    } 

    return index;
}

sStage::sStage(int n_x, int n_y, int n_width, int n_heigh) {
    x= n_x, y = n_y;
    width = n_width, heigth = n_heigh;

    // Set default light position
    light_pos = Vector3(250, 60, 250);
}

bool sStage::testStageCollisionsWith(Vector3 position, float radius, Vector3 &coll_pos, Vector3 &normal) {
    std::vector<sRenderEntity*>::iterator it;
    // Render elements
    for (it = render_elements.begin(); it < render_elements.end(); it++) {
        sRenderEntity* curr_item = *it;
        
        if (curr_item->testCollisionsWith(position, radius, coll_pos, normal)) {
            return true;
        }
    }
    return false;
}
