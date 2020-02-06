Texture2D shaderTexture;
SamplerState SampleType;

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};
float4 main(PixelInputType input) : SV_TARGET
{
    float4 textureColor;


// Sample the pixel color from the texture using the sampler at this texture coordinate location.

textureColor = shaderTexture.Sample(SampleType, input.tex);
//textureColor.a = 1.f;

return textureColor;
}