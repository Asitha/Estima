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

#include "boqtablemodel.h"

#include <QFont>

BOQTableModel::BOQTableModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    pItemList = new QList<TableItem>();
    this->rows = 0;
    this->columns = 6;
}

BOQTableModel::BOQTableModel(int rowCount, QObject *parent) :
    QAbstractTableModel(parent)
{
    this->rows = rowCount;
    this->columns = 6;
    pItemList = new QList<TableItem>();
    for(int i = 0; i < rowCount; i++){
        TableItem *item = new TableItem;
        pItemList->append(*item);
    }
}

int BOQTableModel::rowCount(const QModelIndex &parent) const
{
    return rows;
}

int BOQTableModel::columnCount(const QModelIndex &parent) const
{
    return columns;
}

QVariant BOQTableModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int col = index.column();

    TableItem item = pItemList->at(index.row());

    switch(role){

    case Qt::DisplayRole:
    case Qt::EditRole:
        switch(index.column()){
        case 0:
            return item.refNum;
        case 1:
            return item.description;
        case 2:
            return item.qty;
        case 3:
            return item.unit;
        case 4:
            return item.rate;
        case 5:
            return item.amount;
        }
        return QString();

    case Qt::TextAlignmentRole:
        switch(col){
        case 0:
        case 1:
            return Qt::AlignLeft;
        case 2:
        case 3:
        case 4:
        case 5:
            return Qt::AlignRight;
        }
    }

    return QVariant();
}

bool BOQTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role  == Qt::EditRole && index.row() < rows){
        if(index.row() < pItemList->size() ) {
            addToItemList(index, value);

            emit editCompleted(value.toString());
            return true;
        }else {
            return false;
        }
    }else {
        return false;
    }
}

Qt::ItemFlags BOQTableModel::flags(const QModelIndex &index) const
{
    return Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

void BOQTableModel::addToItemList(const QModelIndex &index, const QVariant &value)
{
    switch(index.column()){
    case 0:
        pItemList->operator [](index.row()).refNum = value.toString();
        break;
    case 1:
        pItemList->operator [](index.row()).description = value.toString();
        break;
    case 2:
        pItemList->operator [](index.row()).qty = value.toString();
        break;
    case 3:
        pItemList->operator [](index.row()).unit = value.toString();
        break;
    case 4:
        pItemList->operator [](index.row()).rate = value.toString();
        break;
    case 5:
        pItemList->operator [](index.row()).amount = value.toString();
        break;
    }
}

bool BOQTableModel::insertRow(int row, const QModelIndex &parent)
{
    return insertRows(row, 1, parent);
}

bool BOQTableModel::insertRows(int row, int count, const QModelIndex &parent)
{
    int firstRow = row;
    int lastRow = row +count -1;
    if(row < rowCount(parent)){
        beginInsertRows(parent, firstRow, lastRow);

        for(int i = row; i <= lastRow; i++){
            TableItem *item = new TableItem;
            pItemList->insert(i, *item);
            ++rows;
        }
        endInsertRows();
        return true;
    }else {
        beginInsertRows(parent,firstRow, lastRow);
        for(int i = 0; i <count; i++){
            TableItem *item = new TableItem;
            pItemList->append(*item);
            ++rows;
        }
        endInsertRows();
        return true;
    }
    return false;
}

bool BOQTableModel::removeRow(int row, const QModelIndex &parent)
{
    return removeRows(row, 1, parent);
}

bool BOQTableModel::removeRows(int row, int count, const QModelIndex &parent)
{
    int firstRow = row;
    int lastRow = row +count -1;
    (lastRow >= rowCount(parent)) ? ( lastRow = ( rowCount(parent) -1 ) ): lastRow;

    if(row < rowCount(parent)){
        beginRemoveRows(parent,firstRow,lastRow);
        for(int i = lastRow; i >= firstRow;  --i){
            pItemList->removeAt(i);
            rows--;
        }
        endRemoveRows();
        return true;
    }
    return false;
}

QVariant BOQTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole){
        if(orientation == Qt::Horizontal){
            switch (section){
            case 0:
                return QString("Ref Num");
            case 1:
                return QString("Description");
            case 2:
                return QString("Qty");
            case 3:
                return QString("Unit");
            case 4:
                return QString("Rate");
            case 5:
                return QString("Amount");
            }
        }
        else if(orientation == Qt::Vertical){
            return section+1;
        }
    }
    return QVariant();
}

QString BOQTableModel::toCSV()
{
}

QList<TableItem> * BOQTableModel::getTableData()
{
    return pItemList;
}



