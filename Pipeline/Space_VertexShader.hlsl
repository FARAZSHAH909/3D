cbuffer staticMeshBuffer : register(b0)
{
    float4x4 W;       // World Matrix
    float4x4 VP;      // View * Projection Matrix
    float time;       // Time in seconds
    float3 cameraPos; // Camera Position for lighting
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
    float2 TexCoords : TEXCOORD;
};

// Vertex Shader for Space Objects
PS_INPUT VS(VS_INPUT input)
{
    PS_INPUT output;
    
    // We want to create a floating effect for the objects in space.
    // So we'll mess with the local position a bit before transforming it.
    float4 animatedPos = input.Pos;
    
    // How high it floats up and down
    float amplitude = 0.5f; 
    
    // How fast the floating animation plays
    float frequency = 2.0f;
    
    // Make the movement ripple a bit by using the X position as an offset
    float offset = animatedPos.x * 0.5f;
    
    // Apply the vertical Bobbing effect using a sine wave
    animatedPos.y += sin(time * frequency + offset) * amplitude;
    
    // Transform to World Space
    float4 worldPos = mul(animatedPos, W);
    
    // Transform to Clip Space so it shows up on screen correctly
    output.Pos = mul(worldPos, VP);
    
    // Pass the data down the pipeline to the pixel shader
    output.WorldPos = worldPos.xyz;
    output.Normal = mul(input.Normal, (float3x3)W); // Rotate the normal to match the object's rotation
    output.Tangent = mul(input.Tangent, (float3x3)W); // Rotate tangent as well
    output.TexCoords = input.TexCoords;
    
    return output;
}
