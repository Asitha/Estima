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

SQLiteDBManager::~SQLiteDBManager()
{
}


/**
  * This method needs to be called before usage.
  * Arguments are just a formality. SQLite does not need these.
  */
bool SQLiteDBManager::createConnection(QString username, QString passsword,QString hostname, QString database )
{

    db = QSqlDatabase::addDatabase( "QSQLITE" );
    QFile file();

    db.setDatabaseName( "D:/Software/Project/Estima-build-desktop-Qt_4_8_0_for_Desktop_-_MinGW__Qt_SDK__Debug/estima.db" );

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
    QSqlQuery query;
    if (!runQuery(QString("SELECT * FROM item WHERE ID='%1'").arg(itemID), &query, true)){
         qDebug() << "No item found on DB";
    }
    return processItems(query).first();
}


/**
  * try to find the exact description in Items table
  */
Item SQLiteDBManager::getItem(QString description)
{
    QSqlQuery query;
    QList<Item> itemList;

    if(!runQuery(QString("SELECT * FROM item WHERE Description='%1'").arg(description),&query, true )){
        qDebug() << "No item found on DB";
    }
    itemList=  processItems(query);
    return itemList.first();
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
    QSqlRecord record = query.record();
    while (query.next()){
        Item tmpItem;
        tmpItem.ID = query.value(record.indexOf("ID")).toInt() ;
        tmpItem.categoryID = query.value(record.indexOf("Category_ID")).toInt() ;
        tmpItem.refNum = query.value(record.indexOf("Reference_No")).toString() ;
        tmpItem.description =  query.value(record.indexOf("Description")).toString() ;
        tmpItem.unit = query.value(record.indexOf("Unit")).toString() ;
        tmpItem.dateModified = query.value(record.indexOf("Date_Created")).toString() ;
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
    QList<Item> itemList;
    QSqlQuery query;
    QString stmt = QString("SELECT item.ID, category_ID, Formula_ID, Reference_No, Description, Unit, "
                           "Date_Created FROM category join item WHERE category.name='%1' and "
                           "category.ID = item.Category_ID").arg(category);
    if(runQuery(stmt, &query, true))
        itemList = processItems(query);

    foreach(Item itm, itemList)
        itm.category = category;

    return itemList;
}


Resource SQLiteDBManager::getResource(int ID)
{
    QString stmt = QString("SELECT * FROM resource WHERE ID='%1'").arg(ID);
    Resource tmpResource;
    tmpResource.ID = -1;
    QSqlQuery query;

    if(runQuery(stmt, &query, true))
    {
        if(query.next())
            tmpResource = fillResourceData(query);
    }
    return tmpResource;
}

void SQLiteDBManager::updateItemQueryModel(QSqlQueryModel &model,QString description)
{
    QString txt = QString("SELECT Description FROM item WHERE Description LIKE '\%%1\%' ").arg(description);
    model.setQuery(txt, db);
}


QList<Resource> SQLiteDBManager::getResource(QString resourceName)
{
    QString stmt = QString("SELECT * FROM resource WHERE Name='%1'").arg(resourceName);
    QSqlQuery query;
    QList<Resource> resourceList;
    if(runQuery(stmt, &query,true)){
        while(query.next()){
            resourceList.append(fillResourceData(query));
        }
    }
    return resourceList;
}


Resource SQLiteDBManager::fillResourceData(QSqlQuery query)
{
    Resource tmpResource;
    tmpResource.ID = -1;
    if(query.isValid()){
        tmpResource.ID = query.value(0).toInt();
        tmpResource.name = query.value(1).toString();
        tmpResource.unit = query.value(2).toString();
        tmpResource.rate = query.value(3).toFloat();
        tmpResource.lastModified = query.value(4).toString();
        tmpResource.type = query.value(5).toChar();
        tmpResource.quantity = query.value(6).toFloat();
    }
    return tmpResource;
}



/**
  *
  * ERROR: BUG IN THIS METHOD NEED DEBUGGING
  * Prepared statements are processed using this member.
  * bind value is fed as a QString
  */
QSqlQuery SQLiteDBManager::runPrepQuery(QString preprdStmt,QStringList bindValues, bool isForwardOnly, bool *isSuccess)
{
    QSqlQuery query;
    if(db.open()){

        query.setForwardOnly(isForwardOnly);
        query.prepare(preprdStmt);
        foreach (QString str, bindValues){
            query.addBindValue(bindValues);
            qDebug() << str;
        }
        *isSuccess = query.exec();

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
    bool isSuccesfull =false;
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
    bool isSuccess = false;
    if(db.open()){
        QSqlQuery query;
        QString prepStmt = "INSERT INTO resource (ID, Name, Unit, Rate, Last_Modified, Type, Quantity, Description) "
                "VALUES(?,?,?,?,?,?,?,?)";

        query.prepare(prepStmt);

        query.addBindValue(resource.ID);
        query.addBindValue(resource.name);
        query.addBindValue(resource.unit);
        query.addBindValue(resource.rate);
        query.addBindValue(resource.lastModified);
        query.addBindValue(resource.type);
        query.addBindValue(resource.quantity);
        query.addBindValue(resource.description);

        isSuccess = query.exec();
//        QStringList attributList;
//        attributList << QString(resource.ID) << resource.name << resource.unit << QString().setNum(resource.rate)   ;
//        attributList << resource.lastModified << resource.type <<  QString().setNum(resource.quantity) << resource.description;

//        runPrepQuery(prepStmt, attributList, true, &isSuccess);
    }
    return isSuccess;
}

