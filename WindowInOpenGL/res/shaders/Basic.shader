#shader vertex
#version 330 core
layout (location = 0) in vec4 position;
out vec4 vertex_color;
void main()
{
	gl_Position = position;
	float x = position.x;
	float y = position.y;
	vertex_color = vec4( 1 - y*x , 1 + x * 1.44, 1 - x * 1.44 , 1.0);
};


#shader fragment
#version 330 core
out vec4 color;
in vec4 vertex_color;
uniform vec4 u_color;


void main()
{
	color = vertex_color;
};
