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


using namespace std;

struct point {
	int r, g, b;
};


// This is not cross platform, need to find a method for windows.
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

int main(int argc, char *argv[])
{

	string chunkDirectory = "world";
	string imageDirectory = "images";
	string outputColor = "color";
	string colorFile = "colors.txt";
	int compressionLevel = 5;
	bool shush = false;
	bool createBlank = true;
	bool greyScale = false;
	bool onlyHeightmap = false;
	
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
			if(level > 0 and level <= 9){
				compressionLevel = level;
			}
		}
		else if ( strcmp(argv[i], "-h") == 0 or strcmp(argv[i], "--help") == 0 ) {
			cout << "\nUsage:\n  MCMapper -i <worldDirectory> -o <imageDirectory> [-gs] [-nb] [-q] [-h]\n\n";
			cout << "-i          Input Directory, contains world cunks.\n";
			cout << "            Default: world\n";
			cout << "-o          Output Directory, chunk images are saved here.\n";
			cout << "            Default: images\n";
			cout << "-c [0-9]    Compression Level. Sacrifices compression for speed.\n";
			cout << "            Default: 5\n";
			cout << "-b          Use a different block color file.\n";
			cout << "            Default: colors.txt (included)\n";
			cout << "-nb         Preserve an already existing 'blank.png'.\n";
			cout << "-q          Suppress output.\n";
			cout << "-h --help   Output this message.\n\n";
			cout << "Output Options:\n";
			cout << "-hm         Save only the height map (buggy).\n";
			cout << "-gs         Save images in greyscale.\n\n";
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
	
	if(not shush) cout << "Loading Colors...\n";
	// load colors
	ifstream color_file( colorFile.c_str() );
	std::map<int, point> m;
	point p;
	int id;
	int colorCount = 0;
	while(color_file >> id >> p.r >> p.g >> p.b)
	{
		m[id] = p;
		colorCount++;
	}
	
	if(colorCount == 0){
		cout << "Could not load colors from colors.txt! File is missing or wrong format.\n";
		return 0;
	}
	
	// loop over files
	string dir = string(chunkDirectory.c_str());
    vector<string> files = vector<string>();
    getdir(dir,files);
	
	int total_chunks = 0;
	
	if(not shush) cout << "Building heightmaps and creating chunk images...\n";
	
    for (unsigned int i = 0;i < files.size();i++) {
		
		string file = files[i];

		int chunkX, chunkY, chunkZ;
		int cordMatch = sscanf( file.c_str(), "X%d_Y%d_Z%d.bin", &chunkX, &chunkY, &chunkZ );

		// check if we are a proper chunk file
		if (cordMatch != 3){
			cout << "Warning: " << file.c_str() << " does not have proper file naming.  Ignoring.\n";
			continue;
		}
		
		int byte = 0;
		int hm[16][16];
		int depth[16][16];
		
		// read in the chunk data
		// 16*16*128 bytes
		ifstream chunk;
		string fopen = chunkDirectory + string("/") + file;
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
						hm[x][z] = block;
						depth[x][z] = y;
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
					depth[x][z] = waterDepth;
				}
			}
		}
		
		// clear the file data
		delete[] data;

		// the file name of the new png
		char pngName[16];
		sprintf( pngName, "%i-%i.png", chunkX, chunkZ );
		string out = imageDirectory + "/" + pngName;
		
		// create the image
		pngwriter image(16, 16, 1.0, out.c_str() );
		image.setcompressionlevel(compressionLevel);
		
		for (int x=0; x < 16; x++) { 
			for (int z=0; z < 16; z++) {
				int block = hm[x][z];
				float dpth = depth[x][z];
				if (greyScale == true) {
					float luma = ((((0.299*m[block].r) + (0.587*m[block].g) + (0.114*m[block].b)) * (dpth/128)) / 255.0f);
					image.plot(x+1, z+1, luma, luma, luma);
				}
				else if (onlyHeightmap == true) {
					float height = (dpth*2) / 255.0f;
					image.plot(x+1, z+1, height, height, height);
				}
				else {
					float r = m[block].r * (dpth/128) / 255.0f;
					float g = m[block].g * (dpth/128) / 255.0f;
					float b = m[block].b * (dpth/128) / 255.0f;
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
	
	
	if(not shush) cout << "Processed " << total_chunks << " chunks.\n";
	if(not shush) cout << "Done!\n\n";
	
	return 0;
}

