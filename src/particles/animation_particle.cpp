#include "animation_particle.h"

    sAnimationParticles::sAnimationParticles(Texture* text, int width, int height, int max_fram, float anim_duration) {
        texture_sheet = text;
        text_width = width, text_height = height;
        max_frames = max_fram;
        animation_duration = anim_duration;

        for (int i = 0; i < PARTICLE_SIZE; i++) {
            elem_timer[i] = 0.0f;
            is_active[i] = false;
        }


    }

    // Creates a Kinetic particle
    void sAnimationParticles::set_kinetic(Vector3 dir) {
        is_kinetic = true;
        direction = dir;
    }

    // Find an empty particel spot and adds it
    bool sAnimationParticles::add_instance(Vector3 position) {
        for (int i = 0; i < PARTICLE_SIZE; i++) {
            if (is_active[i])
                continue;

            positions[i] = position;
            is_active[i] = true;
            elem_timer[i] = 0.0f;

            return true;
        }

        return false;
    }

    // Update movement or timers of the particles
    void sAnimationParticles::update(float elaped_time) {
        for (int i = 0; i < PARTICLE_SIZE; i++) {
            if (!is_active[i])
                continue;

            elem_timer[i] += elaped_time;

            if (elem_timer[i] >= animation_duration) {
                is_active[i] = false;
            }

            if (is_kinetic) {
                positions[i] = positions[i] + direction;
            }
        }
    }



    void sAnimationParticles::render(Camera *cam) {
        Shader *curr_shader = Shader::Get("data/shaders/basic.vs", "data/shaders/particle.ps");
        curr_shader->enable();

        Mesh plane_mesh;

        // First triangle
        plane_mesh.vertices.push_back(Vector3(-0.25f, 0.25f, 0.0f)); // p1
        plane_mesh.vertices.push_back(Vector3(0.25f, 0.25f, 0.0f)); // p2
        plane_mesh.vertices.push_back(Vector3(-0.25f, -0.25f, 0.0f)); // p3

        // Second triangle
        plane_mesh.vertices.push_back(Vector3(0.25f, 0.25f, 0.0f)); // p2
        plane_mesh.vertices.push_back(Vector3(-0.25f, -0.25f, 0.0f)); // p3
        plane_mesh.vertices.push_back(Vector3(0.25f, -0.25f, 0.0f)); // p4

        glDisable(GL_CULL_FACE);
            
        curr_shader->setUniform("u_color", Vector4(1, 1, 1, 1));
        curr_shader->setUniform("u_texture", texture_sheet);
        curr_shader->setUniform("camera_pos", cam->eye);
        curr_shader->setUniform("u_viewprojection", cam->viewprojection_matrix);

        for (int i = 0; i <= PARTICLE_SIZE; i++) {
            if (!is_active[i])
                continue;

            // Get the texture fragment of the current animation
            int index = floor(lerp(animation_duration, max_frames, elem_timer[i]));
            // Get the UV coordinates of the quad that we want to presenet
            int frame_x = index % text_width, frame_y = floor(index/text_height);

            // Assuming that the tiles in the texture are squares
            float uv_frag_size = 1.0f / text_width;
            float frame_u = frame_x * uv_frag_size, frame_v = frame_y * uv_frag_size;
            
            
            // Configure the UVs of the quad, to reflect the part of the image that is drawing
            plane_mesh.uvs.clear();

            // First triangle
            plane_mesh.uvs.push_back(Vector2(frame_u, frame_v)); // p1
            plane_mesh.uvs.push_back(Vector2(frame_u + uv_frag_size, frame_v)); // p2
            plane_mesh.uvs.push_back(Vector2(frame_u, frame_v + uv_frag_size));// p3

            // Second triangle
            plane_mesh.uvs.push_back(Vector2(frame_u, frame_v + uv_frag_size)); // p3
            plane_mesh.uvs.push_back(Vector2(frame_u + uv_frag_size, frame_v)); // p2
            plane_mesh.uvs.push_back(Vector2(frame_u + uv_frag_size, frame_v + uv_frag_size)); // p4

            Matrix44 model;
            model.setTranslation(positions[i].x, positions[i].y, positions[i].z);

            // Set rotation to facing to camera
            //std::cout << abs(atan2(cam->eye.y - positions[i].y, cam->eye.x - positions[i].x) * 180.f / PI) << std::endl;
            model.rotate(abs(atan2(cam->eye.y - positions[i].y, cam->eye.x - positions[i].x) * 180.f / PI), Vector3(0.0f, 1.0f, 0.0f));

            curr_shader->setUniform("u_model", model);

            plane_mesh.render(GL_TRIANGLES);
        }

        glEnable(GL_CULL_FACE);

        curr_shader->disable();
    }