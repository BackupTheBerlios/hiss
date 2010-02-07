#ifndef PROCESS_DOWNLOAD_H_INCLUDED
#define PROCESS_DOWNLOAD_H_INCLUDED

#include "globals.h"
#include "Process.h"

#include "Crypto.h"
#include "MetaHandler.h"
#include "Stegano.h"
#include "Transmission.h"

/** Downloads and assembles a file from information hidden in distributed images. **/
class Process_Download : public Process
{
		Q_OBJECT

	public:
		/** Constructor (Download)
		 * 
		 *  @param _jobName Name of this job
		 *  @param _file Path and name of the metaFile (image, xml, url)
		 *  @param _outputDir Output destination directory
		 *  @param _tempDir Directory in which to create the working directory
		 *  @param _userKey User password (used for decryption)
		 */
		Process_Download(const QString& _jobName, const QString& _file, const QDir& _ouputDir, const QDir& _tempDir, const QString& _userKey);
		virtual ~Process_Download() {}

		//! Runs the actual Process
		void run();

	private:
		MetaHandler metaHandler;	//!< Maintains XML MetaFile

		QDir outputDir;				//!< Output destination directory
		QFile outputFile;			//!< Output file (Hidden file)

		bool metaFileIsValid;		//!< MetaFile is valid

		//! Handles download and decryption based on information from a given metaFile
		//! @param _file MetaFile that is to be processed
		void pipeline(const QString& _metaFile);
};

#endif
