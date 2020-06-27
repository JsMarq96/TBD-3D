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

    ammo = STARTING_AMMO;
    health = STARTING_HEALTH;

    texture[THIRD_PERSON] = Texture::Get("data/textures/player_text.png");
    texture[FIRST_PERSON] = Texture::Get("data/textures/player_arms_text.png");

    shaders[FIRST_PERSON] = Shader::Get("data/shaders/basic.vs", "data/shaders/phong.ps");
    shaders[THIRD_PERSON] = Shader::Get("data/shaders/skinning.vs", "data/shaders/phong.ps");

    meshes[THIRD_PERSON] = Mesh::Get("data/meshes/player.mesh");
    meshes[FIRST_PERSON] = Mesh::Get("data/meshes/player_arms.obj");

    animations[STANDING] = Animation::Get("data/animations/animations_idle.skanim");
    animations[RUNNING] = Animation::Get("data/animations/animations_walking.skanim"); 

    muzzle_flash = sAnimationParticles(Texture::Get("data/particles/fire_particle.png"), 8, 8, 0.8f, 40, MUZZLE_FLASH_DURATION); 
    player_blood = sAnimationParticles(Texture::Get("data/particles/blood_hit.png"), 4, 4, 2.0f, 13, 0.15);
}

sPlayer::sPlayer() {
    // Init default player
    position = Vector3(0,0,0);
    speed = Vector3(0,0,0);
    rotation = Vector3(0,0,0);

    ammo = STARTING_AMMO;
    health = STARTING_HEALTH;

    texture[THIRD_PERSON] = Texture::Get("data/textures/player_text.png");
    texture[FIRST_PERSON] = Texture::Get("data/textures/player_arms_text.png");

    shaders[FIRST_PERSON] = Shader::Get("data/shaders/basic.vs", "data/shaders/phong.ps");
    shaders[THIRD_PERSON] = Shader::Get("data/shaders/skinning.vs", "data/shaders/phong.ps");

    meshes[THIRD_PERSON] = Mesh::Get("data/meshes/player.mesh");
    meshes[FIRST_PERSON] = Mesh::Get("data/meshes/player_arms.obj");

    animations[THIRD_PERSON] = Animation::Get("data/animations/animations_idle.skanim");

    muzzle_flash = sAnimationParticles(Texture::Get("data/particles/fire_particle.png"), 8, 8, 0.8f, 40, MUZZLE_FLASH_DURATION);  
    player_blood = sAnimationParticles(Texture::Get("data/particles/blood_hit.png"), 4, 4, 2.0f, 13, 0.15);
}

/**
*   Calculate the camera and animate the camera
*/
void sPlayer::get_camera(Camera *cam) {
    Vector3 eye, center, up, fp_eye, fp_up, fp_center, tp_eye, tp_up, tp_center;

    model.setTranslation(position.x, position.y, position.z);
    model.rotate(rotation.y, Vector3(0,1,0));    

    Matrix44 cam_model = model;
    Matrix44 player_model = model;

    // Calculate the First person cam
    player_model.rotate(lerp(rotation.x,  rotation.x + -0.1f, shoot_anim), Vector3(1,0,0));
    cam_model.rotate(rotation.x, Vector3(1,0,0));
    cam_model.translate(0,1.2f + sin(Game::instance->time *2.6) * 0.005,0); // Sin for the breathing

    fp_eye = cam_model * Vector3(0, 1.2f, 0.8f);
    fp_center = cam_model * Vector3(0, 1.2f, 0);
    fp_up = cam_model.rotateVector(Vector3(0, 1, 0));

    // Restablish the camera
    cam_model = model;

    // calculate the Third person cam
    cam_model.rotate(rotation.x * 0.15, Vector3(1,0,0));
    cam_model.translate(0,1.85,0.5);

    tp_eye = cam_model * Vector3(0, 1.1, 2);
    tp_center = cam_model * Vector3(0, 1, 0);
    tp_up = cam_model.rotateVector(Vector3(0, 1, 0));

    // Interpolate between the camaras
    eye = lerp(tp_eye, fp_eye, camera_animation);
    center = lerp(tp_center, fp_center, camera_animation);
    up = lerp(tp_up, fp_up, camera_animation);
    
    if (player_state == DIED) {
        camera_animation = std::min(camera_animation, 0.86f);
        Vector3 dc_eye, dc_center, dc_up; // Death camera

        dc_eye = Vector3(position.x + 1.5f, 0.1f, position.z - 1.5f);
        dc_center = Vector3(position.x + 2.0f, 0.1f, position.z - 2.0f) + direction;
        dc_up = cam_model.rotateVector(Vector3(1, 1, 0));

        eye = lerp(eye, dc_eye, camera_animation);
        center = lerp(center, dc_center, camera_animation);
        up = lerp(up, dc_up, camera_animation);
    }
    

    cam->lookAt(eye, center, up); 

    if (cam_mode == FIRST_PERSON) {
        model = player_model;
    }   
}


