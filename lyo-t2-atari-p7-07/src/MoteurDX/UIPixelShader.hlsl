Texture2D textureMap : register(t0); // Texture bindée à la pipeline
SamplerState samplerState : register(s0); // Échantillonneur

struct PixelInput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

float4 main(PixelInput input) : SV_TARGET
{
    // Récupérer la couleur du texel à partir des UV
    float4 color = textureMap.Sample(samplerState, input.uv);
    
    // Retourner la couleur avec son alpha (permet le blending)
    return color;
}
