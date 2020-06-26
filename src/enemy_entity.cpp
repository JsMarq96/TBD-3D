#include "enemy_entity.h"

sEnemyEntity::sEnemyEntity() {
    shader_fs_id = "data/shaders/phong_with_alpha.ps";
    shader_vs_id = "data/shaders/skinning.vs";
    mesh_id = "data/meshes/enemy.mesh";
    texture_id = "data/textures/enemy_text.png";
    last_inserted_index = -1;

    for (int i = 0; i < ENEMYS_PER_AREA; i++) {
        action_index[i] = -1;
        state[i] = STOPPED;
        enemy_health[i] = ENEMY_STARTING_HEALTH;
        enemy_hit_cooldown_timer[i] = 0.0f;
        enemy_recovering_timers[i] = 0.0f;
        enemy_alphas[i] = 1.0f;

        for (int j = 0; j < MAX_STEPS_NUM; j++) {
            enemy_steps[i][j] = -1;
        }
    }

    // Load the particles
    blood = sAnimationParticles(Texture::Get("data/particles/blood_hit.png"), 4, 4, 2.0f, 13, 0.15);

    // Fetch animations
    animations[STOPPED] = Animation::Get("data/animations/animations_zombie_idle.skanim");
    animations[ROAM] = Animation::Get("data/animations/animations_walking_zombie.skanim"); 
    animations[RUN_AFTER] = Animation::Get("data/animations/animations_zombie_run.skanim");
    animations[ATTACK] = Animation::Get("data/animations/animations_zombie_attack.skanim");
}

// NOTE: This part is kid of a mess, so before trying to make sense about it, I would
// take a look to the README...
void sEnemyEntity::update(float elapsed_time, sGameMap &map, Vector3 player_pos) {
    Vector2 player_2d_pos = Vector2(player_pos.x, player_pos.z);

    for(int i = 0; i <= last_inserted_index; i++) {
        if (state[i] == RECOVERING) {
            kinetic_elems[i].speed = Vector3(0,0,0);

            enemy_recovering_timers[i] -= elapsed_time;

            enemy_alphas[i] = lerp(1.0f, 0.2f, enemy_recovering_timers[i] / ENEMY_RECOVERING_TIMER);

            if (enemy_recovering_timers[i] <= 0.0f) {
                state[i] = STOPPED;
                enemy_health[i] = ENEMY_STARTING_HEALTH;

                std::cout << "Enemy recovered!" << std::endl;
            }
            continue;
        }
        
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
        // NOTE: In order to prevent estability, the minimi distance to maintian the ATTACk state
        // is bigger than the value to enter the ATTACK state
        if (state[i] == ATTACK && ((angle < 50.f && angle > 143.f) || enemy_player_distance > 2.2f)) {
            state[i] = ROAM;
        }

        if (state[i] != ATTACK && angle > 50.f && angle < 143.f && enemy_player_distance <= 1.5f) {
            // If it is facing to the player and its near, attack him
            state[i] = ATTACK;
            attack_animation_duration = 0.0f;
            std::cout << "ATTACK MODE" << std::endl;
        } else if (state[i] != ATTACK && angle > 50.f && angle < 143.f && enemy_player_distance <= 20.0f) {
            // If it is in the eyesight of the enemy
            float dist = map.raycast_from_point_to_point(enemy_pos_2d, player_2d_pos, 20.f);

            if (dist >= 0) {
                state[i] = RUN_AFTER;
            } else {
                state[i] = STOPPED;
            }
        }

        Vector3 move_direction = Vector3(0.f, 0.f, 0.f);
        Vector2 new_pos = Vector2(0.f, 0.f);

        // Execute the code of the corresponding state
        if (state[i] == ROAM) {
            Vector2 next_pos;

            map.parse_map_index_to_coordinates(enemy_steps[i][action_index[i]], next_pos);

            // Go the next point in the A* defined path
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
            // Simply runs towards the player
            new_pos = to_player_dir;
            move_direction = Vector3(to_player_dir.x, 0.f, to_player_dir.y).normalize() * ENEMY_RUN_SPEED;
        } else if (state[i] == STOPPED) {
            // Traces a route to a random point near the enemy
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
        } else if (state[i] == ATTACK) {
            // Attacks the player
            if (enemy_hit_cooldown_timer[i] >= 0.0) { // Attack cooldown timer
                enemy_hit_cooldown_timer[i] -= elapsed_time;
            } else {
                attack_animation_duration += elapsed_time;

                if (attack_animation_duration >= 1.9f) {
                // Hit moment!
                if (enemy_player_distance <= 1.7f) {
                    // If it is near, hit
                    sPlayer::instance->hit(kinetic_elems[i].position);
                    enemy_hit_cooldown_timer[i] = ENEMY_ATTACK_COOLDOWN; // Cooldown timer
                    attack_animation_duration = 0;
                } else {
                    attack_animation_duration = 0;
                }
            }
            }
        }

        // Apply kinetic changes
        kinetic_elems[i].speed = kinetic_elems[i].speed + (move_direction - kinetic_elems[i].speed) * elapsed_time;
        kinetic_elems[i].position = kinetic_elems[i].position + (move_direction * elapsed_time);
        
        float tmp_angle = acos( clamp(enemy_facing.dot(new_pos), -1.0f, 1.0f) );
        kinetic_elems[i].angle += (tmp_angle - kinetic_elems[i].angle) * elapsed_time;
    }

    blood.update(elapsed_time);
}



