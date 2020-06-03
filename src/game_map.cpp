#include "game_map.h"

sGameMap::sGameMap(int width, int height) { 
    real_heigth = height;
    real_width = width;
    map_height = height / 2;
    map_width = width / 2;
    map = new uint8[map_width * map_height];
};

uint8 sGameMap::get(int x, int y) { 
    return map[(x * map_width) + y];
};


// Randomly selects a random empty coordinate of the map
Vector2 sGameMap::get_empty_coordinate() {
    uint8 tmp = 1;
    Vector2 coords;
    while (tmp != 0) {
        coords.random(map_width);
        tmp = get(coords.x, coords.y);
    }

    return coords;
}

void sGameMap::get_path_to(Vector2 start, Vector2 goal, int* steps, int max_steps, int &result) {
    result = AStarFindPathNoTieDiag(start.x, start.y, goal.x, goal.y, map, map_width, map_height, steps, max_steps);
}

// Casts a Ray from one point in the map to other,
// usign the Bressham line algortihm
float sGameMap::raycast_from_point_to_point(Vector2 p1, Vector2 p2, float max_size) {
    int delta_x = p2.x - p1.x;
    int delta_y = p2.y - p1.y;
    int delta_err = 2*(delta_y / delta_x);
    int error = 0;
    int x, y = p1.y;

    for (x = 0; x < p2.x; x++) {
        // Is occuded
        if (get(x,y) != 0) {
            return -1;
        }

        // It is to far
        if ((Vector2(x,y) - p1).length() > max_size) {
            return -1;
        }

        error += delta_err;

        if (delta_err > 0) {
            y += 1;
            delta_err -= 2 * delta_x;
        }

        delta_err += 2 * delta_y;
    }

    return (Vector2(x,y) - p1).length();
}