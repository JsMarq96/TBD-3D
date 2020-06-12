attribute vec3 a_vertex;
attribute vec3 a_normal;
attribute vec2 a_uv;

uniform vec3 u_camera_pos;
uniform mat4 u_model;
uniform mat4 u_viewprojection;
uniform sampler2D u_heigh_map;


//this will store the color for the pixel shader
varying vec3 v_position;
varying vec3 v_world_position;
varying vec3 v_normal;
varying vec2 v_uv;

void main()
{	
	// Get Normal pixels
	vec3 heigh = (length(texture2D( norm_texture, a_uv ).xyz) * 2.) - 1.;

    //calcule the normal in camera space (the NormalMatrix is like ViewMatrix but without traslation)
	v_normal = (u_model * vec4( a_normal, 0.0) ).xyz;
	
	//calcule the vertex in object space
	v_position = a_vertex;
	v_world_position = (u_model * vec4( v_position, 1.0) ).xyz;

    // add displacement
    v_world_position = v_world_position + vec3(0., heigh, 0.);
	
	//store the texture coordinates
	v_uv = a_uv;

	//calcule the position of the vertex using the matrices
	gl_Position = u_viewprojection * vec4( v_world_position, 1.0 );
}