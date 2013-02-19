#include "glass.h"

Glass::Glass()
{
    // окно
    glass = new QLabel();
    glass->setStyleSheet("background-color: rgb(0, 0, 0);");
    glass->setAutoFillBackground(true);

    // текст
    infoTextContaiter = new QLabel();
    infoTextContaiter->setStyleSheet("color: rgb(255, 255, 255);");
    QFont font = infoTextContaiter->font();
    font.setBold(true);
    infoTextContaiter->setFont(font);

    // анимация
    animationContainer = new QLabel();
    movie = new QMovie(":/animations/loading");
    animationContainer->setMovie(movie);

    // эффект прозрачности
    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect();
    effect->setOpacity(0.7);
    glass->setGraphicsEffect(effect);
}

Glass::~Glass()
{
    glass->deleteLater();
    animationContainer->deleteLater();
    infoTextContaiter->deleteLater();
}

// установка стекла
void Glass::install(QWidget* widget, const QString& text)
{
    remove();

    glass->setParent(widget);
    if (!text.isNull()) showInfoTextBlock(text);

    // начнем отлавливать все события, установив фильтр
    glass->parentWidget()->installEventFilter(this);

    glass->show();

    QEvent event(QEvent::Resize);
    eventFilter(0, &event);
}

// удаление стекла
void Glass::remove()
{
    if (glass->parentWidget() != 0)
    {
        glass->parentWidget()->removeEventFilter(this);

        glass->hide();
        movie->stop();
        glass->setParent(0);
        animationContainer->hide();
        infoTextContaiter->hide();
        animationContainer->setParent(0);
        infoTextContaiter->setParent(0);
    }
}

// скрытие элементов
void Glass::hide()
{
    if (glass->parentWidget() != 0)
    {
        movie->stop();
        animationContainer->hide();
        infoTextContaiter->hide();
    }
}

// Реализуем фильтр событий на виджете. Назначение этого фильтра - не
// допустить работу с виджетом под стеклом и обеспечить актуальные размеры и перерисовку стекла
bool Glass::eventFilter(QObject* /* object */, QEvent* event)
{
    // Если показывается виджет или изменились его размеры, изменяем размеры
    // стекла и позиционируем информационный блок
    if ((event->type() == QEvent::Show) || (event->type() == QEvent::Resize))
    {
        glass->resize(glass->parentWidget()->size());
        glass->move(0, 0);
        infoBlockPositioning();
        return true;
    }

    // Всегда отбираем фокус, чтобы запретить работу с виджетом под стеклом
    glass->setFocus();
    return false;
}

// позиционирование
void Glass::infoBlockPositioning() {
    if (animationContainer->isVisible() && infoTextContaiter->isVisible())
    {
        animationContainer->move((glass->width() - animationContainer->width()) / 2,
                glass->height() / 2 - animationContainer->height());
        infoTextContaiter->move((glass->width() - infoTextContaiter->width()) / 2,
                glass->height() / 2 + infoTextContaiter->height());
    }
    else
    {
        if (animationContainer->isVisible())
            animationContainer->move((glass->width() - animationContainer->width()) / 2,
                    (glass->height() - animationContainer->height()) / 2);

        if (infoTextContaiter->isVisible())
            infoTextContaiter->move((glass->width() - infoTextContaiter->width()) / 2,
                    (glass->height() - infoTextContaiter->height()) / 2);
    }
}

// показ информационного блока
void Glass::showInfoTextBlock(const QString& text)
{
    infoTextContaiter->setParent(glass->parentWidget());
    infoTextContaiter->setText(text);

    infoTextContaiter->show();
    infoTextContaiter->adjustSize();

    movie->start();
    animationContainer->setParent(glass->parentWidget());
    animationContainer->show();
    animationContainer->adjustSize();

    infoBlockPositioning();
}

