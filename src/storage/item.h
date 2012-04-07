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

#ifndef ITEM_H
#define ITEM_H

#include <QString>
#include <QList>
/**
  * Header file contains all the structure declerations used by
  * storage classes to transfer data.
  */


/**
  * Structure to transfer item (in BOQ) details among objects.
  */
struct Item {
    int ID;
    int categoryID;
    QString category;
    QString refNum;             // standard reference Number( eg SLS:573 number)
    QString description;
    QString unit;
    QString dateModified;

};

/**
  * Structure to transfer resource (eg: cement, sand) details among objects.
  */
struct Resource{
    int ID;
    QString name;
    QString unit;
    float rate;
    QString lastModified;
    QChar type;
    float quantity;
    QString description;

};

/**
  * struct to transfer resource data (eg: cement, sand) to and from unit rate calculation (URC) resource files.
  */

struct ResourceURC {

    int ID;                     // resourceID
    QString name;               // resource name
    float quantity;             // quantity used in the calculation
    char type;                  // Labour or Material

};


/**
  * Item unit rate is calculated using this data. Storage classes use this to transfer data
  * among classes
  */
struct URCData{

    int itemID;
    QList<ResourceURC> resources;
    QString unitURC;            // URC done in this unit
    double URCQuantity;          // quantity of the unit rate done in unitURC
    double convConst;            // unitURC to "item unit" conversion constant
    QString description;        // details about unit rate calculation
};


#endif // ITEM_H
