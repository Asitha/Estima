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



#include <QtGui/QApplication>
#include <QPluginLoader>
#include <QtPlugin>
#include <QDebug>


#include "storage/xmlwriter.h"
#include "storage/xmlrenderer.h"
#include "core/boqgenerator.h"
#include "main/estima.h"
#include "storage/item.h"
#include "storage/sqlitedbmanager.h"



//TEST CODE
void testDB()
{
    SQLiteDBManager sqlMan;
    sqlMan.createConnection("user", "pass", "host", "data" );
//    Item item =  sqlMan.getItem(1);
//    qDebug()<< "item ID "<<item.ID ;
//    qDebug()<< "category ID "<<item.categoryID;
//    qDebug()<< "ref No "<<item.refNum;
//    qDebug()<< "Description "<<item.description;
//    qDebug()<< "Unit "<<item.unit;
       Resource item = sqlMan.getResource(1);
        qDebug()<< "item ID "<<item.ID ;
            qDebug()<< "category ID "<<item.name;
            qDebug()<< "ref No "<<item.quantity;
            qDebug()<< "Description "<<item.rate;
            qDebug()<< "Unit "<<item.unit;
}



int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("Estima");

    Estima estima;
    estima.show();
//    testDB();



    return app.exec();
}

