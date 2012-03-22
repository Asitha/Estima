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

#include "projectpropertiesform.h"
#include "ui_projectpropertiesform.h"
#include "core/units.h"

#include <QMessageBox>

ProjectPropertiesForm::ProjectPropertiesForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProjectPropertiesForm)
{
    ui->setupUi(this);
    setWindowTitle(tr("Project Properties"));
    ;
    ui->unitSystemComboBox->addItems(Units::getInstance()->getUnitSystems());
}

ProjectPropertiesForm::~ProjectPropertiesForm()
{
    delete ui;
}

void ProjectPropertiesForm::on_cancelButton_clicked()
{
    this->close();
}

void ProjectPropertiesForm::on_createProjButton_clicked()
{
    ProjData data;
    if(isValidInput()){
    data.name = ui->projNameLineEdit->text();
    data.projRefNum = ui->referenceNumberLineEdit->text();
    data.preparedBy = ui->preparedByLineEdit->text();
    data.currency = ui->currencyLineEdit->text();
    data.unitSystem = ui->unitSystemComboBox->currentText();
    data.markup = ui->markupSpinBox->value();

    emit newProjData(data);
    }else{
        QMessageBox::warning(this,"Invalid Information", "Invalid data entered");
    }

}

bool ProjectPropertiesForm::isValidInput()
{
    // user input needs to be validated

    return true;
}
