#include "ProcessListView.h"
#include <QModelIndex>
#include <QDebug>
#include <QMenu>
#include <QToolTip>
#include <QInputDialog>

#include "Process_Download.h"
#include "Process_Upload.h"
#include "DialogResult.h"

ProcessListView::ProcessListView(QWidget *parent) : QTreeView(parent)
{
	queueMode = false;
	
	//Model
	model = new ProcessListModel(this);
	proxyModel = new QSortFilterProxyModel();
	proxyModel->setDynamicSortFilter(true);
	proxyModel->setSourceModel(model);
	proxyModel->setFilterKeyColumn(Name);
	proxyModel->setFilterRole(Qt::DisplayRole);
	setModel(proxyModel);

	delegate = new ProcessListDelegate(this, model);
	setItemDelegate(delegate);

	//View Settings
	setRootIsDecorated(false);
	setAllColumnsShowFocus(true);
	setSortingEnabled(false);
	setSelectionMode(QAbstractItemView::SingleSelection);
	setItemsExpandable(false);
	setAutoScroll(true);
	setContextMenuPolicy(Qt::CustomContextMenu);
	setAcceptDrops(true);
// 	setDragDropOverwriteMode(true);
	setDropIndicatorShown(true);
// 	setDragEnabled(true);
	setDragEnabled(true);
// 	setDragDropMode(QAbstractItemView::DragDrop);


	//Actions
	actionRemove = new QAction(this);
	actionRemove->setText(tr("&Remove"));
	actionStart = new QAction(this);
	actionStart->setText(tr("&Start"));
	actionCancel = new QAction(this);
	actionCancel->setText(tr("&Cancel"));
	actionResult = new QAction(this);
	actionResult->setText(tr("show R&esult"));

	//Connections
	//context menu
	connect(this, SIGNAL(customContextMenuRequested(QPoint)),
		  this, SLOT(showContextMenu(QPoint)));

	//start selected process
	connect(actionStart, SIGNAL(triggered(bool)),
		  this, SLOT(start()));

	//remove selected process
	connect(actionRemove, SIGNAL(triggered(bool)),
		  this, SLOT(remove()));
		
	connect(actionCancel, SIGNAL(triggered(bool)),
		this, SLOT(cancel()));
		
	connect(actionResult, SIGNAL(triggered(bool)),
		this, SLOT(showResult()));
		
	connect(this, SIGNAL(clicked(QModelIndex)),
		  this, SLOT(showProcessTooltip(QModelIndex)));
		  
	connect(this, SIGNAL(doubleClicked(QModelIndex)),
		this, SLOT(showResult(QModelIndex)));
		
	connect(this, SIGNAL(clicked(QModelIndex)),
		this, SLOT(displaySelected(QModelIndex)));
}

ProcessListView::~ProcessListView()
{

}

int ProcessListView::getProcessCount()
{
	return model->getListSize();
}

int ProcessListView::getCompletedCount()
{
	return model->getCompletedCount();
}

void ProcessListView::setSettings(QSettings* _settings)
{
	settings = _settings;
}

void ProcessListView::createDownloadProcess(QString _metaFile, QDir _destination, QDir _tempDir, QString _password, QString _jobName)
{
	qDebug() << "ProcessListView: Create download process";
	Process *process = new Process_Download(_jobName, _metaFile, _destination, _tempDir, _password);
	model->addProcess(process);

	//updates view for every progress change
	connect(process, SIGNAL(progressChanged(qint32)),
		  this, SLOT(updateView()));
	connect(process, SIGNAL(stateStringChanged(QString)),
		  this, SLOT(updateView()));
	connect(process, SIGNAL(invalidUserKey(Process*)),
		  this, SLOT(showInvalidPWDialog(Process*)));
		  
	connect(process, SIGNAL(displayResult(QString,Process*)),
		this, SLOT(showResult(QString,Process*)));
		
	connect(process, SIGNAL(finished()),
		this, SLOT(startNextInQueue()));
		
}

void ProcessListView::createUploadProcess(QString _file, QDir _imageDir, QDir _tempDir, QString _password, QString _jobName)
{
	qDebug() << "ProcessListView: Create upload process";
	Process *process = new Process_Upload(_jobName, _file, _imageDir, _tempDir, _password);
	model->addProcess(process);

	//updates view for every progress change
	connect(process, SIGNAL(progressChanged(qint32)),
		  this, SLOT(updateView()));
	connect(process, SIGNAL(stateStringChanged(QString)),
		  this, SLOT(updateView()));
	connect(process, SIGNAL(invalidUserKey(Process*)),
		  this, SLOT(showInvalidPWDialog(Process*)));
		  
	connect(process, SIGNAL(displayResult(QString,Process*)),
		this, SLOT(showResult(QString,Process*)));
		
	connect(process, SIGNAL(finished()),
		this, SLOT(startNextInQueue()));
}


