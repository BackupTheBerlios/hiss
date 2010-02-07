#ifndef METAHANDLER_H_INCLUDED
#define METAHANDLER_H_INCLUDED

#include <QDomDocument>
#include <QFile>

#include "globals.h"

/** This class contains all basic process functions. **/
class MetaHandler
{
	public:
		//! Constructor
		MetaHandler();
		virtual ~MetaHandler() {}

		//! Verify MetaDoc and import content
		bool load(const QByteArray& _data);
		//! Verify MetaDoc and import content
		bool load(QFile _file);
		//! Generate MetaDoc from variables and save to disk
		void save(const QString& _outputName);

		//! Add an item
		void add(const Item& _newItem);
		//! Clear metaFile
		void clear();

		//! Set name
		void setName(const QString& _name);
		//! Set key
		void setKey(const QString& _key);

		//! Check if item exists
		bool exists(const qint32& _n);
		//! Get item from list
		Item* get(const qint32& _n);
		//! Get name
		QString getName();
		//! Get key
		QString getKey();

		quint64 getDataSize();
		quint64 getDataSize(const qint32& _n);

	private:
		QDomDocument metaDoc;
		QString name;
		QString key;
		QVector<Item> itemList;

		//! Interpret MetaDoc and fill-in all variables accordingly
		void import();
};

#endif
