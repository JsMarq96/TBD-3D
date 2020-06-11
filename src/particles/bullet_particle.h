#ifndef _BULLET_PARTICLE_H_
#define _BULLET_PARTICLE_H_

#include "../framework.h"
#include "../camera.h"
#include "../shader.h"
#include "../mesh.h"
#include "../texture.h"

#define MAX_BULLET_SIZE 100
#define BULLET_SPEED 15.f
#define BULLET_TIMER_RANGE 1.5f

struct sBulletEntity {
    Vector3 position[MAX_BULLET_SIZE];
    Vector3 direction[MAX_BULLET_SIZE];
    float particle_timer[MAX_BULLET_SIZE];
    bool is_active[MAX_BULLET_SIZE];

    Mesh bullet_mesh;

    sBulletEntity();

    void update(float elapsed_time);
    void render(Camera *cam);

    bool add_bullet(Vector3 origin, Vector3 dir) {
        for (int i = 0; i < MAX_BULLET_SIZE; i++) {
            if (is_active[i])
                continue;

            position[i] = origin;
            direction[i] = dir;
            particle_timer[i] = 0.f;
            is_active[i] = true;
            return true;
        }

        return false;
    };
};

#endif