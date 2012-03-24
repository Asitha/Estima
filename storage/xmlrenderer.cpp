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



#include <QtXml>

/**
  * Project specific constants to be used in xml attributes
  */
const QString projNameTag = "name";
const QString projRefNumTag = "referenceNumber";
const QString projAuthorTag = "author";
const QString projUnitSystemTag = "unitSystem";
const QString projCurrencyTag = "currency";
const QString projMarkupTag = "markup";

/**
  * BOQ item specific constants to be used in naming xml attributes
  */
const QString itemRefNumTag = "referenceNumber";
const QString itemIDTag = "id";
const QString itemCategoryIDTag = "categoryID";
const QString itemCategoryTag = "category";
const QString itemDescriptionTag = "description";
const QString itemUnitTag = "unit";
const QString itemModifiedDateTag = "lastModified";
const QString itemUnitRateTag = "unitRate";
const QString itemQtyTag = "qty";
const QString itemAmountTag = "amount";


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

URCData XMLRenderer::retrieveURC(QString fileName)
{

    QDomDocument URCFile = xmlwriter.retrieveFile(fileName);
    URCData tmpData;

    tmpData.itemID = -1;  /* if error occur in retrieving file or has no attribute
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


/**
  * writes the BOQData into an XML file to be saved in the filepath given
  */

bool XMLRenderer::saveBOQ(const QString &filepath, const BOQData &data)
{
    QDomDocument BOQFile;
    QDomElement root = BOQFile.createElement("EstimaProjectXML");

    // setup XMLFile attributes. (Estima version information)
    root.setAttribute("version", 0.5 );
    BOQFile.appendChild(root);

    QDomElement metaData = BOQFile.createElement("ProjectData");

    metaData.setAttribute(projNameTag , data.projectData.name);
    metaData.setAttribute(projRefNumTag , data.projectData.projRefNum);
    metaData.setAttribute(projAuthorTag , data.projectData.preparedBy);
    metaData.setAttribute(projCurrencyTag , data.projectData.currency);
    metaData.setAttribute(projUnitSystemTag , data.projectData.unitSystem);
    metaData.setAttribute(projMarkupTag , data.projectData.markup);

    root.appendChild(metaData);

    QDomElement itemList = BOQFile.createElement("BOQItems");
    root.appendChild(itemList);

    QList<BOQTableItem>::const_iterator iter, end;
    end = data.itemList.end();


    for(iter=data.itemList.begin(); iter < end; ++iter){
        QDomElement item = BOQFile.createElement("BOQItem");
        item.setAttribute(itemRefNumTag, (*iter).refNum);
        item.setAttribute(itemDescriptionTag, (*iter).description);
        item.setAttribute(itemQtyTag, (*iter).qty);
        item.setAttribute(itemUnitTag, (*iter).unit);
        item.setAttribute(itemUnitRateTag, (*iter).rate);
        item.setAttribute(itemAmountTag, (*iter).amount);
        itemList.appendChild(item);
    }


    return xmlwriter.saveFile(filepath, BOQFile);
}

BOQData XMLRenderer::retrieveBOQ(const QString &filepath, bool *noError)
{
    QString version;

    QDomDocument BOQFile = xmlwriter.retrieveFile(filepath);
    BOQData data;
    if(!BOQFile.isNull()){

        QDomElement root = BOQFile.firstChildElement();
        if(root.hasAttributes()){
            version = root.attribute("version");
            QDomElement metaData = root.firstChildElement("ProjectData");
            if(!metaData.isNull()){
                data.projectData.name = metaData.attribute(projNameTag);
                data.projectData.projRefNum = metaData.attribute(projRefNumTag);
                data.projectData.preparedBy = metaData.attribute(projAuthorTag);
                data.projectData.currency = metaData.attribute(projCurrencyTag);
                data.projectData.unitSystem = metaData.attribute(projUnitSystemTag);
                data.projectData.markup = metaData.attribute(projMarkupTag).toFloat();
            }else{
                *noError = false;
            }
            QDomElement itemList = root.firstChildElement("BOQItems");
            if(!itemList.isNull()){
                QDomNodeList ItemNodeList = root.elementsByTagName("BOQItem");
                BOQTableItem BOQItem;
                QDomElement itemElement;
                QDomNode tmpNode;

                for(int i = 0; i < ItemNodeList.size(); ++i ){
                    if((tmpNode = ItemNodeList.at(i)).isElement()){
                        itemElement  =  tmpNode.toElement();
                        BOQItem.refNum = itemElement.attribute(itemRefNumTag);
                        BOQItem.description = itemElement.attribute(itemDescriptionTag);
                        BOQItem.qty = itemElement.attribute(itemQtyTag);
                        BOQItem.rate = itemElement.attribute(itemUnitRateTag);
                        BOQItem.amount = itemElement.attribute(itemAmountTag);
                        BOQItem.unit = itemElement.attribute(itemUnitTag);
                        data.itemList.append(BOQItem);
                    }
                }
            }else{
               *noError = false;
            }
        }else {
            *noError = false;
        }
    }else {
        *noError = false;
    }

    return data;
}
