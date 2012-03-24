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

#include "tabledataconvertor.h"

TableDataConvertor::TableDataConvertor(QObject *parent) :
    QObject(parent)
{
}

QTextDocument * TableDataConvertor::toTextDocument(QTableView *tableView)
{
//    QTextDocument *document = new QTextDocument(this) ;
//    QTextCursor cursor(document);

//    cursor.insertText("BOQ Header");

//    QList<TableItem > *pItemList = ((BOQTableModel *)ui->tableView->model())->getTableData();
//    // create Table
//    int rowCount = ui->tableView->model()->rowCount();
//    int columnCount = ui->tableView->model()->columnCount();
//    QTextTableFormat tableFormat;
//    QVector<QTextLength> constraints;
//         constraints << QTextLength(QTextLength::PercentageLength, 14);
//         constraints << QTextLength(QTextLength::PercentageLength, 40);
//         constraints << QTextLength(QTextLength::PercentageLength, 15);
//         constraints << QTextLength(QTextLength::PercentageLength, 8);
//         constraints << QTextLength(QTextLength::PercentageLength, 15);
//         constraints << QTextLength(QTextLength::PercentageLength, 15);
//    tableFormat.setColumnWidthConstraints(constraints);
//    tableFormat.setHeaderRowCount(0);

//    // to give room for the header ( rowCount +1 ) is used
//    QTextTable *pTextTable = cursor.insertTable(rowCount +1 , columnCount, tableFormat);



//    // set headers
//    cursor = pTextTable->cellAt(0, 0).firstCursorPosition();
//    cursor.insertText("Ref Num");
//    cursor = pTextTable->cellAt(0, 1).firstCursorPosition();
//    cursor.insertText("Description");
//    cursor = pTextTable->cellAt(0, 2).firstCursorPosition();
//    cursor.insertText("Qty");
//    cursor = pTextTable->cellAt(0, 3).firstCursorPosition();
//    cursor.insertText("Unit");
//    cursor = pTextTable->cellAt(0, 4).firstCursorPosition();
//    cursor.insertText("Rate");
//    cursor = pTextTable->cellAt(0, 5).firstCursorPosition();
//    cursor.insertText("Amount");

//    // insert data
//    for(int i = 0; i < rowCount; i++){
//        cursor = pTextTable->cellAt(i+1, 0).firstCursorPosition();
//        cursor.insertText(pItemList->at(i).refNum);
//        cursor = pTextTable->cellAt(i+1, 1).firstCursorPosition();
//        cursor.insertText(pItemList->at(i).description);
//        cursor = pTextTable->cellAt(i+1, 2).firstCursorPosition();
//        cursor.insertText(pItemList->at(i).qty);
//        cursor = pTextTable->cellAt(i+1, 3).firstCursorPosition();
//        cursor.insertText(pItemList->at(i).unit);
//        cursor = pTextTable->cellAt(i+1, 4).firstCursorPosition();
//        cursor.insertText(pItemList->at(i).rate);
//        cursor = pTextTable->cellAt(i+1, 5).firstCursorPosition();
//        cursor.insertText(pItemList->at(i).amount);
//    }

//    return document;

}
