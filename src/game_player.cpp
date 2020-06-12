#include "game_player.h"
#include <iostream>
/**
 * Todo: - Animate between states
 *      - Change eye and up vectors to match model
 *      - Trail camera on thrid person view (?)
*/
sPlayer::sPlayer(Vector3 start_pos) {
    // Init default player
    position = start_pos;
    speed = Vector3(0,0,0);
    rotation = Vector3(0,0,0);

    texture[THIRD_PERSON] = Texture::Get("data/textures/player_text.png");
    texture[FIRST_PERSON] = Texture::Get("data/textures/player_arms_text.png");

    shader = Shader::Get("data/shaders/basic.vs", "data/shaders/phong.ps");
    meshes[THIRD_PERSON] = Mesh::Get("data/meshes/player_stading.OBJ");
    meshes[FIRST_PERSON] = Mesh::Get("data/meshes/player_arms.obj"); 
}

sPlayer::sPlayer() {
    // Init default player
    position = Vector3(0,0,0);
    speed = Vector3(0,0,0);
    rotation = Vector3(0,0,0);

    texture[THIRD_PERSON] = Texture::Get("data/textures/player_text.png");
    texture[FIRST_PERSON] = Texture::Get("data/textures/player_arms_text.png");

    shader = Shader::Get("data/shaders/basic.vs", "data/shaders/phong.ps");
    meshes[THIRD_PERSON] = Mesh::Get("data/meshes/player_stading.OBJ");
    meshes[FIRST_PERSON] = Mesh::Get("data/meshes/player_arms.obj");
}

void sPlayer::get_camera(Camera *cam) {
    Vector3 eye, up, center;

    model.setTranslation(position.x, position.y, position.z);
    model.rotate(rotation.y, Vector3(0,1,0));

    Matrix44 cam_model = model;

    if (cam_mode == FIRST_PERSON) {
        model.rotate(rotation.x, Vector3(1,0,0));
        cam_model.rotate(rotation.x, Vector3(1,0,0));
        cam_model.translate(0,1.3,0);

        eye = cam_model * Vector3(0, 1.2, 0.8);
        center = cam_model * Vector3(0, 1.2, 0);
        up = cam_model.rotateVector(Vector3(0, 1, 0));
    } else if (cam_mode == THIRD_PERSON) {
        cam_model.rotate(rotation.x * 0.15, Vector3(1,0,0));
        cam_model.translate(0,1.85,0.5);

        eye = cam_model * Vector3(0, 1.2, 2);
        center = cam_model * Vector3(0, 1, 0);
        up = cam_model.rotateVector(Vector3(0, 1, 0));
    }

    cam->lookAt(eye, center, up);    
}

void sPlayer::render(Camera *cam) {
    shader->enable();
    shader->setUniform("u_color", Vector4(1,1,1,1));
    shader->setUniform("u_viewprojection", cam->viewprojection_matrix);
    shader->setUniform("u_texture", texture[cam_mode]);
    shader->setUniform("u_model", model);
    meshes[cam_mode]->render(GL_TRIANGLES);
    shader->disable();
}

void sPlayer::render_camera_fog(Camera *cam) {
    Mesh mesh;
    Matrix44 mod = model;
    mesh.createQuad(0, 0, 50, 50, false);
    mod.translate(0,0,-10);
    Shader *shade = Shader::Get("data/shaders/basic.vs", "data/fog_perlin.fs");

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  

    shade->enable();
    shade->setUniform("u_color", Vector4(1,1,1,0.4));
    shade->setUniform("u_viewprojection", cam->viewprojection_matrix);
    //shade->setUniform("u_texture", texture);
    shade->setUniform("u_model", mod);
    mesh.render(GL_TRIANGLES);
    shade->disable();

    glDisable(GL_BLEND);
}

void sPlayer::update(float elapsed_time) {
    // Get the direction and the rotation of the displacement
    rotation.y = rotation.y - (Input::mouse_delta.x * CHAR_ROT_SPEED);
    rotation.x = rotation.x - (Input::mouse_delta.y * CHAR_ROT_SPEED);
    
    if (cam_mode == THIRD_PERSON) {
        rotation.x = clamp(rotation.x, -0.99, 1.6);
    } else {
        rotation.x = clamp(rotation.x, -0.99, 0.95);
    }

    speed = Vector3(0,0,0);
    
    // WASD movements
    if (Input::isKeyPressed(SDL_SCANCODE_W)) {
        speed = speed + Vector3(.0f, .0f, -.1f * charecter_speed[cam_mode]);
    } else if (Input::isKeyPressed(SDL_SCANCODE_S)) {
        speed =  speed + Vector3(.0f, .0f, .1f * charecter_speed[cam_mode]);
    }

    if (Input::isKeyPressed(SDL_SCANCODE_A)) {
        speed = speed + Vector3(-.1f * charecter_speed[cam_mode], .0f, .0f);
    } else if (Input::isKeyPressed(SDL_SCANCODE_D)) {
        speed = speed + Vector3(.1f * charecter_speed[cam_mode], .0f, .0f);
    }

    // Manage camera switching
    cam_mode = THIRD_PERSON;
    if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT) || Input::isKeyPressed(SDL_SCANCODE_RSHIFT))
        cam_mode = FIRST_PERSON;
    Vector3 can_displ = (speed * elapsed_time);
        
    // Rotare and add the displacement
    Vector3 disp = Vector3(
        (can_displ.x * cos(rotation.y)) - (can_displ.z * sin(rotation.y)),
        0.f,
        (can_displ.x * sin(rotation.y)) + (can_displ.z * cos(rotation.y))
    );

    position = position + disp;

    // Get front player direction
    direction = model.frontVector() * -1.f;
}