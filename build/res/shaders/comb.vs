#version 330

// Input vertex attributes
in vec3 vertexPosition;
in vec2 vertexTexCoord;
in vec3 vertexNormal;
in vec4 vertexTangent;
in vec4 vertexColor;

// Input uniform values
uniform mat4 mvp;
uniform mat4 matModel;
uniform mat4 matNormal;

uniform float time;
uniform sampler2D perlinNoiseMap;

// Output vertex attributes (to fragment shader)
out vec3 fragPosition;
out vec2 fragTexCoord;
out vec3 fragNormal;
out vec4 fragColor;
out float height;
out mat3 TBN;

void main()
{
    // Calculate animated texture coordinates based on time and vertex position
    vec2 animatedTexCoord = sin(vertexTexCoord + vec2(sin(time + vertexPosition.x*0.1), cos(time + vertexPosition.z*0.1))*0.3);

    // Normalize animated texture coordinates to range [0, 1]
    animatedTexCoord = animatedTexCoord*0.5 + 0.5;

    // Fetch displacement from the perlin noise map
    float displacement = texture(perlinNoiseMap, animatedTexCoord).r*7.0; // Amplified displacement

    // Displace vertex position
    vec3 displacedPosition = vertexPosition + vec3(0.0, displacement, 0.0);

    // Compute binormal from vertex normal and tangent. W component is the tangent handedness
    vec3 vertexBinormal = cross(vertexNormal, vertexTangent.xyz)*vertexTangent.w;

    // Compute normal matrix for proper normal transformations
    mat3 normalMatrix = transpose(inverse(mat3(matModel)));

    // Send vertex attributes to fragment shader
    fragPosition = vec3(matModel*vec4(displacedPosition, 1.0));
    fragTexCoord = vertexTexCoord;
    fragColor = vertexColor;
    height = displacedPosition.y*0.2; // send height to fragment shader for coloring

    // Compute fragment normal based on normal transformations
    fragNormal = normalize(normalMatrix*vertexNormal);

    // Create TBN matrix for transforming the normal map values from tangent-space to world-space
    vec3 fragTangent = normalize(normalMatrix*vertexTangent.xyz);
    fragTangent = normalize(fragTangent - dot(fragTangent, fragNormal)*fragNormal);

    vec3 fragBinormal = normalize(normalMatrix*vertexBinormal);
    fragBinormal = cross(fragNormal, fragTangent);

    TBN = transpose(mat3(fragTangent, fragBinormal, fragNormal));

    // Calculate final vertex position
    gl_Position = mvp*vec4(displacedPosition, 1.0);
}

