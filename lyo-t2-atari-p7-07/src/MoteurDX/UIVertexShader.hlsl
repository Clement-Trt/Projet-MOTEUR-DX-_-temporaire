cbuffer TransformBuffer : register(b0)
{
    float4x4 orthoMatrix; // Matrice orthographique (passée par le CPU)
};

struct VertexInput
{
    float3 position : POSITION; // Position en espace local
    float2 uv : TEXCOORD; // Coordonnées UV
};

struct PixelInput
{
    float4 position : SV_POSITION; // Position en espace écran
    float2 uv : TEXCOORD; // Coordonnées UV
};

PixelInput main(VertexInput input)
{
    PixelInput output;
    
    // Transformation en espace homogène
    output.position = mul(float4(input.position, 1.0f), orthoMatrix);
    
    // Passer les coordonnées UV
    output.uv = input.uv;
    
    return output;
}
