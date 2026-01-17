cbuffer SkyboxConstantBuffer : register(b0)
{
    float4x4 VP; // ViewProjection Matrix only
    float3 cameraPos;
    float padding; // Padding for 16-byte alignment
};

struct VS_INPUT
{
    float3 Pos : POSITION;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float3 LocalPos : POSITION;
};

PS_INPUT VS(VS_INPUT input)
{
    PS_INPUT output;
    
    // Adjust position to be centered on camera
    float3 pos = input.Pos + cameraPos;
    
    // Use VP to transform
    output.Pos = mul(float4(pos, 1.0), VP);
    
    // Ensure skybox is always at far plane (z=1.0)
    output.Pos.z = output.Pos.w; 
    
    output.LocalPos = input.Pos;
    
    return output;
}

float4 PS(PS_INPUT input) : SV_Target0
{
    // Simple Gradient for Space Skybox
    float3 dir = normalize(input.LocalPos);
    
    float3 topColor = float3(0.0, 0.0, 0.1); // Deep Space Blue
    float3 horizonColor = float3(0.05, 0.0, 0.15); // Purple-ish
    float3 bottomColor = float3(0.0, 0.0, 0.05); // Black
    
    float t = dir.y * 0.5 + 0.5; // Map [-1, 1] to [0, 1]
    
    float3 color = lerp(bottomColor, topColor, t);
    
    // Add some stars? (Noise function needed, skipped for simplicity)
    
    return float4(color, 1.0);
}
