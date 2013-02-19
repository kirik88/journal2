#include "journalitemdelegate.h"

JournalItemDelegate::JournalItemDelegate()
{
}

void JournalItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                               const QModelIndex &index) const
{
    QPixmap icon = qvariant_cast<QPixmap>(index.data(Qt::UserRole));
    if (!icon.isNull())
    {
        int x = option.rect.left() + (option.rect.right()  - option.rect.left() - icon.size().width() ) / 2;
        int y = option.rect.top()  + (option.rect.bottom() - option.rect.top()  - icon.size().height()) / 2;

        painter->drawPixmap(x, y+1, icon);

        // отрисовать значок комментария
        if (index.data(Qt::UserRole+1).toBool())
        {
            painter->drawPixmap(option.rect.right() - 12, y, QPixmap(":/images/comments_small"));
        }
    } else QItemDelegate::paint(painter, option, index);
}
