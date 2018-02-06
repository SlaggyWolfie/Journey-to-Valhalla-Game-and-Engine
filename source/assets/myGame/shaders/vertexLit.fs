//LIT COLOR FRAGMENT SHADER
#version 330 // for glsl version (12 is for older versions , say opengl 2.1

uniform vec3 diffuseColor;
uniform vec3 ambientLightColor;
uniform vec3 directionalLightColor;

in float diffuseIntensity;
out vec4 sColor;

void main (void) {
    vec3 ambientTerm = ambientLightColor * diffuseColor;
    vec3 diffuseTerm = diffuseIntensity * directionalLightColor * diffuseColor;

    sColor = vec4 (ambientTerm + diffuseTerm , 1);
}
