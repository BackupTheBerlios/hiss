#ifndef CHUNKER_H_INCLUDED
#define CHUNKER_H_INCLUDED

#include "globals.h"
#include <QFile>

class Chunker
{
	public:
		/** Constructor (Chunker)
		 * 
		 *  @param _file path to the file that is to be chunked
		 *  @param _blocksize blocksize that chunks have to adhere to
		 */
		Chunker(const QString& _file, const quint8 _blocksize);
		virtual ~Chunker();

		/**
		* @return true if there is an unprocessed chunk left
		*/
		bool hasNext();
		
		/** 
		* @param _size size of the data which is requested
		* @return a QByteArray with the wanted data
		*/
		QByteArray getNext(const quint64& _size);
		
		/** 
		* @return amount of padded bytes for the current chunk
		*/
		quint8 getPaddedBytes();

	private:
		QFile file;
		quint8 blocksize;
		quint8 alignRest;
};

#endif
