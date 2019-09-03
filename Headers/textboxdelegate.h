#ifndef TEXTBOXDELEGATE_H
#define TEXTBOXDELEGATE_H

#include <QStyledItemDelegate>

class TextBoxDelegate : public QStyledItemDelegate
{
    Q_OBJECT
    const QValidator *m_validator;

public:
    TextBoxDelegate(const QValidator *validator = nullptr, QObject *parent = nullptr);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const override;

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const override;
};

#endif // CUSTOMLISTMODEL_H