void ProcessListView::startProcess(int _processNr)
{
	model->startProcess(_processNr);
}

void ProcessListView::removeProcess(int _processNr)
{
	model->removeProcess(_processNr);
}

void ProcessListView::cancelProcess(int _processNr)
{
	model->cancelProcess(_processNr);
}

void ProcessListView::startAllProcesses()
{
	for (int i = 0; i < model->getListSize(); i++)
	{
		model->startProcess(i);
	}
}

void ProcessListView::removeAllProcesses()
{
	while (model->getListSize() > 0)
		model->removeProcess(0);
}

void ProcessListView::startNextProcess()
{
	if(queueMode == true)
	{
		int i = 0;
		while( i < model->getListSize())
		{
			if(model->getProcess(i)->getState() == QUEUED)
			{
				qDebug() << "ProcessListView: Start next process " << i;
				model->startProcess(i);
				break;
			}
			i++;
		}
	}
}

void ProcessListView::removeCompletedProcesses()
{
	for (int i = model->getListSize() - 1; i >= 0; i--)
		if (model->getProcess(i)->getState() == COMPLETED)
			removeProcess(i);
}

void ProcessListView::remove()
{
	qDebug() << "ProcessListView: Action Remove";
	QModelIndexList indexlist = this->selectedIndexes();

	for (int i = indexlist.size() - 1 ; i >= 0 ; i -= model->getHeaderSize())
	{
		model->removeProcess(indexlist.at(i).row());
	}
}

void ProcessListView::start()
{
	qDebug() << "ProcessListView: Action Start";
	QModelIndexList indexlist = this->selectedIndexes();
	queueMode = false;
	for (int i = indexlist.size() - 1; i >= 0; i -= model->getHeaderSize())
	{
		model->startProcess(indexlist.at(i).row());
	}
}

void ProcessListView::cancel()
{
	qDebug() << "ProcessListView: Action Cancel";
	QModelIndexList indexlist = this->selectedIndexes();
	queueMode = false;
	for (int i = indexlist.size() - 1; i >= 0; i -= model->getHeaderSize())
	{
		model->cancelProcess(indexlist.at(i).row());
	}
}

void ProcessListView::displaySelected(QModelIndex _index)
{
	for (int i = 0; i < selectedIndexes().size(); i += model->getHeaderSize())
		qDebug() << "ProcessListView: List Indexes: " << selectedIndexes().at(i).row();

	qDebug() << "ProcessListView: finished? " << model->getProcess(_index.row())->isFinished();
	qDebug() << "ProcessListView: Current Index: " << _index.row();
}

void ProcessListView::setQueue(bool _enable)
{
	qDebug() << "ProcessListView: Queue Mode = " << _enable;
	queueMode = _enable;
}

void ProcessListView::showContextMenu(const QPoint &_position)
{
	if (indexAt(_position).isValid())
	{
		QList<QAction*> actions;
		switch(model->getProcess(selectedIndexes().at(0).row())->getState())
		{
			case QUEUED:
				actions.append(actionStart);
				actions.append(actionRemove);
				break;
				
			case RUNNING:
				actions.append(actionCancel);
				actions.append(actionRemove);
				break;
				
			case COMPLETED:
				actions.append(actionRemove);
				actions.append(actionResult);
				break;
				
			case CANCELLED:
				actions.append(actionRemove);
				break;
			default:
				actions.append(actionCancel);
		}
		if (actions.count() > 0)
			QMenu::exec(actions, mapToGlobal(_position));
	}
}

void ProcessListView::showInvalidPWDialog(Process* _process)
{
	qDebug() << "ProcessListView: Invalid Password";
	bool OK;
	QString password = QInputDialog::getText(this,
				 tr("Invalid Password"),
				 tr("Enter new Password for ").append(_process->getJobName()),
				 QLineEdit::Password,
				 (QString) "",
				 &OK);

	if (OK)
	{
		qDebug() << "ProcessListView: Set new Password for " <<  _process->getJobName();
		_process->setUserKey(password);
	}
	else
	{
		_process->cancel();
		qDebug() << "ProcessListView: No new Password for " << _process->getJobName();
	}
}

void ProcessListView::showResult(QString _url, Process* _process)
{
	bool hide = settings->value("Default/hide_ResultDialog", false).value<bool>();
	qDebug() << "ProcessListView: hide Result: " << hide;
	if(hide == false)
	{
		qDebug()<< "ProcessListView: Show Result";
		DialogResult result(this, _url, _process);
		result.show();
		result.exec();
	}
}

