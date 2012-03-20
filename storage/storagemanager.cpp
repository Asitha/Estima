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

#include "storage/storagemanager.h"


StorageManager::StorageManager(DBManager& newDBManager, XMLRenderer& newXMlRenderer, QObject *parent):
    QObject(parent)
{
    this->dbManager = &newDBManager;
    this->xmlRenderer = &newXMlRenderer;
}

Item StorageManager::getItem(int itemID)
{
    return dbManager->getItem(itemID);
}

Item StorageManager::getItem(QString description)
{
    return dbManager->getItem(description);
}

void StorageManager::setDBManager(DBManager& newDBManager)
{
    this->dbManager = &newDBManager;
}

void StorageManager::setXMLRenderer(XMLRenderer& newXMLRenderer)
{
    this->xmlRenderer= &newXMLRenderer;
}

bool StorageManager::createConnection(QString userName, QString password, QString hostname, QString Database)
{
    return dbManager->createConnection(userName,password,hostname,Database);
}

void StorageManager::getItemsOf(QString category)
{
    dbManager->getItemsof(category);

}

void StorageManager::searchItem(QString description)
{
    dbManager->searchItem(description);
}

void StorageManager::saveURC(QString fileName, const URCData &data)
{
    xmlRenderer->saveURC(fileName, data);
}

URCData StorageManager::retrieveURC(QString fileName)
{
    return xmlRenderer->retrieveFile(fileName);
}

Resource StorageManager::getResource(int ID)
{
    return dbManager->getResource(ID);
}

QList<Resource> StorageManager::getResource(QString resourceName)
{
    return dbManager->getResource(resourceName);
}

bool StorageManager::addResource(Resource resource)
{
    return dbManager->addResource(resource);
}
