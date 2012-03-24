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

#include "unitratecalculator.h"
#include "storage/item.h"
#include <QDebug>

UnitRateCalculator::UnitRateCalculator(StorageManager& storageManager, QObject *parent) :
    QObject(parent)
{
    this->pStorageManager = &storageManager;

}


/**
  * Calculates the unit rate for the given item with reference number (refNum) and adds the markup for it.
  * markup should be given as a float value (eg: 30% markup should be given as 0.3f)
  * if error occur minus value will be returned.
  */
CalcData UnitRateCalculator::getUnitRate(QString& refNum, float& markup)
{
    URCData urcData = pStorageManager->retrieveURC(refNum);
    return  getUnitRate(urcData, markup);
}



CalcData UnitRateCalculator::getUnitRate(URCData urcData, float& markup)
{

    calcData.markup = markup;
    if(urcData.itemID != pStorageManager->INVALID_Item_ID ){
       calcData.unitRateBM = calculateUnitRate(urcData);
       calcData.unitRate = calcData.unitRateBM * (1+markup);

    }else {
        calcData.unitRate = -1; /*any negative value*/

    }

    return calcData;
}




void UnitRateCalculator::setStorageManager(StorageManager& storageManager)
{
    this->pStorageManager = &storageManager;
}

float UnitRateCalculator::calculateUnitRate(URCData dataSet)
{
    float rate=0;
    float stdRate = 0;
    int ID;
    Resource RsrcAtDB;

    QList<ResourceURC>::const_iterator resorcePtr, end;
    end= dataSet.resources.end();

    // iterate though resources and add the rates of each item.
    for(resorcePtr = dataSet.resources.begin() ; resorcePtr< end; ++resorcePtr){

        ID = (*resorcePtr).ID;
        RsrcAtDB = pStorageManager->getResource(ID);

        if(RsrcAtDB.ID != pStorageManager->INVALID_Item_ID){
            //TODO: NO CHECK FOR COMPATIBILTY DONE (unit comparison of xml file and DB should be done)
            rate += (*resorcePtr).quantity * (RsrcAtDB.rate/RsrcAtDB.quantity);

        }else{
            rate = -1;
            break;
        }
    }

    calcData.rateBeforeConv = rate;
    // check for correct calculation
    if(rate != -1 ){
        stdRate = rate/ dataSet.convConst;
    }else
        stdRate = -1;


    return stdRate;
}

//TODO: create a method to store new unit rate calculations
