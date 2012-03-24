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

#include <QAbstractTableModel>

struct TableItem{
    QString refNum;
    QString description;
    QString qty;
    QString unit;
    QString rate;
    QString amount;
};


class BOQTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit BOQTableModel(QObject *parent = 0);
    explicit BOQTableModel(int rowCount, QObject *parent);
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool insertRow(int row, const QModelIndex &parent = QModelIndex());
    bool removeRow(int row, const QModelIndex &parent = QModelIndex());
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    QList<TableItem>* getTableData();
    QString toCSV();
private:
    int rows;
    int columns;
    QList<TableItem> *pItemList;

    void addToItemList(const QModelIndex &index, const QVariant &value);
signals:
    void editCompleted(const QString &);

public slots:

};

#endif // BOQTABLEMODEL_H
