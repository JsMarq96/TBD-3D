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

    sGameMap(int width, int height, float scale);

    sGameMap() {};

    uint8 get(int x, int y);

    void set(int x, int y, uint8 value);

    void add_area(float x, float y, float radius);

    // Todo
    Vector2 translate_to_world_coord(Vector2 map_coord) {
        Vector2 coord;
        coord.x = map_coord.x / 0.75;
        coord.y = map_coord.y / 0.75;

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
        result.x = (index % map_width) / 0.75;
        result.y = (floor(index / map_width)) / 0.75;
        //std::cout << result.x << " " << result.y << std::endl;
    }

    void parse_coordinates_to_map(Vector2 &coords) {
        coords = coords * 0.75;
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