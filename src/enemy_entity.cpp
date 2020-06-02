#include "enemy_entity.h"

sEnemyEntity::sEnemyEntity() {
    shader_fs_id = "";
    shader_vs_id = "";
    mesh_id = "";
    last_inserted_index = -1;
}

void sEnemyEntity::init() {
    for (int i = 0; i <  ENEMYS_PER_AREA; i++) {
        action_index[i] = -1;
    }
}

void sEnemyEntity::update(float elapsed_time, sGameMap &map, Vector3 player_pos) {
    Vector2 player_2d_pos = Vector2(player_2d_pos.x, player_2d_pos.y);
    for(int i = 0; i < ENEMYS_PER_AREA; i++) {
        Vector2 pos_2d = Vector2(kinetic_elems[i].position.x, kinetic_elems[i].position.z);
        
        // Test if the enemy sees the player
        Vector2 enemy_facing = pos_2d.normalize();
        Vector2 to_player_dir = (player_2d_pos - pos_2d).normalize();
        float angle = acos(enemy_facing.dot(to_player_dir) / (enemy_facing.length() * to_player_dir.length()));

        if (angle >= HALF_ENEMY_FOV && angle <= (HALF_ENEMY_FOV * 2)) {
            int player_distance = map.raycast_from_point_to_point(pos_2d, player_2d_pos, 20);
            
            if (player_distance > 0) {
                // Player is in sight and kinda far
                state[i] = RUN_AFTER;
            } else if (player_distance == 0) {
                // Attacks the player
                state[i] = ATTACK;
            }
        }
        

        // No player in sight, just chilling
        if (state[i] ==  ROAM) {
            // Roaming part
            if (action_index[i] != -1) {
                poi[i] = map.get_empty_coordinate();
                // Trace path
                int result;
                map.get_path_to(pos_2d, poi[i], enemy_steps[i], MAX_STEPS_NUM, result);

                if (result == -1) {
                    action_index[i] = -1;
                }
            } else {
                // Go to the next point in the path
                // set to -1 if it reaches the end

            }

            // If the target is in sight
        }

        // The player is is sight
        if (state[i] == RUN_AFTER) {
            if (action_index[i] == -1) {
                int result;
                map.get_path_to(pos_2d, poi[i], enemy_steps[i], MAX_STEPS_NUM / 2, result);
            } else {
                
            }

        }

        // If its close enought with the player
        if (state[i] == ATTACK) {

        }

        // If it has been shoot
        if (state[i] == RECOVERING) {

        }
        
    }
}

void sEnemyEntity::render(Camera *camara)  {
    Shader *curr_shader = Shader::Get(shader_vs_id.c_str(), shader_fs_id.c_str());
    Mesh *mesh = Mesh::Get(mesh_id.c_str());
    curr_shader->enable();
        
    curr_shader->setUniform("u_color", Vector4(1, 1, 1, 1));
    curr_shader->setUniform("u_texture", Texture::Get(texture_id.c_str()));
    curr_shader->setUniform("light_pos", Vector3(250, 120, 250));
    curr_shader->setUniform("camera_pos", camara->eye);

    curr_shader->setUniform("u_viewprojection", camara->viewprojection_matrix);
}