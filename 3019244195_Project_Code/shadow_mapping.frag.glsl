#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform bool shadows;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // Transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // Get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // Calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    // Keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;
}

void phone(
	vec3 worldPos, vec3 cameraPos, vec3 normal, vec3 lightPos,
	inout vec4 I_a, inout vec4 I_d, inout vec4 I_s
	) {
	// 计算四个归一化的向量 N,V,L,R(或半角向量H)
    vec3 N = normalize(normal);
    vec3 V = normalize(cameraPos - worldPos);
    vec3 L = normalize(lightPos - worldPos);
    vec3 R = reflect(-L, N);

    // 环境光分量I_a
    I_a = vec4(0.3);

    // 计算漫反射系数alpha和漫反射分量I_d
    float alpha = max(dot(L, N), 0);
    I_d = alpha * vec4(0.85);

    // 计算高光系数beta和镜面反射分量I_s
    float beta = pow(max(dot(R, V), 0), 50.0);
    I_s = beta * vec4(1.0);

    // 注意如果光源在背面则去除高光
    if( dot(L, N) < 0.0 ) {
	    I_s = vec4(0.0, 0.0, 0.0, 1.0);
    }
}

void main()
{   
    // phone光照
	vec4 I_a, I_d, I_s;
	phone(fs_in.FragPos, viewPos, fs_in.Normal, lightPos, I_a, I_d, I_s);
    FragColor.rgb = texture2D(diffuseTexture, fs_in.TexCoords).rgb;
    
    // Calculate shadow
    float shadow = shadows ? ShadowCalculation(fs_in.FragPosLightSpace) : 0.0;       
    // 降低一点阴影强度，允许阴影区域有一点点的漫反射和高光
    shadow = min(shadow, 0.8); 
    FragColor.rgb *= (I_a.rgb + (1.0 - shadow) * (I_d.rgb + I_s.rgb));    
}