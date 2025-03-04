cbuffer TransformBuffer : register(b0)
{
    float4x4 orthoMatrix; // Matrice orthographique (pass�e par le CPU)
};

struct VertexInput
{
    float3 position : POSITION; // Position en espace local
    float2 uv : TEXCOORD; // Coordonn�es UV
};

struct PixelInput
{
    float4 position : SV_POSITION; // Position en espace �cran
    float2 uv : TEXCOORD; // Coordonn�es UV
};

PixelInput main(VertexInput input)
{
    PixelInput output;
    
    // Transformation en espace homog�ne
    output.position = mul(float4(input.position, 1.0f), orthoMatrix);
    
    // Passer les coordonn�es UV
    output.uv = input.uv;
    
    return output;
}
