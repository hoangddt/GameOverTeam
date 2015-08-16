precision mediump float;

uniform vec3 u_diffuse;
uniform vec3 u_specular;
uniform vec3 u_ambient;
uniform float u_time;

uniform sampler2D u_texture1;
uniform sampler2D u_texture2;
uniform float u_transparent;
uniform vec3 u_cameraPos;

varying vec3 v_posW;
varying vec3 v_normalW;
varying vec2 v_uv;

void main()
{
	//vec3 toEye = u_cameraPos - v_posW;
	//vec3 reflectDir = reflect(normalize(-toEye), normalize(v_normalW));
	float diffVal = -0.2;
	vec4 diffuse = vec4(diffVal, diffVal, diffVal, 1.0);
	vec2 disp = texture2D(u_texture2, vec2(v_uv.x, v_uv.y + u_time * 0.1)).rg;
	float dMax = 0.6;
	vec2 offset = (2.0 * disp - 1.0) * dMax;
	vec2 uv_displaced = v_uv + offset;
	vec4 texture1 = texture2D(u_texture1, uv_displaced);
	gl_FragColor = texture1 + diffuse ; //vec4(reflectDir, 1.0);
}