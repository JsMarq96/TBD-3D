#include "game_map.h"

sGameMap::sGameMap(int m_width, int m_height) { 
    height = m_height;
    width = m_width;
    map = new uint8[width * height];

    for (int i = 0; i < (width * height); i++) {
        map[i] = 1;
    }
};

uint8 sGameMap::get(float x, float y) { 
    if (x >= width || y >= height)
        return 1;
    return map[int((x * width) + y)];
};

void sGameMap::set(float x, float y, uint8 value) {
    if (x >= width || y >= height)
        return;
    map[int((x * width) + y)] = value;
};

void sGameMap::add_area(float x, float y, float size) {
	int d_size = ceil((size) );
	int c_x = ceil(x - ceil(d_size/2)), c_y = ceil(y - ceil(d_size/2));

	for (int i = 0; i < d_size; i++) {
		for (int j = 0; j < d_size; j++) {
			set(c_x + i, c_y + j, 0);

		}
	}
};

// Randomly selects a random empty coordinate of the map
Vector2 sGameMap::get_empty_coordinate() {
    uint8 tmp = 0;
    Vector2 coords;
    while (tmp != 1) {
        //coords.random(map_width);
        coords.x = (random(width) );
        coords.y = (random(height) );

        coords.x *= (coords.x < 0) ? -1 : 1;
        coords.y *= (coords.y < 0) ? -1 : 1;

        tmp = get(coords.x, coords.y);
    }

    return coords;
}

void sGameMap::get_path_to(Vector2 start, Vector2 goal, int* steps, int max_steps, int &result) {
    //std::cout << goal.x << " - " << goal.y  << " : "  << start.x << " - " << start.y << std::endl;
    result = AStarFindPathNoTieDiag(start.x, start.y, goal.x, goal.y, map, width, height, steps, max_steps);

    /*std::cout << result << " <- " << map_width * map_height << std::endl;
    for (int i = 0; i < max_steps; i++) {
        std::cout << steps[i] << std::endl;
    }
    std::cout << "=========" << std::endl;*/
}

// Casts a Ray from one point in the map to other,
// usign the DDA line algortihm
float sGameMap::raycast_from_point_to_point(Vector2 p2, Vector2 p1, float max_size) {
    float dx = p2.x - p1.x, dy = p2.y - p1.y;
	float len = max( abs(dx), abs(dy));
	// Calculate slopes
	float vx = dx / len;
	float vy = dy / len;
    len -= 1;
    float xi, yi;
    xi = p1.x+SIGN(p1.x)*0.5;
    yi = p1.y+SIGN(p1.y)*0.5;

	for (float i = 0; len >= i; i++) {
        // Is occuded
        if (map[int((xi * width) + yi)] != 1) {
            return -1;
        }

        // It is to far
        if ((Vector2(xi,yi) - p1).length() > max_size) {
            return -1;
        }

		xi += vx;
		yi += vy;
	}

    return (Vector2(xi,yi) - p1).length();
    /*

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

    return (Vector2(x,y) - p1).length();*/
}