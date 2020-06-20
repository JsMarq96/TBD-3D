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
        // Enemy 2d position
        Vector2 enemy_pos_2d = Vector2(kinetic_elems[i].position.x, kinetic_elems[i].position.z);
        
        // Calculate angle between the player position and the enemy's direction
        Vector2 enemy_facing = Vector2(cos(kinetic_elems[i].angle), sin(kinetic_elems[i].angle));
        Vector2 to_player_vector = (player_2d_pos - enemy_pos_2d);
        Vector2 to_player_dir = to_player_vector.normalize();

        Vector3 to_player_3d = Vector3(to_player_dir.x, 0.f, to_player_dir.y).normalize();
        
        float enemy_player_distance = (player_2d_pos - enemy_pos_2d).length();
        float angle = acos(enemy_facing.dot(to_player_dir) / (enemy_facing.length() * to_player_dir.length())) * 180 / PI;

        // State transitions
        if (angle < 60.f && enemy_player_distance <= 0.7f) {
            // If it is facing to the player and its near, attack him
            state[i] = ATTACK;
            //std::cout << "atack" << std::endl;
        } else if (angle < 60.f && enemy_player_distance <= 20.0f) {
            // If it is in the eyesight of the player and it is
            float dist = map.raycast_from_point_to_point(enemy_pos_2d, player_2d_pos, 20.f);

            if (dist >= 0) {
                state[i] = RUN_AFTER;
            } else if (state[i] == RUN_AFTER) {
                state[i] = STOPPED;
            }
        }

        Vector3 move_direction = Vector3(0.f, 0.f, 0.f);
        Vector2 new_pos = Vector2(0.f, 0.f);

        //std::cout << (STOPPED == state[i]) << std::endl;

        if (state[i] == ROAM) {
            Vector2 next_pos;

            map.parse_map_index_to_coordinates(enemy_steps[i][action_index[i]], next_pos);

            new_pos = (next_pos - enemy_pos_2d).normalize();
            move_direction = Vector3(next_pos.x - enemy_pos_2d.x, 0.f, next_pos.y - enemy_pos_2d.y).normalize() * ENEMY_ROAM_SPEED;

            if ((enemy_pos_2d - next_pos).length() < 1) {
                if (action_index[i] == ENEMYS_PER_AREA-1 || enemy_steps[i][action_index[i] + 1]  == -1 ) {
                    action_index[i] = -1;

                    state[i] = STOPPED;
                } else {
                    action_index[i]++;
                }
            }
        } else if (state[i] == RUN_AFTER) {
            new_pos = to_player_dir;
            move_direction = Vector3(to_player_dir.x, 0.f, to_player_dir.y).normalize() * ENEMY_RUN_SPEED;
        } else if (state[i] == STOPPED) {
            if (random(1.0f) > 0.5f) {
                int result;
                Vector2 poi = map.get_near_empty_coordinate(enemy_pos_2d);

                map.get_path_to(enemy_pos_2d, poi, enemy_steps[i], MAX_STEPS_NUM, result);

                if (result > 0) {
                    action_index[i] = 0;
                    state[i] = ROAM;
                }
            } else {
                state[i] = STOPPED;
            }
        }

        // Apply directions
        float new_direction = atan2(new_pos.y, new_pos.x);
        kinetic_elems[i].position = kinetic_elems[i].position + (move_direction * elapsed_time);
        
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
        mesh->render(GL_TRIANGLES);
    }

    mesh->disableBuffers(curr_shader);
    curr_shader->disable();
}

void sEnemyEntity::enemy_is_shoot(int index, Vector3 coll_point) {

    if (coll_point.y > 1.9) { // Headshot!
        std::cout << "HEADSHOT";
    }
    std::cout << "  SHOT y: " << coll_point.y << std::endl;
}


void sEnemyEntity::testBulletCollisions(sBulletEntity &bullet_controller) {
    Mesh* mesh = Mesh::Get(mesh_id.c_str());

    for (int i = 0; i <= last_inserted_index; i++) {
        Matrix44 model;

        kinetic_elems[i].get_model_matrix(&model);

        Vector3 normal, coll_point;
        for (int j = 0; j <= MAX_BULLET_SIZE; j++) {
            if (!bullet_controller.is_active[j])
                continue;
            if (mesh->testRayCollision(model,
                    bullet_controller.position[i], 
                    bullet_controller.direction[i], 
                    coll_point, 
                    normal, 
                    BULLET_LENGHT,
                    true)) {

                bullet_controller.remove_bullet(j);
                enemy_is_shoot(j, coll_point);
            }
        }
    }

    //return false;
}
