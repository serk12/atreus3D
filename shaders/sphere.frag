#version 330 core
out vec4 FragColor;

in vec2 UV;

uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
    float l = distance(UV, vec2(0.5, 0.5));
    if (l > 0.5) discard;
    vec3 N;
    N.xy = gl_PointCoord* 2.0 - vec2(1.0);
    float mag = dot(N.xy, N.xy);
    if (mag > 1.0) discard;
    N.z = sqrt(1.0-mag);
    float diffuse = max(0.2, dot(lightColor, N));
    FragColor = vec4(objectColor, 1.0) * diffuse;
}
