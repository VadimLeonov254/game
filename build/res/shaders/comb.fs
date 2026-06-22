#version 330

// Input fragment attributes (from vertex shader)
in vec3 fragPosition;
in vec2 fragTexCoord;
in vec3 fragNormal;
in vec4 fragColor;
in float height;
in mat3 TBN;

// Input uniform values
uniform sampler2D texture0;
uniform sampler2D normalMap;
uniform sampler2D perlinNoiseMap;
uniform vec4 colDiffuse;

uniform vec3 viewPos;
uniform vec4 tintColor;

uniform vec3 lightPos;
uniform bool useNormalMap;
uniform float specularExponent;

// Output fragment color
out vec4 finalColor;

void main()
{
    vec4 texelColor = texture(texture0, vec2(fragTexCoord.x, fragTexCoord.y));
    vec3 specular = vec3(0.0);
    vec3 viewDir = normalize(viewPos - fragPosition);
    vec3 lightDir = normalize(lightPos - fragPosition);

    vec3 normal;
    if (useNormalMap)
    {
        normal = texture(normalMap, vec2(fragTexCoord.x, fragTexCoord.y)).rgb;

        // Transform normal values to the range -1.0 ... 1.0
        normal = normalize(normal*2.0 - 1.0);

        // Transform the normal from tangent-space to world-space for lighting calculation
        normal = normalize(normal*TBN);
    }
    else
    {
        normal = normalize(fragNormal);
    }

    vec4 tint = colDiffuse*fragColor;

    vec3 lightColor = vec3(1.0, 1.0, 1.0);
    float NdotL = max(dot(normal, lightDir), 0.0);
    vec3 lightDot = lightColor*NdotL;

    float specCo = 0.0;

    if (NdotL > 0.0) specCo = pow(max(0.0, dot(viewDir, reflect(-lightDir, normal))), specularExponent);

    specular += specCo;

    // Combine base lighting with displacement-based height coloring
    vec4 darkblue = vec4(0.0, 0.13, 0.18, 1.0);
    vec4 lightblue = vec4(1.0, 1.0, 1.0, 1.0);
    vec4 heightColor = mix(darkblue, lightblue, height);

    // Blend height-based coloring with normal map lighting (50/50 blend)
    finalColor = mix(heightColor, texelColor*((tint + vec4(specular, 1.0))*vec4(lightDot, 1.0)), 0.5);
    finalColor += texelColor*(vec4(1.0, 1.0, 1.0, 1.0)/40.0)*tint;

    // Gamma correction
    finalColor = pow(finalColor, vec4(1.0/2.2));
}
