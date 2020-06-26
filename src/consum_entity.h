#ifndef _CONSUM_ENTITY_H_
#define _CONSUM_ENTITY_H_

#include "framework.h"
#include "camera.h"
#include "texture.h"
#include "shader.h"
#include "mesh.h"

#define CONSUMABLES_NUM 20
#define CATCH_RADIUS 1.5f

enum eConsumType: int {
    AMMO = 0,
    HEALTH = 1,
    EMPTY = 2
};

struct sConsumableEntities {
    eConsumType type[CONSUMABLES_NUM];
    Vector3 position[CONSUMABLES_NUM];
    Vector3 origin[CONSUMABLES_NUM];
    float timer[CONSUMABLES_NUM];
    
    Texture *texts[2];
    Mesh *box_mesh;
    Shader *shader;

    sConsumableEntities();

    void add_random_element(Vector3 pos);

    void add_element(Vector3 elem, eConsumType items);

    void update(float elapsed_time);

    void render(Camera* cam);

    void pickup_element(int index) {
        type[index] = EMPTY;
    };

    eConsumType test_collide(Vector3 pos, int &index) {
        for(int i = 0; i <= CONSUMABLES_NUM; i++) {
            if (type[i] != EMPTY)
                continue;
            
            if ((pos - position[i]).length() <= 1.5) {
                index = i;
                return type[i];
            } 
        }

        return EMPTY;
    }
};

#endif