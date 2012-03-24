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

#ifndef UNITRATECALCULATOR_H
#define UNITRATECALCULATOR_H

#include <QObject>

#include "storage/storagemanager.h"

struct CalcData {

    float markup;
    QString refNum;         // item reference number
    float unitRateBM;       // before markup
    float unitRate;         // after markup
    float rateBeforeConv;
    float convConst;

};


class UnitRateCalculator : public QObject
{
    Q_OBJECT
public:
    explicit UnitRateCalculator(StorageManager& pStorageManager, QObject *parent = 0);
    CalcData getUnitRate(QString& refNum, float& markup);
    CalcData getUnitRate(URCData urcData, float& markup);
    // should be changed to this
    //  float getUnitRate(QString refNum, float markup, QString *errorMsg);
    void setStorageManager(StorageManager& pStorageManager);

signals:

public slots:
private:
    float calculateUnitRate(URCData dataSet);
    StorageManager *pStorageManager;
    CalcData calcData;
};

#endif // UNITRATECALCULATOR_H
