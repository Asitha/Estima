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
#include <QTextCharFormat>

const QString FILE_FORMAT = ".xest";

WorkSheetWidget::WorkSheetWidget(ProjData projData, StorageManager& storageManager, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WorkSheetWidget)
{
    ui->setupUi(this);
    this->pStorageManager = &storageManager;
    this->boqData.projectData =  projData;
    this->pBOQGenerator = new BOQGenerator(storageManager,this);
    pBOQGenerator->setMarkup(projData.markup);
    currentSavePath = "";
    ui->quantitySpinBox->setRange(0.00, 99999999);

    setupBOQTable();
    setupCompleters();
    creatContextMenu();
}


WorkSheetWidget::~WorkSheetWidget()
{
    delete ui;
    delete pBOQTableModel;
}

void WorkSheetWidget::on_addItemButton_clicked()
{
    QString searchItem = ui->ItemEdit->text();
    float qty = ui->quantitySpinBox->value();
    if(qty > 0){
        BOQItem boqItem = pBOQGenerator->getItemData(searchItem, qty);
        if(boqItem.itemStruct.ID != pStorageManager->INVALID_Item_ID){
             addBOQItem(boqItem);
        }else{
            showError("Error...can't add... ");
        }
    }else{
        QMessageBox::warning(this,tr("Invalid Quatity"), tr("<p>Enter a non zero value for quantity</p>") );
    }
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
    this->pStorageManager = &storageManager;
}



void WorkSheetWidget::addBOQItem(BOQItem &boqItem)
{
    bool itemExist = false;
    int editRow = getActiveRow(boqItem.itemStruct.refNum, &itemExist);
    QModelIndex index= pBOQTableModel->index(editRow   ,   0, QModelIndex());

    if(itemExist){
        index= pBOQTableModel->index(editRow    ,   2, QModelIndex());
        pBOQTableModel->setData(index, QString().setNum(boqItem.qty, 'f', 2));
        index= pBOQTableModel->index(editRow    ,   3, QModelIndex());
        pBOQTableModel->setData(index, boqItem.itemStruct.unit);
        index= pBOQTableModel->index(editRow    ,   4, QModelIndex());
        pBOQTableModel->setData(index, QString().setNum(boqItem.unitRate,'f', 2));
        index= pBOQTableModel->index(editRow    ,   5, QModelIndex());
        pBOQTableModel->setData(index, QString().setNum(boqItem.amount, 'f', 2));

    }else{

        pBOQTableModel->setData(index, boqItem.itemStruct.refNum);
        index= pBOQTableModel->index(editRow   ,   1, QModelIndex());
        pBOQTableModel->setData(index, boqItem.itemStruct.description);
        index= pBOQTableModel->index(editRow    ,   2, QModelIndex());
        pBOQTableModel->setData(index, QString().setNum(boqItem.qty, 'f', 2));
        index= pBOQTableModel->index(editRow    ,   3, QModelIndex());
        pBOQTableModel->setData(index, boqItem.itemStruct.unit);
        index= pBOQTableModel->index(editRow    ,   4, QModelIndex());
        pBOQTableModel->setData(index, QString().setNum(boqItem.unitRate,'f', 2));
        index= pBOQTableModel->index(editRow    ,   5, QModelIndex());
        pBOQTableModel->setData(index, QString().setNum(boqItem.amount, 'f', 2));
    }
    ui->tableView->resizeRowToContents(editRow);

}

