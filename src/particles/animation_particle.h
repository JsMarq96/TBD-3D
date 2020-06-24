#ifndef _ANIMATION_PARTICLE_H_
#define _ANIMATION_PARTICLE_H_

#include "../framework.h"
#include "../camera.h"
#include "../shader.h"
#include "../mesh.h"
#include "../texture.h"

#define PARTICLE_SIZE 20

struct sAnimationParticles {
    Vector3 positions[PARTICLE_SIZE];
    float elem_timer[PARTICLE_SIZE];
    bool is_active[PARTICLE_SIZE];

    Texture* texture_sheet;

    float size;

    int text_width, text_height;
    int max_frames;
    float animation_duration;
    bool is_kinetic;
    Vector3 direction;

    sAnimationParticles() {};
    sAnimationParticles(Texture* text, int width, int height, float size, int max_fram, float anim_duration);

    // Creates a Kinetic particle
    void set_kinetic(Vector3 dir);

    // Find an empty particel spot and adds it
    bool add_instance(Vector3 position);

    // Update movement or timers of the particles
    void update(float elaped_time);

    void render(Camera *cam);
};

#endif