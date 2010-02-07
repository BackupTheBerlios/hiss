#ifndef PROGRESSBARDELEGATE_H
#define PROGRESSBARDELEGATE_H

#include <QItemDelegate>
#include <QModelIndex>
#include <QObject>
#include <QProgressBar>
#include "ProcessListModel.h"

/*! delegation class for displaying a progress bar in another widget */

class ProcessListDelegate : public QItemDelegate
{
		Q_OBJECT
	public:
		/*! constructor */
		ProcessListDelegate(QObject *parent, ProcessListModel *_model);
		~ProcessListDelegate();

		/*! default progress bar size */
		QSize sizeHint(const QStyleOptionViewItem&, const QModelIndex&) const
		{
			return QSize(60, 16);
		}

		/*! paints progressbar */
		void paint(QPainter* _painter, const QStyleOptionViewItem& _option, const QModelIndex& _index) const;

	private:
		ProcessListModel *model;
};

#endif
