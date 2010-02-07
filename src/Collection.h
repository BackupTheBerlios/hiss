#ifndef COLLECTION_H_INCLUDED
#define COLLECTION_H_INCLUDED

#include "globals.h"
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <list>
#include <stdlib.h> // for random

#define HMD_MININUM_IMAGES 100 //we have to move this into a config file... later ;)
#define HMD_MAXIMUM_IMAGEFOLDER_SIZE 1024*1024*100 //we have to move this into a config file... later ;)

class Collection
{
	public:
	Collection(string _imageDir = "./images/");

	void readImages();
	string& getNextImage();

	// Optional
	bool downloadImages(unsigned int _amount); //http://de.tinypic.com/random.php?page=4&type=images
	bool flushImages();

        unsigned int getNumberOfImages();
        unsigned int getSizeOfImageFolder();
        string getImageDir(); //bits are cheap. better a big copy instead of a reference which can mess up our class

	protected:
	string imageDir;
	vector<string> images;
        size_t folderSize;
        unsigned int imageCount;

        // Optional
        unsigned int getImageFolderInformation();
        bool deleteOldImages(size_t _size);
};

#endif
