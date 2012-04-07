/* This file is part of Estima.
   Copyright 2012, Asitha Nanayakkara <daun07@gmail.com>

   Estima is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   Estima is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Estima.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "boqdelegate.h"

#include <QtGui>

BOQDelegate::BOQDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

QWidget * BOQDelegate::createEditor(QWidget *parent,
    const QStyleOptionViewItem &option,
    const QModelIndex &index) const
{
    if(index.column() == 2 || index.column() > 3){
        QDoubleSpinBox *editor = new QDoubleSpinBox(parent);
        editor->setMinimum(0);
        editor->setMaximum(100000000000000000);
        editor->setSpecialValueText("NULL");
        editor->setValue(0);
        return editor;
    }else{
        QPlainTextEdit *editor = new QPlainTextEdit(parent);
        return editor;
    }
}

void BOQDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{

    if(index.column() == 2 || index.column() > 3){
        double value = index.model()->data(index,Qt::EditRole).toDouble();
        QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox *>(editor);
        spinBox->setValue(value);
    }else{
        QString txt = index.model()->data(index,Qt::EditRole).toString();
        QPlainTextEdit *plainTextEdit = static_cast<QPlainTextEdit *>(editor);
        plainTextEdit->setPlainText(txt);
    }
}

void BOQDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if(index.column() == 2 || index.column() > 3){
        QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox *>(editor);
        double value = spinBox->value();
        QString str = QString().setNum(value, 'f', 2);
        if(value == 0){
            model->setData(index, QString("NULL"),Qt::EditRole);

        }else{
            model->setData(index, str ,Qt::EditRole);
        }
    }
    else {
        QPlainTextEdit *plainTextEdit = static_cast<QPlainTextEdit *>(editor);
        model->setData(index, plainTextEdit->toPlainText(), Qt::EditRole);
    }

}

void BOQDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}

//void BOQDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
//{
//    if(index.column() == 2 || index.column() > 3){
////        option.decorationAlignment = Qt::AlignRight;

//    }else{

//    }
//}
