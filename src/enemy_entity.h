#ifndef _ENEMY_ENTITY_H_
#define _ENEMY_ENTITY_H_

#include <string>
#include <queue>
#include <set>
#include <unordered_map>
#include "framework.h"
#include "stage.h"
#include "shader.h"
#include "mesh.h"
#include "texture.h"
#include "camera.h"
#include "game.h"

#define ENEMYS_PER_AREA 100
#define MAX_STEPS_NUM 40
#define DATA_DIR_LEN 40.f
#define HALF_ENEMY_FOV 30.0f

// Note: this Entity uses the coliders of Radius and Mesh shapes

struct sKinetics {
    Vector3 position;
    Vector3 speed;
    Vector2 rotation;
};

enum eEnemyState: uint8 {
    ROAM = 0,
    ATTACK,
    RUN_AFTER,
    RECOVERING
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

    sEnemyEntity() {
        shader_fs_id = "";
        shader_vs_id = "";
        mesh_id = "";
        last_inserted_index = -1;
    }

    void init() {
        for (int i = 0; i <  ENEMYS_PER_AREA; i++) {
            action_index[i] = -1;
        }
    }

    void update(float elapsed_time, sGameMap &map, Vector3 player_pos);

    bool add_element(Matrix44 &model);
    void render(Camera *camara);

    // Todo: ray collisions
    bool testCollisionsWith(Vector3 position, float radius, Vector3 &coll_pos, Vector3 &normal);

    void set_collider_cilinder(float c_radius) { radius = c_radius; };
};

#endif