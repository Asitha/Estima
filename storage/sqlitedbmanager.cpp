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

#include "storage/sqlitedbmanager.h"
#include "storage/item.h"

#include <QDebug>

#include <QtSql>
#include <QString>


static const int DB_NOT_OPEN = 1;
static const int QUERY_FAILED = 2;


SQLiteDBManager::SQLiteDBManager(QObject *parent) :
    DBManager(parent)
{

}


/**
  * This method needs to be called before usage.
  * Arguments are just a formality. SQLite does not need these.
  */
bool SQLiteDBManager::createConnection(QString username, QString passsword,QString hostname, QString database )
{

    db = QSqlDatabase::addDatabase( "QSQLITE" );
    db.setDatabaseName( "Estima.db" );

    if( !db.open() )
    {
        qDebug() << db.lastError();
        qFatal( "Failed to connect." );
        return false;
    }else
        qDebug( "Connected!" );
    return true;
}


/**
  * try to find the exact item using item ID
  */
Item SQLiteDBManager::getItem(int itemID)
{
    QList<Item> items;

    QSqlQuery query;
    if (runQuery(QString("SELECT * FROM item WHERE ID='%1'").arg(itemID), &query, true)){
        items = processItems(query);
    }

    return items.first();

}


/**
  * try to find the exact description in Items table
  */
Item SQLiteDBManager::getItem(QString description)
{
    QSqlQuery query;
    QList<Item> items;

    if(runQuery(QString("SELECT * FROM item WHERE Description='%1'").arg(description),&query, true )){
        items = (processItems(query));
    }
    return items.first();
}


/**
  * search for similar items with the keyword
  */
void SQLiteDBManager::searchItem(QString description)
{
    QSqlQuery query;
    if(runQuery(QString("SELECT * FROM item WHERE Description LIKE '%1'").arg(description),&query , true))
        processItems(query);
}


/**
  * private member of the class to process a returned query to extrat data
  * and fill the "Item" structure.
  * returns a QList with "Item" structs. if query is empty returns a list with an Item with ID = -1;
  */
QList<Item> SQLiteDBManager::processItems(QSqlQuery& query)
{
    QList<Item> items;

    while (query.next()){
        Item tmpItem;
        tmpItem.ID = query.value(0).toInt() ;
        tmpItem.categoryID = query.value(1).toInt() ;
        tmpItem.formulaID = query.value(2).toInt() ;
        tmpItem.refNum = query.value(3).toString() ;
        tmpItem.description =  query.value(4).toString() ;
        tmpItem.unit = query.value(5).toString() ;
        tmpItem.dateModified = query.value(6).toString() ;
        items.append(tmpItem);
    }
    if(items.isEmpty())
    {
        Item item;
        item.ID = -1;
        items.append(item);
    }
    return items;
}

/**
  * get category_ID from category table and get items form Items table
  * that has the retrieved cat_ID
  */
QList<Item> SQLiteDBManager::getItemsof(QString category)
{
    // method needs to be implementded

    QSqlQuery query;
    if(runQuery(QString("SELECT * FROM category WHERE Name='%1'").arg(category), &query, true))
    {
        while (query.next()){
            qDebug() << "ID" <<  query.value(0).toInt() ;
            qDebug() << "ID" << query.value(1).toString() ;
        }
    }
}


Resource SQLiteDBManager::getResource(int ID)
{
    QString stmt = QString("SELECT * FROM resource WHERE ID='%1'").arg(ID);
    Resource tmpResource;

    QSqlQuery query;

    if(runQuery(stmt, &query, true))
    {

        while(query.next()){
            tmpResource.ID = query.value(0).toInt();
            tmpResource.name = query.value(1).toString();
            tmpResource.unit = query.value(2).toString();
            tmpResource.rate = query.value(3).toFloat();
            tmpResource.lastModified = query.value(4).toString();
            tmpResource.type = query.value(5).toChar();
            tmpResource.quantity = query.value(6).toFloat();
        }
    }

    return tmpResource;
}
QList<Resource> SQLiteDBManager::getResource(QString resourceName)
{
    QList<Resource> resourceList;
    // implementation needed

    return resourceList;
}

SQLiteDBManager::~SQLiteDBManager()
{

}

/**
  *
  * ERROR: BUG IN THIS METHOD NEED DEBUGGING
  * Prepared statements are processed using this member.
  * bind value is fed as a QString
  */
QSqlQuery SQLiteDBManager::runPrepQuery(QString preprdStmt,QString bindValue, bool isForwardOnly)
{
    QSqlQuery query;
    if(db.open()){

        query.setForwardOnly(isForwardOnly);
        query.prepare(preprdStmt);
        query.addBindValue(bindValue);
        //        for(int i = 0; i < bindValues.size(); i++){
        //            query.addBindValue(bindValues.at(i));
        //        }
        query.exec();

    }else{
        isConnected =false;
        if(!isConnected){
            qDebug("not connected to database!");
        }else{
            qDebug() << "Error:"  << db.lastError().text() ;
        }
    }
    return query;
}

/**
  * the qeury object referece given will be executed and boolean true is returnec if
  * query was succesfull else false is returned.
  *
  */
bool SQLiteDBManager::runQuery(QString stmt, QSqlQuery *query, bool isForwardOnly)
{
    bool isSuccesfull;
    if(db.open()){
        query->setForwardOnly(isForwardOnly);
        isSuccesfull = query->exec(stmt);

    }else{
        isConnected =false;
        if(!isConnected){
            qDebug("not connected to database!");
        }else{
            qDebug() << "Error:"  <<db.lastError().text() ;
        }
    }
    return isSuccesfull;

}

bool SQLiteDBManager::addResource(Resource resource)
{
}


