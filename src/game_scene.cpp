#include "game_scene.h"

Matrix44 setTranslationMat(int x, int y, int z) {
    Matrix44 new_mat = Matrix44();
    new_mat.setTranslation(x, y, z);

    return new_mat;
}

sGameScene::sGameScene() {
    curr_area = 0;
    
    player_model.setTranslation(0,0,0);
    player_shader = Shader::Get("data/shaders/basic.vs", "data/shaders/basic.vs");
    player_body->Get("meshes/player.obj");

    // Create test area
    sArea* test_area = new sArea(0, 0, 50, 50);

    Matrix44 tree1_model = Matrix44();
    Matrix44 tree2_model = Matrix44();
    Matrix44 tree3_model = Matrix44();
    Matrix44 house1_model = Matrix44();
    Matrix44 house2_model = Matrix44();

    tree1_model.setTranslation(20, 0, 0);
    tree2_model.setTranslation(15, 5, 0);
    tree3_model.setTranslation(30, 30, 0);
    house1_model.setTranslation(0, 50, 0);
    house2_model.setTranslation(50, 10, 0);

    test_area->add_tree(tree1_model);
    test_area->add_tree(tree2_model);
    test_area->add_tree(tree3_model);
    test_area->add_tree(house1_model);
    test_area->add_tree(house2_model);

    game_areas.push_back(test_area);
}

int sGameScene::create_area() {
    return -1;
}

void sGameScene::add_element_to_area() {}

void sGameScene::render_scene() {
    // Render player
    Camera* curr_camera = scene_cam.get_camera(player_model);

    
    if (scene_cam.cam_mode == THIRD_PERSON) {
        player_shader->enable();
        player_shader->setUniform("u_color", Vector4(1,1,1,1));
        player_shader->setUniform("u_viewprojection", curr_camera->viewprojection_matrix);
        player_shader->setUniform("u_texture", player_texture);
        player_shader->setUniform("u_model", player_model);
        player_body->drawCall(GL_TRIANGLES, 1, 0);
        player_shader->disable();
    }

    // TODO: set delimitation by fog?
    // Render area
    game_areas[curr_area]->render_area(scene_cam);
    // Render next areas??
}