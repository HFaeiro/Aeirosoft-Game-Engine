Texture2D shaderTexture;
SamplerState SampleType;

cbuffer LightBuffer
{
    float4 ambientColor;
    float4 diffuseColor;
    float4 lightDirection;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

//static const float4 matColor = { .7f, .7f, .9f , 1.f};
static const float4 ambient = { .1f, .1f, .1f , 1.0f};
static const float4 diffColor = { .5f, .5f, .5f, 1.0f };
static const float4 lightDirect = { 1.0f, 1.0f, 0.f, 0.f };


float4 main(PixelInputType input) : SV_TARGET
{
    float4 textureColor;
    float3 lightDir;
    float lightIntensity;
    float4 color;


// Sample the pixel color from the texture using the sampler at this texture coordinate location.
    textureColor = shaderTexture.Sample(SampleType, input.tex);

    color = ambientColor;

    ////invert the light direction for calculations.
    lightDir = lightDirection;

    //////calculate the amount of light on this pixel.
    lightIntensity = saturate(dot(input.normal, lightDir));

    if (lightIntensity > 0.0f)
    {
        //Determine the final diffuse color based on the diffuse color and the amount of light intensity
        color += diffuseColor * lightIntensity;
    }

    //////Determine the final amount of diffuse color based on the diffuse color combine with th elight intensity.
    color = saturate(color);

    //////Multiply the texture pixel and the final diffuse color to get the final pixel color result
    color = color * textureColor;

    return color;
    //return textureColor;
}