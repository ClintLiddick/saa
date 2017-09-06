#version 330 core
layout (location = 0) in vec3 point;

uniform mat4 clip_from_local;

void main() {
  gl_Position = clip_from_local * vec4(point.x, point.y, point.z, 1.0);
}
