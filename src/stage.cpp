#include "stage.h"

// Todo: frustum coling
void sStage::render_stage(Camera *camera, bool double_light, Vector3 sec_light) {
    // First, we render the floor
    Mesh floor_mesh = Mesh();
    floor_mesh.createPlane(150);
    Shader *shad = Shader::Get("data/shaders/basic.vs", "data/shaders/phong.ps");
    shad->enable();
    shad->setUniform("u_texture", Texture::Get(floor_img_dir.c_str()));
    shad->setUniform("u_color", Vector4(1, 1, 1, 1));
    shad->setUniform("u_viewprojection", camera->viewprojection_matrix);
    shad->setUniform("u_model", Matrix44());
    shad->setUniform("u_light_pos", Vector3(250, 120, 250));
    shad->setUniform("u_camera_pos", camera->eye);
    shad->setUniform("u_double_light", double_light);
    shad->setUniform("u_second_light_pos", sec_light);
    floor_mesh.render(GL_TRIANGLES);
    shad->disable();

    // Render enviorment elements elements
    std::vector<sEnviormentEntity*>::iterator it;
    for (it = render_elements.begin(); it < render_elements.end(); it++) {
        sEnviormentEntity* curr_item = *it;
        curr_item->render(camera, double_light, sec_light);
    }

    // Render enemys
    enemys.render(camera);
}

void sStage::update_stage(float elapsed_time, sBulletEntity &bullet_controller, Vector3 player_position) {
    // Update the kinectic objects
    enemys.testBulletCollisions(bullet_controller);

    enemys.update(elapsed_time, map, player_position);
}

void sStage::add_enviorment_instance(int type, Matrix44 model) {
    render_elements[type]->add_element(model);
    
    Vector3 position = model.getTranslation();

    // Add scaled area
    map.add_area(position.x, position.z, render_elements[type]->radius);
    //map[int((position.x * width/2) + position.z)] = 1;
}

void sStage::add_enemy_entity(Vector3 position) {
    enemys.add_element(position);
}

int sStage::add_element(std::string mesh_name, std::string text_name, std::string shader_fs, std::string shader_vs, bool is_instanced) {
    sEnviormentEntity* new_elem = new sEnviormentEntity(shader_fs, shader_vs, mesh_name, text_name, is_instanced);

    render_elements.insert(render_elements.end(), new_elem);
    
    return render_elements.size()-1;
}

int sStage::add_element(std::string elem_name) {
    int index = -1;
    if (elem_name == "tree.obj") {
        index = add_element("data/meshes/tree.obj","data/textures/tree.png","data/shaders/phong.ps", "data/shaders/instanced.vs", true);
        render_elements[index]->set_collider_as_cilinder(1.0);
    } else if (elem_name == "tree1.obj") {
        index = add_element("data/meshes/tree1.obj","data/textures/tree_text.png","data/shaders/phong.ps", "data/shaders/instanced.vs", true);
        render_elements[index]->set_collider_as_cilinder(1.0);
    } else if (elem_name == "tree2.obj") {
        index = add_element("data/meshes/tree2.obj","data/textures/tree_text.png","data/shaders/phong.ps", "data/shaders/instanced.vs", true);
        render_elements[index]->set_collider_as_cilinder(1.0);
    } else if (elem_name == "rock.obj") {
        index = add_element("data/meshes/rock.obj","data/textures/rock.png","data/shaders/phong.ps", "data/shaders/instanced.vs", true);
        render_elements[index]->set_collider_as_cilinder(1.5);
    } else if (elem_name == "rock2.obj") {
        index = add_element("data/meshes/rock2.obj","data/textures/rock.png","data/shaders/phong.ps", "data/shaders/instanced.vs", true);
        render_elements[index]->set_collider_as_cilinder(1.5);
    }else if (elem_name == "enemy.obj") {
    }

    return index;
}

sStage::sStage(int n_x, int n_y, int n_width, int n_heigh) {
    width = n_width, heigth = n_heigh;

    // Set default light position
    light_pos = Vector3(250, 120, 250);

    map = sGameMap(width, heigth);
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

// Todo: Accesory in order to add bullet holes... If i have the time
bool sStage::testStageParticleCollisions(Vector3* origins, Vector3* direction, int part_num, float lenght) {
    std::vector<sEnviormentEntity*>::iterator it;
    // Render elements
    for (it = render_elements.begin(); it < render_elements.end(); it++) {
        sEnviormentEntity* curr_item = *it;
        
        //curr_item->
        //if (curr_item->testCollisionsWith(position, radius, coll_pos, normal)) {
        //    return true;
        //}
    }
    return false;
}
