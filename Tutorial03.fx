//--------------------------------------------------------------------------------------
// File: Tutorial02.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------

struct VS_INPUT
{
	float4 Pos : POSITION;
	float4 Color : COLOR;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR;
};


float4 VS( float4 Pos : POSITION ) : SV_POSITION
{
    return Pos;
}

PS_INPUT VS_COLOR(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	output.Pos = input.Pos;
	output.Color.rgba = input.Color.rgba;
	return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( float4 Pos : SV_POSITION ) : SV_Target
{
    return float4( 1.0f, 1.0f, 0.0f, 1.0f );    // Yellow, with Alpha = 1
}

float4 PS_COLOR(PS_INPUT input) : SV_Target
{
	//input.Color.rgba = float4(1.0f, 0.0f, 0.0f, 1.0f);
	return input.Color;
}

float4 PS_WHITE(float4 Pos : SV_POSITION) : SV_Target
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);    // WHite, with Alpha = 1
}