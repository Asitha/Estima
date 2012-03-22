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

#include "storage/xmlrenderer.h"
#include "storage/xmlwriter.h"
#include "storage/item.h"

#include <QtXml>

XMLRenderer::XMLRenderer(QObject *parent):
    QObject(parent)
{

}

void XMLRenderer::saveURC(QString fileName, const URCData &data)
{

    // TODO: NEED A MECHANISM TO CHECK THIS IS SUCCESSFULL OR NOT

    QDomDocument URCFile;               // unit rate calculation(URC) resource file

    QDomElement root = URCFile.createElement("URCResource");

    // setup URCResource file attributes
    root.setAttribute("itemID", data.itemID);
    root.setAttribute("URCQuantity", data.URCQuantity);
    root.setAttribute("UnitURC", data.unitURC);
    root.setAttribute("ConvConst", data.convConst);    
    URCFile.appendChild(root);

    // add resources
    QDomElement resourse;

    //const_iterator used for read only access and optimise speed.
    QList<ResourceURC>::const_iterator iter, end;
    end = data.resources.end();
    for(iter= data.resources.begin(); iter < end ; ++iter){
        resourse = URCFile.createElement("Resource");
        resourse.setAttribute("ID", (*iter).ID );
        resourse.setAttribute("Name",(*iter).name );
        resourse.setAttribute("Qty", (*iter).quantity);
        resourse.setAttribute("Type", (*iter).type);
        root.appendChild(resourse);
    }


    xmlwriter.saveFile(fileName, URCFile);

}

URCData XMLRenderer::retrieveFile(QString fileName)
{

    QDomDocument URCFile = xmlwriter.retrieveFile(fileName);
    URCData tmpData;

    tmpData.itemID = -1;  /* if error occur inretrieving file or has no attribute
                                                            item ID  = -1 is returned */

    if(!URCFile.isNull()){ // if no error in retrieving the file then continue

        QDomElement root = URCFile.firstChildElement();

        // fill URCData struct
        if(root.hasAttributes()){
            tmpData.itemID = root.attribute("itemID").toInt();
            tmpData.convConst = root.attribute("ConvConst").toFloat();
            tmpData.URCQuantity = root.attribute("URCQuantity").toFloat();
            tmpData.unitURC = root.attribute("UnitURC");

            QDomNodeList ResourceItems = root.elementsByTagName("Resource");
            ResourceURC tmpResource;
            QDomElement resource;
            QDomNode tmpNode;
            for(int i=0; i < ResourceItems.count();i++){

                if((tmpNode = ResourceItems.at(i)).isElement()){
                    resource =  tmpNode.toElement();
                    tmpResource.ID = resource.attribute("ID").toInt();
                    tmpResource.name =  resource.attribute("Name");
                    tmpResource.quantity = resource.attribute("Qty").toFloat();
                    tmpResource.type = resource.attribute("Type").at(0).toAscii();
                    tmpData.resources.append(tmpResource);
                }
            }
        }
    }
    return tmpData;
}
