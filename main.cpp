/*
 *  main.cpp
 *  MCMapper
 *
 *  Created by Ben Phelps on 1/25/11.
 *  Copyright 2011. All rights reserved.
 *
 */


// libs
#include "pngwriter.h"
#include "base64.h"

// dir listing
#include <errno.h>
#include <vector>

// other stuff
#include <fstream>
#include <map>
#include <string>

// noise
#include "cNoise.h"

#ifndef _WIN32	// windows has 'safe' type functions
#define	sprintf_s(buffer, buffer_size, stringbuffer, ...) (sprintf(buffer, stringbuffer, __VA_ARGS__))
#define	vsprintf_s(buffer, stringbuffer, ...) (vsprintf(buffer, stringbuffer, __VA_ARGS__))
#define	sscanf_s(buffer, stringbuffer, ...) (sscanf(buffer, stringbuffer, __VA_ARGS__))
#endif


using namespace std;

struct point {
	int r, g, b;
};

cNoise m_Noise;
float GetNoise( float x, float y )
{
	float oct1 = m_Noise.CubicNoise2D( x*0.100000, y*0.100000 )*1.000000;
	float oct2 = m_Noise.CubicNoise2D( x*1.000000, y*1.000000 )*0.500000;
	float oct3 = m_Noise.CubicNoise2D( x*2.000000, y*2.000000 )*0.500000;
	
	float height = m_Noise.CosineNoise2D( x*0.1f, y*0.1f )*2;
	
	float flatness = ((m_Noise.CubicNoise2D( x*0.5f, y*0.5f ) + 1.f ) * 0.5f) * 1.1f; // 0 ... 1.5
	flatness *= flatness * flatness;
	
	return (oct1 + oct2 + oct3) * flatness + height;
}

int main(int argc, char *argv[])
{
	
	int width = 100;
	int height = 100;
	int imageWidth = width * 16;
	int imageHeight = height * 16;
	
	string fileName = "test.png";
	
	// create the image
	pngwriter image(imageWidth, imageHeight, 1.0, fileName.c_str() );
	image.setcompressionlevel(9);
	
	for (int mX = 0; mX < width; mX++) {
		for (int mY = 0; mY < height; mY++) {
			
			for (int x = 0; x < 16; x++) {
				int worldX = mX * 16 + x;
				for (int y = 0; y < 16; y++) {
					int worldY = mY * 16 + y;
					int height = (float)(GetNoise( worldX*0.05f, worldY*0.05f )*16);
					float scale = ((height*-1*16) / 255.0f - -1.0f) * 1.0f / 2;
					
					const int lower = 64;
					const int top = lower+height;
					//cout << lower - height - top << "\n";
					if (lower - height - top > 3.0f && lower - height - top < 5.0f) {
						image.plot(worldX+1, worldY+1, scale, scale, 0.0f);
					}
					else if (lower - height - top < 5.0f) {
						image.plot(worldX+1, worldY+1, 0.0f, 0.0f, scale);
					}
					else {
						
						image.plot(worldX+1, worldY+1, 0.0f, scale, 0.0f);
					}

					
					
				}
			}
			
		}
	}
	
	image.close();
	
	return 0;
}
