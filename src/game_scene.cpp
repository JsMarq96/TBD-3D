#include "game_scene.h"

Matrix44 setTranslationMat(int x, int y, int z) {
    Matrix44 new_mat = Matrix44();
    new_mat.setTranslation(x, y, z);

    return new_mat;
}

sGameScene::sGameScene() {
    curr_area = 0;
    
    player_model.setTranslation(0,0,0);
    player_shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
    player_body = Mesh::Get("data/meshes/player_t.obj");
    player_texture = Texture::Get("data/textures/player_text.tga");

    // Create test area
    sArea* test_area = new sArea(0, 0, 50, 50);

    Matrix44 tree1_model = Matrix44();
    Matrix44 tree2_model = Matrix44();
    Matrix44 tree3_model = Matrix44();
    Matrix44 house1_model = Matrix44();
    Matrix44 house2_model = Matrix44();

    tree1_model.translate(20, 0, 0);
    tree2_model.translate(15, 5, 0);
    tree3_model.translate(30, 30, 0);
    house1_model.translate(0, 50, 0);
    house2_model.translate(50, 10, 0);

    test_area->add_tree(tree1_model);
    test_area->add_tree(tree2_model);
    test_area->add_tree(tree3_model);

    test_area->add_house(house1_model);
    test_area->add_house(house2_model);

    game_areas.push_back(test_area);
}

//Vector3 eye1 = Vector3(0,1,2.7), up1, center1 = Vector3(0,1,0);
void sGameScene::render_scene() {
    Camera *curr_camera = Camera::current;
    // Render player
    scene_cam.get_camera(curr_camera, player_model);

    /*
    For manual camera adjust
    Vector3 eye, up, center;

    eye = player_model * eye1;
    center = player_model * center1;

    up = player_model.rotateVector(Vector3(0,1,0));

    curr_camera->lookAt(eye, center, up);*/

    glClearColor(0.0, 0.0, 0.0, 1.0);

	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//set the camera as default
	//camera->enable();
    curr_camera->enable();
    glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
    
    if (scene_cam.cam_mode == THIRD_PERSON) {
        player_shader->enable();
        player_shader->setUniform("u_color", Vector4(1,1,1,1));
        player_shader->setUniform("u_viewprojection", curr_camera->viewprojection_matrix);
        player_shader->setUniform("u_texture", player_texture);
        player_shader->setUniform("u_model", player_model);
        player_body->render(GL_TRIANGLES);
        player_shader->disable();
    }

    // TODO: set delimitation by fog?
    // Render area
    game_areas[0]->render_area(curr_camera);
    // Render next areas??
    //drawText(2, 2, "x:" + std::to_string(player_model.m[12]) + " y:" + std::to_string(player_model.m[13]) + " z:" + std::to_string(player_model.m[14]), Vector3(1, 1, 1), 2);
    //drawText(2, 2, "x:" + std::to_string(eye1.x) + " y:" + std::to_string(eye1.y) + " z:" + std::to_string(eye1.z), Vector3(1, 1, 1), 2);
    //drawText(2, 17, "x:" + std::to_string(center1.x) + " y:" + std::to_string(center1.y) + " z:" + std::to_string(center1.z), Vector3(1, 1, 1), 2);

}

void sGameScene::update_scene(float elapsed_time, uint8 pressed_keys) {  
    player_model.rotate(Input::mouse_delta.x * CHAR_ROT_SPEED, Vector3(0.f,-1.f,0.f));
  
    if (Input::isKeyPressed(SDL_SCANCODE_W)) {
        player_model.translate(.0f, .0f, -.1f * CHAR_SPEED);
    } else if (Input::isKeyPressed(SDL_SCANCODE_S)) {
        player_model.translate(.0f, .0f, .1f * CHAR_SPEED);
    }

     if (Input::isKeyPressed(SDL_SCANCODE_A)) {
        player_model.translate(-.1f * CHAR_SPEED, .0f, .0f);
    } else if (Input::isKeyPressed(SDL_SCANCODE_D)) {
        player_model.translate(.1f * CHAR_SPEED, .0f, .0f);
    }


    //camera->rotate(Input::mouse_delta.x * 0.005f, Vector3(0.0f,-1.0f,0.0f));
	//camera->rotate(Input::mouse_delta.y * 0.005f, camera->getLocalVector( Vector3(-1.0f,0.0f,0.0f)));

    /*if (Input::isKeyPressed(SDL_SCANCODE_UP)) eye1 = eye1 + Vector3(0,0,0.1);
    if (Input::isKeyPressed(SDL_SCANCODE_DOWN)) eye1 = eye1 - Vector3(0,0,0.1);

    if (Input::isKeyPressed(SDL_SCANCODE_LEFT)) eye1 = eye1 - Vector3(0,0.1,0);
    if (Input::isKeyPressed(SDL_SCANCODE_RIGHT)) eye1 = eye1 + Vector3(0,0.1,0);

    if (Input::isKeyPressed(SDL_SCANCODE_R)) center1 = center1 - Vector3(0,0.1,0);
    if (Input::isKeyPressed(SDL_SCANCODE_T)) center1 = center1 + Vector3(0,0.1,0);

    if (Input::isKeyPressed(SDL_SCANCODE_F)) center1 = center1 - Vector3(0.1,0,0);
    if (Input::isKeyPressed(SDL_SCANCODE_G)) center1 = center1 + Vector3(0.1,0,0);*/
}