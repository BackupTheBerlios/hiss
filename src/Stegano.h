#ifndef STEGANO_H_INCLUDED
#define STEGANO_H_INCLUDED

#include <QDir>
#include <QString>
#include <QtGui/QImage>
#include <QtGui/QColor>
#include <QFile>
#include <list>
#include <time.h>
#include <sys/stat.h>
#include <iostream>
//For Exiv2
#include <exiv2/image.hpp>
#include <exiv2/exif.hpp>
//#include <stdio.h>

#include "globals.h"
//#include "steghide/libfunctions.h"

#define HMD_MODE_EXIF = 1
#define HMD_MODE_LSB = 2
#define HMD_ERROR = 0

using namespace std;

class Stegano
{
	public:
	Stegano(string& _imageDir, string& _tempDir);
	Stegano(const QDir& _imageDir,const QDir& _tempDir);



	string getImageString();
	QString getImage();

	qint64 getCapacity(QString& _image);		 // _image = ""
	qint64 getCapacity();
	string& getCurrentImageString();

   	Item_Image write(QByteArray& _data);


	//bool read(string& _image, string& buffer, unsigned int length, unsigned int mode); // später ggf. "uint method"
	QByteArray read(Item_Image _image);

	ostream& display(ostream& given);

	void shuffleImages();
	void readTag(const string& name, string& buffer);
	bool tagImage(const string& name, const string& data);
	bool copy(const char* source, const char* destination);

	bool writeLsbPng(const string& _srcImage, const string& _destImage, const string& _buffer);
	bool readLSB(const string& _image, string& _buffer, unsigned int _length);
	string convertPNG(const string& _image);
	string getExtension(string& _image);
	unsigned int getNumberMethods();
	QByteArray getMetaFileFromImage(QString& _file, unsigned int _method);

	protected:
	unsigned int curImage;
	vector<string*> images;
	string imageDir;
	string tempDir;
	QDir qImageDir;
	QDir qTempDir;
	char noise[32];
	unsigned char nextMode;

	void readImages();
	bool isImage(const string& _name);
	bool hasExtension(const string& _fileName, const  string& _extension);
};

ostream& operator<<(ostream& given, Stegano& givenSteg);
#endif
