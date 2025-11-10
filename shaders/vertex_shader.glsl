#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec4 Acolor;

out vec4 Color;

uniform mat4 u_Model;      // Object Matrix (Translation, Rotation, Shape Scaling) 
uniform mat4 u_View;       // Camera Matrix (Pan, Zoom)
uniform mat4 u_Projection; // Projection Matrix (2D World)

void main(){

	gl_Position = u_Projection * u_View * u_Model * vec4(aPos, 0.0, 1.0);
	Color = Acolor; 
}
