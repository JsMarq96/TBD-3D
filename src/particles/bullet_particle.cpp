#include "bullet_particle.h"

    sBulletEntity::sBulletEntity() {
        for (int i = 0; i < MAX_BULLET_SIZE; i++) {
            particle_timer[i] = 0.0f;
            is_active[i] = false;
        }

        can_shoot_timer = 0.0f;
        
        // Create model
        bullet_mesh.vertices.push_back(Vector3(0,0,0));
        bullet_mesh.uvs.push_back(Vector2(0,0));

        bullet_mesh.vertices.push_back(Vector3(0.2,0,0));
        bullet_mesh.uvs.push_back(Vector2(1,0));

        bullet_mesh.vertices.push_back(Vector3(0.0,0.1,0));
        bullet_mesh.uvs.push_back(Vector2(0,0));

        bullet_mesh.vertices.push_back(Vector3(0.2,0.1,0));
        bullet_mesh.uvs.push_back(Vector2(1,0));

         bullet_mesh.vertices.push_back(Vector3(0.2,0.1,0.2));
        bullet_mesh.uvs.push_back(Vector2(1,1));

         bullet_mesh.vertices.push_back(Vector3(-0.2,0.1,0.2));
        bullet_mesh.uvs.push_back(Vector2(1,1));
    }

    void sBulletEntity::update(float elapsed_time) {
        for (int i = 0; i < MAX_BULLET_SIZE; i++) {
            if (is_active[i]) {
                // Update position
                position[i] = position[i] + (direction[i] * elapsed_time * BULLET_SPEED);

                particle_timer[i] += elapsed_time;
                // Deactivate if the timer is exceded
                if (particle_timer[i] >= BULLET_TIMER_RANGE) {
                    is_active[i] = false;
                }
            }
        }

        can_shoot_timer -= elapsed_time;
    }

    void sBulletEntity::render(Camera *cam) {
        Shader *curr_shader = Shader::Get("data/shaders/basic.vs", "data/shaders/bullet.ps");
        curr_shader->enable();
            
        curr_shader->setUniform("u_color", Vector4(1, 1, 1, 1));
        curr_shader->setUniform("u_texture", Texture::Get("data/textures/bullet_texture.png"));
        curr_shader->setUniform("camera_pos", cam->eye);
        curr_shader->setUniform("u_viewprojection", cam->viewprojection_matrix);

        bullet_mesh.enableBuffers(curr_shader);

        for (int i = 0; i <= MAX_BULLET_SIZE; i++) {
            if (!is_active[i])
                continue;
            Matrix44 model;
            model.setTranslation(position[i].x, position[i].y, position[i].z);

            curr_shader->setUniform("u_model", model);

            bullet_mesh.drawCall(GL_TRIANGLES, 1, 0);
        }

        bullet_mesh.disableBuffers(curr_shader);
        curr_shader->disable();
    }