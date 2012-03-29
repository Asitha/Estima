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

#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QString>
#include <QObject>
#include <QtSql>

#include "storage/item.h"


class DBManager : public QObject
{
    Q_OBJECT

public:
    DBManager(QObject *parent =0);
    virtual bool createConnection(QString username, QString passsword, QString hostname, QString database) = 0;

    virtual Item getItem(QString description) = 0;
    virtual Item getItem(int itemID) =0;
    virtual void searchItem(QString description)= 0;
    virtual void updateItemQueryModel(QSqlQueryModel &model, QString description) = 0;
    virtual QList<Item> getItemsof(QString category)= 0;
    virtual Resource getResource(int ID) = 0;
    virtual QList<Resource> getResource(QString resourceName) = 0;

    virtual bool addResource(Resource resource) = 0;
};




#endif // DBMANAGER_H


