#include "game_map.h"

sGameMap::sGameMap(int width, int height, float scale) { 
    real_heigth = height;
    real_width = width;
    map_height = height * (scale);
    map_width = width * (scale);
    map = new uint8[map_width * map_height];
    map_scale = scale;

    std::cout << map_width << std::endl;

    for (int i = 0; i <= (map_width * map_height); i++) {
        map[i] = 1;
    }
};

uint8 sGameMap::get(int x, int y) { 
    x *= 0.5;
    y *= 0.5;
    if (x >= map_width-1 || y >= map_height-1)
        return 1;
    return map[(x * map_width) + y];
};

void sGameMap::set(int x, int y, uint8 value) {
    x *= 0.5;
    y *= 0.5;
    //std::cout << x << "  " << y << std::endl << "---------" << std::endl;

    if (x >= map_width-1 || y >= map_height-1)
        return;
    map[(x * map_width) + y] = value;
};

void sGameMap::add_area(int x, int y, float radius) {
    x *= 0.5;
    y *= 0.5;
    radius *= 0.5;

    x -= radius;
    y -= radius;
    for (int dx = 0; dx < radius*2; dx++) {
        for (int dy = 0; dy < radius*2; dy++) {
            set(x + dx, y + dy, 0);
        }
    }
}

// Randomly selects a random empty coordinate of the map
Vector2 sGameMap::get_empty_coordinate() {
    uint8 tmp = 0;
    Vector2 coords;
    while (tmp != 1) {
        //coords.random(map_width);
        coords.x = (random(map_width) );
        coords.y = (random(map_height) );

        coords.x *= (coords.x < 0) ? -1 : 1;
        coords.y *= (coords.y < 0) ? -1 : 1;

        tmp = get(coords.x, coords.y);
        std::cout << coords.x << " = " << coords.y  << " = " << map_width << std::endl;
    }

    return coords;
}

void sGameMap::get_path_to(Vector2 start, Vector2 goal, int* steps, int max_steps, int &result) {
    start = start * 0.5;
    goal = goal * 0.5;
    result = AStarFindPathNoTieDiag(start.x, start.y, goal.x, goal.y, map, map_width, map_height, steps, max_steps);
}

// Casts a Ray from one point in the map to other,
// usign the Bressham line algortihm
float sGameMap::raycast_from_point_to_point(Vector2 p1, Vector2 p2, float max_size) {
    p1 = p1 * 0.5;
    p2 = p2 * 0.5;

    int delta_x = p2.x - p1.x;
    int delta_y = p2.y - p1.y;
    int delta_err = 2*(delta_y / delta_x);
    int error = 0;
    int x, y = p1.y;

    for (x = 0; x < p2.x; x++) {
        // Is occuded
        if (get(x,y) != 1) {
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