#ifndef PROCESS_H_INCLUDED
#define PROCESS_H_INCLUDED

#include <QThread>

#include "globals.h"

/** Process States **/
enum ProcessState {
	QUEUED,
	RUNNING,
	CANCELLED,
	ERROR,
	COMPLETED
};

/** This class contains all basic process functions. **/
class Process : public QThread
{
		Q_OBJECT

	public:
		Process();
		virtual ~Process();

		QString getType();
		QString getResult();
		QString getJobName();
		QString getFileName();
		QString getStateString();
		quint32 getProgress();
		ProcessState getState();

		void setUserKey(const QString& _userKey);
		QString getUserKey();

	public slots:
		//! Run the Process thread
		virtual void run() {}
		//! Abort Process
		void cancel();

	signals:
		//! Signal that the current password is invalid for decryption
		void invalidUserKey(Process* _process);
		//! Signal a progress update
		void progressChanged(const qint32& _progress);
		//! Signal a state update
		void stateStringChanged(const QString& _stateString);
		//! Display result
		void displayResult(const QString& _url, Process* _process);

	protected:
		//! Initialize Process
		void init();
		//! Custom initializations
		virtual void init_custom() {}

		//! Clear temporary files and left-overs
		void clear();
		//! Custom clearing
		virtual void clear_custom() {}

		//! Update the Process's name
		void setJobName(const QString& _jobName);
		//! Update Status and emit a signal
		void setStateString(const QString& _stateString);
		//! Update Progress and emit a signal
		void setProgress(const qint32& _progress);

		QString type;				//!< Type of process (Upload, Download, ...)
		QString file;				//!< Path and name of currently processed file

		QDir tempDir;				//!< Path to create working directory in
		QString tempFolder;			//!< Name of created working directory
		QVector<QString> tempFiles;	//!< Temporary files created by the process

		bool userKeyChanged;		//!< userKey has changed

		ProcessState state;			//!< Simplified Prcmakeocess State
		QString result; 			//!< Result of the Process

		QString userKey;			//!< User password (used for encryption)


	private:
		QString jobName;			//!< Job Name (used in GUI)
		QString stateString;		//!< String describing current state (used in GUI)
		quint32 progress;			//!< Progress in percent [0, 100] (used in GUI)
};

#endif
