#ifndef PROCESSLISTVIEW_H
#define PROCESSLISTVIEW_H

#include <QTreeView>
#include <QSortFilterProxyModel>
#include <QAction>
#include <QDir>
#include <QDropEvent>
#include <QTreeWidgetItem>
#include <QSettings>

#include "Process.h"
#include "ProcessListModel.h"
#include "ProcessListDelegate.h"

class ProcessListView : public QTreeView
{
		Q_OBJECT
	public:
		ProcessListView(QWidget *parent);
		~ProcessListView();
		enum Colums {Type, Name, State, Progress};
		
		int getProcessCount();
		int getCompletedCount();
		
		void setSettings(QSettings* _settings);
		
	public slots:

		/*! creates a new download process */
		void createDownloadProcess(QString _metaFile, QDir _destination, QDir _tempDir, QString _password, QString _jobName);
		/*! creates new upload process */
		void createUploadProcess(QString _file, QDir _imageDir, QDir _tempDir, QString _password, QString _jobName);

		void startProcess(int _processNr);
		void removeProcess(int _processNr);
		void cancelProcess(int _processNr);
		void startAllProcesses();
		void removeAllProcesses();
		void startNextProcess();
		void removeCompletedProcesses();
		/*! removes all selected processes */
		void remove();
		/*! starts all selected processes */
		void start();
		/*! cancels all selected processes */
		void cancel();
		
		/*! debug function to display the current selected entry in the list */
		void displaySelected(QModelIndex _index);
		
		void setQueue(bool _enable);
		
	private slots:
		/*! shows dialog for a new password */
		void showInvalidPWDialog(Process* _process);
		/*! shows result emmited from process */
		void showResult(QString _url, Process* _process);
		/*! shows result when double clicking on process */
		void showResult(QModelIndex _index);
		/*! shows result by context menu */
		void showResult();
		/*! shows tooltip with information from process when single clicking on process */
		void showProcessTooltip(QModelIndex _index);
		void showContextMenu(const QPoint &_position);
		void updateView();
		
		void startNextInQueue();
	private:
		//Drag & Drop
		QStringList mimeTypes() const;
		Qt::DropActions supportedDropActions () const;
		void dragEnterEvent(QDragEnterEvent *_event);
		void dragMoveEvent(QDragMoveEvent *_event);
		void dropEvent(QDropEvent *_event);
		
		ProcessListDelegate *delegate;
		ProcessListModel *model;
		QSortFilterProxyModel *proxyModel; 
		
		bool queueMode;
		
		QAction *actionRemove;
		QAction *actionStart;
		QAction *actionCancel;
		QAction *actionResult;
		
		QSettings *settings;
		
	signals:
		void createUpload(QString _filename);
		void createDownload(QString _metafile);
};

#endif