attribute vec3 a_position;
attribute vec2 a_uv;
attribute vec3 a_normal;

uniform mat4 u_WVP;
uniform mat4 u_W;

varying vec2 v_uv;
varying vec3 v_normalW;
varying vec3 v_posW;

void main()
{
	v_uv = a_uv;
	vec4 positionL = vec4(a_position, 1.0);
	gl_Position = u_WVP *  positionL;
	v_normalW = (u_W * vec4(a_normal, 0.0)).xyz;
	v_posW = (u_W * positionL).xyz;
}