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

#include <QDebug>

ResourceDataBrowser::ResourceDataBrowser(StorageManager &storageManager, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ResourceDataBrowser)
{
    ui->setupUi(this);
    this->storageManager = &storageManager;
    setWindowTitle(tr("Resource Data Browser"));

    initActions();
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
    ui->tableView->resizeColumnsToContents();
    ui->tableView->resizeRowsToContents();
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
}

void ResourceDataBrowser::fillUIData()
{
    QStringList typeList;
    typeList << "Material" << "Labour" ;
    ui->resourceTypeComboBox->addItems(typeList);
}

void ResourceDataBrowser::initActions()
{
    removeResource = new QAction(tr("Remove Resource"), this);
    addResource = new QAction(tr("Add Resource"), this);
    editResource = new QAction(tr("Edit Resource"), this);

    ui->tableView->addAction(addResource);
    ui->tableView->addAction(editResource);
    ui->tableView->addAction(removeResource);

    ui->tableView->setContextMenuPolicy(Qt::ActionsContextMenu);
    ui->tableView->setAlternatingRowColors(true);
    connect(removeResource, SIGNAL(triggered()), this, SLOT(removeSelectedResource()));

}

void ResourceDataBrowser::removeSelectedResource()
{
    // Even when an item is not selected this will be activated;
    QModelIndex idx = ui->tableView->selectionModel()->currentIndex();

    ui->tableView->model()->removeRow(idx.row(), idx);


}

void ResourceDataBrowser::addNewResource()
{


}

void ResourceDataBrowser::editSelectedResource()
{
}
