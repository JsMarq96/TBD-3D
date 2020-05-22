#include "game_scene.h"


sGameScene::sGameScene() {
    curr_area = 0;
    speed = Vector3(0,0,0);
    
    player_model.setTranslation(0,0,0);
    player_shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
    //player_body = Mesh::Get("data/spitfire/spitfire.ASE");
    player_body = Mesh::Get("data/meshes/player_t1.obj");
    player_arm = Mesh::Get("data/meshes/player_arm.obj");
    player_texture = Texture::Get("data/torpedo.tga");
    //player_texture = Texture::Get("data/spitfire/spitfire_color_spec.tga");

    // Create test area
    sStage* test_stage = new sStage(0, 0, 50, 50);

    parse_stage(test_stage, "data/stages/01.stg");

    scene_stages.push_back(test_stage);
}

void sGameScene::render_scene() {
    Camera *curr_camera = Camera::current;
    // Render player
    scene_cam.get_camera(curr_camera, player_model);

    glClearColor(0.0, 0.0, 0.0, 1.0);

	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
    
    if (scene_cam.cam_mode == THIRD_PERSON) {
        player_shader->enable();
        player_shader->setUniform("u_color", Vector4(1,1,1,1));
        player_shader->setUniform("u_viewprojection", curr_camera->viewprojection_matrix);
        player_shader->setUniform("u_texture", player_texture);
        player_shader->setUniform("u_model", player_model);
        player_body->render(GL_TRIANGLES, 1);
        player_shader->disable();
    } else {
        player_shader->enable();
        player_shader->setUniform("u_color", Vector4(1,1,1,1));
        player_shader->setUniform("u_viewprojection", curr_camera->viewprojection_matrix);
        player_shader->setUniform("u_texture", player_texture);
        player_shader->setUniform("u_model", player_model);
        player_arm->render(GL_TRIANGLES, 1);
        player_shader->disable();
    }

    // TODO: set delimitation by fog?
    // Render area
    scene_stages[0]->render_stage(curr_camera);
    // Render next areas??
    drawText(2, 2, "x:" + std::to_string(player_model.m[12]) + " y:" + std::to_string(player_model.m[13]) + " z:" + std::to_string(player_model.m[14]), Vector3(1, 1, 1), 2);
}

void sGameScene::update_scene(float elapsed_time, uint8 pressed_keys) {
    player_model.rotate(Input::mouse_delta.x * CHAR_ROT_SPEED, Vector3(0.f,-1.f,0.f));
  
    if (Input::isKeyPressed(SDL_SCANCODE_W)) {
        speed =  speed + Vector3(.0f, .0f, -.1f * CHAR_SPEED);
    } else if (Input::isKeyPressed(SDL_SCANCODE_S)) {
        speed =  speed + Vector3(.0f, .0f, .1f * CHAR_SPEED);
    }

    if (Input::isKeyPressed(SDL_SCANCODE_A)) {
        speed =  speed + Vector3(-.1f * CHAR_SPEED, .0f, .0f);
    } else if (Input::isKeyPressed(SDL_SCANCODE_D)) {
        speed =  speed + Vector3(.1f * CHAR_SPEED, .0f, .0f);
    }

    scene_cam.cam_mode = THIRD_PERSON;
    if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT) || Input::isKeyPressed(SDL_SCANCODE_RSHIFT))
        scene_cam.cam_mode = FIRST_PERSON;


    // Update positions
    speed = speed * elapsed_time;
    player_model.translate(speed.x, speed.y, speed.z);
    speed = Vector3(0,0,0);

    // Test collitions
    // Set elevated player position
    // Todo: generalize to kinetic entities
    Vector3 player_position = Vector3(player_model.m[12], player_model.m[13], player_model.m[14]);
    Vector3 collision_normals;
    if (scene_stages[0]->testStageCollisionsWith(player_position, 1.0, collision_normals)) {
        collision_normals = collision_normals.normalize();
        collision_normals = collision_normals * 0.09;
        player_model.translate(collision_normals.x, 0., collision_normals.z);
    }
}