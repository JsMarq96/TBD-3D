#ifndef _ENEMY_ENTITY_H_
#define _ENEMY_ENTITY_H_

#include <string>
#include <queue>
#include <set>
#include <unordered_map>
#include "extra/tweeny/tweeny.h"
#include "framework.h"
#include "game_map.h"
#include "shader.h"
#include "mesh.h"
#include "texture.h"
#include "camera.h"
#include "game.h"
#include "particles/bullet_particle.h"

#define ENEMY_ROAM_SPEED 2.5
#define ENEMY_RUN_SPEED 4.5
#define ENEMY_ROT_SPEED 0.5
#define ENEMYS_PER_AREA 100
#define MAX_STEPS_NUM 100
#define DATA_DIR_LEN 40.f
#define HALF_ENEMY_FOV 30.0f

// Note: this Entity uses the coliders of Radius and Mesh shapes

struct sKinetics {
    Vector3 position;
    Vector3 speed;
    float angle= 0.0f;

    void get_model_matrix(Matrix44 *model) {
        model->setTranslation(position.x, 0., position.z);
        model->rotate(angle, Vector3(0,1,0));
    }
};

enum eEnemyState: uint8 {
    ROAM = 0,
    RUN_AFTER = 1,
    STOPPED = 2,
    ATTACK = 3,
    RECOVERING = 4
};

// Generic Rendering type
struct sEnemyEntity {
    std::string mesh_id;
    std::string texture_id;
    std::string shader_vs_id;
    std::string shader_fs_id;

    float radius;

    // Position and movement
    sKinetics kinetic_elems[ENEMYS_PER_AREA];

    // Behabiour
    Vector2 poi[ENEMYS_PER_AREA];
    eEnemyState state[ENEMYS_PER_AREA];
    int action_index[ENEMYS_PER_AREA];
    int enemy_steps[ENEMYS_PER_AREA][MAX_STEPS_NUM];

    int last_inserted_index;

    sEnemyEntity(std::string &shader_fs,
                 std::string &shader_vs,
                 std::string &mesh,
                 std::string &text_id);

    sEnemyEntity();

    void update(float elapsed_time, sGameMap &map, Vector3 player_pos);

    bool add_element(Vector3 position) {
        last_inserted_index++;

        if (last_inserted_index >= ENEMYS_PER_AREA) {
            return false;
        }
        action_index[last_inserted_index] = -1;
        state[last_inserted_index] = STOPPED;
        kinetic_elems[last_inserted_index].position = position;
        kinetic_elems[last_inserted_index].speed = Vector3(0,0,0);

        for (int i = 0; i < ENEMYS_PER_AREA; i++) {
            enemy_steps[last_inserted_index][i] = -1;
        }

        return true;
    };
    void render(Camera *camara);

    void enemy_is_shoot(int index, Vector3 coll_point);

    // Todo: ray collisions
    void testBulletCollisions(sBulletEntity &bullet_controller);

    bool testCollisionsWith(Vector3 position, float radius, Vector3 &coll_pos, Vector3 &normal);

    void set_collider_cilinder(float c_radius) { radius = c_radius; };
};

#endif