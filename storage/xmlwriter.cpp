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

#include "xmlwriter.h"
#include "storage/item.h"

#include <QtXml>
#include <QDebug>
#include <QFile>
#include <QDir>



XMLWriter::XMLWriter()
{
}

bool XMLWriter::saveFile(QString fileName, const QDomDocument& domDoc  )
{

//    // writing to file
//    QDir dir;
//    QString path = "./UnitRateData";
//    if(!dir.exists(path)){
//        dir.mkdir(path);
//    }

    QFile file(fileName);
    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream stream(&file);
        stream << domDoc.toString();
        file.close();
        qDebug()<< QString("Written to file \"%1\" succesfully").arg(fileName);
        return true;
    }else{
        qDebug("Couldn't write to file");
        return false;
    }
}


/**
  * reads an xml file from a specific location  given by fileName
  * before reading from the output node QDomDocument::isNull() method value
  * should be checked. if it returns true then there is an error in reading file
  */
QDomDocument XMLWriter::retrieveFile(QString fileName)
{
    QDomDocument domDoc;
    bool rdSuccess = true;
    QString errorMsg;
    int errorLine = 0;
    int errorColumn = 0;

    // read form  file
    QFile file(fileName);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        rdSuccess = domDoc.setContent(&file, &errorMsg, &errorLine, &errorColumn);
        file.close();
    }else{
        qDebug()<< "Couldn't read from file " + file.fileName();
        domDoc.clear();
    }
    if(!rdSuccess){
        qWarning() << "XML Read Error: " << errorMsg;
        qWarning() << "@line " << errorLine << "| @column " << errorColumn;
        domDoc.clear();
    }

    return domDoc;
}


