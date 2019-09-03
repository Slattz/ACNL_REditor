#include "Headers/textboxdelegate.h"
#include <QLineEdit>
#include <QDebug>

TextBoxDelegate::TextBoxDelegate(const QValidator *validator, QObject *parent)
    : QStyledItemDelegate(parent), m_validator(validator)
{
}

QWidget *TextBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/* option */,
                                       const QModelIndex &/* index */) const
{
    QLineEdit *editor = new QLineEdit(parent);
    if (m_validator) {
         editor->setValidator(m_validator);
    }
    return editor;
}

void TextBoxDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const
{
    int value = index.model()->data(index, Qt::EditRole).toInt();

    QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
    lineEdit->setText(QString::number(value));
}

void TextBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
    QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
    QString value = lineEdit->text();
    qDebug() << "value: " << value;
    model->setData(index, value, Qt::EditRole);
}

void TextBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                                           const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}

