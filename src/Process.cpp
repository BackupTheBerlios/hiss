#include "Process.h"

Process::Process()
{
	// Process properties
	setJobName("Untitled");

	// File properties
	type = "";
	file = "";
	userKey = "";
	userKeyChanged = false;
	tempFolder = "";

	// State properties
	state = QUEUED;
	result = "";
	stateString = "";
	setProgress(0);
}

Process::~Process()
{
	// Remove temporary files
	clear();
}




QString Process::getType()
{
	return type;
}

QString Process::getResult()
{
	return result;
}

QString Process::getJobName()
{
	return jobName;
}

QString Process::getFileName()
{
	return file.section('/', -1);
}

QString Process::getStateString()
{
	return stateString;
}

quint32 Process::getProgress()
{
	return progress;
}

ProcessState Process::getState()
{
	return state;
}




void Process::setUserKey(const QString& _userKey)
{
	userKey = _userKey;
	userKeyChanged = true;
}

QString Process::getUserKey()
{
	return userKey;
}




void Process::cancel()
{
	setStateString(tr("Cancelling..."));
	state = CANCELLED;
}




void Process::init()
{
	// Reset progress
	setStateString("Initializing");
	setProgress(0);

	// Create a temporary subfolder to store all working files
	tempFolder = getJobName() + "_tmp";
	tempDir.mkdir(tempFolder);
	tempDir.cd(tempFolder);

	// Allow for custom initializations
	init_custom();

	// Begin running
	state = RUNNING;
}




void Process::clear()
{
	// Remove temporary files
	for(qint32 i = 0; i < tempFiles.size(); i++)
		tempDir.remove(tempFiles.at(i));
	tempFiles.clear();

	// Move up one directory...
	tempDir.cdUp();
	// ...remove temporary working folder
	tempDir.rmdir(tempFolder);
}




void Process::setJobName(const QString& _jobName)
{
	if(!_jobName.isEmpty())
		jobName = _jobName;
	else
		jobName = getFileName();
}

void Process::setStateString(const QString& _stateString)
{
	if(stateString != _stateString)
	{
		stateString = _stateString;
		//qDebug() << jobName << "->" << stateString;
		emit stateStringChanged(stateString);
	}
}

void Process::setProgress(const qint32& _progress)
{
	if(progress != _progress)
	{
		progress = _progress;

		// Catch possible errors
		if(progress < 0)
			progress = 0;
		if(progress > 100)
			progress = 100;

		// Emit progress change
		emit progressChanged(progress);
	}
}

#include "Process.moc"

