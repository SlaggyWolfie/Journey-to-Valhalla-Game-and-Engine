//WOBBLE TEXTURE FRAGMENT SHADER
#version 330 // for glsl version (12 is for older versions , say opengl 2.1

uniform sampler2D diffuseTexture;
in vec2 texCoord;
in float vtime;
out vec4 fragment_color;

void main( void ) {
	
	// //texture animation
	// //rotation
	// float angle = mod(vtime, 6.28f);
	// vec2 coord = texCoord;
	// float sinR = sin(angle);
	// float cosR = cos(angle);
	// coord = (coord - 0.5) * mat2(cosR, sinR, -sinR, cosR);
	// coord += 0.5;
	
	// // texCoord = coord;

	// fragment_color = texture(diffuseTexture,coord);
	// fragment_color = texture(diffuseTexture,texCoord);

	float brightness = clamp(abs(sin(mod(vtime, 6.28f))), 0.0f, 1.0f);
	
	vec3 color = texture2D(diffuseTexture, texCoord).rgb;
	vec3 brightColor = (color) * brightness;
	fragment_color.rgb = brightColor;
	fragment_color.a = 1;

	//fragment_color = texture(diffuseTexture,texCoord);
}
