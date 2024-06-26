
Texture2D TexDifuse1 : register(t0);
SamplerState Sample0 : register(s0);
cbuffer ConstantBuffer : register(b0)
{
    matrix g_WorldMatrix : packoffset(c0);
    matrix g_ViewMatrix : packoffset(c4);
    matrix g_ProjMatrix : packoffset(c8);
};

cbuffer CollisionBuffer : register(b1)
{
    float4 TargetColor; // 검사할 색상 (RGBA)
    float2 TargetPosition; // 검사할 위치 (텍스처 좌표)
};

float4 PS() : SV_Target
{
    
    float4 color = TexDifuse1.Sample(Sample0, TargetPosition);
    
    if (color.r == TargetColor.r && color.g == TargetColor.g && 
        color.b == TargetColor.b && color.a == TargetColor.a)
    {
        color = float4(1, 0, 0, 1);
    }
    else
    {
        color = float4(0, 0, 0, 0);
    }
    
    return color;
	//return TexDifuse1.Sample(Sample0,vIn.Tex);//
}

