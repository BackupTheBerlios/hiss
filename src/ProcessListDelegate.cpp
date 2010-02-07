#include "ProcessListDelegate.h"
#include "ProcessListModel.h"

#include <QStyleOptionProgressBarV2>
#include <QStyle>
#include <QApplication>

ProcessListDelegate::ProcessListDelegate(QObject *parent, ProcessListModel *_model)
		: QItemDelegate(parent),
		model(_model)
{

}

ProcessListDelegate::~ProcessListDelegate()
{
}

void ProcessListDelegate::paint(QPainter* _painter, const QStyleOptionViewItem& _option, const QModelIndex& _index) const
{
	if (_index.column() == 3)
	{
		if (model == 0)
		{
			QItemDelegate::paint(_painter, _option, _index);
			return;
		}

		QStyleOptionProgressBarV2 progressBarOptions;

		Process *process = model->getProcess(_index.row());

		if (process != 0)
		{

			progressBarOptions.text = QString("%1%").arg(process->getProgress());
			progressBarOptions.maximum = 100;
			progressBarOptions.minimum = 0;
			progressBarOptions.progress = process->getProgress();
			progressBarOptions.rect = _option.rect;
			progressBarOptions.rect.setRight(_option.rect.right() - 1);
			progressBarOptions.rect.setHeight(_option.rect.height() - 1);
			progressBarOptions.textVisible = true;
			progressBarOptions.textAlignment = Qt::AlignCenter;
			QApplication::style()->drawControl(QStyle::CE_ProgressBar, &progressBarOptions, _painter);
			return;

		}
	}

	QItemDelegate::paint(_painter, _option, _index);
}

#include "ProcessListDelegate.moc"
