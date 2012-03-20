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

#include "units.h"

Units::Units(StorageManager *storageManager, QObject *parent) :
    QObject(parent)
{
    this->storageManager = storageManager;
    initUnitLists();
}
Units::Units(QObject *parent) :
    QObject(parent)
{
    initUnitLists();
}

void Units::initUnitLists()
{
    unitSystems << "Metric" << "Imperial";

    stdUnits << "metre" << "square metre" << "cubic metre"  << "milli metre" << "number" << "kilogram" << "tonne" << "hour";

    nonStdUnits << "cube" ;

}

QStringList Units::getStdUnits()
{
    return stdUnits;
}

QStringList Units::getNonStdUnits()
{
    return nonStdUnits;
}

QStringList Units::getUnits()
{
    QStringList units;
    units << stdUnits << nonStdUnits;
    return units;
}

QStringList Units::getUnitSystems()
{
    return unitSystems;
}
