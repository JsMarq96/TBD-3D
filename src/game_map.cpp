#include "game_map.h"

sGameMap::sGameMap(int width, int height, float scale) { 
    real_heigth = height;
    real_width = width;
    map_height = height * (0.75);
    map_width = width * (0.75);
    map = new uint8[map_width * map_height];

    std::cout << map_width << std::endl;

    for (int i = 0; i <= (map_width * map_height); i++) {
        map[i] = 1;
    }
};

uint8 sGameMap::get(float x, float y) { 
    x *= 0.75;
    y *= 0.75;
    if (x >= map_width-1 || y >= map_height-1)
        return 1;
    return map[int((x * map_width) + y)];
};

void sGameMap::set(float x, float y, uint8 value) {
    x *= 0.75;
    y *= 0.75;

    if (x >= map_width-1 || y >= map_height-1)
        return;
    map[int((x * map_width) + y)] = value;
};

void sGameMap::add_area(float x, float y, float radius) {
    /*float x = 0, y = radius, v = 1- radius;

	set(x1, y1, 0);

	for (; y > x; x++) {
		if (v < 0) {
			v += 2 * x + 3;
		} else {
			v += 2 * (x - y) + 5;
			y--; 
		}
			
		// Draw pixel with the reflections
		for (int i = x * (-1); i < x; i++) {
			set(i + x1, y + y1, 0);
			set(i + x1, (-1 * y) + y1, 0);
		}
		for (int i = (-1 * y); i < y; i++) {
			set(i + y1, x + x1, 0);
			set(i + y1, (-1 * x) + x1, 0);
		}
	}*/
    
    for (float dx = -1 * radius; dx <= radius; dx ++) {
        for (float dy = -1 * radius; dy <= radius; dy ++) {
            //set(x + dx, y + dy, 0);
            //std::cout << std::to_string(x / 0.75) << " " << std::to_string(y) << " - " << std::to_string(int(((x + dx) * map_width) + y + dy)) << std::endl;
            //map[int((x + dx) * map_width + (y + dy))] = 0;
            set(dx + x, dy + y, 0);
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
    }

    return coords;
}

void sGameMap::get_path_to(Vector2 start, Vector2 goal, int* steps, int max_steps, int &result) {
    start = start * 0.75;
    goal = goal * 0.75;
    //std::cout << goal.x << " - " << goal.y  << " : "  << start.x << " - " << start.y << std::endl;
    result = AStarFindPathNoTieDiag(start.x, start.y, goal.x, goal.y, map, map_width, map_height, steps, max_steps);

    /*std::cout << result << " <- " << map_width * map_height << std::endl;
    for (int i = 0; i < max_steps; i++) {
        std::cout << steps[i] << std::endl;
    }
    std::cout << "=========" << std::endl;*/
}

// Casts a Ray from one point in the map to other,
// usign the DDA line algortihm
float sGameMap::raycast_from_point_to_point(Vector2 p2, Vector2 p1, float max_size) {
    p1 = p1 * 0.75;
    p2 = p2 * 0.75;

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
        if (map[int((xi * map_width) + yi)] != 1) {
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