#include "Collection.h"

Collection::Collection(string _imageDir)
{
    this->imageDir = _imageDir;
    this->getImageFolderInformation();
    srand((unsigned int)time(NULL));

    this->deleteOldImages( (int)(this->getSizeOfImageFolder()/10));
    if(this->imageCount < HMD_MININUM_IMAGES)
    {
        this->downloadImages(HMD_MININUM_IMAGES - imageCount);
    }
}

unsigned int Collection::getNumberOfImages()
{
    return(this->imageCount);
}

unsigned int Collection::getSizeOfImageFolder()
{
    return(this->folderSize);
}

string Collection::getImageDir()
{
    return(this->imageDir);
}

void Collection::getImageInformation()
{
    DIR Dir;
    struct dirent *DirEntry;
    Dir = opendir(imageDir);
    this->folderSize = 0;
    this->imageCount = 0;


    while(Dir=readdir(Dir))
    {
        if ( DirEntry->d_type == isFile)
        {
            this->folderSize += DirEntry->st_size;
            this->images.push_back(DirEntry->d_name);
            this->imageCount++;
        }
    }
    closedir(imageDir);
}

bool Collection::deleteOldImages(size_t _size)
{
    if(this->imageCount == 0 || _size = 0)
    {
        return(true);
    }

    //list<struct dirent*> images;

    struct dirent *DirEntry;
    size_t delSize = 0;

    while(delSize < _size)
    {
        Dir = opendir(imageDir);
        while(Dir=readdir(Dir))
        {
            if ( DirEntry->d_type == isFile)
            {
                //images.push_back(DirEntry);
                if(rand()%4 == 3)
                {
                    delSize += DirEntry->st_size;
                    remove(file);
                    if(!(delSize < _size))
                    {
                        break;
                    }
                }
            }
        }
        closedir(imageDir);
    }

    this->getImageFolderInformation();
    //we could just use math. but do it again is much more secure
}

bool Collection::flushImages()
{
    if(this->imageCount == 0)
    {
        return(true);
    }

    struct dirent *DirEntry;

    Dir = opendir(imageDir);
    while(Dir=readdir(Dir))
    {
        if ( DirEntry->d_type == isFile)
        {
            remove(file);
        }

    }
    closedir(imageDir);
    this->getImageFolderInformation();
    //we could just use math. but do it again is much more secure
}

bool Collection::downloadImages(unsigned int _amount)
{

}
