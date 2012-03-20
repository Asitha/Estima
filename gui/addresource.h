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

#ifndef ADDRESOURCE_H
#define ADDRESOURCE_H

#include <QDialog>

#include "storage/storagemanager.h"

namespace Ui {
    class AddResource;
}

class AddResource : public QDialog
{
    Q_OBJECT

public:
    explicit AddResource(StorageManager &storageManager,QWidget *parent = 0);
    ~AddResource();

private slots:
    void on_applyButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::AddResource *ui;
    StorageManager *storageManager;
};

#endif // ADDRESOURCE_H
