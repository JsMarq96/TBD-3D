#ifndef _GAME_CAMERA_H_
#define _GAME_CAMERA_H_

#include "framework.h"
#include "camera.h"

enum CamType : uint8 {
    THIRD_PERSON = 0,
    FIRST_PERSON,
    MENU // Todo: far future
};

struct sGameCamera {
    Camera cam;
    CamType cam_mode;

    sGameCamera() {
        cam_mode = THIRD_PERSON;
    }

    Camera* get_camera(Matrix44 player_model);
    void set_cam_mode(CamType n_type) { cam_mode = n_type; }
};

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
}


#endif