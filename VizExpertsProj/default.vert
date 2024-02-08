#version 330 core

//Positions/Coordinates
layout (location = 0) in vec3 aPos;
// Colors
layout (location = 1) in vec3 aColor;


// Outputs the color for the Fragment Shader
out vec3 color;
// Controls the scale of the vertices



void main()
{
	// Outputs the positions/coordinates of all vertices
	gl_Position = vec4(aPos.x , aPos.y , aPos.z , 1.0);
	// Assigns the colors from the Vertex Data to "color"
	color = aColor;
}