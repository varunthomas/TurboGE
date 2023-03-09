#text vertex
#version 330 core
			
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TextCoord;
uniform mat4 u_ViewProjection;

out vec4 v_Color;
out vec2 v_TextCoord;

void main()
{
	v_Color = a_Color;
	v_TextCoord = a_TextCoord;
	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);	
}

#text fragment
#version 330 core
			
layout(location = 0) out vec4 color;

in vec4 v_Color;
in vec2 v_TextCoord;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
	//color = texture(u_Texture, v_TextCoord*10.0f) * u_Color;
	color = v_Color;
}