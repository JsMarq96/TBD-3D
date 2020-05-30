varying vec3 v_world_position;
uniform vec4 u_color;
uniform float hash_lookup_table[257];

float get_noise(float x, float y) {
	float pseudo = hash_lookup_table[ int( mod( hash_lookup_table[ int(mod(x, 255.)) ] + y, 255. ) ) ];
	float gradient = pseudo;
	gradient += 1.;

	if (pseudo > 8.) {
		return gradient * -1.;
	} else {
		return gradient;
	}
}

void main()
{
	float value = get_noise(float(v_world_position.x), float(v_world_position.y));
    gl_FragColor = vec4(value, value, value, 0.5);
}