void sEnemyEntity::render(Camera *camara)  {
    Shader *curr_shader = Shader::Get(shader_vs_id.c_str(), shader_fs_id.c_str());
    Mesh *mesh = Mesh::Get(mesh_id.c_str());

    // Enable transparency for enemy recovering
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    curr_shader->enable();
        
    curr_shader->setUniform("u_color", Vector4(1, 1, 1, 1));
    curr_shader->setUniform("u_texture", Texture::Get(texture_id.c_str()));
    curr_shader->setUniform("u_light_pos", Vector3(250, 120, 250));
    curr_shader->setUniform("u_camera_pos", camara->eye);
    curr_shader->setUniform("u_viewprojection", camara->viewprojection_matrix);

    mesh->enableBuffers(curr_shader);

    for (int i = 0; i <= last_inserted_index; i++) {
        animations[STOPPED]->assignTime(Game::instance->time + i);
        animations[ROAM]->assignTime(Game::instance->time + i);
        animations[RUN_AFTER]->assignTime(Game::instance->time + i);
        animations[ATTACK]->assignTime(attack_animation_duration);

        Skeleton blend_sk;

        // Blend the First 3 animations based on the speed of the speed of the enemy, with two boundaries, one for each speed
        // And then, blend the Attack skelleton based on how many time it has spent on the ATTACK state;
        // also, the punch happens at the 1.9f aprox of the animation, so, the interpolations is done
        // with 1.3f in order to ensure that when the time comes, it is completely on the attack animation
        blendSkeleton(&animations[STOPPED]->skeleton, &animations[ROAM]->skeleton, kinetic_elems[i].speed.length() / ENEMY_ROAM_SPEED, &blend_sk);
        blendSkeleton(&blend_sk, &animations[RUN_AFTER]->skeleton, kinetic_elems[i].speed.length() / ENEMY_RUN_SPEED, &blend_sk);
        blendSkeleton(&blend_sk, &animations[ATTACK]->skeleton, (state[i] == ATTACK) ? attack_animation_duration / 1.3f : 0.0, &blend_sk);

        Matrix44 model;
        kinetic_elems[i].get_model_matrix(&model);

        curr_shader->setUniform("u_model", model);
        curr_shader->setUniform("u_alpha", enemy_alphas[i]);

        mesh->renderAnimated( GL_TRIANGLES, &blend_sk);
    }

    mesh->disableBuffers(curr_shader);
    curr_shader->disable();

    glDisable(GL_BLEND);

    blood.render(camara);
}

void sEnemyEntity::enemy_is_shoot(int index, Vector3 coll_point, Vector3 coll_normal, sConsumableEntities &cons) {

    if (coll_point.y > 1.78) { // Headshot!
        std::cout << "  HEADSHOT!" << std::endl;
        enemy_health[index] = 0;
    } else {
        std::cout << "  SHOT y: " << coll_point.y << std::endl;
       enemy_health[index]--; 
       
    }

    if (enemy_health[index] == 0) {
        state[index] = RECOVERING;
        enemy_recovering_timers[index] = ENEMY_RECOVERING_TIMER;
        cons.add_random_element(kinetic_elems[index].position + coll_point);
        std::cout << "Enemy OUT" << std::endl;
    }

    // Added blood splatter
    blood.add_instance(kinetic_elems[index].position + coll_point);
}


void sEnemyEntity::testBulletCollisions(sBulletEntity &bullet_controller, sConsumableEntities &cons) {
    Mesh* mesh = Mesh::Get(mesh_id.c_str());

    for (int i = 0; i <= last_inserted_index; i++) {
        Matrix44 model;

        kinetic_elems[i].get_model_matrix(&model);

        Vector3 normal, coll_point;
        for (int j = 0; j <= MAX_BULLET_SIZE; j++) {
            if (!bullet_controller.is_active[j])
                continue;
            if (mesh->testRayCollision(model,
                    bullet_controller.position[j], 
                    bullet_controller.direction[j], 
                    coll_point, 
                    normal, 
                    BULLET_LENGHT,
                    true)) {

                bullet_controller.remove_bullet(j);
                enemy_is_shoot(i, coll_point, normal, cons);
            }
        }
    }
}
