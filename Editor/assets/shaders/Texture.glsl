#text vertex
#version 330 core
			
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TextCoord;
layout(location = 3) in float a_TextIndex;
layout(location = 4) in float a_TilingFactor;
layout(location = 5) in int a_entityID;

uniform mat4 u_ViewProjection;

out vec4 v_Color;
out vec2 v_TextCoord;
out float v_TextIndex;
out float v_TilingFactor;
flat out int v_entityID;

void main()
{
	v_Color = a_Color;
	v_TextCoord = a_TextCoord;
	v_TextIndex = a_TextIndex;
	v_TilingFactor = a_TilingFactor;
	v_entityID = a_entityID;
	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);	
}

#text fragment
#version 330 core
			
layout(location = 0) out vec4 color;
layout(location = 1) out int color2;

in vec4 v_Color;
in vec2 v_TextCoord;
in float v_TextIndex;
in float v_TilingFactor;
flat in int v_entityID;

uniform vec4 u_Color;
uniform sampler2D u_Texture[32];

void main()
{
	color = texture(u_Texture[int(v_TextIndex)], v_TextCoord * v_TilingFactor) * v_Color;
	//color = v_Color;
	color2 = v_entityID;
}