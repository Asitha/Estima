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

#include "boqgenerator.h"

BOQGenerator::BOQGenerator(StorageManager& storageManager ,QObject *parent) :
    QObject(parent)
{
    this->pStorageManager = &storageManager;
    pURCalc = new UnitRateCalculator(storageManager);
    MARKUP = 0.3;
}
void BOQGenerator::setItem(QString itemName)
{

}

void BOQGenerator::setStorageManager(StorageManager& storageManager)
{
    this->pStorageManager = &storageManager;
    this->pURCalc->setStorageManager(storageManager);

}

/**
  * process item data and calculate the unit rate and return the
  * processed item back.
  */
BOQItem BOQGenerator::getItemData(QString itemDesc, float qty)
{
     Item item = pStorageManager->getItem(itemDesc);

    BOQItem boqItem;
    boqItem.qty= qty;
    boqItem.itemStruct = item;

    if(item.ID != pStorageManager->INVALID_Item_ID){
        // calculate unit rate
        boqItem.unitRate = pURCalc->getUnitRate(boqItem.itemStruct.refNum, MARKUP).unitRate;

        if(boqItem.unitRate < 0){
            //TODO: option should be given to edit unit rate calculation
            boqItem.itemStruct.ID = pStorageManager->INVALID_Item_ID;
            qDebug("error in unit rate calculation");
        }else{
            // calculate the amount
            boqItem.amount = qty * boqItem.unitRate;
        }
    }
    // return the processed item    
    return boqItem;
}

BOQGenerator::~BOQGenerator()
{
    delete pURCalc;
}

CalcData BOQGenerator::getUnitRate(URCData urcData)
{
    return pURCalc->getUnitRate(urcData, MARKUP);
}

void BOQGenerator::setMarkup(float markup)
{
    this->MARKUP = markup;
}
