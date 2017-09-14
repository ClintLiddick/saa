#version 330 core

in float radius;
in vec4 center;

out vec4 frag_color;

void main() {
    if(distance(gl_FragCoord.xy, center.xy) > radius) {
        discard;
    }

    frag_color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}
