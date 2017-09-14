#version 330 core
layout(location = 0) in vec3 vert;
layout(location = 1) in float radius_;
layout(location = 2) in vec2 center_;

uniform mat4 clip_from_local;

out float radius;
out vec4 center;

void main() {
    gl_Position = clip_from_local * vec4(vert, 1.0);
    radius = length(clip_from_local * vec4(radius_, 0, 0, 1));
    center = clip_from_local * vec4(center_, 0.0, 1.0);
}
