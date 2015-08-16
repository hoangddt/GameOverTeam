precision mediump float;
uniform samplerCube u_texture1;
varying vec3 v_pos;

void main()
{
	gl_FragColor = textureCube(u_texture1, v_pos.xyz);
}