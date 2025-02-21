Texture2D g_Texture : register(t0);
SamplerState g_Sampler : register(s0);

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
    float2 tex : TEXCOORD;
};

float4 PSMain(PS_INPUT input) : SV_Target // SV_Target est utilise pour le rendu moderne
{
    // return g_Texture.Sample(g_Sampler, input.tex);
    float4 texColor = g_Texture.Sample(g_Sampler, input.tex);
    return input.color * texColor;
    
    //return float4(0, 1, 0, 1); green par defaut
    // return input.color; // Renvoie la couleur du pixel
}