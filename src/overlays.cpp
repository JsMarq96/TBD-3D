#include "overlays.h"

void add_overlay(Texture *text, float alpha) {
    glEnable(GL_BLEND);
    glDisable(GL_CULL_FACE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Shader *curr_shader = Shader::Get("data/shaders/quad.vs", "data/shaders/overlay.ps");
    curr_shader->enable();

    Mesh elem;
    // First triangle
    elem.vertices.push_back(Vector3( -1.0f, 1.0f, 0.0f ));
    elem.vertices.push_back(Vector3( 1.0f, 1.0f, 0.0f ));
    elem.vertices.push_back(Vector3( -1.0f, -1.0f, 0.0f ));
    // Second triangle
    elem.vertices.push_back(Vector3( 1.0f, 1.0f, 0.0f  ));
    elem.vertices.push_back(Vector3( 1.0f, -1.0f, 0.0f ));
    elem.vertices.push_back(Vector3( -1.0f, -1.0f, 0.0f ));

    elem.uvs.push_back(Vector2(0,1));
    elem.uvs.push_back(Vector2(1,1));
    elem.uvs.push_back(Vector2(0,0));

    elem.uvs.push_back(Vector2(1,1));
    elem.uvs.push_back(Vector2(1,0));
    elem.uvs.push_back(Vector2(0,0));

    /*if (i == selected_index) {
        gui_shader->setUniform("u_texture", gui[i].text[1]);
    } else {
        gui_shader->setUniform("u_texture", gui[i].text[0]);           
    } */

    curr_shader->setUniform("u_alpha", alpha);
    curr_shader->setUniform("u_texture", text);

    elem.render(GL_TRIANGLES);

    curr_shader->disable();

    glEnable(GL_CULL_FACE);
    glDisable(GL_BLEND);
}