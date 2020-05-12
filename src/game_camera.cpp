#include "game_camera.h"

/**
 * Todo: - Animate between states
 *      - Change eye and up vectors to match model
 *      - Trail camera on thrid person view (?)
*/
Camera* sGameCamera::get_camera(Matrix44 player_model) {
    Vector3 eye, up, center;
    if (cam_mode == FIRST_PERSON) {
        eye = player_model * Vector3(0, 10, 30);
        center = player_model * Vector3();
    } else if (cam_mode == THIRD_PERSON) {
        eye = player_model * Vector3();
        center = player_model * Vector3();
    }

    up = player_model.rotateVector(Vector3(0,1,0));

    cam.lookAt(eye, center, up);

    return &cam;
}