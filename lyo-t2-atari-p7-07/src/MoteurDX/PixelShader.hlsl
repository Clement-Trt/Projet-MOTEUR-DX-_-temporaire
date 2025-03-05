//#define MAX_POINT_LIGHTS 64

//struct PointLight
//{
//    float3 Position;
//    float pad1;
//    float3 Color;
//    float pad2;
//    float ConstantAtt;
//    float LinearAtt;
//    float QuadraticAtt;
//    float pad3;
//};

// Le constant buffer pour l’eclairage se trouve sur le slot b1.
cbuffer PassConstants : register(b1)
{
    float4 AmbientLight;
    
    // Lumiere directionnelle
    float3 DirLightDirection;
    float pad1;
    float3 DirLightColor;
    float pad2;
    
    // Lumiere point
    //int NumPointLights;
    //int pad[3]; // Alignement

    //PointLight PtLights[MAX_POINT_LIGHTS];
    float3 PtLightPosition;
    float pad3;
    float3 PtLightColor;
    float pad4;
    float PtLightConstantAtt;
    float PtLightLinearAtt;
    float PtLightQuadraticAtt;
    float pad5;
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
};

float4 PSMain(PS_INPUT input) : SV_Target // SV_Target est utilise pour le rendu moderne
{
    float4 texColor = g_Texture.Sample(g_Sampler, input.tex);
    
   // Normaliser la normale
    float3 norm = normalize(input.normal);

    // Calcul de l'eclairage directionnel
    float ndotlDir = max(dot(norm, -normalize(DirLightDirection)), 0.0);
    float3 diffuseDir = DirLightColor * ndotlDir;

    // Calcul de l'eclairage point (omnidirectionnel)
    float3 lightDir = PtLightPosition - input.worldPos;
    float distance = length(lightDir);
    lightDir = normalize(lightDir);
    float ndotlPt = max(dot(norm, lightDir), 0.0);
    float attenuation = 1.0 / (PtLightConstantAtt + PtLightLinearAtt * distance + PtLightQuadraticAtt * (distance * distance));
    float3 diffusePt = PtLightColor * ndotlPt * attenuation;
    //float3 diffusePtTotal = 0;
    //for (int i = 0; i < NumPointLights; i++)
    //{
    //    float3 lightDir = PtLights[i].Position - input.worldPos;
    //    float distance = length(lightDir);
    //    lightDir = normalize(lightDir);
    //    float ndotlPt = max(dot(norm, lightDir), 0.0);
    //    float attenuation = 1.0 / (PtLights[i].ConstantAtt + PtLights[i].LinearAtt * distance + PtLights[i].QuadraticAtt * (distance * distance));
    //    diffusePtTotal += PtLights[i].Color * ndotlPt * attenuation;
    //}

    // Combiner ambiant, directionnel et point
    // float3 finalLight = AmbientLight.rgb + diffuseDir + diffusePtTotal;
    float3 finalLight = AmbientLight.rgb + diffuseDir + diffusePt;
    float3 finalColor = finalLight * input.color.rgb * texColor.rgb;
    return float4(finalColor, texColor.a);
}

