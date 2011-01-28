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
#include <dirent.h>
#include <errno.h>
#include <vector>

// other stuff
#include <fstream>
#include <map>
#include <string>

#ifndef _WIN32	// windows has 'safe' type functions
#define	sprintf_s(buffer, buffer_size, stringbuffer, ...) (sprintf(buffer, stringbuffer, __VA_ARGS__))
#define	vsprintf_s(buffer, stringbuffer, ...) (vsprintf(buffer, stringbuffer, __VA_ARGS__))
#define	sscanf_s(buffer, stringbuffer, ...) (sscanf(buffer, stringbuffer, __VA_ARGS__))
#endif


using namespace std;

struct point {
	int r, g, b;
};


#ifndef _WIN32
int getdir (string dir, vector<string> &files)
{
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        cout << "Error(" << errno << ") opening " << dir << endl;
        return errno;
    }
	
    while ((dirp = readdir(dp)) != NULL) {
		if ( dirp->d_type != DT_DIR){
			files.push_back(string(dirp->d_name));
		}
    }
    closedir(dp);
    return 0;
}
#else
#include <Windows.h>
void getdir (string dir, vector<string> &files)
{
	HANDLE hFind;
	WIN32_FIND_DATA FindFileData;
	int numfiles = 0;
	string Query = dir + "*.bin";
	if((hFind = FindFirstFile(Query.c_str(), &FindFileData)) != INVALID_HANDLE_VALUE){
		do{
			files.push_back( string( FindFileData.cFileName ) );
		}while(FindNextFile(hFind, &FindFileData));
		FindClose(hFind);
	}
}
#endif