void WorkSheetWidget::addBOQItem(BOQTableItem &boqTableItem )
{
    int editRow = getActiveRow(boqTableItem.refNum);
    QModelIndex index= pBOQTableModel->index(editRow   ,   0, QModelIndex());
    pBOQTableModel->setData(index, boqTableItem.refNum);
    index= pBOQTableModel->index(editRow   ,   1, QModelIndex());
    pBOQTableModel->setData(index, boqTableItem.description);
    index= pBOQTableModel->index(editRow    ,   2, QModelIndex());
    pBOQTableModel->setData(index, boqTableItem.qty);
    index= pBOQTableModel->index(editRow    ,   3, QModelIndex());
    pBOQTableModel->setData(index, boqTableItem.unit);
    index= pBOQTableModel->index(editRow    ,   4, QModelIndex());
    pBOQTableModel->setData(index, boqTableItem.rate);
    index= pBOQTableModel->index(editRow    ,   5, QModelIndex());
    pBOQTableModel->setData(index, boqTableItem.amount);
    ui->tableView->resizeRowToContents(editRow);


}

void WorkSheetWidget::showError(const QString errorMsg)
{
    //TODO: NEED to implement a dialog box error

    qDebug() << errorMsg;
}

void WorkSheetWidget::on_pushButton_3_clicked()
{
    AddRemoveURC urcEdit(pStorageManager, pBOQGenerator);
    Item item = pStorageManager->getItem(ui->ItemEdit->text());
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


}

void WorkSheetWidget::setupCompleters()
{
    QTreeView *itemTreeView =  new QTreeView;
    itemModel.setQuery("SELECT Description FROM item");
    pItemCompleter = new QCompleter(&itemModel,this);
    pItemCompleter->setPopup(itemTreeView);
    itemTreeView->setRootIsDecorated(false);
    itemTreeView->header()->hide();
    itemTreeView->header()->setStretchLastSection(false);
    itemTreeView->header()->setResizeMode(0, QHeaderView::ResizeToContents);

    pItemCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    pItemCompleter->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
    ui->ItemEdit->setCompleter(pItemCompleter);

    QTreeView *categoryTreeView =  new QTreeView;
    categoryModel.setQuery("SELECT Name FROM category");
    pCategoryCompleter = new QCompleter(&categoryModel, this);
    pCategoryCompleter->setPopup(categoryTreeView);
    categoryTreeView->setRootIsDecorated(false);
    categoryTreeView->header()->hide();
    categoryTreeView->header()->setStretchLastSection(false);
    categoryTreeView->header()->setResizeMode(0, QHeaderView::ResizeToContents);

    pCategoryCompleter->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
    pCategoryCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    ui->categoryEdit->setCompleter(pCategoryCompleter);


}

void WorkSheetWidget::on_categoryEdit_textEdited(const QString &arg1)
{
   QString txt = QString("SELECT Name FROM category WHERE Name LIKE '\%%1\%' ").arg(arg1);
   categoryModel.setQuery(txt);
}

void WorkSheetWidget::on_ItemEdit_textEdited(const QString &arg1)
{
    pStorageManager->updateItemQueryModel(itemModel,arg1);
}


void WorkSheetWidget::on_categoryEdit_editingFinished()
{
    // TODO:
    QList<Item> itemList = pStorageManager->getItemsOf(ui->categoryEdit->text());
    if(!itemList.isEmpty()){

    }else{
//        QString txt = QString("SELECT Description FROM item WHERE Description LIKE '\%%1\%' ").arg(arg1);
//        itemModel.setQuery(txt);
    }
}

void WorkSheetWidget::setupBOQTable()
{
    activeRow = 0;
    pBOQTableModel = new BOQTableModel(0,  this);
    ui->tableView->setModel(pBOQTableModel);
    ui->tableView->setColumnWidth(0, 80);
    ui->tableView->setColumnWidth(1, 300);
    ui->tableView->setColumnWidth(2, 100);
    ui->tableView->setColumnWidth(3, 100);
    ui->tableView->setColumnWidth(4, 100);
    ui->tableView->setColumnWidth(5, 100);
    ui->tableView->setItemDelegate(&boqItemDelgate);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);

    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

}



