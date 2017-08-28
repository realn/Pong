#version 330

uniform sampler2D texFont;

in vec2 vVFTCoord;
in vec4 vVFColor;

out vec4 vOutColor;

void main() {
	vOutColor = vec4(1.0f, 0.0f, 0.0f, 1.0f); //texture(texFont, vVFTCoord) * vVFColor + 
}