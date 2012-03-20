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

#include "addremoveurc.h"
#include "ui_addremoveurc.h"


AddRemoveURC::AddRemoveURC(StorageManager *storageManager,BOQGenerator *boqGenerator, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddRemoveURC)
{
    ui->setupUi(this);
    this->storageManager = storageManager;
    this->boqGenerator =  boqGenerator;
    setWindowTitle("unit rate calculation properties");

    setupCompleters();
    fillUIData();

}

AddRemoveURC::~AddRemoveURC()
{

    delete ui;

}

void AddRemoveURC::on_AddRsrcPushButton_clicked()
{

    ResourceURC rsrcURC;
    rsrcURC.name = ui->rsrcNamelineEdit->text();
    QList<Resource> list = storageManager->getResource(rsrcURC.name);


    if(!list.isEmpty() && list.first().ID != StorageManager::INVALID_Resource_ID){
        rsrcURC.quantity = ui->rsrcQtySpinBox->value();
        rsrcURC.ID = list.first().ID;
        rsrcURC.type = list.first().type.toAscii();
        urcData.resources.append(rsrcURC);
        addURCItem(rsrcURC);

    }else{
        // ERROR HANDLING
    }


}


void AddRemoveURC::on_URCEditButtonBox_accepted()
{
    QString itemDesc = ui->itemlineEdit->text();
    int row =    ui->itemlineEdit->completer()->currentRow();
//    QModelIndex index = ui->itemlineEdit->completer()->model()->index(row, 0);
//    QVariant i = ui->itemlineEdit->completer()->model()->data(index);
//    int id;
//    i.convert(id);
    item =  storageManager->getItem(itemDesc);
    if(item.ID != StorageManager::INVALID_Item_ID){

        urcData.convConst = ui->convConstSpinBox->value();
        urcData.URCQuantity = ui->itemQtySpinBox->value();
        urcData.itemID = item.ID;
        urcData.unitURC = ui->calcUnitComboBox->currentText();
//        urcData.description = ui->itemDescTextEdit->toPlainText();

        storageManager->saveURC(item.refNum, urcData);
    }else{
        // qmessage needed
    }

    qDebug("Acc");
}

void AddRemoveURC::on_URCEditButtonBox_rejected()
{
    qDebug("Rej");
}

bool AddRemoveURC::addURCItem(ResourceURC rsrcURCData)
{
    QStandardItem *item = resourceTableModel->item(0,0);
    resourceTableModel->appendRow(item);
    Resource rsrc = storageManager->getResource(rsrcURCData.ID);
    CalcData calcData = boqGenerator->getUnitRate(urcData);
    ui->stdUnitRateLabel->setText(QString("Standard unit rate(with markup) : %1").arg(calcData.unitRate));

    if(rsrc.ID != StorageManager::INVALID_Resource_ID){
        QModelIndex index= resourceTableModel->index(currentRow   ,   0, QModelIndex());
        resourceTableModel->setData(index, rsrc.name);
        index= resourceTableModel->index(currentRow   ,   1, QModelIndex());
        resourceTableModel->setData(index, rsrcURCData.quantity);
        index= resourceTableModel->index(currentRow    ,   2, QModelIndex());
        resourceTableModel->setData(index, rsrc.unit);
        index= resourceTableModel->index(currentRow    ,   3, QModelIndex());
        resourceTableModel->setData(index, rsrc.rate);
        index= resourceTableModel->index(currentRow    ,   4, QModelIndex());
        resourceTableModel->setData(index, rsrc.rate*rsrcURCData.quantity);
        index= resourceTableModel->index(currentRow+1    ,   4, QModelIndex());
        resourceTableModel->setData(index, calcData.unitRateBM);
        //        resourceTableModel->setData(index, ur);
//        index= resourceTableModel->index(currentRow    ,   5, QModelIndex());


        currentRow++;



        return true;
    }else
        return false;
}

