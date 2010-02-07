#ifndef PROCESS_UPLOAD_H_INCLUDED
#define PROCESS_UPLOAD_H_INCLUDED

#include "globals.h"
#include "Process.h"

#include "Chunker.h"
#include "Crypto.h"
#include "MetaHandler.h"
#include "Stegano.h"
#include "Transmission.h"

/** Divides a file, hides the parts (Chunks) in images.
 *  Distributes these images on randomly selected remote servers.
 *  Generates an XML file (metaFile) containing all information necessary for
 *  the retrieval of such hidden files.
 *  Hides that XML in another image and uploads it to a remote server.
 **/
class Process_Upload : public Process
{
		Q_OBJECT

	public:
		/** Constructor (Upload)
		 * 
		 *  @param _jobName Name of this job
		 *  @param _file Path and name of the file that is to be hidden
		 *  @param _outputDir Output destination directory
		 *  @param _imageDir Image collection directory
		 *  @param _tempDir Working directory
		 *  @param _userKey User password (used for encryption)
		 */
		Process_Upload(const QString& _jobName, const QString& _file, const QDir& _imageDir, const QDir& _tempDir, const QString& _userKey);
		virtual ~Process_Upload() {}

		//! Runs the actual Process
		void run();
	
	private:
		MetaHandler metaHandler;	//!< Maintains XML MetaFile

		QDir imageDir;				//!< Image collection directory
		QString metaFileName;		//!< Name of generated MetaFile

		//! Handles chunking, encryption and upload of a given file
		//! @param _file File that is to be processed
		void pipeline(const QString& _file);
};

#endif