void WorkSheetWidget::creatContextMenu()
{
    pAddRowAboveAct = new QAction("Add row above", this);
    pAddRowBelowAct = new QAction("Add row Below", this);
    pRemoveRowAct   = new QAction("Remove Selected", this);

    ui->tableView->addAction(pAddRowAboveAct);
    ui->tableView->addAction(pAddRowBelowAct);
    ui->tableView->addAction(pRemoveRowAct);    

    ui->tableView->setContextMenuPolicy(Qt::ActionsContextMenu);

    connect(pRemoveRowAct, SIGNAL(triggered()),this, SLOT(removeSelectedRow()));
    connect(pAddRowAboveAct, SIGNAL(triggered()),this, SLOT(addRowAboveSelected()));
    connect(pAddRowBelowAct, SIGNAL(triggered()),this, SLOT(addRowBelowSelected()));
}

QTextDocument* WorkSheetWidget::createTextDocument()
{
    QTextDocument *document = new QTextDocument(this) ;
    QTextCursor cursor(document);
    QTextCharFormat charFormat;
    charFormat.setFont(QFont("Arial",11));
    cursor.setCharFormat(charFormat);
    cursor.insertText(this->boqData.projectData.name);

    QList<BOQTableItem > *pItemList = ((BOQTableModel *)ui->tableView->model())->getTableData();

    // create Table

    int rowCount = ui->tableView->model()->rowCount();
    int columnCount = ui->tableView->model()->columnCount();
    QTextTableFormat tableFormat;
    tableFormat.setColumnWidthConstraints(getColumnWidthConstraints());
    tableFormat.setHeaderRowCount(3);
    tableFormat.setCellPadding(0);
    tableFormat.setCellSpacing(0);
    tableFormat.setMargin(0);
    // to give room for the header and the total( rowCount +2 ) is used
    QTextTable *pTextTable = cursor.insertTable(rowCount +2 , columnCount, tableFormat);

    QTextBlockFormat rightAlign, center;
    rightAlign.setAlignment(Qt::AlignRight);

    center.setAlignment(Qt::AlignCenter);

    // set headers
    pTextTable->cellAt(0, 0).firstCursorPosition().setBlockFormat(center);
    pTextTable->cellAt(0, 0).firstCursorPosition().insertText("Ref Num");

    pTextTable->cellAt(0, 1).firstCursorPosition().setBlockFormat(center);
    pTextTable->cellAt(0, 1).firstCursorPosition().insertText("Description");

    pTextTable->cellAt(0, 2).firstCursorPosition().setBlockFormat(center);
    pTextTable->cellAt(0, 2).firstCursorPosition().insertText("Qty");

    pTextTable->cellAt(0, 3).firstCursorPosition().setBlockFormat(center);
    pTextTable->cellAt(0, 3).firstCursorPosition().insertText("Unit");;

    pTextTable->cellAt(0, 4).firstCursorPosition().setBlockFormat(center);
    pTextTable->cellAt(0, 4).firstCursorPosition().insertText("Rate");

    pTextTable->cellAt(0, 5).firstCursorPosition().setBlockFormat(center);
    pTextTable->cellAt(0, 5).firstCursorPosition().insertText("Amount");


    // insert table data
     int row;
    for(row = 0; row < rowCount; row++){
        // i = 0 row is used for the header, hence data is stored from row 1
        pTextTable->cellAt(row+1, 0).firstCursorPosition().insertText(pItemList->at(row).refNum);
        pTextTable->cellAt(row+1, 1).firstCursorPosition().insertText(pItemList->at(row).description);
        pTextTable->cellAt(row+1, 2).firstCursorPosition().setBlockFormat(rightAlign);
        pTextTable->cellAt(row+1, 2).firstCursorPosition().insertText(pItemList->at(row).qty);
        pTextTable->cellAt(row+1, 3).firstCursorPosition().setBlockFormat(rightAlign);
        pTextTable->cellAt(row+1, 3).firstCursorPosition().insertText(pItemList->at(row).unit);
        pTextTable->cellAt(row+1, 4).firstCursorPosition().setBlockFormat(rightAlign);
        pTextTable->cellAt(row+1, 4).firstCursorPosition().insertText(pItemList->at(row).rate);
        pTextTable->cellAt(row+1, 5).firstCursorPosition().setBlockFormat(rightAlign);
        pTextTable->cellAt(row+1, 5).firstCursorPosition().insertText(pItemList->at(row).amount);
    }
    pTextTable->cellAt(rowCount+1, 1).firstCursorPosition().insertText("Total");
    pTextTable->cellAt(rowCount+1, 5).firstCursorPosition().setBlockFormat(rightAlign);
    pTextTable->cellAt(rowCount+1, 5).firstCursorPosition().insertText(QString().setNum(pBOQTableModel->getTotal(), 'f', 2));
    return document;
}

