attribute vec3 a_position;
attribute vec2 a_uv;

varying vec2 fs_uv;

uniform mat4 u_WVP;

void main()
{
	gl_Position = u_WVP * vec4(a_position, 1.0);
	fs_uv = a_uv;
}