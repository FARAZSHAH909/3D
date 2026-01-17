// Anti-Gravity Vertex Shader

cbuffer staticMeshBuffer : register(b0)
{
    float4x4 W;       // World Matrix
    float4x4 VP;      // View * Projection Matrix
    float time;       // Time in seconds
    float3 cameraPos; // Camera Position
};

struct VS_INPUT
{
    float4 Pos : POSITION;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float2 TexCoords : TEXCOORD;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float3 WorldPos : POSITION;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float3 Binormal : BINORMAL; // Added for TBN
    float2 TexCoords : TEXCOORD;
};

PS_INPUT VS(VS_INPUT input)
{
    PS_INPUT output;
    
    // 1. Calculate Floating Effect (Anti-Gravity)
    float4 animatedPos = input.Pos;
    
    // Amplitude and Frequency
    float amplitude = 0.5f; 
    float frequency = 1.5f;
    
    // Offset based on position for wave effect
    float offset = animatedPos.x * 0.5f + animatedPos.z * 0.3f;
    
    // Apply sine wave to Y
    animatedPos.y += sin(time * frequency + offset) * amplitude;
    
    // 2. World Transform
    float4 worldPos = mul(animatedPos, W);
    output.WorldPos = worldPos.xyz;
    
    // 3. Clip Space Transform
    output.Pos = mul(worldPos, VP);
    
    // 4. Normal/Tangent Transform (for Lighting)
    // Rotate by World Matrix (assuming uniform scale, otherwise use InverseTranspose)
    output.Normal = normalize(mul(input.Normal, (float3x3)W)); // Cast to 3x3
    output.Tangent = normalize(mul(input.Tangent, (float3x3)W));
    
    // Calculate Binormal (Bitangent)
    output.Binormal = cross(output.Normal, output.Tangent);

    output.TexCoords = input.TexCoords;
    
    return output;
}
