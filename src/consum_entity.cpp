#include "consum_entity.h"

    sConsumableEntities::sConsumableEntities() {
        texts[AMMO] = Texture::Get("data/textures/ammo_text.png");
        texts[HEALTH] = Texture::Get("data/textures/health_text.png");

        box_mesh = Mesh::Get("data/meshes/box.obj");

        shader = Shader::Get("data/shaders/basic.vs", "data/shaders/phong.ps");

        for(int i= 0 ; i < CONSUMABLES_NUM; i++) {
            type[i] = EMPTY;
            position[i].y = 0.5;
        }
    }

    void sConsumableEntities::add_random_element(Vector3 pos) {
        int index = random(1.0f);
        add_element(pos, (eConsumType) index);
    }

    void sConsumableEntities::add_element(Vector3 elem, eConsumType items) {
        for(int i = 0; i < CONSUMABLES_NUM; i++) {
            if (type[i] != EMPTY)
                continue;

            type[i] = items;
            position[i] = elem;
            position[i].y = 0.5;
            timer[i] = 0.0f;

            return;
        }
    }

    void sConsumableEntities::update(float elapsed_time) {
        for(int i = 0; i < CONSUMABLES_NUM; i++) {
            if (type[i] == EMPTY)
                continue;

            if (position[i].y <= 0.0f)
                continue;
            
            timer[i] += elapsed_time;

            float elevation = sin((timer[i] + 0.4f) * 2.3f) * 2.f;

            position[i].y = elevation;
            position[i].x += elapsed_time * 0.20f;
            position[i].z += elapsed_time * 0.20f;
        }
    }

    void sConsumableEntities::render(Camera* cam) {
        shader->enable();

        shader->setUniform("u_color", Vector4(1, 1, 1, 1));
        shader->setUniform("u_camera_pos", cam->eye);
        shader->setUniform("u_viewprojection", cam->viewprojection_matrix);

        for(int i = 0; i < CONSUMABLES_NUM; i++) {
            if (type[i] == EMPTY)
                continue;

            Matrix44 m;
            m.setTranslation(position[i].x, position[i].y, position[i].z);

            shader->setUniform("u_texture", texts[type[i]]);
            shader->setUniform("u_model", m);

            box_mesh->render(GL_TRIANGLES);
        }

        shader->disable();
    }