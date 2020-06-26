#include "menu_scene.h"

sMenuScene::sMenuScene() {
    menu_stage = new sStage(0,0, 500, 500);
    //parse_stage(menu_stage, "data/stages/01.stg");

    env_trees = new sEnviormentEntity();
    env_trees->mesh_id = "data/meshes/tree.obj";
    env_trees->texture_id = "data/textures/tree.png";
    env_trees->shader_fs_id = "data/shaders/phong.ps";
    env_trees->shader_vs_id = "data/shaders/instanced.vs";
    env_trees->fl_instancing = true;

    for(int i = 0; i < 200; i++) {
        Matrix44 model;
        model.setTranslation(random(1.0f) * 200 ,0, random(1.0f) * 200);

        env_trees->add_element(model);
    }

    screen_progress = 0.0f;
    last_inserted_elem = -1;
    selected_index = 1;

    cam = Camera::current;

    camera_pos = Vector2(150, 100);

    gui_shader = Shader::Get("data/shaders/quad.vs", "data/shaders/gui.ps");

    // Title Screen's elements
    add_gui_element(Texture::Get("data/gui/title.png"), NULL, -0.80f, 0.80f, 0.65f, -0.45f);
    add_gui_element(Texture::Get("data/gui/start_button.png"), Texture::Get("data/gui/start_button_act.png"), 0.30f, 0.00f, 0.4f , -0.20f);
    add_gui_element(Texture::Get("data/gui/quit_button.png"), Texture::Get("data/gui/quit_button_act.png"), 0.30f, -0.5f, 0.4f , -0.20f);
};

sMenuScene::~sMenuScene() {
        
};

bool sMenuScene::add_gui_element(Texture* text1, Texture* text2, float x, float y, float w, float h) {
    last_inserted_elem++;
    if (last_inserted_elem >= MAX_GUI_ELEMS) {
        return false;
    }

    gui[last_inserted_elem] = sGUI_Elem(text1, text2, x, y, w, h);

    return true;
};

void sMenuScene::render_scene() {
    glClearColor(0.07, 0.07, 0.07, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

    // Config camera
    //float camera_flipflopping = (sin(Game::instance->time * 5.0f) * 0.25f);
    Vector3 eye = Vector3(camera_pos.x, 2.5f, camera_pos.y);
    Vector3 center = Vector3(camera_pos.x + 1, 2.5f, camera_pos.y + 1);
    Vector3 up =  Vector3(0,1,0);

    cam->lookAt(eye, center, up);

    // Render enviorment
    // Floor
    Mesh floor_mesh = Mesh();
    floor_mesh.createPlane(350);
    Shader *shad = Shader::Get("data/shaders/basic.vs", "data/shaders/phong.ps");
    shad->enable();
    shad->setUniform("u_texture", Texture::Get("data/floor.png"));
    shad->setUniform("u_color", Vector4(1, 1, 1, 1));
    shad->setUniform("u_viewprojection", cam->viewprojection_matrix);
    shad->setUniform("u_model", Matrix44());
    shad->setUniform("u_light_pos", Vector3(250, 120, 250));
    shad->setUniform("u_camera_pos", eye);
    shad->setUniform("u_double_light", false);
    shad->setUniform("u_second_light_pos", Vector3());
    floor_mesh.render(GL_TRIANGLES);
    shad->disable();

    // Render enviorment element's elements
    env_trees->render(cam, false, Vector3());

    // Draw GUI elements
    gui_shader->enable();
    for (int i = 0; i <= last_inserted_elem; i++) {
        Mesh elem;
        // First triangle
        elem.vertices.push_back(Vector3( gui[i].x, gui[i].y, 0.0f ));
        elem.vertices.push_back(Vector3( gui[i].x + gui[i].w, gui[i].y, 0.0f ));
        elem.vertices.push_back(Vector3( gui[i].x, gui[i].y + gui[i].h, 0.0f ));

        // Second triangle
        elem.vertices.push_back(Vector3( gui[i].x + gui[i].w, gui[i].y, 0.0f ));
        elem.vertices.push_back(Vector3( gui[i].x + gui[i].w, gui[i].y + gui[i].h, 0.0f ));
        elem.vertices.push_back(Vector3( gui[i].x, gui[i].y + gui[i].h, 0.0f ));

        elem.uvs.push_back(Vector2(0,1));
        elem.uvs.push_back(Vector2(1,1));
        elem.uvs.push_back(Vector2(0,0));

        elem.uvs.push_back(Vector2(1,1));
        elem.uvs.push_back(Vector2(1,0));
        elem.uvs.push_back(Vector2(0,0));

        if (i == selected_index) {
            gui_shader->setUniform("u_texture", gui[i].text[1]);
        } else {
            gui_shader->setUniform("u_texture", gui[i].text[0]);            
        }

        gui_shader->setUniform("u_alpha", 1.0f);

        elem.render(GL_TRIANGLES);
    }

    gui_shader->disable();
    glEnable(GL_CULL_FACE);
};

void sMenuScene::update_scene(float elapsed_timer, uint8 pressed_keys) {
    if (Input::isKeyPressed(SDL_SCANCODE_S)) {
        selected_index++;
    } else if (Input::isKeyPressed(SDL_SCANCODE_W)) {
        selected_index--;
    }

    selected_index = clamp(selected_index, 1, 2);

    // Traveling camera
    camera_pos.x = (camera_pos.x * cos(CAMERA_ROTATION_SPEED * elapsed_timer * 0.6)) - (camera_pos.y * sin(CAMERA_ROTATION_SPEED * elapsed_timer * 0.6));
    camera_pos.y = (camera_pos.x * sin(CAMERA_ROTATION_SPEED * elapsed_timer * 0.6)) + (camera_pos.y * cos(CAMERA_ROTATION_SPEED * elapsed_timer * 0.6));
    
    std::cout << camera_pos.x << " " << camera_pos.y  << " " << camera_pos.length() << std::endl;
};