#include "enemy_entity.h"

sEnemyEntity::sEnemyEntity() {
    shader_fs_id = "data/shaders/phong.ps";
    shader_vs_id = "data/shaders/basic.vs";
    mesh_id = "data/meshes/enemy.obj";
    texture_id = "data/textures/player_text.png";
    last_inserted_index = -1;

    for (int i = 0; i < ENEMYS_PER_AREA; i++) {
        action_index[i] = -1;
        state[i] = STOPPED;
    }
}

void sEnemyEntity::update(float elapsed_time, sGameMap &map, Vector3 player_pos) {
    Vector2 player_2d_pos = Vector2(player_pos.x, player_pos.z);

    for(int i = 0; i <= last_inserted_index; i++) {
        Vector2 pos_2d = Vector2(kinetic_elems[i].position.x, kinetic_elems[i].position.z);
        
        // Calculate angle between the player position and the enemy's direction
        Vector2 enemy_facing = Vector2(cos(kinetic_elems[i].angle), sin(kinetic_elems[i].angle)); //Vector2(kinetic_elems[i].rotation.x);
        Vector2 to_player_dir = (pos_2d - player_2d_pos).normalize();
        float enemy_player_distance = (pos_2d - player_2d_pos).length();
        float angle = acos(enemy_facing.dot(to_player_dir) / (enemy_facing.length() * to_player_dir.length())) * 180 / PI;

        if (angle > 100 && enemy_player_distance <= 1) {
            // If it is facing to the player and its near, attack him
            state[i] = ATTACK;
        } else if (angle > 100 & enemy_player_distance <= 30) {
            // If it is in the eyesight of the player and it is 
            state[i] = RUN_AFTER;
        }

        std::cout << "Angle " << angle << std::endl;

        if (state[i] == STOPPED) { // If its stopped, select a new point, and set mode to ROAM
            Vector2 point = map.get_empty_coordinate();

            int result = -1;
            // Save the enemy's path
            map.get_path_to(pos_2d, point, enemy_steps[i], ENEMYS_PER_AREA, result);

            if (result > 0) {
                state[i] = ROAM;
                poi[i] = point;
                action_index[i] = 0;
            }
        } else if (state[i] == ROAM) { // Traverse the Designated Path, and STOP when the end is reached
            Vector2 next_point = Vector2(0,0);
            map.parse_map_index_to_coordinates(enemy_steps[i][action_index[i]], next_point);

            if ((next_point - pos_2d).length() <= 1) {
                // If it very near to the current destination point, go to the next
                action_index[i]++;

                // The path has reched the end, s we aneter STOPPED mode
                if (action_index[i] >= MAX_STEPS_NUM || enemy_steps[i][action_index[i]] == -1) {
                    state[i] = STOPPED;
                    for (int i = 0; i <  ENEMYS_PER_AREA; i++) {
                        action_index[i] = -1;
                    }
                }
            } else {
                Vector3 enemy_facing_3d = Vector3(enemy_facing.x, 0., enemy_facing.y);
                // Move towards the next point in the path
                Vector2 new_pos = (next_point - pos_2d).normalize();
                Vector3 move_direction = Vector3(next_point.x - pos_2d.x , 0., next_point.y - pos_2d.y).normalize();
                kinetic_elems[i].position = kinetic_elems[i].position + (move_direction * ENEMY_SPEED * elapsed_time);

                float new_direction = atan2(new_pos.y, new_pos.x);
                
                float tmp_angle = lerp(new_direction, kinetic_elems[i].angle, 0.5);
                kinetic_elems[i].angle += (tmp_angle - kinetic_elems[i].angle) * elapsed_time * 2;
            }
        } else if (state[i] == RUN_AFTER) {
            
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
        kinetic_elems[i].get_model_matrix(&model);

        curr_shader->setUniform("u_model", model);
        mesh->drawCall(GL_TRIANGLES, 1, 0);
    }

    mesh->disableBuffers(curr_shader);
    curr_shader->disable();
}