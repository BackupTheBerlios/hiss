#ifndef PROCESSLISTMODEL_H
#define PROCESSLISTMODEL_H

#include <QAbstractListModel>
#include <QList>
#include <QStringList>
#include <QVariant>

#include "globals.h"
#include "Process.h"

/*! gui process list backend */
class ProcessListModel : public QAbstractListModel
{
		Q_OBJECT
	public:
		/*! ProcessListModel constructor */
		ProcessListModel(QObject *parent);
		~ProcessListModel();
		
		enum Columns {Type, Name, State, Progress};
		
		//list managing
		/*! returns property flags of widget*/
		virtual Qt::ItemFlags flags(const QModelIndex &_index) const;
		/*! returns number of rows */
		virtual int rowCount(const QModelIndex &) const;
		/*! returns number of columns */
		virtual int columnCount(const QModelIndex &) const;
		/*! returns data under given index */
		virtual QVariant data(const QModelIndex &_index, int _role) const;
		/*! returns the data for the given role and section in the header with the specified orientation*/
		virtual QVariant headerData(int section, Qt::Orientation _orientation, int _role) const;
		
		
		//process managing
		/*! adds a process */
		void addProcess(Process* _process);
		/*! removes a process */
		void removeProcess(int _processNr);
		/*! starts process */
		void startProcess(int _processNr);
		/*! cancels a process */
		void cancelProcess(int _processNr);

		/*! returns size of the process list */
		int getListSize();
		/*! returns size of the columns */
		int getHeaderSize();
		Process* getProcess(int _processNr);
		int getCompletedCount();
		
		//for preventing segfaults due to python
		void raiseRunningProcessesCount();
		void lowerRunningProcessesCount();
		int getRunningProcessesCount();

	private:
		/*! list containing the processes */
		QList<Process*> processList;
		
		/*! contains the column names */
		QStringList headers;
		
		/*! saves the column index where the progress bar will be placed */
		quint32 progressBarColumn;
		
		/*! saves the count of running processes */
		quint32 runningProcesses;
};
#endif