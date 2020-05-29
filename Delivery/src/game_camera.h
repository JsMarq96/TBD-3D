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
    CamType cam_mode;

    sGameCamera() {
        cam_mode = THIRD_PERSON;
    }

    void get_camera(Camera* cam, Matrix44 player_model);
    void set_cam_mode(CamType n_type) { cam_mode = n_type; }
};


#endif