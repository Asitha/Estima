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

#include "worksheetwidget.h"
#include "ui_worksheetwidget.h"
#include "gui/addremoveurc.h"
#include "addons/printview.h"

#include <QDebug>
#include <QPrinter>


WorkSheetWidget::WorkSheetWidget(ProjData projData, StorageManager& storageManager, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WorkSheetWidget)
{
    ui->setupUi(this);
    this->storageManager = &storageManager;
    this->boqData.projectData =  projData;
    this->boqGenerator = new BOQGenerator(storageManager,this);
    boqGenerator->setMarkup(projData.markup);
    setupBOQTable();
    setupCompleters();
    creatContextMenu();
}

WorkSheetWidget::~WorkSheetWidget()
{
    delete ui;
    delete model;
}

void WorkSheetWidget::on_addItemButton_clicked()
{

    QString searchItem = ui->ItemEdit->text();
    float qty = ui->quantitySpinBox->value();
    if(qty > 0){
        BOQItem boqItem = boqGenerator->getItemData(searchItem, qty);
        if(boqItem.itemStruct.ID != storageManager->INVALID_Item_ID){
             addBOQItem(boqItem);
        }else{
            showError("Error...can't add... ");
        }
    }else
        QMessageBox::warning(this,"Invalid Quatity", "Please enter a valid quantity" );


}

void WorkSheetWidget::setTabIndex(int index)
{
    indexNum = index;
}

int WorkSheetWidget::getTabIndex()
{
    return indexNum;
}

void WorkSheetWidget::setStorageManager(StorageManager& storageManager)
{
    this->storageManager = &storageManager;
}



void WorkSheetWidget::addBOQItem(BOQItem &boqItem)
{
    QStandardItem *item = model->item(0,0);
    model->appendRow(item);


    QModelIndex index= model->index(currentRow   ,   0, QModelIndex());
    model->setData(index, boqItem.itemStruct.refNum);
    index= model->index(currentRow   ,   1, QModelIndex());
    model->setData(index, boqItem.itemStruct.description);
    index= model->index(currentRow    ,   2, QModelIndex());
    model->setData(index, boqItem.qty);
    index= model->index(currentRow    ,   3, QModelIndex());
    model->setData(index, boqItem.itemStruct.unit);
    index= model->index(currentRow    ,   4, QModelIndex());
    model->setData(index, boqItem.unitRate);
    index= model->index(currentRow    ,   5, QModelIndex());
    model->setData(index, boqItem.amount);
    ui->tableView->resizeRowToContents(currentRow);
    currentRow++;

    boqData.itemList.append(boqItem);

}

void WorkSheetWidget::showError(const QString errorMsg)
{
    //TODO: NEED to implement a dialog box error

    qDebug() << errorMsg;
}

void WorkSheetWidget::on_pushButton_3_clicked()
{
    AddRemoveURC urcEdit(storageManager, boqGenerator);
    Item item = storageManager->getItem(ui->ItemEdit->text());
    if(item.ID != StorageManager::INVALID_Item_ID){
        urcEdit.fillData(item);}
    urcEdit.exec();
}



void WorkSheetWidget::on_categoryEdit_lostFocus()
{
    // TODO:

//    QString txt = QString("SELECT item.ID, Description category join item WHERE category.name='%1' and "
//         "category.ID = item.Category_ID").arg(ui->categoryEdit->text());
//    categoryModel.setQuery(txt);
}



void WorkSheetWidget::on_reset_Button_clicked()
{
#ifndef QT_NO_PRINTER
     QPrinter printer(QPrinter::ScreenResolution);
     QPrintPreviewDialog dlg(&printer);
     PrintView view;
     view.setModel(ui->tableView->model());
     connect(&dlg, SIGNAL(paintRequested(QPrinter*)),
             &view, SLOT(print(QPrinter*)));
     dlg.exec();
#endif
}

