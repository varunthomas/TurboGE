#text vertex
#version 330 core
			
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TextCoord;
layout(location = 3) in float a_TextIndex;
layout(location = 4) in float a_TilingFactor;

uniform mat4 u_ViewProjection;

out vec4 v_Color;
out vec2 v_TextCoord;
out float v_TextIndex;
out float v_TilingFactor;

void main()
{
	v_Color = a_Color;
	v_TextCoord = a_TextCoord;
	v_TextIndex = a_TextIndex;
	v_TilingFactor = a_TilingFactor;
	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);	
}

#text fragment
#version 330 core
			
layout(location = 0) out vec4 color;

in vec4 v_Color;
in vec2 v_TextCoord;
in float v_TextIndex;
in float v_TilingFactor;

uniform vec4 u_Color;
uniform sampler2D u_Texture[32];

void main()
{
	color = texture(u_Texture[int(v_TextIndex)], v_TextCoord * v_TilingFactor) * v_Color;
	//color = v_Color;
}