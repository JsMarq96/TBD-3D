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

    texture = Texture::Get("data/textures/player_text.png");
    shader = Shader::Get("data/shaders/basic.vs", "data/shaders/phong.ps");
    meshes[THIRD_PERSON] = Mesh::Get("data/meshes/player_stading.OBJ");
    meshes[FIRST_PERSON] = Mesh::Get("data/meshes/player_arm.obj"); 
}

sPlayer::sPlayer() {
    // Init default player
    position = Vector3(0,0,0);
    speed = Vector3(0,0,0);
    rotation = Vector3(0,0,0);

    texture = Texture::Get("data/textures/player_text.png");
    shader = Shader::Get("data/shaders/basic.vs", "data/shaders/phong.ps");
    meshes[THIRD_PERSON] = Mesh::Get("data/meshes/player_stading.OBJ");
    meshes[FIRST_PERSON] = Mesh::Get("data/meshes/player_arm.obj"); 
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
        /*model.setTranslation(
            position.x,//- (-5. * sin(rotation.y)), 
            position.y, 
            position.z);+ (-5. * cos(rotation.y)));*/

        eye = cam_model * Vector3(0, 1.2, 0.8);
        center = cam_model * Vector3(0, 1.2, 0);
        up = cam_model.rotateVector(Vector3(0, 1, 0));
    } else if (cam_mode == THIRD_PERSON) {
        cam_model.rotate(rotation.x * 0.25, Vector3(1,0,0));

        cam_model.translate(0,1.85,0.5);
        // Fill the player model
        //eye = model * Vector3(.0f, 2.7f, 1.80f);
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
    shader->setUniform("u_texture", texture);
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
    shade->setUniform("u_texture", texture);
    shade->setUniform("u_model", mod);
    mesh.render(GL_TRIANGLES);
    shade->disable();

    glDisable(GL_BLEND);
}

void sPlayer::calculate_next_step(float elapsed_time) {
    Vector3 can_displ = (speed * elapsed_time);
        
    // Rotare and add the displacement
    Vector3 disp = Vector3(
        (can_displ.x * cos(rotation.y)) - (can_displ.z * sin(rotation.y)),
        0.f,
        (can_displ.x * sin(rotation.y)) + (can_displ.z * cos(rotation.y))
    );

    position = position + disp;

    // Get front player direction
    direction = Vector3(
        (0.f * cos(rotation.y)) - (-1 * sin(rotation.y)),
        0.f,
        (0.f * sin(rotation.y)) + (-1 * cos(rotation.y))
    );
}