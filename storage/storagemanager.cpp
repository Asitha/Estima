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
    this->pDBManager = &newDBManager;
    this->pXMLRenderer = &newXMlRenderer;
}

Item StorageManager::getItem(int itemID)
{
    return pDBManager->getItem(itemID);
}

Item StorageManager::getItem(QString description)
{
    return pDBManager->getItem(description);
}

void StorageManager::setDBManager(DBManager& newDBManager)
{
    this->pDBManager = &newDBManager;
}

void StorageManager::setXMLRenderer(XMLRenderer& newXMLRenderer)
{
    this->pXMLRenderer= &newXMLRenderer;
}

bool StorageManager::createConnection(QString userName, QString password, QString hostname, QString Database)
{
    return pDBManager->createConnection(userName,password,hostname,Database);
}

void StorageManager::getItemsOf(QString category)
{
    pDBManager->getItemsof(category);

}

void StorageManager::searchItem(QString description)
{
    pDBManager->searchItem(description);
}

void StorageManager::saveURC(QString fileName, const URCData &data)
{
    pXMLRenderer->saveURC(fileName, data);
}

URCData StorageManager::retrieveURC(QString fileName)
{
    return pXMLRenderer->retrieveFile(fileName);
}

Resource StorageManager::getResource(int ID)
{
    return pDBManager->getResource(ID);
}

QList<Resource> StorageManager::getResource(QString resourceName)
{
    return pDBManager->getResource(resourceName);
}

bool StorageManager::addResource(Resource resource)
{
    return pDBManager->addResource(resource);
}
