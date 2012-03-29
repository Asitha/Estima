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

#ifndef BOQTABLEMODEL_H
#define BOQTABLEMODEL_H

#include "core/CoreStructures.h"

#include <QAbstractTableModel>
#include <QHash>
#include <QMultiHash>


struct SummaryStruct{
    QChar category;
    double subTotal;
};

class BOQTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit BOQTableModel(QObject *parent = 0);
    explicit BOQTableModel(int rowCount, QObject *parent);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool insertRow(int row, const QModelIndex &parent = QModelIndex());
    bool removeRow(int row, const QModelIndex &parent = QModelIndex());
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    QList<BOQTableItem>* getTableData();
    QString toCSV();
    int rowOfItem(QString refNum);
    double getTotal();
    QList<SummaryStruct> getSummary();
private:
    int rows;
    int columns;

    QMultiHash<QChar, BOQTableItem> ItemHashList;
    QList<BOQTableItem> *pItemList;

    void addToItemList(const QModelIndex &index, const QVariant &value);
signals:


public slots:

};

#endif // BOQTABLEMODEL_H
