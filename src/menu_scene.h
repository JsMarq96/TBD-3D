#ifndef _MENU_SCENE_H_
#define _MENU_SCENE_H_

#include "framework.h"
#include "stage.h"
#include "stage_parser.h"
#include "camera.h"
#include "texture.h"
#include "input.h"


#define MAX_GUI_ELEMS 10

enum eMenuSelection {
    START = 1,
    QUIT = 2
};

struct sGUI_Elem {
    Texture *text[2];
    float x, y; // Note: on clip space
    float w, h;

    sGUI_Elem() {};

    sGUI_Elem(Texture* t1, Texture* t2, float x_i, float  y_i, float w_i, float h_i) {
        text[0] = t1;
        text[1] = t2;
        x = x_i, y = y_i;
        w = w_i, h = h_i;
    }
};

struct sMenuScene {
    sStage *menu_stage;

    sGUI_Elem gui[MAX_GUI_ELEMS];
    Shader *gui_shader;

    int last_inserted_elem;

    int selected_index;

    float screen_progress;

    sMenuScene();

    ~sMenuScene();

    bool add_gui_element(Texture* text1, Texture* text2, float x, float y, float w, float h);

    void render_scene();
    void update_scene(float elapsed_timer, uint8 pressed_keys);
};

#endif