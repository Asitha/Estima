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
#include "core/units.h"


#include <QMessageBox>

AddResource::AddResource(StorageManager &storageManager, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddResource)
{
    ui->setupUi(this);
    this->pStorageManager = &storageManager;
    setWindowTitle(tr("Resource Properties"));
    fillUIData();
}

AddResource::~AddResource()
{
    delete ui;
}

void AddResource::on_applyButton_clicked()
{
    Resource rsrc;
    QString errMsg;
    if(validateData(&errMsg)){
        rsrc.name = ui->nameLineEdit->text();
        rsrc.unit = ui->unitRsrcComboBox->currentText();
        if(ui->typeComboBox->currentText().compare("Material")){
            rsrc.type = 'M';
        }else {
            rsrc.type = 'L';
        }
        rsrc.description = ui->descTextEdit->toPlainText();
        rsrc.quantity = ui->quantityDoubleSpinBox->value();
        rsrc.rate = ui->rateDoubleSpinBox->value();
        QDate  date;
        rsrc.lastModified = date.currentDate().toString("dd-MMM-yyyy");
        pStorageManager->addResource(rsrc);
        this->close();
    }else{
        QMessageBox::warning(this, tr("Invalid Input"), errMsg);

    }

}

void AddResource::on_cancelButton_clicked()
{
    this->close();
}

bool AddResource::validateData(QString *ErrorMessage)
{
    bool isValid = true;
    *ErrorMessage = ("<h5>Re-enter data avoiding following errors </h5> <ul>");
    if(ui->nameLineEdit->text() == ""){
        isValid = false;
        ErrorMessage->append(" <li>Name can't be empty</li>");
    }
    if(ui->quantityDoubleSpinBox->value() == 0){
        isValid = false;
        ErrorMessage->append(" <li>Quantity should be a non zero value</li>");
    }
    if(ui->rateDoubleSpinBox->value() == 0 ){
        isValid = false;
        ErrorMessage->append(" <li>Rate should be a non zero value</li>");
    }
    ErrorMessage->append(" </ul>");

    return isValid;
}

void AddResource::fillUIData()
{
    QStringList types;
    types << "Material" << "Labour";
    ui->typeComboBox->addItems(types);
    ui->unitRsrcComboBox->addItems(Units::getInstance()->getUnits());
}
