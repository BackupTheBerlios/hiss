#include "Stegano.h"

Stegano::Stegano(string& _imageDir, string& _tempDir) : qImageDir(_imageDir.c_str()), qTempDir(_tempDir.c_str())
{
    imageDir = _imageDir;
    tempDir = _tempDir;
    nextMode = 0;

    curImage = 0;
    readImages();
    shuffleImages();
}

Stegano::Stegano(const QDir& _imageDir,const QDir& _tempDir)  : qImageDir(_imageDir), qTempDir(_tempDir)
{
    imageDir = _imageDir.absolutePath().toStdString();
    tempDir = _tempDir.absolutePath().toStdString();
    nextMode = 0;

    curImage = 0;
    readImages();
    shuffleImages();
}

unsigned int Stegano::getNumberMethods()
{
	return(2);
}

QByteArray Stegano::getMetaFileFromImage(QString& _file, unsigned int _method)
{
	string buffer;
	if(_method == 1)
	{
		cout << "Using Exif for metafile extraction of file: " << _file.toStdString() << endl;
		readTag(_file.toStdString(), buffer);
		return(QByteArray(buffer.c_str(), buffer.length()));
	}

	else if(_method == 2)
	{
		cout << "Using LSB for metafile extraction of file: " << _file.toStdString() << endl;
		QImage dataImage(_file);
		readLSB(_file.toStdString(), buffer, (unsigned int)dataImage.width()*dataImage.height()/2);
		return(QByteArray(buffer.c_str(), buffer.length()));
	}

	else
	{
		cout << "Invalid Method: " << _method << endl;
		return(QByteArray());
	}
}

bool Stegano::hasExtension(const string& _fileName, const string& _extension)
{
   	if(_fileName.length() > _extension.length())
   	{
        string extension = _fileName.substr(_fileName.length()-_extension.length());
        return(_extension == extension);
   	}
   	return(false);
}

bool Stegano::isImage(const string& _name)
{
    return(hasExtension(_name, ".png") || hasExtension(_name, ".jpg"));// || hasExtension(_name, ".PNG") || hasExtension(_name, ".JPG") || hasExtension(_name, ".jpeg") || hasExtension(_name, ".JPEG"));
}

void Stegano::readImages()
{
	//QString qimage(imageDir.c_str());
	//QDir directory(qimage);
	QStringList files = qImageDir.entryList();
	unsigned int i;
	string temp;
	images.reserve(files.size());

	for(i = 0; i < files.size(); i++)
	{
		temp = files.at(i).toStdString();
		if(isImage(temp))
		{
            //cout << temp << endl;
            images.push_back(new string(temp));
		}
	}
}

ostream& Stegano::display(ostream& given)
{
    int i;

    given << qImageDir.absolutePath().toStdString() << endl;
    given << qTempDir.absolutePath().toStdString() << endl;

    for(i = 0; i < images.size(); i++)
    {
        given << *images[i] << endl;
    }
    return(given);
}

ostream& operator<<(ostream& given, Stegano& givenSteg)
{
    return(givenSteg.display(given));
}

void Stegano::shuffleImages()
{
    srand((unsigned)time(NULL));
    curImage = 0;
    unsigned int length = images.size(), newPosition;
    int i;
    string* temp;

    for(i = 0; i < length; i++)
    {
        newPosition = rand()%length;
        temp = images[i];
        images[i] = images[newPosition];
        images[newPosition] = temp;
    }
}


string Stegano::getImageString()
{
    if(curImage >= images.size())
    {
        shuffleImages();
    }
    curImage++;
    return(*images[curImage-1]);
}

QString Stegano::getImage()
{
    if(curImage >= images.size())
    {
        shuffleImages();
    }
    curImage++;
    return(QString(images[curImage-1]->c_str()));
}

string& Stegano::getCurrentImageString()
{
    if(curImage >= images.size())
    {
        shuffleImages();
    }
    return(*images[curImage]);
}

bool Stegano::copy(const char* source, const char* destination)
{
	bool useQt = true;
	if(useQt)
	{
		if(!QFile::copy(QString(source), QString(destination)))
		{
			cout << "Copy(" << source << ", "<<destination<<"); was not successful" << endl;
			return(false);
		}

		else
		{
			return(true);
		}
	}

	else
	{

		struct stat buf;
		stat(source,&buf);
		unsigned long fileSize = buf.st_size;
		unsigned char* buffer = (unsigned char*) malloc(sizeof(unsigned char) * fileSize);

		FILE* copysource = fopen(source, "r");
		FILE* copydestination = fopen(destination, "w");

		fread(buffer, sizeof(unsigned char), fileSize, copysource);
		fwrite(buffer, sizeof(unsigned char), fileSize, copydestination);
		fclose(copysource);
		fclose(copydestination);
		free(buffer);
	}
}

