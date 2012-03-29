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

#include <QTableWidget>
#include "addremoveurc.h"
#include "ui_addremoveurc.h"


AddRemoveURC::AddRemoveURC(StorageManager *storageManager,BOQGenerator *boqGenerator, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddRemoveURC)
{
    ui->setupUi(this);
    this->pStorageManager = storageManager;
    this->pBOQGenerator =  boqGenerator;
    setWindowTitle(tr("Unit Rate Calculation Properties"));

    ui->convConstSpinBox->setRange(0.01, 1000000);
    ui->rsrcQtySpinBox->setRange(0.01, 1000000000);
    setupResourceTable();
    setupCompleters();
    fillUIData();
    createContextMenu();

}

AddRemoveURC::~AddRemoveURC()
{
    delete ui;
}

void AddRemoveURC::on_AddRsrcPushButton_clicked()
{
    ResourceURC rsrcURC;
    rsrcURC.name = ui->rsrcNamelineEdit->text();
    QList<Resource> list = pStorageManager->getResource(rsrcURC.name);


    if(!list.isEmpty() && list.first().ID != StorageManager::INVALID_Resource_ID){
        rsrcURC.quantity = ui->rsrcQtySpinBox->value();
        rsrcURC.ID = list.first().ID;
        rsrcURC.type = list.first().type.toAscii();        
        addToURCTable(rsrcURC);

    }else{
        // ERROR HANDLING
    }


}


void AddRemoveURC::on_URCEditButtonBox_accepted()
{
    QString itemDesc = ui->itemlineEdit->text();

    item =  pStorageManager->getItem(itemDesc);
    if(item.ID != StorageManager::INVALID_Item_ID){

        urcData.convConst = ui->convConstSpinBox->value();
        urcData.URCQuantity = ui->itemQtySpinBox->value();
        urcData.itemID = item.ID;
        urcData.unitURC = ui->calcUnitComboBox->currentText();


        if(urcData.resources.isEmpty()){
            QMessageBox::warning(this, tr("No Resources added"),
               tr("Please add resources before saving unit rate calculation"));
        }else{
            pStorageManager->saveURC(item.refNum, urcData);
            qDebug("URC data stored");
        }
    }else{
        // qmessage needed
    }
    this->deleteLater();
    qDebug("Acc");
}

void AddRemoveURC::on_URCEditButtonBox_rejected()
{
    qDebug("Rej");
}



bool AddRemoveURC::fillData(Item item)
{
    urcData = pStorageManager->retrieveURC(item.refNum);

    if(urcData.itemID != StorageManager::INVALID_Item_ID){
        ui->itemlineEdit->setText(item.description);
        ui->convConstSpinBox->setValue(urcData.convConst);
        ui->itemQtySpinBox->setValue(urcData.URCQuantity);
        int index  = ui->calcUnitComboBox->findText(urcData.unitURC);
        ui->calcUnitComboBox->setCurrentIndex(index);
        ui->textBrowser->setText(urcData.description);
        index  = ui->stdUnitComboBox->findText(item.unit);
        ui->stdUnitComboBox->setCurrentIndex(index);
        foreach(ResourceURC resrcURC, urcData.resources){
            addToURCTable(resrcURC);
        }
        return true;
    }
    return false;
}

void AddRemoveURC::on_itemlineEdit_editingFinished()
{
    ShowItemData();
}



void AddRemoveURC::fillUIData()
{
    // combo box data
    pUnits = Units::getInstance();
    ui->calcUnitComboBox->addItems(pUnits->getUnits());
    ui->calcUnitComboBox->setDuplicatesEnabled(false);

    ui->stdUnitComboBox->addItems(pUnits->getStdUnits());
    ui->stdUnitComboBox->setDuplicatesEnabled(false);
}



