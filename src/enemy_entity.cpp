#include "enemy_entity.h"

sEnemyEntity::sEnemyEntity() {
    shader_fs_id = "data/shaders/phong.ps";
    shader_vs_id = "data/shaders/basic.vs";
    mesh_id = "data/meshes/player_stading.OBJ";
    texture_id = "data/textures/player_text.png";
    last_inserted_index = -1;

    for (int i = 0; i < ENEMYS_PER_AREA; i++) {
        action_index[i] = -1;
        state[i] = STOPPED;
    }
}

void sEnemyEntity::update(float elapsed_time, sGameMap &map, Vector3 player_pos) {
    Vector2 player_2d_pos = Vector2(player_2d_pos.x, player_2d_pos.y);

    map.parse_coordinates_to_map(player_2d_pos);

    for(int i = 0; i <= last_inserted_index; i++) {
        Vector2 pos_2d = Vector2(kinetic_elems[i].position.x, kinetic_elems[i].position.z);
        map.parse_coordinates_to_map(pos_2d);
        
        Vector2 enemy_facing = pos_2d.normalize();
        Vector2 to_player_dir = (player_2d_pos - pos_2d).normalize();
        float angle = acos(enemy_facing.dot(to_player_dir) / (enemy_facing.length() * to_player_dir.length()));

        if (state[i] == STOPPED) {
            Vector2 point = map.get_empty_coordinate();

            int result = -1;
            map.get_path_to(pos_2d, point, enemy_steps[i], ENEMYS_PER_AREA, result);

            if (result > 0) {
                state[i] = ROAM;
                poi[i] = point;
                action_index[i] = 0;
                std::cout << "Go to: " << poi[i].x << " "  << poi[i].y << std::endl;
            }
        } else if (state[i] == ROAM) {
            Vector2 next_point = Vector2(0,0);
            map.parse_map_index_to_coordinates(enemy_steps[i][action_index[i]], next_point);
            std::cout << "Curr point " << next_point.x << " " << next_point.y << std::endl;
            std::cout << "Enemy pso " << pos_2d.x << " " << pos_2d.y << std::endl;

            if ((next_point - pos_2d).length() <= 3) {
                action_index[i]++;

                if (action_index[i] >= MAX_STEPS_NUM || enemy_steps[i][action_index[i]] == -1) {
                    state[i] = STOPPED;
                    std::cout << "stopped" << std::endl;
                    for (int i = 0; i <  ENEMYS_PER_AREA; i++) {
                        action_index[i] = -1;
                    }
                }
            } else {
                Vector3 move_direction = Vector3(next_point.x - pos_2d.x, 0., next_point.y - pos_2d.y).normalize() * ENEMY_SPEED;
                kinetic_elems[i].position = kinetic_elems[i].position + (move_direction * elapsed_time);
            }
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