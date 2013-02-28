#include "journalitemdelegate.h"

JournalItemDelegate::JournalItemDelegate()
{
}

void JournalItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                               const QModelIndex &index) const
{
    int x = option.rect.left() + (option.rect.right()  - option.rect.left() - 16) / 2;
    int y = option.rect.top()  + (option.rect.bottom() - option.rect.top()  - 16) / 2;

    QPixmap icon = qvariant_cast<QPixmap>(index.data(Qt::UserRole+1));
    if (!icon.isNull()) painter->drawPixmap(x, y+1, icon);
    else QItemDelegate::paint(painter, option, index);

    // отрисовать значок комментария
    if (index.data(Qt::UserRole+2).toBool())
    {
        painter->drawPixmap(option.rect.right() - 12, y, QPixmap(":/images/comments_small"));
    }
}
