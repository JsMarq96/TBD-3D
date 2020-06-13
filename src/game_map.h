#ifndef _GAME_MAP_H_
#define _GAME_MAP_H_

#include <iostream>
#include "framework.h"
#include "extra/pathfinders.h"

#define SIGN(x) ((x > 0) ? 1 : ((x == 0) ? 0 : -1))

// Game Map for the AI to move in
struct sGameMap {
    uint8 *map;
    int real_width;
    int real_heigth;
    int map_width;
    int map_height;
    int map_scale;

    sGameMap(int width, int height);

    sGameMap() {};

    uint8 get(float x, float y);

    void set(float x, float y, uint8 value);

    void set_global(float x, float y, uint8 value);

    void add_area(float x, float y, float radius);

    // Todo
    Vector2 translate_to_world_coord(Vector2 map_coord) {
        Vector2 coord;
        coord.x = map_coord.x / 0.5;
        coord.y = map_coord.y / 0.5;

        return coord;
    }

    // Randomly selects a random empty coordinate of the map
    Vector2 get_empty_coordinate();

    Vector2 get_near_empty_coordinate(Vector2 pos) {
        pos = pos * 0.5;
        uint8 tmp = 0;
        Vector2 coords;
        while (tmp != 1) {
            coords.x = pos.x + (random(1.0f) * 20.f) - 10.f;
            coords.y = pos.y + (random(1.0f) * 20.f) - 10.f;

            coords.x = max(coords.x, 0.f);
            coords.y = max(coords.y, 0.f);

            coords.x = min(coords.x, (float) map_width);
            coords.y = min(coords.y, (float) map_height);            

            tmp = get(coords.x, coords.y);
        }

        return coords;
    }

    void get_path_to(Vector2 start, Vector2 goal, int* steps, int max_steps, int &result);
    // Casts a Ray from one point in the map to other,
    // usign the Bressham line algortihm
    float raycast_from_point_to_point(Vector2 p1, Vector2 p2, float max_size);

    // for parsing the indexes produce by the A* search algrotihm
    void parse_map_index_to_coordinates(int index, Vector2& result) {
        result.x = (index % map_width) * 2;
        result.y = (floor(index / map_width)) *2;
        //std::cout << result.x << " " << result.y << std::endl;
    }

    void parse_coordinates_to_map(Vector2 &coords) {
        coords = coords * 0.5;
    }

    void print_map() {
        std::cout << "====== MAP ======" << std::endl;
        for (int i = 0; i <= map_width; i++) {
            for (int j = 0; j <= map_height; j++) {
                std::cout << " " << std::to_string(map[(i * map_width) + j]) << " ";
            }
            std::cout << std::endl;
        }
        std::cout << "=================" << std::endl;
    }    
};   
#endif