void AddRemoveURC::on_rsrcNamelineEdit_editingFinished()
{

    QString resourceName = ui->rsrcNamelineEdit->text();
    QList<Resource> list = pStorageManager->getResource(resourceName);
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


void AddRemoveURC::clearTable()
{
    ui->URCTableWidget->clear();
    setupResourceTable();
}


/**
  * name - name of the string that it should search for
  * duplicityChkCol - the column in which the duplicate strings should be checked for
  */
int AddRemoveURC::rowToAdd(QString name, int duplicityChkCol, bool *itemExist)
{
    QList<QTableWidgetItem *> itemList;
    itemList = ui->URCTableWidget->findItems(name, Qt::MatchExactly);

    for(int i =0; i < itemList.size(); i++){
        if(itemList.at(i)->column() == duplicityChkCol){
           *itemExist = true;
           return itemList.at(i)->row();

        }
    }
    *itemExist = false;
    int row = ui->URCTableWidget->rowCount();
    ui->URCTableWidget->insertRow(row);
    return row;


//    if(!hasDuplicate){
//        editingRow = firstBlankRow;
//        if(ui->URCTableWidget->rowCount() == (firstBlankRow+1) || ui->URCTableWidget->rowCount() == 0 ){
//            ui->URCTableWidget->insertRow(firstBlankRow);
//        }
//        ++firstBlankRow;
//    }
//    qDebug() << "[Adding item]First blank row " << firstBlankRow;
//    return editingRow;
}


bool AddRemoveURC::addToURCTable(ResourceURC rsrcURCData)
{
    QTableWidgetItem *items[6];

    Resource rsrc = pStorageManager->getResource(rsrcURCData.ID);
    CalcData calcData = pBOQGenerator->getUnitRate(urcData);
    ui->stdUnitRateLabel->setText(QString("Standard unit rate(with markup)"
                              " : %1").arg(calcData.unitRate));

    if(rsrc.ID != StorageManager::INVALID_Resource_ID){

        bool itemExist = false;
        int editingRow = rowToAdd(rsrcURCData.name, 0, &itemExist);
        if(itemExist){
            urcData.resources.operator [](editingRow) = rsrcURCData;
        }else{
            urcData.resources.append(rsrcURCData);
        }

        items[0] = new QTableWidgetItem(rsrc.name);
        double qty =  rsrcURCData.quantity;
        items[1] = new QTableWidgetItem(QString().setNum(qty, 'f', 2));
        items[1]->setTextAlignment(Qt::AlignRight);
        items[2] = new QTableWidgetItem(rsrc.unit);
        items[2]->setTextAlignment(Qt::AlignRight);
        items[3] = new QTableWidgetItem(QString().setNum(rsrc.rate, 'f',2));
        items[3]->setTextAlignment(Qt::AlignRight);
        items[4] = new QTableWidgetItem(QString().setNum(qty* rsrc.rate, 'f',2));
        items[4]->setTextAlignment(Qt::AlignRight);
        items[5] = new QTableWidgetItem(rsrc.description);

        for(int col = 0; col < 6; ++col){
            ui->URCTableWidget->setItem(editingRow, col, items[col]);
        }
        ui->URCTableWidget->resizeRowToContents(editingRow);
        return true;
    }else{
        return false;
    }
}



void AddRemoveURC::setupCompleters()
{
    itemModel.setQuery("SELECT Description FROM item");
    QTreeView *treeView =  new QTreeView;

    pItemCompleter = new QCompleter(&itemModel,this);
    pItemCompleter->setPopup(treeView);
    treeView->setRootIsDecorated(false);
    treeView->header()->hide();
    treeView->hideColumn(0);

    treeView->header()->setStretchLastSection(true);
    treeView->header()->setResizeMode(0, QHeaderView::ResizeToContents);

    pItemCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    pItemCompleter->setCompletionColumn(1);
    pItemCompleter->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
    ui->itemlineEdit->setCompleter(pItemCompleter);



    QTreeView *resTreeView = new QTreeView ;
    resourceModel.setQuery("SELECT Name FROM resource");
    pResourceCompleter = new QCompleter(&resourceModel, this);
    pResourceCompleter->setPopup(resTreeView);
    resTreeView->setRootIsDecorated(false);
    resTreeView->header()->hide();
    resTreeView->hideColumn(0);
    resTreeView->header()->setStretchLastSection(true);
    resTreeView->header()->setResizeMode(0, QHeaderView::ResizeToContents);

    pResourceCompleter->setCompletionColumn(1);
    pResourceCompleter->setCompletionRole(Qt::DisplayRole);
    pResourceCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    pResourceCompleter->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
    ui->rsrcNamelineEdit->setCompleter(pResourceCompleter);
}


void AddRemoveURC::setupResourceTable()
{
    activeRow = 0;

    ui->URCTableWidget->setColumnCount(6);
    ui->URCTableWidget->setRowCount(0);
    ui->URCTableWidget->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked);
    ui->URCTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    QStringList headers;
    headers << "Name" << "Qty" << "Unit" << "Rate" << "Amount" << "Description";
    ui->URCTableWidget->setHorizontalHeaderLabels(headers);
    ui->URCTableWidget->horizontalHeader()->setStretchLastSection(true);

    ui->URCTableWidget->setColumnWidth(0, 150);
    ui->URCTableWidget->setColumnWidth(1, 100);
    ui->URCTableWidget->setColumnWidth(2, 50);
    ui->URCTableWidget->setColumnWidth(3, 100);
    ui->URCTableWidget->setColumnWidth(4, 100);
    ui->URCTableWidget->setColumnWidth(5, 150);

    ui->URCTableWidget->setAlternatingRowColors(true);
}

QList<ResourceURC> AddRemoveURC::getTableData()
{
    QList<ResourceURC> rsrcURCList;

    // TODO:
    return rsrcURCList;
}

void AddRemoveURC::ShowItemData()
{
    urcData.resources.clear();
    Item item = pStorageManager->getItem(ui->itemlineEdit->text());
    if(item.ID != StorageManager::INVALID_Item_ID){
        activeRow = 0;
        clearTable();
        fillData(item);
    }
}

/**
  * creates the context menu of the URCTableWidget
  */
void AddRemoveURC::createContextMenu()
{
    pRemoveResourceAct = new QAction("Remove resource", this);
    ui->URCTableWidget->addAction(pRemoveResourceAct);

    ui->URCTableWidget->setContextMenuPolicy(Qt::ActionsContextMenu);
    connect(pRemoveResourceAct, SIGNAL(triggered()), this, SLOT(removeRow()));
}


/**
  * SLOT that reacts to the context menu signal for removing items.
  */
void AddRemoveURC::removeRow()
{
    QModelIndex idx = ui->URCTableWidget->selectionModel()->currentIndex();
    ResourceURC rsrcURC;
    QString rsrsNameInTable;
    ui->URCTableWidget->removeRow(idx.row());
    urcData.resources.removeAt(idx.row());
}
