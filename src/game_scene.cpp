#include "game_scene.h"


sGameScene::sGameScene() {
    curr_area = 0;
    srandom(time(NULL));

    player = sPlayer(Vector3(20, 0, 20));
    sPlayer::instance = &player;

    // Create test area
    sStage* test_stage = new sStage(0, 0, 150, 150);

    parse_stage(test_stage, "data/stages/01.stg");

    test_stage->print_map();

    //test_stage->add_enemy_entity(Vector3(150/2 + 4, 0, 150/2));

    scene_stages.push_back(test_stage);
}

void sGameScene::render_scene() {
    Vector3 shoot_light_pos;
    bool has_shot;

    Camera *curr_camera = Camera::current;
    // Render player
    player.get_camera(curr_camera);

    has_shot = player.has_shoot(shoot_light_pos);

    glClearColor(0.07, 0.07, 0.07, 1.0);

	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

    skybox_render(curr_camera);
    
    player.render(curr_camera);

    // Render area
    scene_stages[0]->render_stage(curr_camera, has_shot, shoot_light_pos);

    drawText(2, 2, "Health: " + std::to_string(player.health) + "/5", Vector3(1, 1, 1), 2);
    drawText(2, 20, "Ammo: " + std::to_string(player.ammo) + "/5", Vector3(1, 1, 1), 2);

    //drawText(2, 2, "x:" + std::to_string(player.position.x) + " z:" + std::to_string(player.position.z), Vector3(1, 1, 1), 2);
}


void sGameScene::skybox_render(Camera* cam) {
    //Mesh *skybox = Mesh::Get("data/meshes/cilinder.obj");
    Mesh skybox;
    skybox.createCube();
    Shader *shad = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
    Vector3 player_pos = player.position;

    Matrix44 model;
    model.setTranslation(player_pos.x, 0.0f, player_pos.z);
    model.scale(90.0f, 90.0f, 90.0f);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

    shad->enable();
    shad->setUniform("u_color", Vector4(1,1,1,1));
    shad->setUniform("u_texture", Texture::Get("data/textures/skybox.png"));
    shad->setUniform("u_model", model);
    shad->setUniform("u_viewprojection", cam->viewprojection_matrix);
    skybox.render(GL_TRIANGLES);
    shad->disable();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

void sGameScene::update_scene(float elapsed_time, uint8 pressed_keys) {
    // Move player
    player.update(elapsed_time);

    // Update the current stage (mostly for the enemys)
    scene_stages[0]->update_stage(elapsed_time, player.bullets, player.position);

    // Test for player collisions
    Vector3 coll_pos, coll_normal;
    if (scene_stages[0]->testStageCollisionsWith(player.position, .6f, coll_pos, coll_normal)) {
        Vector3 pos_delta = coll_normal * (coll_pos - player.position).length() * 0.2;
        player.position = player.position + pos_delta * elapsed_time;
    }
}