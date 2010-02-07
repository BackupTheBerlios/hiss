#include "Chunker.h"
#include <QtGlobal>

Chunker::Chunker(const QString& _file, const quint8 _blocksize)
{
	file.setFileName(_file);
	file.open(QIODevice::ReadOnly);
	blocksize = _blocksize;
}

Chunker::~Chunker()
{
	file.close();
}

bool Chunker::hasNext()
{
	char tempArray[1];
	// looks at next byte: returns 0 if end of file
	return file.peek(tempArray,1);
}

QByteArray Chunker::getNext(const quint64& _availSize)
{
	QByteArray chunk;
	if(hasNext()) {
		quint64 bytesToChunk = file.size() - file.pos(); //calculate the remaining bytes

		alignRest = 0; //reset the padding size

		//cases here:
		//1. more data to chunk left than would fit in the image -> if
		//2. as much as or less data(+align bytes) to chunk left than fit in the image-> else
		if (_availSize < (alignRest+bytesToChunk))
		{
			quint8 bytesOverBlocksize = _availSize % blocksize;
			chunk = file.read(_availSize - bytesOverBlocksize); //to blocksize aligned size

			qDebug() << "Chunker: file larger than avail space, read aligned bytes with size " << _availSize - bytesOverBlocksize;
		}
		else
		{
			chunk = file.read(_availSize);
			if ((bytesToChunk % blocksize) != 0)
				alignRest = blocksize - (bytesToChunk % blocksize);

			//add some bytes to match the blocksize of the crypto
			for(quint8 ii=alignRest;ii > 0;ii--)
				chunk.append("0");

			qDebug() << "Chunker: Padding" << alignRest << " bytes, resulting chunk is " << chunk.length() << " bytes";
		}
	}
	return chunk;
}

quint8 Chunker::getPaddedBytes()
{
	return(alignRest);
}