void Stegano::readTag(const string& name, string& buffer)
{
    Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(name.c_str());
    image->readMetadata();
    Exiv2::ExifData exifData = image->exifData();

    Exiv2::Exifdatum& tag = exifData["Exif.Photo.ImageUniqueID"];
    buffer = tag.toString();
    QByteArray qtemp = QByteArray::fromBase64 (buffer.c_str());
    string temp(qtemp.constData(), qtemp.size());
    buffer = temp;

    //std::cout << "DEBUG: " << date << std::endl;
}

bool Stegano::tagImage(const string& name, const string& data)
{
    char* temp = (char*) data.c_str();


    Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(name);

    if(image.get() == 0)
    {
        return(false);
    }



    image->readMetadata();
    Exiv2::ExifData &exifData = image->exifData();

    //saving some small exif information before purge to create a more "normal" Image
    string camera_model = exifData["Exif.Image.Model"].toString();
    string camera_producer = exifData["Exif.Image.Make"].toString();
    /*Exiv2::Exifdatum xres = exifData["Exif.Image.XResolution"];
    Exiv2::Exifdatum yres = exifData["Exif.Image.YResolution"];
    Exiv2::Exifdatum timestamp = exifData["Exif.GPSInfo.GPSTimeStamp"];
    Exiv2::Exifdatum& expo = exifData["Exif.Image.ExposureTime"];
    Exiv2::Exifdatum& focal = exifData["Exif.Image.FocalLength"];
    Exiv2::Exifdatum& flash = exifData["Exif.Image.Flash"];
    Exiv2::Exifdatum& iso = exifData["Exif.Photo.ISOSpeedRatings"];*/

    exifData.clear();

    exifData["Exif.Image.Model"] = camera_model.c_str();
    exifData["Exif.Image.Make"] = camera_producer.c_str();
    /*exifData["Exif.Image.XResolution"] = xres;
    exifData["Exif.Image.YResolution"] = yres;
    exifData["Exif.GPSInfo.GPSTimeStamp"] = timestamp;
    exifData["Exif.Image.ExposureTime"] = expo;
    exifData["Exif.Image.FocalLength"] = focal;
    exifData["Exif.Image.Flash"] = flash;
    exifData["Exif.Photo.ISOSpeedRatings"] = iso;*/


    exifData["Exif.Photo.ImageUniqueID"] = temp;

    image->writeMetadata();

    return(true);
}


//I would write in a buffer from the Main Program. But the other guys want a big binary copy... not clever but hey...
//Stegano::read(string& _image, string& buffer, unsigned int length, unsigned int mode)
QByteArray Stegano::read(Item_Image _image)
{
    unsigned int mode = _image.method;
    unsigned int length = _image.datalength;
    string buffer;

    if(mode == 1)
    {
        if(isImage(_image.file.toStdString()))
        {
            readTag(_image.file.toStdString(), buffer);
            //cout << "Debug from ReadLSB: " << buffer.c_str() << endl;
            return(QByteArray(buffer.c_str(), buffer.length()));
        }
        else
        {
            return(QByteArray(""));
        }
    }

    else if(mode == 2)
    {
        if(hasExtension(_image.file.toStdString(), ".jpg"))
        {
            buffer = "";
            return(QByteArray(buffer.c_str(), buffer.length()));

        }

        else
        {
            readLSB(_image.file.toStdString(), buffer, length);
            //cout << "Debug from ReadLSB: " << buffer << endl;
            return(QByteArray(buffer.c_str(),buffer.length()));
        }
    }

    else
    {
        return(QByteArray(""));
    }

}

/*
bool Stegano::readLsbPng(string& _image, string& buffer, unsigned int length)
{
    QImage readImage(QString::fromStdString(_image));

    QRgb data;
    unsigned int width = width();
    unsigned int height = height();
    unsigned int a,b;
    unsigned char rtemp, gtemp, btemp;

    if(width*height/2 < length)
    {
        return(false);
    }

    for(a = 0; i a < height; a++)
    {
        for(b = 0; b < width; b++)
        {
            data = readImage.pixel(a,b);
            rtemp = data.qRed();
            rtemp = rtemp<<6;
            rtemp = rtemp>>4;

            gtemp = data.qGreen();
            gtemp = gtemp <<7;
            gtemp = gtemp >>6;

            btemp = data.qBlue();
            btemp = btemp <<7
            btemp = btemp >>7;

            if(((a*width)+b)%2 == 0)
            {
                buffer[(a*width+b)/2] = (rtemp+gtemp+btemp)<<4;
            }

            else
            {
                buffer[(a*width+b)/2] += (rtemp+gtemp+btemp);
            }

            if((width*height/2) == length)
            {
                return(true);
            }
        }
    }
}
*/

