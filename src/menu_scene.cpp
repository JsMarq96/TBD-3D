#include "menu_scene.h"

sMenuScene::sMenuScene() {
    menu_stage = new sStage(0,0, 500, 500);
    //parse_stage(menu_stage, "");

    screen_progress = 0.0f;
    last_inserted_elem = -1;
    selected_index = 1;

    gui_shader = Shader::Get("data/shaders/quad.vs", "data/shaders/gui.ps");

    // Title Screen
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

    Camera *cam = Camera::current;
    //menu_stage->render_stage(cam);

    // Draw GUI element
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
};