int main(int argc, char *argv[])
{

	string chunkDirectory = "world/";
	string imageDirectory = "images/";
	string outputColor = "color";
	string colorFile = "colors.txt";
	int compressionLevel = 5;
	bool shush = false;
	bool createBlank = true;
	bool greyScale = false;
	bool onlyHeightmap = false;
	bool densityFile = false;
	
	// super awesome custom user input parser
	for (int i = 0; i < argc; i++) {
		
		if( strcmp(argv[i], "-i") == 0 ){
			chunkDirectory = argv[i+1];
		}
		else if ( strcmp(argv[i], "-o") == 0 ) {
			imageDirectory = argv[i+1];
		}
		else if ( strcmp(argv[i], "-q") == 0 ) {
			shush = true;
		}
		else if ( strcmp(argv[i], "-nb") == 0 ) {
			createBlank = false;
		}
		else if ( strcmp(argv[i], "-gs") == 0 ) {
			greyScale = true;
			onlyHeightmap = false;
		}
		else if ( strcmp(argv[i], "-hm") == 0 ) {
			greyScale = false;
			onlyHeightmap = true;
		}
		else if ( strcmp(argv[i], "-b") == 0 ) {
			colorFile = argv[i+1];
		}
		else if ( strcmp(argv[i], "-c") == 0) {
			int level = atoi(argv[i+1]);
			if(level >= 0 and level <= 9){
				compressionLevel = level;
			}
		}
		else if ( strcmp(argv[i], "-d") == 0) {
			densityFile = true;
		}
		else if ( strcmp(argv[i], "-h") == 0 or strcmp(argv[i], "--help") == 0 ) {
			cout << "\nUsage:\n  MCMapper -i <worldDirectory> -o <imageDirectory> [-gs] [-nb] [-q] [-h]\n\n";
			cout << "-i          Input Directory, contains world chunks.\n";
			cout << "            Default: world\n";
			cout << "-o          Output Directory, chunk images are saved here.\n";
			cout << "            Default: images\n";
			cout << "-c [0-9]    Compression Level. Sacrifices compression for speed.\n";
			cout << "            Default: 5\n";
			cout << "-b          Use a different block color file.\n";
			cout << "            Default: colors.txt (included)\n";
			cout << "-nb         Preserve an already existing 'blank.png'.\n";
			cout << "-q          Suppress output.\n";
			cout << "-h --help   Shows this message.\n\n";
			cout << "Output Options:\n";
			cout << "-hm         Save only the height map.\n";
			cout << "-gs         Save images in greyscale.\n";
			cout << "-d          Save a density file along with the images.\n\n";
			return 0;
		}
		else {
			// ignore it
		}
		
	}
	
	if(not shush) cout << "\nMCMapper - 2D Mapping for MCServer\n";
	if(not shush) cout << "By BenPhelps - www.benphelps.me\n\n";
	if(not shush) cout << "World Directory: " << chunkDirectory << "\n";
	if(not shush) cout << "Image Directory: " << imageDirectory << "\n";
	if(not shush) cout << "Compression Level: " << compressionLevel << "\n";
	if(greyScale) cout << "Saving images in greyscale.\n";
	if(onlyHeightmap) cout << "Saving image height map only.\n";

	// check for colors files
	ifstream color_file(colorFile.c_str(), std::ios::in);
	if( !color_file.good() )
	{
		color_file.close();
		printf( "Could not load colors files %s! File is missing.\n", colorFile.c_str() );
		return 0;
	}
	
	std::map<int, point> colors;
	point p;
	int id;
	int colorCount = 0;
	while(color_file >> id >> p.r >> p.g >> p.b)
	{
		colors[id] = p;
		colorCount++;
	}
	
	if(colorCount == 0){
		cout << "Could not load colors file is wrong format.\n";
		return 0;
	}
	
	// loop over files
	string dir = string(chunkDirectory.c_str());
    vector<string> files = vector<string>();
    getdir(dir,files);
	
	int total_chunks = 0;
	unsigned int density[128];
	
	if(not shush) cout << "Building heightmaps and creating chunk images...\n";
	
    for (unsigned int i = 0;i < files.size();i++) {
		
		string file = files[i];

		int chunkX, chunkY, chunkZ;
		int cordMatch = sscanf_s( file.c_str(), "X%d_Y%d_Z%d.bin", &chunkX, &chunkY, &chunkZ );

		// check if we are a proper chunk file
		if (cordMatch != 3){
			continue;
		}
		
		int byte = 0;
		int heightMap[16][16];
		int depthMap[16][16];
		
		// read in the chunk data
		// 16*16*128 bytes
		ifstream chunk;
		string fopen = chunkDirectory + file;
		chunk.open (fopen.c_str(), ios::binary);

		char * data;
		data = new char [32768];
		
		chunk.read (data,32768);
		chunk.close();

		// build a height map
		for (int x=0; x < 16; x++) { 
			for (int z=0; z < 16; z++) { 	
				bool bFoundWater = false;
				int waterDepth = 0;
				int topBlock = 0;
				for (int y=0; y < 128; y++) { 		
					int block = data[byte];
					byte++;
					
					// set highest not air block
					if (block != 0) {
						heightMap[x][z] = block;
						depthMap[x][z] = y;
						density[block] = density[block] + 1;
						topBlock = block;
					}
					
					// set the first block we find of water to the water depth
					if (block == 8 or block == 9 and bFoundWater == false) {
						waterDepth = y;
						bFoundWater = true;
					}
				}
				// if we found water at this X, Z !AND! the top block is water
				// then set the depth to the int we set in the loop
				if (bFoundWater == true and topBlock == 8 or topBlock == 9) {
					depthMap[x][z] = waterDepth;
				}
			}
		}
		
		// clear the file data
		delete[] data;

		// the file name of the new png
		char pngName[16];
		sprintf_s( pngName, 16, "%i-%i.png", chunkX, chunkZ );
		string fileName = imageDirectory + pngName;
		
		// create the image
		pngwriter image(16, 16, 1.0, fileName.c_str() );
		image.setcompressionlevel(compressionLevel);
		
		for (int x=0; x < 16; x++) { 
			for (int z=0; z < 16; z++) {
				int blockID = heightMap[x][z];
				float depth = depthMap[x][z];
				if (greyScale == true) {
					float luma = ((((0.299*colors[blockID].r) + (0.587*colors[blockID].g) + (0.114*colors[blockID].b)) * (depth/128)) / 255.0f);
					image.plot(x+1, z+1, luma, luma, luma);
				}
				else if (onlyHeightmap == true) {
					float height = (depth*2) / 255.0f;
					image.plot(x+1, z+1, height, height, height);
				}
				else {
					float r = (colors[blockID].r * (depth/128)) / 255.0f;
					float g = (colors[blockID].g * (depth/128)) / 255.0f;
					float b = (colors[blockID].b * (depth/128)) / 255.0f;
					image.plot(x+1, z+1, r, g, b);
				}
			}
		}
		
		image.close();
		
		total_chunks++;
    }

	if (createBlank) {
		// the blank.png file
		std::string decoded = base64_decode(
		"iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAIAAACQkWg2AAAAAnRFWHROQXHJZTwAAAATSURBVHjaYmAYBaNgFMAAQIABAAMQAAGxh94UAAAAAElFTkSuQmCC"
										);
		string blankPNGf = imageDirectory + string("/") + string("blank.png");
		ofstream blankPNG(blankPNGf.c_str() ,ofstream::binary);
		blankPNG.write (decoded.c_str(), 90);
		blankPNG.close();
	}
	
	
	if (densityFile) {
		float totalBlocks;
		char * format;
		format = new char [16];
		
		for(int i = 0; i < 128; i++) {
			totalBlocks += density[i];
		}
		
		ofstream densityFileH("density.txt");
		for(int i = 0; i < 128; i++) {
			int blockDensity = density[i];
			if (blockDensity > 0) {	
				float percent = blockDensity / totalBlocks * 100.0f;
				sprintf( format, "%i:%i:%f\n", i, blockDensity, percent);
				densityFileH << format;
			}
		}
		densityFileH.close();
	}
	
	if(not shush) cout << "Processed " << total_chunks << " chunks.\n";
	if(not shush) cout << "Done!\n\n";
	
	return 0;
}
