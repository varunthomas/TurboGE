#text vertex
#version 450 core
			
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TextCoord;
layout(location = 3) in float a_TextIndex;
layout(location = 4) in float a_TilingFactor;
layout(location = 5) in int a_entityID;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_ViewProjection;
};

struct VertexOutput
{
	vec4 Color;
	vec2 TextCoord;
	float TextIndex;
	float TilingFactor;
};

layout (location = 0) out VertexOutput Output;
layout (location = 4) out flat int v_entityID;

void main()
{
	Output.Color = a_Color;
	Output.TextCoord = a_TextCoord;
	Output.TextIndex = a_TextIndex;
	Output.TilingFactor = a_TilingFactor;
	v_entityID = a_entityID;
	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);	
}

#text fragment
#version 450 core
			
layout(location = 0) out vec4 color;
layout(location = 1) out int color2;

struct VertexOutput
{
	vec4 Color;
	vec2 TextCoord;
	float TextIndex;
	float TilingFactor;
};

layout (location = 0) in VertexOutput Input;
layout (location = 4) in flat int v_entityID;


layout (binding = 0) uniform sampler2D u_Textures[32];

void main()
{
	color = texture(u_Textures[int(Input.TextIndex)], Input.TextCoord * Input.TilingFactor) * Input.Color;
	//color = v_Color;
	color2 = v_entityID;
}