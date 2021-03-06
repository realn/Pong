#version 330

in vec4 vInVertex;
in vec4 vInColor;

out vec4 vVFColor;

uniform mat4 mTransform;

void main() {
	vVFColor = vInColor;
	gl_Position = mTransform * vec4(vInVertex.xy, 0, 1);
}