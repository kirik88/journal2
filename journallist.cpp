#include "journallist.h"

JournalList::JournalList(const QString &xml) : QList()
{
    clear();

    // загружаем журналы
    if (xml != "") load(xml);
}

JournalList::~JournalList()
{
    clear();
}

// загрузка журналов из файла
bool JournalList::load(QFile *file)
{
    QDomDocument doc;
    if (!doc.setContent(file->readAll())) return false;

    QDomElement docElem = doc.documentElement();

    // переходим к парсингу
    parseNode(docElem);

    return true;
}

// загрузка журналов из строки
bool JournalList::load(const QString &xml)
{
    QDomDocument doc;
    if (!doc.setContent(xml)) return false;

    QDomElement docElem = doc.documentElement();

    // переходим к парсингу
    parseNode(docElem);

    return true;
}

// парсим xml
void JournalList::parseNode(QDomNode node)
{
    while (!node.isNull())
    {
        QDomElement e = node.toElement(); // пробуем преобразовать узел в элемент
        if (!e.isNull())
        {
            // для корневого узла спускаемся ниже
            if (e.tagName() == "journals")
            {
                if (e.hasChildNodes()) parseNode(node.firstChild());
            }
            // для узлов-журналов
            else if (e.tagName() == "journal")
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

// добавление копии журнала в список либо обновление имеющегося по id
void JournalList::appendRefresh(Journal *journal)
{
    for (int i = 0; this->count() > i; i++)
    {
        Journal *jrn = this->at(i);

        // обновление имеющегося
        if (jrn->getId() == journal->getId())
        {
            jrn->copyFrom(journal);
            return;
        }
    }

    // добавление копии в список
    Journal *jrn = new Journal(journal);
    this->append(jrn);
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
