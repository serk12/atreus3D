#version 330 core

layout(points) in;
layout(triangle_strip, max_vertices = 8) out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float rSphere;

out vec2 UV;
out vec4 fColor;

//billboard
void billboard(vec4 position)
{
    mat4 MVP = projection * view * model;


    vec3 cameraRi = vec3(view[0][0], view[1][0], view[2][0]);
    vec3 cameraUp = vec3(view[0][1], view[1][1], view[2][1]);

    float r = rSphere;
    vec4 p1 = MVP * vec4(position.xyz + cameraRi*r + cameraUp*r, position.w);
    vec4 p2 = MVP * vec4(position.xyz - cameraRi*r + cameraUp*r, position.w);
    vec4 p3 = MVP * vec4(position.xyz + cameraRi*r - cameraUp*r, position.w);
    vec4 p4 = MVP * vec4(position.xyz - cameraRi*r - cameraUp*r, position.w);

    fColor = vec4(1.0, 0.0, 0.0, 0.0);

    r = 1;
    gl_Position = p1;
    UV = vec2(0.0, 0.0);
    EmitVertex();

    gl_Position = p2;
    UV = vec2(r, 0.0);
    EmitVertex();

    gl_Position = p3;
    UV = vec2(0.0, r);
    EmitVertex();

    gl_Position = p4;
    UV = vec2(r, r);
    EmitVertex();

    gl_Position = p3;
    UV = vec2(0.0, r);
    EmitVertex();

    gl_Position = p2;
    UV = vec2(r, 0.0);
    EmitVertex();

    gl_Position = p1;
    UV = vec2(0.0, 0.0);
    EmitVertex();
    EndPrimitive();
}

void main() {
    billboard(gl_in[0].gl_Position);
}