void WorkSheetWidget::setupCompleters()
{
    QTreeView *itemTreeView =  new QTreeView;
    itemModel.setQuery("SELECT ID, Description FROM item");
    itemCompleter = new QCompleter(&itemModel,this);
    itemCompleter->setPopup(itemTreeView);
    itemTreeView->setRootIsDecorated(false);
    itemTreeView->header()->hide();
    itemTreeView->header()->setStretchLastSection(false);
    itemTreeView->header()->resizeSection(0, 0);
    itemTreeView->header()->setResizeMode(0, QHeaderView::Fixed);
    itemTreeView->header()->setResizeMode(1, QHeaderView::ResizeToContents);

    itemCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    itemCompleter->setCompletionColumn(1);
    itemCompleter->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
    ui->ItemEdit->setCompleter(itemCompleter);

    QTreeView *categoryTreeView =  new QTreeView;
    categoryModel.setQuery("SELECT ID, Name FROM category");
    categoryCompleter = new QCompleter(&categoryModel, this);
    categoryCompleter->setPopup(categoryTreeView);
    categoryTreeView->setRootIsDecorated(false);
    categoryTreeView->header()->hide();
    categoryTreeView->header()->setStretchLastSection(false);
    categoryTreeView->header()->resizeSection(0, 0);
    categoryTreeView->header()->setResizeMode(0, QHeaderView::Fixed);
    categoryTreeView->header()->setResizeMode(1, QHeaderView::ResizeToContents);
    categoryCompleter->setCompletionColumn(1);
    categoryCompleter->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
    categoryCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    ui->categoryEdit->setCompleter(categoryCompleter);


}

void WorkSheetWidget::on_categoryEdit_textEdited(const QString &arg1)
{
   QString txt = QString("SELECT ID, Name FROM category WHERE Name LIKE '\%%1\%' ").arg(arg1);
   categoryModel.setQuery(txt);
}

void WorkSheetWidget::on_ItemEdit_textEdited(const QString &arg1)
{

    QString txt = QString("SELECT ID, Description FROM item WHERE Description LIKE '\%%1\%' ").arg(arg1);
    itemModel.setQuery(txt);
}

void WorkSheetWidget::setupBOQTable()
{
    currentRow = 0;
    model = new QStandardItemModel(25, 6, this);
    ui->tableView->setModel(model);
    ui->tableView->setColumnWidth(0, 80);
    ui->tableView->setColumnWidth(1, 300);
    ui->tableView->setColumnWidth(2, 100);
    ui->tableView->setColumnWidth(3, 100);
    ui->tableView->setColumnWidth(4, 100);
    ui->tableView->setColumnWidth(5, 100);
    ui->tableView->setItemDelegate(&boqItemDelgate);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);


    connect(ui->tableView, SIGNAL(pressed(QModelIndex)), this, SLOT(showPopupMenu(QModelIndex )));
    QStringList headers;
    headers << "Ref No" << "Description"<< "Qty"<< "Unit" << "Rate"<< "Amount";
    model->setHorizontalHeaderLabels(headers);
    ui->tableView->horizontalHeader()->stretchLastSection();
}

// TODO: method needs renaming
void WorkSheetWidget::showPopupMenu(QModelIndex index)
{
    ui->tableView->selectRow(index.row());

    Qt::MouseButtons  buttons =  QApplication::mouseButtons();
    if(buttons == Qt::LeftButton){
        qDebug()<< "left";
    }
//    QTreeView *popup = new QTreeView(this);

//    popup->header()->hide();
//    popup->show();
}

void WorkSheetWidget::creatContextMenu()
{
    addRowAbove = new QAction("Add row above", this);
    addRowBelow = new QAction("Add row Below", this);
    ui->tableView->addAction(addRowAbove);
    ui->tableView->addAction(addRowBelow);
    ui->tableView->setContextMenuPolicy(Qt::ActionsContextMenu);
}
