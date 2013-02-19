#include "value.h"

Value::Value(const QString &xml)
{
    columnId = -1;
    studentId = -1;
    value = "";
    description = "";

    // загружаем значение
    if (xml != "") load(xml);
}

Value::Value(Value *other)
{
    this->id = other->id;

    this->columnId = other->columnId;
    this->studentId = other->studentId;
    this->value = other->value;
    this->description = other->description;
}

// загрузка колонки из файла
bool Value::load(QFile *file)
{
    QDomDocument doc;
    if (!doc.setContent(file->readAll())) return false;

    QDomElement docElem = doc.documentElement();

    // переходим к парсингу
    parseNode(docElem);

    return true;
}

// загрузка колонки из строки
bool Value::load(const QString &xml)
{
    QDomDocument doc;
    if (!doc.setContent(xml)) return false;

    QDomElement docElem = doc.documentElement();

    // переходим к парсингу
    parseNode(docElem);

    return true;
}

// вернуть идентификатор значения
int Value::getId()
{
    return id;
}

// парсим xml
void Value::parseNode(QDomNode node)
{
    while (!node.isNull())
    {
        QDomElement e = node.toElement(); // пробуем преобразовать узел в элемент
        if (!e.isNull())
        {
            // для корневого узла
            if (e.tagName() == "student_value")
            {
                this->id = e.attribute("id").toInt();
                if (e.hasChildNodes()) parseNode(node.firstChild());
            }
            // другие используем
            else if (e.tagName() == "column")
            {
                this->columnId = e.text().toInt();
            }
            else if (e.tagName() == "student")
            {
                this->studentId = e.text().toInt();
            }
            else if (e.tagName() == "value")
            {
               this->value = e.text();
            }
            else if (e.tagName() == "description")
            {
                this->description = e.text();
            }
        }
        node = node.nextSibling();
    }
}
