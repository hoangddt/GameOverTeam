precision mediump float;

uniform vec3 u_diffuse;
uniform vec3 u_specular;
uniform vec3 u_ambient;

uniform sampler2D u_texture1;
uniform float u_transparent;
uniform vec3 u_cameraPos;

varying vec3 v_posW;
varying vec3 v_normalW;
varying vec2 v_uv;

void main()
{
	vec4 texture1 = texture2D(u_texture1, v_uv);
	vec3 v_normal = normalize(v_normalW);
	vec3 v_pos = v_posW;
	
	vec4 ambient = vec4(u_ambient, 1.0);
	vec3 u_lightPos = vec3(0.0, 10.0, -10.0);
	vec3 lightDir = normalize(v_pos - u_lightPos);
	
	vec4 lightDiffColor = vec4(u_diffuse, 1.0);
	vec4 diffuse = max(dot(-lightDir, v_normal.xyz), 0.0) * lightDiffColor;
	
	vec3 reflectVector = normalize(reflect(lightDir, v_normal.xyz));
	vec4 lightSpecColor = vec4(u_specular, 1.0);
	
	vec3 toEye = normalize(u_cameraPos - v_posW);
	vec4 specular = pow(max(dot(reflectVector, toEye), 0.0), 20.0) * lightSpecColor;
	
	//gl_FragColor = vec4(v_normal, 1.0);
	gl_FragColor = u_transparent * mix(diffuse, texture1, 0.7) + (1.0 - u_transparent) * diffuse + ambient;
	//gl_FragColor = texture1 + diffuse + ambient + specular;
}