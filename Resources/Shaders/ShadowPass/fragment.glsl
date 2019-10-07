#version 330 core

in vec3 fsPosition;
in vec3 fsNormal;
in vec4 fsPositionLightSpace;

uniform sampler2D shadowMap;

uniform bool enableShadows;
uniform vec3 lightPosition;
uniform vec3 cameraPosition;
uniform vec4 fsColor;

out vec4 outColor;

float shadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDirection) {
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // calculate bias (based on depth map resolution and slope)
    float bias = max(0.05 * (1.0 - dot(normal, lightDirection)), 0.005);
    // check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for (int x = -1; x <= 1; ++x) {
        for (int y = -1; y <= 1; ++y) {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;
    
    // Keep the shadow at 0.0 when outside the light's frustum.
    if (projCoords.z > 1.0f) {
        shadow = 0.0f;
    }
        
    return shadow;
}

void main() {
    // Run through the Phong illumination model.
    vec3 color = fsColor.xyz;
    vec3 normal = normalize(fsNormal);
    vec3 lightColor = vec3(1.0f);
    
    // Ambient color.
    vec3 ambientColor = vec3(0.15f);// * color;
    
    // Diffuse color.
    vec3 lightDirection = normalize(lightPosition - fsPosition);
    float phongDiff = max(dot(lightDirection, normal), 0.0f);
    vec3 diffuseColor = phongDiff * lightColor;
    
    // Specular color.
    vec3 viewDirection = normalize(cameraPosition - fsPosition);
    vec3 reflectOverNormal = reflect(-lightDirection, normal);
    float phongSpec = dot(viewDirection, reflectOverNormal);
    
    vec3 specularColor = vec3(0.0f);
    if (phongSpec > 0.0f) {
        float shineCoefficient = 32.0f;
//        vec3 halfwayDirection = normalize(lightDirection + viewDirection);
        phongSpec = pow(phongSpec, shineCoefficient);
        specularColor = phongSpec * lightColor;
    }
    
    // TODO: Figure out wtf this means.
    // Calculate shadow.
    float shadow = 0.0f;
    if (enableShadows) {
        shadow = shadowCalculation(fsPositionLightSpace, normal, lightDirection);
    }
    vec3 lighting = (ambientColor + (1.0 - shadow) * (diffuseColor + specularColor)) * color;
    
    outColor = vec4(lighting, 1.0);
}
