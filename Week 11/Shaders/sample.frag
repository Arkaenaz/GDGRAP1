#version 330 core
/* * * * * * * * * * * * * * * * * * * *
 *  		   POINT LIGHT			   *
 * * * * * * * * * * * * * * * * * * * */

uniform sampler2D tex0;
uniform sampler2D norm_tex;

uniform vec3 lightPos;
uniform vec3 lightColor;

uniform float ambientStr;
uniform vec3 ambientColor;

uniform vec3 cameraPos;
uniform float specStr;
uniform float specPhong;

in vec2 texCoord;

in vec3 normCoord;
in vec3 fragPos;

in mat3 TBN;

out vec4 FragColor;



void main() {
	vec4 pixelColor = texture(tex0, texCoord);
	if (pixelColor.a < 0.5) {
		discard;
	}
	//vec3 normal = normalize(normCoord);
	vec3 normal = texture(norm_tex, texCoord).rgb;
	normal = normalize(normal * 2.0 - 1.0);

	normal = normalize(TBN * normal);

	vec3 lightDir = normalize(lightPos - fragPos);

	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	vec3 ambientCol = ambientColor * ambientStr;

	vec3 viewDir = normalize(cameraPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, normal);

	float spec = pow(max(dot(reflectDir, viewDir), 0.1), specPhong);
	vec3 specColor = spec* specStr * lightColor;

	// This is the already squared distance, using distance formula we use the position of the objects and the position of the light.
	float distance = pow(lightPos.x - fragPos.x, 2) + pow(lightPos.y - fragPos.y, 2) + pow(lightPos.z - fragPos.z, 2);

	// Intensity of light depending on the distance from the light position
	float intensity = 100.0f / distance;

	// Multiplies all the light's values with the intensity to correspond on how strong it is.
	specColor  *= intensity;
    diffuse  *= intensity;
    ambientCol *= intensity;

	FragColor = vec4(specColor + diffuse + ambientCol, 1.0) * texture(tex0, texCoord);
}