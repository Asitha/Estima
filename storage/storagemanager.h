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

#ifndef PERSISTENTDATAMANUPULATOR_H
#define PERSISTENTDATAMANUPULATOR_H

#include <QObject>
#include "storage/dbmanager.h"
#include "storage/xmlrenderer.h"
#include "storage/item.h"



class StorageManager: public QObject
{
    Q_OBJECT
public:

    enum Error_Flag {INVALID_Item_ID = -1,  /* always use minus values for this as valid ID's are positive */
                      INVALID_Resource_ID = -2,
                      INVALID_URCFILE_PATH = -3
                     };


    StorageManager(DBManager& newDBManager, XMLRenderer& newXMlRenderer, QObject *parent = 0);

    // setup members
    bool createConnection(QString userName, QString password, QString hostname, QString Database);
    void setDBManager(DBManager& newDBManager);
    void setXMLRenderer(XMLRenderer& newXMLRenderer);


    // utility members
    Item getItem(int itemID);
    Item getItem(QString description);
    void searchItem(QString description);
    void getItemsOf(QString category);
    URCData retrieveURC(QString fileName);
    Resource getResource(int ID);
    QList<Resource> getResource(QString resourceName);
    bool addResource(Resource resource);
    bool addItem(Item item);

    // NEED TO IMPLEMENT
    void save();
    void load();

    void saveURC(QString fileName,const URCData& data);
    void setItem();


private:

    DBManager *pDBManager;
    XMLRenderer *pXMLRenderer;



};

#endif // PERSISTENTDATAMANUPULATOR_H
