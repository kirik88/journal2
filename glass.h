#ifndef GLASS_H
#define GLASS_H

#include <QObject>
#include <QEvent>
#include <QColor>
#include <QMovie>
#include <QLabel>
#include <QGraphicsOpacityEffect>

class Glass : public QObject
{
    Q_OBJECT
public:
    Glass();
    ~Glass();

    void install(QWidget* widget, const QString& text = QString::null);
    void remove();
    void hide();

protected:
    bool eventFilter(QObject* object, QEvent* event);

private:
    void showInfoTextBlock(const QString& text = QString::null);
    void showAnimationBlock( QMovie * movie = 0);
    void infoBlockPositioning();

    QLabel* glass;
    QLabel* animationContainer;
    QLabel* infoTextContaiter;
    QMovie* movie;
};

#endif // GLASS_H
