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

#ifndef SQLITEDBMANAGER_H
#define SQLITEDBMANAGER_H

#include <QtSql>
#include <QSqlQuery>
#include <QString>


#include "storage/dbmanager.h"
#include "storage/item.h"

class SQLiteDBManager: public DBManager
{
    Q_OBJECT



private:
    //    QSqlQuery query;
    bool isConnected;
    QSqlDatabase db;

    QList<Item> processItems(QSqlQuery& query);
    QSqlQuery runPrepQuery(QString query, QString bindvalue ,bool isForwardOnly );
    bool runQuery(QString stmt, QSqlQuery *query, bool isForwardOnly =  true ) ;


public:
    explicit SQLiteDBManager(QObject *parent = 0);


    // overiden methods
    bool createConnection(QString username, QString passsword, QString hostname, QString database);
    Item getItem(int itemID);
    Item getItem(QString description);
    void searchItem(QString description);
    QList<Item> getItemsof(QString category);
    Resource getResource(int ID);
    QList<Resource> getResource(QString resourceName);
    bool addResource(Resource resource);

    ~SQLiteDBManager();

};

#endif // SQLITEDBMANAGER_H
