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
	
	//float rad = u_shift * (x*x + y*y);
	float rad = u_shift;

	float X = x*cos(rad) - y*sin(rad);
	float Y = x*sin(rad) + y*cos(rad);
	//color = vec4( (x + u_shift)/2 , (-x + u_shift)/2 , (x/2 + u_shift)/2 , 1.0f);
	color = vec4( X + 0.5, Y  + 0.5, X*Y + 0.5, 1.0f);

};
