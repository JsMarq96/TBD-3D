#ifndef _GAME_MAP_H_
#define _GAME_MAP_H_

#include "framework.h"
#include "extra/pathfinders.h"

// Game Map for the AI to move in
struct sGameMap {
    uint8 *map;
    int real_width;
    int real_heigth;
    int map_width;
    int map_height;

    sGameMap(int width, int height);

    uint8 get(int x, int y);

    // Todo
    Vector2 translate_to_world_coord(Vector2 map_coord) {
        Vector2 coord;
        coord.x = map_coord.x * 2;
        coord.y = map_coord.y * 2;

        return coord;
    }

    // Randomly selects a random empty coordinate of the map
    Vector2 get_empty_coordinate();

    void get_path_to(Vector2 start, Vector2 goal, int* steps, int max_steps, int &result);
    // Casts a Ray from one point in the map to other,
    // usign the Bressham line algortihm
    float raycast_from_point_to_point(Vector2 p1, Vector2 p2, float max_size);

    // for parsing the indexes produce by the A* search algrotihm
    void parse_map_index_to_coordinates(int index, Vector2& result) {
        float t = index / map_width;
        int t_int = t;
        float t_float_part = t - t_int;
        result.x = (int) t_float_part * map_width;
        result.y = (int) t_int;

        result = result * 2.0f;
    }
};   
#endif