#include "ProcessListModel.h"

ProcessListModel::ProcessListModel(QObject *parent)
		: QAbstractListModel(parent),
		progressBarColumn(Progress),
		runningProcesses(0)
{
	headers << tr("Type") << tr("Name") << tr("State") << tr("Progress");
}

ProcessListModel::~ProcessListModel()
{
}

Qt::ItemFlags ProcessListModel::flags(const QModelIndex &_index) const
{
	return QAbstractItemModel::flags(_index);
}

int ProcessListModel::rowCount(const QModelIndex &) const
{
	return processList.size();
}

int ProcessListModel::columnCount(const QModelIndex &) const
{
	return headers.size();
}

QVariant ProcessListModel::data(const QModelIndex &_index, int _role) const
{
	if (!_index.isValid() || _index.row() >= processList.size())
		return QVariant();

	int rowNr = _index.row();

	int colNr = _index.column();

	Process* import = processList.at(rowNr);

	if (_role == Qt::DecorationRole)
		return QVariant();

	if (_role == Qt::BackgroundColorRole)
		return QVariant();

	if (_role != Qt::DisplayRole)
		return QVariant();

	switch (colNr)
	{
		case Type:
			return import->getType();
		case Name:
			return import->getJobName();
		case State:
			return import->getStateString();
		case Progress:
			return import->getProgress();
		default:
			return QVariant();
	}
}

QVariant ProcessListModel::headerData(int _section, Qt::Orientation _orientation, int _role) const
{
	if (_role != Qt::DisplayRole)
		return QVariant();

	if (_section < 0 || _section >= headers.size())
		return QVariant();

	return headers.at(_section);
}

int ProcessListModel::getListSize()
{
	return processList.size();
}

int ProcessListModel::getHeaderSize()
{
	return headers.size();
}

Process* ProcessListModel::getProcess(int _processNr)
{
	return processList.at(_processNr);
}

int ProcessListModel::getCompletedCount()
{
	int count = 0;
	for(int i = 0; i < processList.size(); i++)
	{
		if(processList.at(i)->getState() == COMPLETED)
			count++;
	}
	
	return count;
}

void ProcessListModel::raiseRunningProcessesCount()
{
	runningProcesses++;
}

void ProcessListModel::lowerRunningProcessesCount()
{
	runningProcesses--;
}

int ProcessListModel::getRunningProcessesCount()
{
	return runningProcesses;
}

void ProcessListModel::cancelProcess(int _processNr)
{
	if(processList.at(_processNr)->getState() == RUNNING )
	{
		qDebug() << "ProcessListModel: Cancel Process " << _processNr;
		processList.at(_processNr)->cancel();
	}
}

void ProcessListModel::removeProcess(int _processNr)
{
	qDebug() <<"ProcessListModel: Remove Process " << _processNr;
	cancelProcess(_processNr);
	processList.at(_processNr)->disconnect();
	processList.at(_processNr)->wait();
	delete processList.at(_processNr);
	processList.removeAt(_processNr);
	emit layoutChanged();
}

void ProcessListModel::startProcess(int _processNr)
{
	if(getRunningProcessesCount() == 0)
		if(processList.at(_processNr)->getState() == QUEUED)
		{
			qDebug() <<"ProcessListModel: Start Process " << _processNr;
			processList.at(_processNr)->start();
			raiseRunningProcessesCount();
		}
}

void ProcessListModel::addProcess(Process* _process)
{
	qDebug() <<"ProcessListModel: Add Process " << _process->getJobName();
	processList.append(_process);
	emit layoutChanged();
}

#include "ProcessListModel.moc"
