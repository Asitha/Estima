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

#include "resourcedatabrowser.h"
#include "ui_resourcedatabrowser.h"
#include "addresource.h"

ResourceDataBrowser::ResourceDataBrowser(StorageManager &storageManager, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ResourceDataBrowser)
{
    ui->setupUi(this);
    this->storageManager = &storageManager;
    setWindowTitle(tr("Resource Data Browser"));
    setupResourceTable();
}

ResourceDataBrowser::~ResourceDataBrowser()
{
    delete ui;
}

void ResourceDataBrowser::on_closeButton_clicked()
{
    this->~ResourceDataBrowser();
}

void ResourceDataBrowser::on_AddResourceButton_clicked()
{
    AddResource rsrsForm(*storageManager);
    rsrsForm.exec();
}

void ResourceDataBrowser::setupResourceTable()
{
    model = new QSqlTableModel(this);
    model->setTable("resource");
    model->select();
    model->removeColumn(0);
    ui->tableView->setModel(model);
    ui->tableView->resizeRowsToContents();
    ui->tableView->resizeColumnsToContents();
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
}
