struct DirectionalLight
{
    float3 Direction;
    float pad1;
    float3 Color;
    float pad2;
};
struct PointLight
{
    float3 Position;
    float pad1;
    float3 Color;
    float pad2;
    float ConstantAtt;
    float LinearAtt;
    float QuadraticAtt;
    float pad3;
};

// Le constant buffer pour l�eclairage se trouve sur le slot b1.
cbuffer PassConstants : register(b1)
{
    float4 AmbientLight;
    
    // Lumiere directionnelle
    DirectionalLight DirLight;
    
    // Lumiere point
    PointLight PtLight;
};

Texture2D g_Texture : register(t0);
SamplerState g_Sampler : register(s0);

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float3 worldPos : TEXCOORD1;
    float3 normal : NORMAL;
    float4 color : COLOR;
    float2 tex : TEXCOORD;
    float highlightActive : TEXCOORD2;
    float highlightIntensity : TEXCOORD3;
};

float4 PSMain(PS_INPUT input) : SV_Target // SV_Target est utilise pour le rendu moderne
{
    float4 texColor = g_Texture.Sample(g_Sampler, input.tex);
    
   // Normaliser la normale
    float3 norm = normalize(input.normal);

    // Calcul de l'eclairage directionnel
    float ndotlDir = max(dot(norm, -normalize(DirLight.Direction)), 0.0);
    float3 diffuseDir = DirLight.Color * ndotlDir;

    // Calcul de l'eclairage point (omnidirectionnel)
    float3 lightDir = PtLight.Position - input.worldPos;
    float distance = length(lightDir);
    lightDir = normalize(lightDir);
    float ndotlPt = max(dot(norm, lightDir), 0.0);
    float attenuation = 1.0 / (PtLight.ConstantAtt + PtLight.LinearAtt * distance + PtLight.QuadraticAtt * (distance * distance));
    float3 diffusePt = PtLight.Color * ndotlPt * attenuation;

    // Combiner ambiant, directionnel et point
    float3 finalLight = AmbientLight.rgb + diffuseDir + diffusePt;
    
    if (input.highlightActive > 0.5)
    {
        finalLight *= input.highlightIntensity;
    }
    float3 finalColor = finalLight * input.color.rgb * texColor.rgb;
    
    return float4(finalColor, texColor.a);
}

