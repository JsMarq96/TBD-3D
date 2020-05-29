#include "game_scene.h"


sGameScene::sGameScene() {
    curr_area = 0;    
    // Create test area
    sStage* test_stage = new sStage(0, 0, 50, 50);

    parse_stage(test_stage, "data/stages/01.stg");

    scene_stages.push_back(test_stage);
}

void sGameScene::render_scene() {
    Camera *curr_camera = Camera::current;
    // Render player
    player.get_camera(curr_camera);

    glClearColor(0.1, 0.1, 0.1, 1.0);

	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
    
    player.render(curr_camera);

    // TODO: set delimitation by fog?
    // Render area
    scene_stages[0]->render_stage(curr_camera);
    // Render next areas??
    //drawText(2, 2, "x:" + std::to_string(player_model.m[12]) + " y:" + std::to_string(player_model.m[13]) + " z:" + std::to_string(player_model.m[14]), Vector3(1, 1, 1), 2);
}

void sGameScene::update_scene(float elapsed_time, uint8 pressed_keys) {
    // Get the direction and the rotation of the displacement
    player.rotation.y = player.rotation.y - (Input::mouse_delta.x * CHAR_ROT_SPEED);
    player.speed = Vector3(0,0,0);
    if (Input::isKeyPressed(SDL_SCANCODE_W)) {
        player.speed = player.speed + Vector3(.0f, .0f, .-1f * CHAR_SPEED);
    } else if (Input::isKeyPressed(SDL_SCANCODE_S)) {
        player.speed =  player.speed + Vector3(.0f, .0f, .1f * CHAR_SPEED);
    }

    if (Input::isKeyPressed(SDL_SCANCODE_A)) {
        player.speed = player.speed + Vector3(-.1f * CHAR_SPEED, .0f, .0f);
    } else if (Input::isKeyPressed(SDL_SCANCODE_D)) {
        player.speed = player.speed + Vector3(.1f * CHAR_SPEED, .0f, .0f);
    }

    // Manage camera switching
    player.cam_mode = THIRD_PERSON;
    if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT) || Input::isKeyPressed(SDL_SCANCODE_RSHIFT))
        player.cam_mode = FIRST_PERSON;

    player.calculate_next_step(elapsed_time);
    /*// Test collitions
    // Set elevated player position
    // Todo: generalize to kinetic entities
    Vector3 player_position = Vector3(player_model.m[12], 0, player_model.m[14]);
    Vector3 collison_position, collision_normals;
    if (scene_stages[0]->testStageCollisionsWith(player_position, 1.0, collison_position, collision_normals)) { 
        collison_position.y = 0;   
        // Substract the margin to the 
        Vector3 player_col = (player_position - collison_position);
        //Vector3 new_position = old_player_position - push;

        // Reflect collision
        speed = speed -2*(dot(speed, collision_normals)) * collision_normals;
        Vector3 delta_mov = player_col -2*(dot(player_col, collision_normals)) * collision_normals;
        
        delta_mov = delta_mov.normalize() * 0.8;

        player_model.translate(delta_mov.x, 0., delta_mov.z);
    }

    // Update positions
    speed.y = 0;
    old_player_position = Vector3(player_model.m[12],  1.0, player_model.m[14]);
    speed = speed * elapsed_time;
    player_model.translate(speed.x, speed.y, speed.z);
    speed = Vector3(0,0,0);*/

    
}