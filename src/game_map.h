#ifndef _GAME_MAP_H_
#define _GAME_MAP_H_

#include <iostream>
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

    sGameMap() {};

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
        result.x = index % map_width;
        result.y = floor(index / map_width);

        result = result * 2.0f;
    }

    void parse_coordinates_to_map(Vector2 &coords) {
        coords = coords * 0.5f;
    }

    void print_map() {
        std::cout << "====== MAP ======" << std::endl;
        for (int i = 0; i < map_width; i++) {
            for (int j = 0; j < map_height; j++) {
                std::cout << " " << std::to_string(map[(i * map_width/2) + j]) << " ";
            }
            std::cout << std::endl;
        }
        std::cout << "=================" << std::endl;
    }
};   
#endif