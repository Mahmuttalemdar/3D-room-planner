uniform vec3 LightPos;
uniform vec3 LightColor;

uniform vec3 DiffColor;
uniform vec3 SpecColor;
uniform vec3 AmbientColor;
uniform float SpecExp;

uniform sampler2D DiffuseTexture;

varying vec3 Normal;
varying vec3 Position;
varying vec2 Texcoord;

varying vec3 dTime;

float sat(float a){
	return clamp(a, 0.0, 1.0);
}

void main(void)
{
	vec3 WatchDir = normalize(-Position);

	float CosAngle = 1.0 - abs( dot( Normal, WatchDir));
	float minRimLight = 0.4;
	vec3 pulsatingRimlight = vec3((dTime.x*0.5)+0.5, (dTime.x*0.5)+0.5, (dTime.x*0.5)+0.5); 




	vec4 newLightPos = gl_ModelViewMatrix * vec4(LightPos,0);

	vec3 N = normalize(Normal);
	vec3 L = normalize(newLightPos.xyz-Position);
	vec3 E = normalize(-Position);
	vec3 R = reflect(-L, N);

	vec3 DiffuseComponent = DiffColor * sat(dot(N,L));
	vec3 SpecularComponent = SpecColor * pow(sat(dot(E,R)), SpecExp);

	vec3 DiffuseTexColor  = texture2D(DiffuseTexture, Texcoord).rgb;
	DiffuseComponent *= DiffuseTexColor;

	vec3 AmbientComponent = AmbientColor * DiffuseTexColor;

	vec3 phongShading = DiffuseComponent + SpecularComponent + AmbientComponent;

	if(dTime.z == 1.0)
    {
		gl_FragColor = vec4( pulsatingRimlight * pow(CosAngle, 1.0)+ phongShading, 0) ;
	}
	else
    {
		gl_FragColor = vec4(phongShading, 0) ;
	}

	
	
}