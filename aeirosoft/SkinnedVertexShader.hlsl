cbuffer WorldMatrixBuffer : register(b0)
{
    matrix worldMatrix;

};
cbuffer ViewProjectionBuffer : register(b1)
{
    matrix viewMatrix;
    matrix projectionMatrix;
};
cbuffer BoneBuffer 
{
    matrix boneTransforms[50];
}
struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    int4 boneIndecies : BONES;
    float4 weight : WEIGHTS;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL0;
};

PixelInputType main(VertexInputType input)
{
    PixelInputType output;
   
    float4 blendPos = float4(0, 0, 0, 0);
    float4 normal = float4(0, 0, 0, 0);
    //int i = 0;
    input.position.w = 1.0f;
    blendPos += input.weight.x * mul(input.position, boneTransforms[input.boneIndecies.x]);
    blendPos +=  input.weight.y * mul(input.position, boneTransforms[input.boneIndecies.y]);
    blendPos += input.weight.z * mul(input.position, boneTransforms[input.boneIndecies.z]);
    blendPos += input.weight.w * mul(input.position, boneTransforms[input.boneIndecies.w]);
    blendPos.w = 1.0f;
    normal += input.weight.x * mul(input.normal, boneTransforms[input.boneIndecies.x]);
    normal += input.weight.y * mul(input.normal, boneTransforms[input.boneIndecies.y]);
    normal += input.weight.z * mul(input.normal, boneTransforms[input.boneIndecies.z]);
    normal += input.weight.w * mul(input.normal, boneTransforms[input.boneIndecies.w]);

    
    output.position = mul(blendPos, worldMatrix);
   // output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

    // Store the texture coordinates for the pixel shader.
    output.tex = input.tex;


    //Calculate the normal vector against the world matrix only.
    output.normal = mul(normal, worldMatrix);

    //normalize the normal vector after multiplication
    output.normal = normalize(output.normal);

    return output;
}