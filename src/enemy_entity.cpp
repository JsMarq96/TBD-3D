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

        for (int j = 0; j < MAX_STEPS_NUM; j++) {
            enemy_steps[i][j] = -1;
        }
    }
}

void sEnemyEntity::update(float elapsed_time, sGameMap &map, Vector3 player_pos) {
    Vector2 player_2d_pos = Vector2(player_pos.x, player_pos.z);

    for(int i = 0; i <= last_inserted_index; i++) {
        Vector2 pos_2d = Vector2(kinetic_elems[i].position.x, kinetic_elems[i].position.z);
        
        // Calculate angle between the player position and the enemy's direction
        Vector2 enemy_facing = Vector2(cos(kinetic_elems[i].angle), sin(kinetic_elems[i].angle)); //Vector2(kinetic_elems[i].rotation.x);
        Vector2 to_player_dir = (pos_2d - player_2d_pos).normalize();
        Vector3 to_player_3d = Vector3(to_player_dir.x, 0.f, to_player_dir.y).normalize();
        float enemy_player_distance = (pos_2d - player_2d_pos).length();
        float angle = acos(enemy_facing.dot(to_player_dir) / (enemy_facing.length() * to_player_dir.length())) * 180 / PI;

        std::cout << " " << angle << std::endl;

        if (state[i] == RUN_AFTER && angle > 80 && enemy_player_distance <= 1.5) {
            // If it is facing to the player and its near, attack him
            state[i] = ATTACK;
            //std::cout << "atack" << std::endl;
        } else if (angle > 80 && enemy_player_distance <= 20) {
            // If it is in the eyesight of the player and it is
            float dist = map.raycast_from_point_to_point(pos_2d, player_2d_pos, 20);

            if (dist >= 0) {
                state[i] = RUN_AFTER;
            }
        }

        Vector3 move_direction = Vector3(0.f, 0.f, 0.f);
        float new_direction = 0.f;

        if (state[i] == RUN_AFTER) {
            Vector3 enemy_facing_3d = Vector3(enemy_facing.x, 0., enemy_facing.y);
            // Move towards the next point in the path
            Vector2 new_pos = (player_2d_pos - pos_2d).normalize();
            move_direction = Vector3(player_2d_pos.x - pos_2d.x , 0., player_2d_pos.y - pos_2d.y).normalize();
            new_direction = atan2(new_pos.y, new_pos.x);
        } else if (state[i] == ATTACK) {

        }

        kinetic_elems[i].position = kinetic_elems[i].position + (move_direction * ENEMY_SPEED * elapsed_time);
        float tmp_angle = lerp(new_direction, kinetic_elems[i].angle, 0.5);
        kinetic_elems[i].angle += (tmp_angle - kinetic_elems[i].angle) * elapsed_time * 2;
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