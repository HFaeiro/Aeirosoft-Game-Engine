cbuffer WorldMatrixBuffer : register(b0)
{
    matrix worldMatrix;

};
cbuffer ViewProjectionBuffer : register(b1)
{
    matrix viewMatrix;
    matrix projectionMatrix;
};

struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
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




    // Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

    // Calculate the position of the vertex against the world, view, and projection matrices.

    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);


    // Store the texture coordinates for the pixel shader.
    output.tex = input.tex;



    //Calculate the noraml vector against the world matrix only.
    output.normal = mul(input.normal, (float3x3)worldMatrix);

    //normalize the normal vector after multiplication
    output.normal = normalize(output.normal);

    return output;
}