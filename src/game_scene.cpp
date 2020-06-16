#include "game_scene.h"


sGameScene::sGameScene() {
    curr_area = 0;
    srandom(time(NULL));

    player = sPlayer(Vector3(150/2, 0, 150/2));

    // Create test area
    sStage* test_stage = new sStage(0, 0, 150, 150);

    parse_stage(test_stage, "data/stages/01.stg");

    test_stage->print_map();

    test_stage->add_enemy_entity(Vector3(150/2 + 4, 0, 150/2));

    scene_stages.push_back(test_stage);
}

void sGameScene::render_scene() {
    Camera *curr_camera = Camera::current;
    // Render player
    player.get_camera(curr_camera);

    glClearColor(0.07, 0.07, 0.07, 1.0);

	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
    
    player.render(curr_camera);

    // TODO: set delimitation by fog?
    // Render area
    scene_stages[0]->render_stage(curr_camera);

    bullets.render(curr_camera);

    // Todo:  THICC FOG
    //player.render_camera_fog(curr_camera);
    // Render next areas??
    drawText(2, 2, "x:" + std::to_string(player.position.x) + " z:" + std::to_string(player.position.z), Vector3(1, 1, 1), 2);
}

void sGameScene::update_scene(float elapsed_time, uint8 pressed_keys) {
    // Move player
    player.update(elapsed_time);

    // Player Shoot
    bool is_pressed = Input::isMousePressed(SDL_BUTTON_LEFT);
    // Shoot bullet only when it is pressed
    if (is_pressed && prev_mouse_press != is_pressed && player.cam_mode == FIRST_PERSON) {
        if (bullets.add_bullet(player.position + Vector3(0,2,0), player.direction)) {
            player.shoot_animation();
        }
    }
    prev_mouse_press = is_pressed;

    // Update bullets
    bullets.update(elapsed_time);

    // Update the current stage (mostly for the enemys)
    scene_stages[0]->update_stage(elapsed_time, bullets, player.position);

    // Test for player collisions
    Vector3 coll_pos, coll_normal;
    if (scene_stages[0]->testStageCollisionsWith(player.position, .6f, coll_pos, coll_normal)) {
        Vector3 pos_delta = coll_normal * (coll_pos - player.position).length();
        player.position = player.position + pos_delta * elapsed_time;
    }
}