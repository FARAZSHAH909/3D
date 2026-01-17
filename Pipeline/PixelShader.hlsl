cbuffer staticMeshBuffer : register(b0)
{
    float4x4 W;
    float4x4 VP;
    float time;
    float3 cameraPos;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float3 WorldPos : POSITION;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float3 Binormal : BINORMAL;
    float2 TexCoords : TEXCOORD;
};

float4 PS(PS_INPUT input) : SV_Target0
{
    // --- 1. Alpha Testing (Procedural) ---
    // Simulate a grate/mesh pattern or simply clip edges
    // Example: discard pixels based on sine wave pattern
    // float alphaPattern = sin(input.TexCoords.x * 50.0) * sin(input.TexCoords.y * 50.0);
    // if (alphaPattern < -0.5) discard; // Simple clip

    // --- 2. Normal Mapping (Procedural) ---
    // Construct TBN Matrix
    float3 N = normalize(input.Normal);
    float3 T = normalize(input.Tangent);
    float3 B = normalize(input.Binormal);
    float3x3 TBN = float3x3(T, B, N);

    // Procedural Normal Map (Ripples)
    float3 localNormal = float3(0.0, 0.0, 1.0); // Default flat normal
    // Perturb based on texture coords
    localNormal.x = sin(input.TexCoords.x * 20.0 + time) * 0.2;
    localNormal.y = cos(input.TexCoords.y * 20.0 + time) * 0.2;
    localNormal = normalize(localNormal);

    // Transform local normal to World Space
    float3 worldNormal = normalize(mul(localNormal, TBN));

    // Override N for lighting
    N = worldNormal;

    // --- 3. Lighting ---
    float3 V = normalize(cameraPos - input.WorldPos);
    
    // Hardcoded Directional Light
    float3 lightDir = normalize(float3(0.5, 1.0, -0.5));
    float3 lightColor = float3(1.0, 0.95, 0.9);
    float ambientStrength = 0.3f;
    
    // Diffuse
    float diff = max(dot(N, lightDir), 0.0);
    
    // Specular (Blinn-Phong)
    float3 H = normalize(lightDir + V);
    float spec = pow(max(dot(N, H), 0.0), 32.0);
    float3 specularColor = float3(1.0, 1.0, 1.0);
    
    // --- 4. Anti-Gravity Effects ---
    // Rim Lighting
    float rim = 1.0 - max(dot(N, V), 0.0);
    rim = smoothstep(0.4, 1.0, rim);
    float3 rimColor = float3(0.2, 0.6, 1.0); // Cyan rim

    // Pulse effect
    float pulse = (sin(time * 3.0) * 0.5 + 0.5);
    
    // Combine
    float3 ambient = ambientStrength * lightColor;
    float3 diffuse = diff * lightColor;
    float3 specular = spec * specularColor;
    float3 emissive = rim * rimColor * pulse;
    
    float3 objectColor = float3(0.8, 0.8, 0.8); // Base color
    
    float3 finalColor = (ambient + diffuse) * objectColor + specular + emissive;
    
    return float4(finalColor, 1.0);
}