bool AddRemoveURC::fillData(Item item)
{
    urcData = storageManager->retrieveURC(item.refNum);
    ui->itemlineEdit->setText(item.description);
    ui->convConstSpinBox->setValue(urcData.convConst);
    ui->itemQtySpinBox->setValue(urcData.URCQuantity);
    int index  = ui->calcUnitComboBox->findText(urcData.unitURC);
    ui->calcUnitComboBox->setCurrentIndex(index);
    ui->textBrowser->setText(urcData.description);
//    ui->stdUnitComboBox->setItemText(urcData.);
    foreach(ResourceURC resrcURC, urcData.resources){
        addURCItem(resrcURC);
    }


}

void AddRemoveURC::on_itemlineEdit_editingFinished()
{
    Item item = storageManager->getItem(ui->itemlineEdit->text());
    if(item.ID != StorageManager::INVALID_Item_ID){
        currentRow = 0;
        clearTable();
        fillData(item);
    }
}

void AddRemoveURC::setupCompleters()
{
    itemModel.setQuery("SELECT ID, Description FROM item");
    QTreeView *treeView =  new QTreeView;

    itemCompleter = new QCompleter(&itemModel,this);
    itemCompleter->setPopup(treeView);
    treeView->setRootIsDecorated(false);
    treeView->header()->hide();
    treeView->header()->setStretchLastSection(false);
    treeView->header()->setResizeMode(0, QHeaderView::Stretch);
    treeView->header()->setResizeMode(1, QHeaderView::ResizeToContents);

    itemCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    itemCompleter->setCompletionColumn(1);
    itemCompleter->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
    ui->itemlineEdit->setCompleter(itemCompleter);

    QTreeView *resTreeView = new QTreeView ;
    resTreeView->setRootIsDecorated(false);
    resTreeView->header()->hide();
//    resTreeView->header()->setStretchLastSection(false);
//    resTreeView->header()->setResizeMode(0, QHeaderView::ResizeToContents);
//    resTreeView->header()->setResizeMode(1, QHeaderView::ResizeToContents);

    resourceModel.setQuery("SELECT ID, NAME From resource");
    resourceCompleter = new QCompleter(&resourceModel, this);
    resourceCompleter->setPopup(resTreeView);
    resourceCompleter->setCompletionColumn(1);
    resourceCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    resourceCompleter->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
    ui->rsrcNamelineEdit->setCompleter(resourceCompleter);

}

void AddRemoveURC::fillUIData()
{
    // table model
    currentRow = 0;
    resourceTableModel = new QStandardItemModel(5, 6 , this);
    ui->URCTableView->setModel(resourceTableModel);

    QStringList headers;
    headers << "Name" << "Qty" << "Unit" << "Rate" << "Amount" << "Description";
    resourceTableModel->setHorizontalHeaderLabels(headers);

    // combo box data
    units = new Units(storageManager, this);
    ui->calcUnitComboBox->addItems(units->getUnits());
    ui->calcUnitComboBox->setDuplicatesEnabled(false);

    ui->stdUnitComboBox->addItems(units->getStdUnits());
    ui->stdUnitComboBox->setDuplicatesEnabled(false);
}

void AddRemoveURC::clearTable()
{
    resourceTableModel->clear();
    QStringList headers;
    headers << "Name" << "Qty" << "Unit" << "Rate" << "Amount" << "Description";
    resourceTableModel->setHorizontalHeaderLabels(headers);

}

void AddRemoveURC::on_rsrcNamelineEdit_editingFinished()
{
    ResourceURC rsrcURC;
    rsrcURC.name = ui->rsrcNamelineEdit->text();
    QList<Resource> list = storageManager->getResource(rsrcURC.name);
    if(!list.isEmpty()){
       ui->rsrcUnitLabel->setText(list.first().unit);
    }
}

void AddRemoveURC::on_itemlineEdit_textEdited(const QString &arg1)
{
    QString txt = ui->itemlineEdit->text();
    txt = QString("SELECT ID, Description FROM item WHERE Description LIKE '\%%1\%' ").arg(txt);
    itemModel.setQuery(txt);

}

void AddRemoveURC::on_rsrcNamelineEdit_textEdited(const QString &arg1)
{

    QString txt = QString("SELECT ID, Name FROM resource WHERE name LIKE '\%%1\%' ").arg(arg1);
    resourceModel.setQuery(txt);

}
