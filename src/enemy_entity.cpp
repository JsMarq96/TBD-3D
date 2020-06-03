#include "enemy_entity.h"

sEnemyEntity::sEnemyEntity() {
    shader_fs_id = "data/shaders/phong.ps";
    shader_vs_id = "data/shaders/basic.vs";
    mesh_id = "data/meshes/player_stading.OBJ";
    texture_id = "data/textures/player_text.png";
    last_inserted_index = -1;

    for (int i = 0; i <  ENEMYS_PER_AREA; i++) {
        action_index[i] = -1;
    }
}

void sEnemyEntity::update(float elapsed_time, sGameMap &map, Vector3 player_pos) {
    Vector2 player_2d_pos = Vector2(player_2d_pos.x, player_2d_pos.y);

    map.parse_coordinates_to_map(player_2d_pos);

    for(int i = 0; i < ENEMYS_PER_AREA; i++) {
        Vector2 pos_2d = Vector2(kinetic_elems[i].position.x, kinetic_elems[i].position.z);
        
        Vector2 enemy_facing = pos_2d.normalize();
        Vector2 to_player_dir = (player_2d_pos - pos_2d).normalize();
        float angle = acos(enemy_facing.dot(to_player_dir) / (enemy_facing.length() * to_player_dir.length()));

        // Test if the enemy sees the player
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
        if (state[i] == ROAM) {
            // Roaming part
            if (action_index[i] == -1) {
                poi[i] = map.get_empty_coordinate();
                // Trace path
                int result;
                map.get_path_to(pos_2d, poi[i], enemy_steps[i], MAX_STEPS_NUM, result);
                std::cout  << result << "> " << poi[i].x << "-" << poi[i].y << std::endl;
                std::cout << "===========" << std::endl;
                for (int j = 0; j < ENEMYS_PER_AREA; j++) {
                    std::cout << std::to_string( enemy_steps[i][j]) << std::endl;
                }
                std::cout << "===========" << std::endl;

                if (result < 0) {
                    action_index[i] = -1;
                } else {
                    action_index[i] = 0;
                }
            } else {
                // Go to the next point in the path
                // set to -1 if it reaches the end
                Vector2 next_position;
                map.parse_map_index_to_coordinates(enemy_steps[i][action_index[i]], next_position);
                //std::cout << action_index[i] << " - " << enemy_steps[i][action_index[i]] << std::endl;

                // Set speed to the direction
                Vector3 direction = Vector3(next_position.x, 0, next_position.y) - kinetic_elems[i].position;
                direction = direction.normalize();
                kinetic_elems[i].speed = direction * ENEMY_SPEED;
                
                float tmp = (next_position - pos_2d).length();
                //std::cout << action_index[i]  << " + " << poi[i].x << " - " << poi[i].y  << "  - " << tmp << std::endl;
                //std::cout << action_index[i] << " EPIC" << std::endl;
                if ((next_position - pos_2d).length() <= 3) {
                    action_index[i]++;
                    if (action_index[i] >= MAX_STEPS_NUM) {
                        action_index[i] = -1;
                    }
                }

                kinetic_elems[i].position = kinetic_elems[i].position + (kinetic_elems[i].speed * elapsed_time);
            }
        }

        // The player is is sight
        if (state[i] == RUN_AFTER) {
            if (action_index[i] == -1) {
                int result;
                //map.get_path_to(pos_2d, poi[i], enemy_steps[i], MAX_STEPS_NUM / 2, result);
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

    mesh->enableBuffers(curr_shader);

    for (int i = 0; i <= last_inserted_index; i++) {
        Matrix44 model;
        kinetic_elems[i].get_model_matrix(model);

        curr_shader->setUniform("u_model", model);
        mesh->drawCall(GL_TRIANGLES, 1, 0);
    }

    mesh->disableBuffers(curr_shader);
    curr_shader->disable();
}