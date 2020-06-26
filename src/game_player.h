#ifndef _GAME_CAMERA_H_
#define _GAME_CAMERA_H_

#include "framework.h"
#include "camera.h"
#include "texture.h"
#include "shader.h"
#include "mesh.h"
#include "input.h"
#include "animation.h"
#include "game.h"
#include "audio_controller.h"
#include "particles/animation_particle.h"
#include "particles/bullet_particle.h"

#define CHAR_SPEED 60.f
#define CHAR_SLOW_SPEED 25.f
#define CHAR_POINTED_SPEED 0.2f
#define CHAR_ROT_SPEED 0.0005f
#define CHAR_ROT_POINT_SPEED 0.00005f
#define MUZZLE_FLASH_DURATION 0.04f

#define GUN_FIRE_SOUND_DIR "data/sounds/gun_shot.wav"
#define GUN_PULL_SOUND_DIR "data/sounds/gun_cock.wav"
#define GUN_HIDE_SOUND_DIR "data/sounds/gun_putaway.wav"
#define GUN_EMPTY_SOUND_DIR "data/sounds/gun_empty.wav"

enum CamType : uint8 {
    THIRD_PERSON = 0,
    FIRST_PERSON = 1,
};

enum ePlayerStates : uint8 {
    STANDING = 0,
    RUNNING = 1
};

struct sPlayer {
    inline static sPlayer *instance;

    Vector3 direction;
    Vector3 position;
    Vector3 speed;
    Vector3 rotation;
    Matrix44 model;

    int health;
    int ammo;

    float camera_animation;
    CamType cam_mode;
    ePlayerStates player_state;

    float shoot_anim;
    bool has_shot_on_frame;
    bool prev_mouse_press;

    Texture *texture[2];
    Shader *shaders[2];
    Mesh *meshes[2];
    Animation *animations[2];

    sAnimationParticles muzzle_flash;
    sAnimationParticles player_blood;
    sBulletEntity bullets;

    float charecter_speed[2] = { CHAR_SPEED, CHAR_SLOW_SPEED };

    sPlayer(Vector3 start_pos);
    sPlayer();
    
    void get_camera(Camera *cam);
    void render(Camera *cam);
    void render_camera_fog(Camera *cam);
    void update(float elapsed_time);
    void shoot();
    void hit(Vector3 enemy_position) {
        health--;
        sAudioController::play_3D("data/sounds/hit.wav", position);
        
        std::cout << "PLAYER HIT " << std::to_string(player_blood.add_instance(position)) <<  std::endl;

        speed = speed + (position - enemy_position).normalize() * 0.5;
        if (health == 0) {
            // Game over
        }
    }

    bool has_shoot(Vector3 &light_position) {
        light_position = position;
        light_position.y += 2.f;
        return shoot_anim > 0.55;
    }
};

//sPlayer* sPlayer::instance;


#endif