#type vertex

#version 330 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_tex_coords;

out vec2 v_tex_coords;

uniform mat4 u_view_projection;
uniform mat4 u_model_transform;

void main()
{
	v_tex_coords = a_tex_coords;
	gl_Position = u_view_projection * u_model_transform * vec4(a_position, 1.0f);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;
in vec2 v_tex_coords;

uniform sampler2D u_texture;

void main()
{
	color = texture(u_texture, v_tex_coords);
}
