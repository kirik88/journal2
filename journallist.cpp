#include "journallist.h"

JournalList::JournalList(const QString &xml) : QList()
{
    QDomDocument doc("journals");
    if (!doc.setContent(xml)) return;

    QDomElement docElem = doc.documentElement();

    // переходим к парсингу
    parseNode(docElem.firstChild());
}

JournalList::~JournalList()
{
    clear();
}

// парсим xml
void JournalList::parseNode(QDomNode node)
{
    while (!node.isNull())
    {
        QDomElement e = node.toElement(); // пробуем преобразовать узел в элемент
        if (!e.isNull())
        {
            // для узлов-журналов
            if (e.tagName() == "journal")
            {
                QString data;
                QTextStream out(&data);
                node.save(out, 1);

                this->append(new Journal(data));
            }
        }
        node = node.nextSibling();
    }
}

// очистка списка журналов
void JournalList::clear()
{
    while (this->count() > 0)
    {
        delete this->at(0);
        this->removeAt(0);
    }
}
