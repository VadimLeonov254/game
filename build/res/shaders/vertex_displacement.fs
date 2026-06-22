#version 330

// Input fragment attributes (from fragment shader)
in vec2 fragTexCoord;
in float height;

// Output fragment color
out vec4 finalColor;

void main()
{
    vec4 brown = vec4(0.588, 0.294, 0.0, 1.0);
    vec4 green = vec4(0.0, 0.0, 0.0, 1.0);
    // interplate between two colors based on height
    finalColor = mix(brown, green, height);
}
