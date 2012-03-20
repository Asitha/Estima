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


#ifndef UNITS_H
#define UNITS_H


#include "storage/storagemanager.h"
#include <QObject>

struct unit{

    QString symbol;
    QString name;
    bool isStd;


};

class Units : public QObject
{
    Q_OBJECT
public:
    explicit Units(StorageManager *storageManager, QObject *parent = 0);
    explicit Units(QObject *parent = 0);

    QStringList getStdUnits();
    QStringList getUnits();
    QStringList getNonStdUnits();
    QStringList getUnitSystems();


signals:

public slots:

private:
    StorageManager *storageManager;
    QStringList stdUnits;
    QStringList nonStdUnits;
    QStringList unitSystems;
    void initUnitLists();
};

#endif // UNITS_H
