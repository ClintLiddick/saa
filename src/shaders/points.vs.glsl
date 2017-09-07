#version 330 core
layout (location = 0) in vec3 point;

uniform mat4 clip_from_local;
uniform float point_size;

void main() {
  gl_Position = clip_from_local * vec4(point, 1.0);
  gl_PointSize = point_size;
}
