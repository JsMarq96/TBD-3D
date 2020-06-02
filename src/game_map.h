#ifndef _GAME_MAP_H_
#define _GAME_MAP_H_

#include "framework.h"
#include "pathfinders.h"

// Game Map for the AI to move in
struct sGameMap {
    uint8 *map;
    int real_width;
    int real_heigth;
    int map_width;
    int map_height;

    sGameMap(int width, int height) { 
        real_heigth = height;
        real_width = width;
        map_height = height / 2;
        map_width = width / 2;

        map = new uint8[map_width * map_height];
    };

    uint8 get(int x, int y);

    // Todo
    Vector2 translate_to_world_coord(Vector2 map_coord);

    // Randomly selects a random empty coordinate of the map
    Vector2 get_empty_coordinate();

    void get_path_to(Vector2 start, Vector2 goal, int* steps, int max_steps, int &result);
    // Casts a Ray from one point in the map to other,
    // usign the Bressham line algortihm
    float raycast_from_point_to_point(Vector2 p1, Vector2 p2, float max_size);
};   
#endif