void WorkSheetWidget::print()
{
    //#ifndef QT_NO_PRINTER
    QPrinter printer(QPrinter::ScreenResolution);
    QPrintDialog dlg(&printer);
    dlg.exec();
    QTextDocument *doc = createTextDocument();
    doc->print(&printer);
    delete doc;
    //#endif

}

QVector<QTextLength> WorkSheetWidget::getColumnWidthConstraints()
{
    QVector<QTextLength> constraints;
    int col = ui->tableView->model()->columnCount();
    qreal columnWidth[col];
    qreal total = 0;
    for(int i = 0; i < col ; ++i){
        columnWidth[i] = ui->tableView->columnWidth(i);
        total += columnWidth[i];
    }
    for(int i =0; i < col; i++){
        constraints << QTextLength(QTextLength::PercentageLength,(columnWidth[i]/total) * 100 );
    }
    return constraints;
}


bool WorkSheetWidget::saveProject()
{
    if(currentSavePath.isEmpty()){
        return saveProjectAs();
    }else{
        boqData.itemList = *(pBOQTableModel->getTableData()) ;
        return pStorageManager->saveProject(currentSavePath, boqData);
    }
}

bool WorkSheetWidget::saveProjectAs()
{
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save Project"), QDir::currentPath());

    if(!filePath.isEmpty()){                                // empty string means user do not intend to save
        currentSavePath = filePath;
        if(!currentSavePath.endsWith(FILE_FORMAT)){         // if the extension is not given
                currentSavePath.append(FILE_FORMAT);        // append the extension data
        }
        boqData.itemList = *(pBOQTableModel->getTableData()) ;
        return pStorageManager->saveProject(currentSavePath,boqData);
    }
    return false;
}


bool WorkSheetWidget::setBOQData(QList<BOQTableItem> tableDataList, QString filepath)
{
    activeRow = 0;
    if(!filepath.isEmpty()){
        currentSavePath =  filepath;
    }
    foreach(BOQTableItem tableItem, tableDataList){
        addBOQItem(tableItem);
    }
    return true;
}


int WorkSheetWidget::getActiveRow(QString refNum, bool *itemExist)
{
    int row = -1;
    if((row = pBOQTableModel->rowOfItem(refNum)) != -1){
        *itemExist = true;
        return row;
    }

    if(activeRow < pBOQTableModel->rowCount()){
        ++activeRow;
    }else {
        pBOQTableModel->insertRow(activeRow);
        activeRow = pBOQTableModel->rowCount();
    }
    *itemExist = false;              // if item doesn't exist
    return (activeRow -1);
}


void WorkSheetWidget::tableClicked(QModelIndex index)
{
    qDebug( ) << "Clicked @ " << index;
}


void WorkSheetWidget::addRowAboveSelected()
{
    QModelIndex indx = ui->tableView->selectionModel()->currentIndex();
    pBOQTableModel->insertRow(indx.row());
}


void WorkSheetWidget::addRowBelowSelected()
{
    QModelIndex indx = ui->tableView->selectionModel()->currentIndex();
    pBOQTableModel->insertRow(indx.row()+1);
}

void WorkSheetWidget::removeSelectedRow()
{    
    QModelIndex indx = ui->tableView->selectionModel()->currentIndex();
    pBOQTableModel->removeRow(indx.row());
}





