#include "stage.h"

// Todo: frustum coling
void sStage::render_stage(Camera *camera) {
    // First, we render the floor
    Mesh floor_mesh = Mesh();
    floor_mesh.createPlane(150);

    // Draw floor
    Shader *shad = Shader::Get("data/shaders/basic.vs", "data/shaders/phong.ps");
    shad->enable();
    shad->setUniform("u_texture", Texture::Get(floor_img_dir.c_str()));
    shad->setUniform("u_color", Vector4(1, 1, 1, 1));
    shad->setUniform("u_viewprojection", camera->viewprojection_matrix);
    shad->setUniform("u_model", Matrix44());
    shad->setUniform("light_pos", Vector3(250, 120, 250));
    shad->setUniform("camera_pos", camera->eye);

    floor_mesh.render(GL_TRIANGLES);
    shad->disable();

    std::vector<sEnviormentEntity*>::iterator it;
    // Render elements
    for (it = render_elements.begin(); it < render_elements.end(); it++) {
        sEnviormentEntity* curr_item = *it;
        curr_item->render(camera);
    }
}

void sStage::add_instance(int type, Matrix44 model) {
    render_elements[type]->add_element(model);
    
    Vector3 position = model.getTranslation() *0.5;

    map[int((position.x * width/2) + position.z)] = 1;
}

int sStage::add_element(std::string mesh_name, std::string text_name, std::string shader_fs, std::string shader_vs, bool is_instanced) {
    sEnviormentEntity* new_elem = new sEnviormentEntity(shader_fs, shader_vs, mesh_name, text_name, is_instanced);

    render_elements.insert(render_elements.end(), new_elem);
    
    return render_elements.size()-1;
}

int sStage::add_element(std::string elem_name) {
    int index = -1;
    std::cout << elem_name << std::endl;
    if (elem_name == "tree.obj") {
        index = add_element("data/meshes/tree.obj","data/textures/tree.png","data/shaders/phong.ps", "data/shaders/instanced.vs", true);
        render_elements[index]->set_collider_as_cilinder(1.0);
    } else if (elem_name == "house.obj") {
        index = add_element("data/house_f2.obj","data/textures/tree.png","data/shaders/flat.fs", "data/shaders/basic.vs", false);
        render_elements[index]->set_collider_as_mesh();
    } 

    return index;
}

sStage::sStage(int n_x, int n_y, int n_width, int n_heigh) {
    x= n_x, y = n_y;
    width = n_width, heigth = n_heigh;

    // Set default light position
    light_pos = Vector3(250, 120, 250);

    map = new uint8[(width * heigth)/2];

    for (int i = 0; i< (width * heigth)/2; i++) {
        map[i] = 0;
    }
}

bool sStage::testStageCollisionsWith(Vector3 position, float radius, Vector3 &coll_pos, Vector3 &normal) {
    std::vector<sEnviormentEntity*>::iterator it;
    // Render elements
    for (it = render_elements.begin(); it < render_elements.end(); it++) {
        sEnviormentEntity* curr_item = *it;
        
        if (curr_item->testCollisionsWith(position, radius, coll_pos, normal)) {
            return true;
        }
    }
    return false;
}
