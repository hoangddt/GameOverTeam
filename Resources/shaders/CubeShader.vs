attribute vec3 a_position;
uniform mat4 u_WVP;
varying vec3 v_pos;
void main()
{
	gl_Position = u_WVP * vec4(a_position, 1.0);
	gl_Position.z = 1.0 - 0.0001 ;
	v_pos = a_position;
}