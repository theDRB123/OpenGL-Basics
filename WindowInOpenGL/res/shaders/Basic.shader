#shader vertex
#version 330 core
layout (location = 0) in vec4 position;
out vec2 vertex_pos;
void main()
{
	gl_Position = position;
	float x = position.x;
	float y = position.y;
	vertex_pos = position.xy;
};


#shader fragment
#version 330 core
out vec4 color;
in vec2 vertex_pos;
uniform float u_shift;


void main()
{
	float x = vertex_pos.x;
	float y = vertex_pos.y;
	// color  = vec4( 1 - y*x , 1 + x * 1.44, 1 - x * 1.44 , 1.0);	
//	color = vec4((1 + u_shift)*x , (1 + u_shift/2)*y , (1 + u_shift/3)*x*y , 0.0f);
	color = vec4( (1 + u_shift * x ) , (1 - u_shift * y) , (1 + u_shift * x * y) , 1.0f);
};
