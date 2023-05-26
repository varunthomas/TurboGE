#text vertex
#version 450 core
			
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in int a_entityID;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_ViewProjection;
};

struct VertexOutput
{
	vec4 Color;
};

layout (location = 0) out VertexOutput Output;
layout (location = 1) out flat int v_entityID;

void main()
{
	Output.Color = a_Color;
	v_entityID = a_entityID;
	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);	
}

#text fragment
#version 450 core
			
layout(location = 0) out vec4 color;
layout(location = 1) out int entityId;

struct VertexOutput
{
	vec4 Color;
};

layout (location = 0) in VertexOutput Input;
layout (location = 1) in flat int v_entityID;


void main()
{
	color = Input.Color;
	entityId = v_entityID;
}