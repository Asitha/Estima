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

#ifndef MYSQLCONNECTOR_H
#define MYSQLCONNECTOR_H

#include "dbmanager.h"
#include "storage/sqlitedbmanager.h"
#include "storage/item.h"

#include <QtSql>
#include <QString>


class MySQLConnector : public DBManager
{
    Q_OBJECT

private:
    //    QSqlQuery query;
    bool isConnected;
    QSqlDatabase db;

    QList<Item> processItems(QSqlQuery& query);
    QSqlQuery runPrepQuery(QString query, QStringList bindvalue ,bool isForwardOnly = true, bool *isSuccess = false);
    bool runQuery(QString stmt, QSqlQuery *query, bool isForwardOnly =  true );
    Resource fillResourceData(QSqlQuery query);

public:
    explicit MySQLConnector(QObject *parent = 0);

    // overiden methods
    bool createConnection(QString username, QString passsword, QString hostname, QString database);
    Item getItem(int itemID);
    Item getItem(QString description);
    void searchItem(QString description);
    void updateItemQueryModel(QSqlQueryModel &model, QString description);
    QList<Item> getItemsof(QString category);
    Resource getResource(int ID);
    QList<Resource> getResource(QString resourceName);
    bool addResource(Resource resource);

};

#endif // MYSQLCONNECTOR_H