/*!
* Fucking important function. Its chooses which mode is taken next (Pixel(LSB), Header(Evif), maybe an succesor added mode)
*/
qint64 Stegano::getCapacity(QString& _image)
{
		if(hasExtension(_image.toStdString(),".jpg"))
			nextMode = 1;
		if(hasExtension(_image.toStdString(),".png"))
			nextMode = 2;
    //nextMode = ((rand()+clock()+noise[rand()%32])%2)+1;

    if(nextMode == 2)
    {
        QImage image(_image);

	if(false)//image.width()*image.height() > 800*600 || image.width()*image.height() < 10*10 ) //around 1 MB, bigger PNG could be a problem for the hosting services
	{
		cout << image.width()<<"*"<<image.height() << " Pixels are to big files for hosters if we use PNG. Using exif" << endl;
		nextMode = 1;
		return(64*1024/4*2.5);
	}

	else
	{
	        return(image.width()*image.height()/2);
	}
    }

    else // if 1
    {
        return(64*1024/4*2.5); //64 KB header for jpg files - base64 encoding 3 to 4 bytes
    }
}

/*!
* Fucking important function. Its chooses which mode is taken next (Pixel(LSB), Header(Evif), maybe an succesor added mode)
*/
qint64 Stegano::getCapacity()
{
		if(hasExtension(getCurrentImageString(),".jpg"))
			nextMode = 1;
		if(hasExtension(getCurrentImageString(),".png"))
			nextMode = 2;
    //nextMode = ((rand()+clock()+noise[rand()%32])%2)+1;

    if(nextMode == 2)
    {
        QImage image(qImageDir.absolutePath()+QDir::separator()+QString::fromStdString(getCurrentImageString()));

	if(false)//image.width()*image.height() > 800*600 || image.width()*image.height() < 10*10 ) //around 1 MB, bigger PNG could be a problem for the hosting services
	{
		cout << image.width()<<"*"<<image.height() << " Pixels are to big files for hosters if we use PNG. Using exif" << endl;
		nextMode = 1;
		return(64*1024/4*2.5);
	}

	else
	{
	        return(image.width()*image.height()/2);
	}
    }

    else // if 1
    {
        return(64*1024/4*2.5);  //64 KB header for jpg files - base64 encoding 3 to 4 bytes
    }
}