void sPlayer::render(Camera *cam) {
    Shader* shader = shaders[cam_mode];

    shader->enable();
    shader->setUniform("u_color", Vector4(1,1,1,1));
    shader->setUniform("u_viewprojection", cam->viewprojection_matrix);
    shader->setUniform("u_texture", texture[cam_mode]);
    shader->setUniform("u_model", model);
    shader->setUniform("u_double_light", has_shot_on_frame);
    shader->setUniform("u_second_light_pos", position);
    shader->setUniform("u_camera_pos", cam->eye);

    // Switch the model depending on the camera animation
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

    // Game over overlay
    if (player_state == DIED && camera_animation >= 0.86) {
        add_overlay(Texture::Get("data/efects/game_over.png"));
        return;
    }

    has_shot_on_frame = false;

    muzzle_flash.render(cam);

    if (has_been_hit_on_frame) {
        sAudioController::play_3D("data/sounds/hit.wav", position);
        add_overlay(Texture::Get("data/efects/hit.png"));
        player_blood.add_instance(position);
    }

    has_been_hit_on_frame = false;

    player_blood.render(cam);
    bullets.render(cam);
}

void sPlayer::shoot() {
    if (ammo == 0) {
        sAudioController::play_3D(GUN_EMPTY_SOUND_DIR, position + (direction * 2.0f));
        return;
    } 

    bullets.add_bullet(position + Vector3(0,2,0), direction);
    shoot_anim = 0.6f;

    has_shot_on_frame = true;

    muzzle_flash.add_instance(position + (direction * 2.0f) + Vector3(0.0f, 2.2f, 0.0f));

    sAudioController::play_3D(GUN_FIRE_SOUND_DIR, position + (direction * 2.0f));
    ammo--;
}

void sPlayer::update(float elapsed_time) {
    // Update particles
    muzzle_flash.update(elapsed_time);
    bullets.update(elapsed_time);
    player_blood.update(elapsed_time);

    if (player_state == DIED) {
        camera_animation += elapsed_time;

        if (Input::isKeyPressed(SDL_SCANCODE_SPACE)) {
            SDL_Delay(500);
            // Restart the game
            Game::instance->start_game();
        }

        return;
    }

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

    float old_cam_anim = camera_animation;
    // Manage camera switching
    if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT) || Input::isKeyPressed(SDL_SCANCODE_RSHIFT)) {
        camera_animation = std::min(camera_animation + (elapsed_time * 2.5f), 1.0f);
    } else {
        camera_animation = std::max(camera_animation - (elapsed_time * 2.5f), 0.0f);
    }

    // Change the objet mode depending on the camera animation
    cam_mode = THIRD_PERSON;
    if (camera_animation >= 0.6) {
        if (camera_animation == 0.6) {
            rotation.x = 0.0f;
        }
        cam_mode = FIRST_PERSON;
    }

    // Play pull/hide gun sound
    if (camera_animation >= 0.6 && old_cam_anim < 0.6) {
        sAudioController::play_3D(GUN_PULL_SOUND_DIR, position + (direction * 2.0f));
    } else if (camera_animation < 0.6 && old_cam_anim >= 0.6) {
        sAudioController::play_3D(GUN_HIDE_SOUND_DIR, position + (direction * 2.0f));
    }
    

    Vector3 can_displ = (speed * elapsed_time);
        
    // Rotare and add the displacement
    Vector3 disp = Vector3(
        (can_displ.x * cos(rotation.y)) - (can_displ.z * sin(rotation.y)),
        0.f,
        (can_displ.x * sin(rotation.y)) + (can_displ.z * cos(rotation.y))
    );

    position = position + disp;

    // Set movement states
    if (speed.length() > 0.01f) {
       player_state = RUNNING;

    } else {
        player_state = STANDING;
    }

    // Player sounds
    if (speed.length() > 0.2f) {
       if (sAudioController::has_finished(walking_sound_channel)) {
           walking_sound_channel = sAudioController::play_3D("data/sounds/step.wav", position);
       }

    } else {
        sAudioController::stop(walking_sound_channel);        
    }

    // Get front player direction
    direction = model.frontVector() * -1.f;

    // Shooting
    bool is_pressed = Input::isMousePressed(SDL_BUTTON_LEFT);
    // Shoot bullet only when it is pressed
    if (is_pressed && prev_mouse_press != is_pressed && cam_mode == FIRST_PERSON) {
        shoot();
    }
    prev_mouse_press = is_pressed;
}