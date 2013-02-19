#include "answer.h"

Answer::Answer(const QString &xml)
{
    this->code   = ANSWER_ERROR;
    this->result = QT_TR_NOOP(QString::fromUtf8("Получен некорректный ответ от сервера.")); // TODO:TR

    QDomDocument doc;
    if (!doc.setContent(xml)) return;

    QDomElement docElem = doc.documentElement();

    // переходим к парсингу
    parseNode(docElem.firstChild());
}

Answer::Answer(AnswerCode code, const QString &result)
{
    this->code   = code;
    this->result = result;
}

// код ответа
AnswerCode Answer::getCode()
{
    return this->code;
}

// текстовая расшифровка кода
QString Answer::getResult()
{
    return this->result;
}

// значение ответа
QString Answer::getValue(const QString &name)
{
    return this->answer.value(name, "").toString();
}

// парсим xml
void Answer::parseNode(QDomNode node)
{
    while (!node.isNull())
    {
        QDomElement e = node.toElement(); // пробуем преобразовать узел в элемент
        if (!e.isNull())
        {
            // для корневого узла
            if (e.tagName() == "result")
            {
                this->code = AnswerCode(e.attribute("code").toInt());
                this->result = e.text();
            }
            // для комбинированных ответов сохраняем весь вывод
            else if (e.tagName() == "journals")
            {
                QString data;
                QTextStream out(&data);
                node.save(out, 1);

                this->answer.insert(e.tagName(), data);
            }
            // другие используем
            else
            {
                this->answer.insert(e.tagName(), e.text());
            }
        }
        node = node.nextSibling();
    }
}
