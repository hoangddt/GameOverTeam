precision highp float;

varying vec2 fs_uv;
uniform sampler2D u_texture1;

void main()
{
	gl_FragColor = texture2D(u_texture1, fs_uv);
}