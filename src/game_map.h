#ifndef _GAME_MAP_H_
#define _GAME_MAP_H_

#include <iostream>
#include "framework.h"
#include "extra/pathfinders.h"

#define SIGN(x) ((x > 0) ? 1 : ((x == 0) ? 0 : -1))

// Game Map for the AI to move in
struct sGameMap {
    uint8 *map;
    int width;
    int height;

    sGameMap(int m_width, int m_height);

    sGameMap() {};

    uint8 get(float x, float y);

    void set(float x, float y, uint8 value);

    void set_global(float x, float y, uint8 value);

    void add_area(float x, float y, float radius);

    // Randomly selects a random empty coordinate of the map
    Vector2 get_empty_coordinate();

    Vector2 get_near_empty_coordinate(Vector2 pos) {
        uint8 tmp = 0;
        Vector2 coords;
        while (tmp != 1) {
            tmp = 1;
            coords.x = pos.x + (random(1.0f) * 30.f) - 15.f;
            coords.y = pos.y + (random(1.0f) * 30.f) - 15.f;

            coords.x = max(coords.x, 0.f);
            coords.y = max(coords.y, 0.f);

            coords.x = min(coords.x, (float) width);
            coords.y = min(coords.y, (float) height);            

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
        result.x = (index % width);
        result.y = (floor(index / width));
    }

    void print_map() {
        std::cout << "====== MAP ======" << std::endl;
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                std::cout << " " << std::to_string(map[(i * width) + j]) << " ";
            }
            std::cout << std::endl;
        }
        std::cout << "=================" << std::endl;
    }    
};   
#endif