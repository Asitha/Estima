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

#include "addresource.h"
#include "ui_addresource.h"

AddResource::AddResource(StorageManager &storageManager, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddResource)
{
    ui->setupUi(this);
    this->storageManager = &storageManager;
    setWindowTitle("Resource Properties");
    QStringList types;
    types << "Material" << "Labour";
    ui->typeComboBox->addItems(types);
}

AddResource::~AddResource()
{
    delete ui;
}

void AddResource::on_applyButton_clicked()
{
    Resource rsrc;
//    ui->nameLineEdit->text() != "" ?
    rsrc.name = "Test";
    rsrc.quantity = 55.0;
    qDebug() << storageManager->addResource(rsrc);
    this->close();
}

void AddResource::on_cancelButton_clicked()
{
    this->close();
}
