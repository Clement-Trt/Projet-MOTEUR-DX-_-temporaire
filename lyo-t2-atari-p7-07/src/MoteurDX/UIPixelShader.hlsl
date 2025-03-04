cbuffer TransformBuffer : register(b0)
{
    float4x4 orthoMatrix; // Matrice orthographique (pass�e par le CPU)
};
Texture2D textureMap : register(t0); // Texture bind�e � la pipeline
SamplerState samplerState : register(s0); // �chantillonneur

struct PixelInput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

float4 main(PixelInput input) : SV_TARGET
{
    // R�cup�rer la couleur du texel � partir des UV
    float4 color = textureMap.Sample(samplerState, input.uv);
    //float4 color = 0;
    
    // Retourner la couleur avec son alpha (permet le blending)
    return color;
}
