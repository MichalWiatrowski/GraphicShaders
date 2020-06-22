cbuffer MatrixBuffer : register(b0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};


struct InputType
{
    float3 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

struct OutputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};




[maxvertexcount(104)]
void main(triangle InputType input[3], inout TriangleStream<OutputType> triStream)
{
    
   // if (g_subdivs != 0 && g_subdivs <= MAX_SUBDIVISION_LEVEL)
  //  {
        uint level_count = (uint) pow(2, 4);
        float3 u = (input[1].position.x - input[0].position.x) / level_count,
               w = (input[2].position.x - input[0].position.x) / level_count,
               v = input[0].position.x;

        for (int level = level_count; level > 0; --level)
        {
            OutputType output;
            output.normal = input[1].normal;
            output.tex = input[1].tex;
            output.position = mul(float4(normalize(v), 1.f), worldMatrix);
            output.position = mul(output.position, viewMatrix);
            output.position = mul(output.position, projectionMatrix);
            triStream.Append(output);

            float3 p = v;
            uint vertex_count = level + 1;

            for (uint vertex = 1; vertex < vertex_count; ++vertex)
            {
             output.position = mul(float4(normalize(p + u), 1.f), worldMatrix);
             output.position = mul(output.position, viewMatrix);
             output.position = mul(output.position, projectionMatrix);
             triStream.Append(output);

            output.position = mul(float4(normalize(p += w), 1.f), worldMatrix);
            output.position = mul(output.position, viewMatrix);
            output.position = mul(output.position, projectionMatrix);
            triStream.Append(output);
            }

            v += u;
            triStream.RestartStrip();
        }
   // }
    //else
    //{
    //    [unroll]
    //    for (uint i = 0; i < 3; ++i)
    //    {
    //        GS_OUT gout;
    //        gout.diff = gin[i].diff;
    //        gout.pos_h = mul(float4(gin[i].pos_l, 1.f), g_mat_wvp);

    //        s.Append(gout);
    //    }
    //}
}