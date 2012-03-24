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

#ifndef BOQGENERATOR_H
#define BOQGENERATOR_H

#include <QObject>

#include "core/unitratecalculator.h"
#include "storage/storagemanager.h"
#include "storage/item.h"

/**
  * Struct to store data related to the item displayed in the BOQ.
  */
struct BOQItem{

    Item itemStruct;
    double qty;
    double amount;
    double unitRate;

};


/**
  * BOQCreation class.Takes
  */
class BOQGenerator : public QObject
{
    Q_OBJECT
public:
    // singleton needed
     explicit BOQGenerator(StorageManager& pStorageManager, QObject *parent = 0);
     void setItem(QString itemName);
     void setStorageManager(StorageManager& pStorageManager);
     BOQItem getItemData(QString itemDesc, float qty);
     void setMarkup(float markup);
     ~BOQGenerator();
     CalcData getUnitRate(URCData urcData);

public slots:

private:
    StorageManager *pStorageManager;
    UnitRateCalculator *pURCalc;

    float MARKUP;
};

#endif // BOQGENERATOR_H
