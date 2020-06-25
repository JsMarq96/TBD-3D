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

    shaders[FIRST_PERSON] = Shader::Get("data/shaders/basic.vs", "data/shaders/phong.ps");
    shaders[THIRD_PERSON] = Shader::Get("data/shaders/skinning.vs", "data/shaders/phong.ps");

    meshes[THIRD_PERSON] = Mesh::Get("data/meshes/player.mesh");
    meshes[FIRST_PERSON] = Mesh::Get("data/meshes/player_arms.obj");

    animations[STANDING] = Animation::Get("data/animations/animations_idle.skanim");
    animations[RUNNING] = Animation::Get("data/animations/animations_walking.skanim"); 

    muzzle_flash = sAnimationParticles(Texture::Get("data/particles/fire_particle.png"), 8, 8, 1.0f, 60, MUZZLE_FLASH_DURATION);  
}

sPlayer::sPlayer() {
    // Init default player
    position = Vector3(0,0,0);
    speed = Vector3(0,0,0);
    rotation = Vector3(0,0,0);

    texture[THIRD_PERSON] = Texture::Get("data/textures/player_text.png");
    texture[FIRST_PERSON] = Texture::Get("data/textures/player_arms_text.png");

    shaders[FIRST_PERSON] = Shader::Get("data/shaders/basic.vs", "data/shaders/phong.ps");
    shaders[THIRD_PERSON] = Shader::Get("data/shaders/skinning.vs", "data/shaders/phong.ps");

    meshes[THIRD_PERSON] = Mesh::Get("data/meshes/player.mesh");
    meshes[FIRST_PERSON] = Mesh::Get("data/meshes/player_arms.obj");

    animations[THIRD_PERSON] = Animation::Get("data/animations/animations_idle.skanim");

    muzzle_flash = sAnimationParticles(Texture::Get("data/particles/fire_particle.png"), 8, 8, 1.0f, 60, MUZZLE_FLASH_DURATION);  
}

void sPlayer::get_camera(Camera *cam) {
    Vector3 eye, up, center;

    model.setTranslation(position.x, position.y, position.z);
    model.rotate(rotation.y, Vector3(0,1,0));    

    Matrix44 cam_model = model;

    if (cam_mode == FIRST_PERSON) {
        model.rotate(lerp(rotation.x,  rotation.x + -0.1f, shoot_anim), Vector3(1,0,0));
        cam_model.rotate(rotation.x, Vector3(1,0,0));
        cam_model.translate(0,1.2f + sin(Game::instance->time *2.6) * 0.005,0);

        eye = cam_model * Vector3(0, 1.2f, 0.8f);
        center = cam_model * Vector3(0, 1.2f, 0);
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
    Shader* shader = shaders[cam_mode];

    shader->enable();
    shader->setUniform("u_color", Vector4(1,1,1,1));
    shader->setUniform("u_viewprojection", cam->viewprojection_matrix);
    shader->setUniform("u_texture", texture[cam_mode]);
    shader->setUniform("u_model", model);
    shader->setUniform("double_light", has_shot_on_frame);
    shader->setUniform("second_light_pos", position);
    shader->setUniform("camera_pos", cam->eye);

    if (cam_mode == THIRD_PERSON) {
        animations[STANDING]->assignTime(Game::instance->time);
        animations[RUNNING]->assignTime(Game::instance->time);

        Skeleton blend_sk; 

        // Blending using the normalized lenht of the speed vector
        blendSkeleton(&animations[STANDING]->skeleton, &animations[RUNNING]->skeleton, speed.length() / 0.6f, &blend_sk);

        meshes[cam_mode]->renderAnimated( GL_TRIANGLES, &blend_sk);
    } else {
        meshes[cam_mode]->render(GL_TRIANGLES);
    }

    shader->disable();

    has_shot_on_frame = false;

    muzzle_flash.render(cam);
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
    shade->setUniform("u_model", mod);
    mesh.render(GL_TRIANGLES);
    shade->disable();

    glDisable(GL_BLEND);

}

void sPlayer::shoot_animation() {
    shoot_anim = 0.6f;

    has_shot_on_frame = true;

    muzzle_flash.add_instance(position + (direction * 1.5f) + Vector3(0.0f, 2.f, 0.0f));
}

void sPlayer::update(float elapsed_time) {
    muzzle_flash.update(elapsed_time);

    if (shoot_anim > 0) {
        shoot_anim -= elapsed_time;
    }

    // Get the direction and the rotation of the displacement
    rotation.y = rotation.y - (Input::mouse_delta.x * CHAR_ROT_SPEED);
    rotation.x = rotation.x - (Input::mouse_delta.y * CHAR_ROT_SPEED);
    
    if (cam_mode == THIRD_PERSON) {
        rotation.x = clamp(rotation.x, -0.99, 1.6);
    } else {
        rotation.x = clamp(rotation.x, -0.99, 0.95);
    }

    Vector3 new_speed = Vector3(0,0,0);
    
    // WASD movements
    if (Input::isKeyPressed(SDL_SCANCODE_W)) {
        new_speed = new_speed + Vector3(.0f, .0f, -.1f * charecter_speed[cam_mode]);
    } else if (Input::isKeyPressed(SDL_SCANCODE_S)) {
        new_speed =  new_speed + Vector3(.0f, .0f, .1f * charecter_speed[cam_mode]);
    }

    if (Input::isKeyPressed(SDL_SCANCODE_A)) {
        new_speed = new_speed + Vector3(-.1f * charecter_speed[cam_mode], .0f, .0f);
    } else if (Input::isKeyPressed(SDL_SCANCODE_D)) {
        new_speed = new_speed + Vector3(.1f * charecter_speed[cam_mode], .0f, .0f);
    }

    // Speed Acceleration / deacceleration
    speed = speed + (new_speed - speed) * elapsed_time * 3.f;

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

    // Set movement states
    if (disp.length() > 0.01f) {
       player_state = RUNNING;
    } else {
        player_state = STANDING;
    }

    // Get front player direction
    direction = model.frontVector() * -1.f;
}