attribute vec3 a_vertex;
attribute vec2 a_uv;

varying vec2 v_uv;

void main() {
    v_uv = a_uv;
    gl_Position = vec4(a_vertex.xy, 0.0, 1.0);
}