Item_Image Stegano::write(QByteArray& _data)
{
    unsigned char chooser = nextMode;
    srand((unsigned)time(NULL));
    Item_Image dataImage;
    string _image = getImageString();
    char separator = QDir::separator().toAscii();
    int i;
    string existCheck;
    FILE *fp;
    //srand((unsigned)clock()+noise[0]);

    for(i = 0; i < 32; i++)
    {
            noise[i] = (char) ((rand()+clock()+noise[i])%26)+97;
    }
    noise[32] = '\0';
    noise[((rand()+clock()+noise[rand()%32])%20)+12] = '\0';


    while(1)
    {

        existCheck = chooser == 1 ? (tempDir+separator+noise+getExtension(_image)) : (tempDir+separator+noise+".png");
        fp = fopen(existCheck.c_str(),"r");
        if( fp )
        {
            cerr << existCheck << " exists allready... this is quite unpropably but possible. No problem I'm generating a new image name." << endl;
            for(i = 0; i < 32; i++)
            {
                    noise[i] = (char) ((rand()*2+clock()+3*noise[i])%26)+97;
            }
            noise[32] = '\0';
            noise[((rand()+clock()+noise[rand()%32])%20)+12] = '\0';

            fclose(fp);
        }

        else
        {
            break;
        }
    }


    //chooser = nextMode; //rand()%2;
    //chooser = 0;

    if(chooser == 1) //needs linking against libexiv2-dev 0.182 or greater for PNG exiv2 support
    {
        //cout << "Debug Stegano _data.size(): " << _data.size() << " Base64: " << _data.toBase64().size()  << endl;
        copy( (imageDir+separator+_image).c_str(), (tempDir+separator+noise+getExtension(_image)).c_str() );
        dataImage.file = (tempDir+separator+noise+getExtension(_image)).c_str();
        dataImage.datalength = 0;
        dataImage.method = 0;

        //cout << (tempDir+separator+noise+_image).c_str() << endl;


        if(tagImage((tempDir+separator+noise+getExtension(_image)), _data.toBase64().constData()))
        {
            dataImage.file = (tempDir+separator+noise+getExtension(_image)).c_str();
            dataImage.datalength = (unsigned int) _data.size();
            dataImage.method = 1;
	    if(!QFile::exists(dataImage.file))
	    {
		cout << "Stegano Tag Image Path: " << dataImage.file.toStdString() << " does not exist" << endl;
	        dataImage.datalength = 0;
		dataImage.method = 0;
	    }

            return(dataImage);
        }
        else
        {
            dataImage.file = _image.c_str();
            dataImage.datalength = 0;
            dataImage.method = 0;

            return(dataImage);
        }
    }

    else //if (chooser == 2)
    {
        string binarySafeData(_data.constData(), _data.size());
        if(hasExtension(_image, ".jpg"))
        {
            //cerr << "Writing LSB for JPG is not Implemented, converting to PNG and write" << endl;
            //string tempConvImage = convertPNG(imageDir+separator+_image);
            //cout << "writeLsbPng("<<tempConvImage<<", " << (tempDir+separator+noise+".png") << ", " <<_data.constData() <<")" << endl;

            if(writeLsbPng((imageDir+separator+_image), (tempDir+separator+noise+".png"), binarySafeData));
            {
                dataImage.file = (tempDir+separator+noise+".png").c_str();
                dataImage.datalength = (unsigned int) _data.size();
                dataImage.method = 2;

		    if(!QFile::exists(dataImage.file))
		    {
			cout << "Stegano Write LSB with jpg2png conversion Path: " << dataImage.file.toStdString() << " does not exist" << endl;
		        dataImage.datalength = 0;
		        dataImage.method = 0;
		    }


                return(dataImage);
            }



            //return(HMD_MODE_LSB);
            dataImage.file = _image.c_str();
            dataImage.datalength = 0;
            dataImage.method = 0;

            return(dataImage);
        }

        else
        {
            //if(writeLsbPng(_image, data))
            //cout << "writeLsbPng("<<(imageDir+separator+_image)<<", " << (tempDir+separator+noise+_image) << ", " <<_data.constData() <<")" << endl;
            //cin >> chooser;
            //string tempConvImage = convertPNG(imageDir+separator+_image);

            //if(writeLsbPng((imageDir+separator+_image), (tempDir+separator+noise+_image), _data.constData()));
            if(writeLsbPng((imageDir+separator+_image), (tempDir+separator+noise+".png"), binarySafeData));
            {
                dataImage.file = (tempDir+separator+noise+".png").c_str();
                dataImage.datalength = (unsigned int) _data.size();
                dataImage.method = 2;

		    if(!QFile::exists(dataImage.file))
		    {
			cout << "Stegano WriteLSB Png Path: " << dataImage.file.toStdString() << " does not exist" << endl;
		        dataImage.datalength = 0;
		        dataImage.method = 0;
		    }

                return(dataImage);
            }
        }
    }

    dataImage.file = _image.c_str();
    dataImage.datalength = 0;
    dataImage.method = 0;

    return(dataImage);
}

