cbuffer Transform : register(b0) // Buffer de constantes contenant la matrice de transformation
{
    matrix WorldViewProj;
};

struct VS_INPUT
{
    float3 pos : POSITION;
    float4 color : COLOR;
    float2 tex : TEXCOORD;
};

struct PS_INPUT
{
    float4 pos : SV_POSITION; // SV_POSITION est utilise pour les shaders modernes
    float4 color : COLOR;
    float2 tex : TEXCOORD;
};

PS_INPUT VSMain(VS_INPUT input)
{
    PS_INPUT output;
    // output.pos = float4(input.pos, 1.0f);
    output.pos = mul(float4(input.pos, 1.0f), WorldViewProj); // Appliquer la transformation
    output.color = input.color;
    output.tex = input.tex;
    return output;
}
