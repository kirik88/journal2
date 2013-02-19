#ifndef JOURNALITEMDELEGATE_H
#define JOURNALITEMDELEGATE_H

#include <QItemDelegate>
#include <QPainter>

class JournalItemDelegate : public QItemDelegate
{
public:
    JournalItemDelegate();

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // JOURNALITEMDELEGATE_H
