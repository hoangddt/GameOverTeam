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
	vec3 v_pos = normalize(v_posW);
	
	vec4 ambient = vec4(u_ambient, 1.0);
	vec3 u_lightPos = vec3(5.0, 10.0, 0.0);
	vec3 lightDir = normalize(v_pos - u_lightPos);
	
	vec4 lightDiffColor = vec4(u_diffuse, 1.0);
	vec4 diffuse = max(dot(-lightDir, v_normal.xyz), 0.5) * lightDiffColor;
	
	vec3 reflectVector = normalize(reflect(lightDir, v_normal.xyz));
	vec4 lightSpecColor = vec4(u_specular, 1.0);
	
	vec3 toEye = normalize(u_cameraPos - v_posW);
	vec4 specular = pow(max(dot(reflectVector, toEye), 0.0), 40.0) * lightSpecColor;
	
	vec4 lightingColor = vec4((mix(ambient, diffuse, 0.3) * texture1 + specular).xyz, texture1.w);
	
	vec4 finalColor = u_transparent * mix(texture1, diffuse, 0.5) + (1.0 - u_transparent) * lightDiffColor;
	
	gl_FragColor = finalColor;
}