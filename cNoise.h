#pragma once

class cNoise
{
public:
	cNoise();
	~cNoise();

	float IntNoise( int a_X );
	float IntNoise2D( int a_X, int a_Y );
	float IntNoise3D( int a_X, int a_Y, int a_Z );

	float LinearNoise1D( float a_X );
	float CosineNoise1D( float a_X );
	float CubicNoise1D( float a_X );
	float SmoothNoise1D( int a_X );

	float LinearNoise2D( float a_X, float a_Y );
	float CosineNoise2D( float a_X, float a_Y );
	float CubicNoise2D( float a_X, float a_Y );

	float CosineNoise3D( float a_X, float a_Y, float a_Z );
	float CubicNoise3D( float a_X, float a_Y, float a_Z );
private:
	float CubicInterpolate( float a_A, float a_B, float a_C, float a_D, float a_Pct );
	float CosineInterpolate( float a_A, float a_B, float a_Pct );
	float LinearInterpolate( float a_A, float a_B, float a_Pct );
};

