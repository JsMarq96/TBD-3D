#include "game_camera.h"
#include <iostream>
/**
 * Todo: - Animate between states
 *      - Change eye and up vectors to match model
 *      - Trail camera on thrid person view (?)
*/
void sGameCamera::get_camera(Camera* cam, Matrix44 player_model) {
    Vector3 eye, up, center;
    if (cam_mode == FIRST_PERSON) {
        eye = player_model * Vector3(.0f, 2.7f, 1.80f);
        center = player_model * Vector3(.0f, 2.3f, -5.0f);
        up = player_model.rotateVector(Vector3(0,1,0));
    } else if (cam_mode == THIRD_PERSON) {
        eye = player_model * Vector3(.0f, 2.7f, 1.80f);
        center = player_model * Vector3(0.2f, 2.6f, 0.0f);
        up = player_model.rotateVector(Vector3(0,1,0));
    }

    cam->lookAt(eye, center, up);

    //cam->rotate(Input::mouse_delta.x * 0.005f, Vector3(0.0f,-1.0f,0.0f));
	//cam->rotate(Input::mouse_delta.y * 0.005f, cam->getLocalVector( Vector3(-1.0f,0.0f,0.0f)));
}