bool Stegano::writeLsbPng(const string& _srcImage, const  string& _destImage, const string& _buffer)
{
    QImage tempImage(QString::fromStdString(_srcImage));
    QImage writeImage = tempImage.convertToFormat(QImage::Format_RGB32, Qt::AutoColor);
    QRgb data;
    unsigned int width = writeImage.width();
    unsigned int height = writeImage.height();
    unsigned int a,b;
	unsigned char* buffer = (unsigned char*)_buffer.data();
    int rtemp, gtemp, btemp;
    QRgb color;
	char* debug;
	size_t length = _buffer.size();
	QColor getParts(data);
	unsigned char temp;

	/*cout << "Test Output" << endl;
	printQImage(tempImage);
	exit(0);*/

    if(width*height/2 < length)
    {
	cout << "Data " << length << " is greater length " << width*height/2 << endl;
        return(false);
    }

    for(a = 0; a < height; a++)
    {
        for(b = 0; b < width; b++)
        {
            //when all data is written in the pixel it will aboard.
            if((a*width+b)/2 >= length)
            {
   				//cout << "Byte: " << (a*width+b)/2 << " of " << length << " / Abort writeLSB: " << _buffer << endl;
                writeImage.save(_destImage.c_str());
                return(true);
            }

            if((a*width+b)%2 == 0)
            {
                    data = writeImage.pixel(b,a);
                    getParts.setRgb(data);
                    rtemp = getParts.red();

                    rtemp = rtemp>>2;
                    rtemp = rtemp<<2;
                    rtemp = rtemp + (buffer[(a*width+b)/2]>>6);

                    gtemp = getParts.green();
                    gtemp = gtemp>>1;
                    gtemp = gtemp<<1;

                    temp = (buffer[(a*width+b)/2] << 2);
                    temp = temp >> 7;

                    gtemp = gtemp + temp;
                    btemp = getParts.blue();
                    //btemp = btemp == 255 ? 254 : btemp;
                    btemp = btemp >>1;
                    btemp = btemp <<1;

                    temp = buffer[(a*width+b)/2] << 3;
                    temp = temp >> 7;

                    btemp = btemp + temp;
                    color = qRgb(rtemp, gtemp, btemp);

                    writeImage.setPixel(b, a, color);
            }

            else
            {
                    data = writeImage.pixel(b,a);
                    getParts.setRgb(data);
                    rtemp = getParts.red();
                    rtemp = rtemp>>2;
                    rtemp = rtemp<<2;

                    temp = buffer[(a*width+b)/2]<<4;
                    temp = temp>>6;

                    rtemp = rtemp + temp;

                    gtemp = getParts.green();
                    gtemp = gtemp>>1;
                    gtemp = gtemp<<1;

                    temp = buffer[(a*width+b)/2] >> 1;
                    temp = temp << 7;
                    temp = temp >> 7;

                    gtemp = gtemp + temp;

                    btemp = getParts.blue();
                    //btemp = btemp == 255 ? 254 : btemp;
                    btemp = btemp >>1;
                    btemp = btemp <<1;

                    temp = buffer[(a*width+b)/2] << 7;
                    temp = temp >> 7;

                    btemp = btemp + temp;
                    color = qRgb(rtemp, gtemp, btemp);

                    writeImage.setPixel(b, a, color);
            }
        }
    }
	writeImage.save(_destImage.c_str());
	return(true);
}

bool Stegano::readLSB(const string& _image, string& _buffer, unsigned int _length)
{
	unsigned char temp = 0;
	int red, blue,green;
	QImage tempImage(QString::fromStdString(_image));
    	QImage readImage = tempImage.convertToFormat(QImage::Format_RGB32, Qt::AutoColor);

	QColor data;
	unsigned int rawData;
	unsigned int width = readImage.width();
	unsigned int height = readImage.height();
	unsigned int a, b;
	unsigned int length = _length;

    //cout << "Empty Buffer in readLSB: " << _buffer << endl;
	for(a = 0; a < height; a++)
	{
		for(b = 0; b < width; b++)
		{
			if((a*width+b)/2 >= length)
			{
				//cout << "Byte: " << (a*width+b)/2 << " of " << length << " / Abort readLSB: " << _buffer << endl;
				return(true);
			}

			rawData = readImage.pixel(b,a);
			data.setRgb(rawData);

			red = data.red()%4;
			green = data.green()%2;
			blue = data.blue()%2;

			temp = red<<6;
			temp += green<<5;
			temp += blue<<4;

			if((b+1) < width)
			{
				b++;
			}
			else
			{
				a++;
				b = 0;

			}
			rawData = readImage.pixel(b,a);
			data.setRgb(rawData);

			red = data.red()%4;
			green = data.green()%2;
			blue = data.blue()%2;

			temp +=	red<<2;
			temp += green<<1;
			temp += blue;

			_buffer += (char) temp;
		}
	}
	//cout << "DEBUG readLSB: " << _buffer << endl;
	return(true);
}

string Stegano::convertPNG(const string& _image)
{
    char separator = QDir::separator().toAscii();
    char noise[15];
    int i;
	QImage tempImage(QString::fromStdString(_image));
   	QImage readImage = tempImage.convertToFormat(QImage::Format_RGB888, Qt::ColorOnly);

    for(i = 0; i < 14; i++)
    {
        noise[i] = (char) (rand()%26)+97;
    }
    noise[14] = '\0';

	readImage.save( (tempDir+separator+noise+".png").c_str(), "PNG");
	return(tempDir+separator+noise+".png");
}

string Stegano::getExtension(string& _image)
{
    int j = _image.length()-1;

    while(j > -1)
    {
        if(_image[j] == '.')
        {
            break;
        }
        j--;
    }

    if(j>-1)
    {
        return(_image.substr(j, _image.length()-j));
    }

    else
    {
        return("");
    }
}
