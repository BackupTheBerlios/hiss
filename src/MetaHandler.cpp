#include "MetaHandler.h"

MetaHandler::MetaHandler()
{
	name = "";
	key = "";
}




bool MetaHandler::load(const QByteArray& _data)
{
	if (!metaDoc.setContent(_data))
	{
		return false;
	}
	else
	{
		import();
		return true;
	}
}

bool MetaHandler::load(QFile _file)
{
	_file.open(QIODevice::ReadOnly);

	if (!metaDoc.setContent(&_file))
	{
		_file.close();
		return false;
	}
	else
	{
		import();
		_file.close();
		return true;
	}
}

void MetaHandler::save(const QString& _outputName)
{
	QFile tempMeta(_outputName);
	tempMeta.open(QIODevice::WriteOnly | QIODevice::Text);

	// Write Head
	QTextStream out(&tempMeta);
	out << "<meta>\n" << endl;
	out << "<setting name=\"" << name << "\" key=\"" << key << "\" />\n" << endl;

	// Write items
	for(qint32 i = 0; i < itemList.size(); i++)
	{
		out << "  <item id=\"" << itemList.at(i).id << "\" file=\"" << itemList.at(i).image.file   << "\" datalength=\"" << itemList.at(i).image.datalength << "\" padding=\"" << itemList.at(i).image.padding
			<< "\" method=\""  << itemList.at(i).image.method << "\" hash=\"" << itemList.at(i).remote.hash << "\">" << endl;

		for(qint32 j = 0; j < itemList.at(i).remote.link.size(); j++)
			out << "    <link referrer=\"" << itemList.at(i).remote.link.at(j).referrer << "\" url=\"" << itemList.at(i).remote.link.at(j).url << "\" />" << endl;

		out << "  </item>\n" << endl;
	}
	out << "</meta>";
	// Add padding
    out << "          " << endl;

	tempMeta.close();
}




void MetaHandler::add(const Item& _newItem)
{
	itemList.push_back(_newItem);
}

void MetaHandler::clear()
{
	itemList.clear();
	name = "";
	key = "";
}




void MetaHandler::setName(const QString& _name)
{
	name = _name;
}

void MetaHandler::setKey(const QString& _key)
{
	key = _key;
}




bool MetaHandler::exists(const qint32& _n)
{
	return (itemList.size() > _n);
}

Item* MetaHandler::get(const qint32& _n)
{
	return &itemList[_n];
}

QString MetaHandler::getName()
{
	return name;
}

QString MetaHandler::getKey()
{
	return key;
}




quint64 MetaHandler::getDataSize()
{
	return getDataSize(itemList.size());
}

quint64 MetaHandler::getDataSize(const qint32& _n)
{
	quint64 size = 0;

	for(qint32 i = 0; i < _n; i++)
		size += itemList.at(i).image.datalength;

	return size;
}




void MetaHandler::import()
{
	// Parse Items from MetaFile
	QDomElement docElem = metaDoc.documentElement();
	QDomNode node = docElem.firstChild();

	while(!node.isNull()) {
		// Try converting Node to Element...
		QDomElement e = node.toElement();
		// ... and check if it worked
		if(!e.isNull()) {
			// Extract Item information
			if(e.tagName().compare("item") == 0)
			{
				Item tempItem;
				tempItem.id = e.attribute("id").toUInt();
				tempItem.image.file = e.attribute("file");
				tempItem.image.datalength = e.attribute("datalength").toUInt();
				tempItem.image.padding = e.attribute("padding").toUInt();
				tempItem.image.method = e.attribute("method").toUInt();
				tempItem.remote.hash = e.attribute("hash");

				// Extract information on backup files
				QDomNode remote = node.firstChild();
				while(!remote.isNull()) {
					// Try converting Node to Element...
					QDomElement r = remote.toElement();
					// ... and check if it worked
					if(!r.isNull() && r.tagName().compare("link") == 0)
					{
						// Extract Link information
						Item_Link tempLink;
						tempLink.url = r.attribute("url");
						tempLink.referrer = r.attribute("referrer");
						tempItem.remote.link.push_back(tempLink);
					}
					// Iterate Links
					remote = remote.nextSibling();
				}
				add(tempItem);
			}
			// Extract Setting information
			if(e.tagName().compare("setting") == 0)
			{
				if(!e.attribute("name").isEmpty())
					name = e.attribute("name");
				if(!e.attribute("key").isEmpty())
					key = e.attribute("key");
			}
		}
		// Iterate Items
		node = node.nextSibling();
	}
	// Done.
}