void ProcessListView::showResult(QModelIndex _index)
{
	if(!model->getProcess(_index.row())->getResult().isEmpty())
	{
		qDebug()<< "ProcessListView: Show Result";
		DialogResult result(this, model->getProcess(_index.row())->getResult(), model->getProcess(_index.row()));
		result.show();
		result.exec();
	}
}

void ProcessListView::showResult()
{
	qDebug() << "ProcessListView: Action ShowResult";
	QModelIndexList indexlist = this->selectedIndexes();
	
	if(indexlist.size() >= 1 & indexlist.at(0).isValid())
	showResult(indexlist.at(0));
	
}

void ProcessListView::showProcessTooltip(QModelIndex _index)
{
	QString jobtype = model->getProcess(_index.row())->getType();
	QString jobname = model->getProcess(_index.row())->getJobName();
	QString jobstate = model->getProcess(_index.row())->getStateString();
	QString jobfile = model->getProcess(_index.row())->getFileName();
	QString jobprogress = QString().setNum(model->getProcess(_index.row())->getProgress());
	QString jobresult = model->getProcess(_index.row())->getResult();
	
	QString text = "<table cellspacing=0 cellpadding=0>";
	text += "<tr><td align=center colspan=2><nobr><b>";
	text += jobname;
	text += "</b></nobr></td></tr>";

	text += "<tr><td><nobr>";
	text += tr("Type:");
	text += "</nobr></td><td><nobr>";
	text += jobtype + "</td></tr>";

	text += "<tr><td><nobr>";
	text += tr("State:");
	text += "</nobr></td><td><nobr>";
	text +=  jobstate + "</td></tr>";

	text += "<tr><td><nobr>";
	text += tr("File:");
	text += "</nobr></td><td><nobr>";
	text += jobfile + "</td></tr>";
	
	text += "<tr><td><nobr>";
	text += tr("Progress:");
	text += "</nobr></td><td><nobr>";
	text += jobprogress + "</td></tr>";
	
	if(jobtype == "Upload" & !jobresult.isEmpty())
	{
		text += "<tr><td><nobr>";
		text += tr("Result:");
		text += "</nobr></td><td><nobr>";
		text += jobresult + "</td></tr>";
	}
	
	text += "</table>";
	QToolTip::showText(QCursor::pos(), text);
}

void ProcessListView::updateView()
{
	reset();
}

void ProcessListView::startNextInQueue()
{
	model->lowerRunningProcessesCount();
	if(queueMode == true)
	{
		startNextProcess();
	}
}

QStringList ProcessListView::mimeTypes() const
{
// 	qDebug() << "ProcessListView: mimeTypes";
	QStringList mimeList;
	mimeList.append("text/uri-list");
	return mimeList;
}

Qt::DropActions ProcessListView::supportedDropActions() const
{
// 	qDebug() << "ProcessListView: supportedDropActions";
	return Qt::CopyAction | Qt::MoveAction;
}

void ProcessListView::dragEnterEvent(QDragEnterEvent *_event)
{
	QString type;
	qDebug() << "ProcessListView: dragEnterEvent";
	foreach(type, mimeTypes())
	{
		if(_event->mimeData()->hasFormat(type))
			_event->accept();
	}

}

void ProcessListView::dragMoveEvent(QDragMoveEvent *_event)
{
// 	qDebug() << "ProcessListView: dragMoveEvent";
	_event->accept();
}

void ProcessListView::dropEvent(QDropEvent *_event)
{
	if(_event->mimeData()->hasFormat("text/uri-list"))
	{
		if(_event->mimeData()->hasUrls())
		{
			if(!((QDir)_event->mimeData()->urls().at(0).toLocalFile()).exists())
			{
				qDebug() << "ProcessListView: dropEvent: " << _event->mimeData()->urls().at(0).toLocalFile();
				emit createUpload(_event->mimeData()->urls().at(0).toLocalFile());
			}
			
			bool isInternetUrl = _event->mimeData()->urls().at(0).toString().contains(QRegExp("^http://*", Qt::CaseSensitive, QRegExp::RegExp2));
			if(isInternetUrl)
			{
				qDebug() << "ProcessListView: dropEvent: Internet Url "<< _event->mimeData()->urls().at(0).toString().left(45) << " --> " << isInternetUrl;
				emit createDownload(_event->mimeData()->urls().at(0).toString());
			}
		}
	}	
}
#include "ProcessListView.moc"
