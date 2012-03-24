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

#ifndef WORKSHEETWIDGET_H
#define WORKSHEETWIDGET_H

#include "storage/storagemanager.h"
#include "core/boqgenerator.h"
#include "core/CoreStructures.h"
#include "core/boqdelegate.h"
#include "core/boqtablemodel.h"

#include <QDialog>
#include <QtCore>
#include <QtGui>



namespace Ui {
    class WorkSheetWidget;
}



class WorkSheetWidget : public QDialog
{
    Q_OBJECT

public:

    explicit WorkSheetWidget(ProjData projData, StorageManager& pStorageManager, QWidget *parent = 0);
    void setTabIndex(int index);
    int getTabIndex();
    ~WorkSheetWidget();
    void setStorageManager(StorageManager& pStorageManager);
    void print();
    void saveProject();
    void saveProjectAs();
    bool setBOQData(QList<BOQTableItem> tableDataList, QString filepath = QString(""));

private slots:

    void on_addItemButton_clicked();
    void on_pushButton_3_clicked();
    void on_categoryEdit_textEdited(const QString &arg1);
    void on_categoryEdit_lostFocus();
    void on_reset_Button_clicked();
    void on_ItemEdit_textEdited(const QString &arg1);
    void showPopupMenu(QModelIndex index);
    void on_categoryEdit_editingFinished();

private:
    int indexNum;

    int activeRow;                              // new items are added to this row;
    QString currentSavePath;                    // wif the string is non-empty save option will save the file to this path.
    QSqlQueryModel itemModel, categoryModel;
    BOQData boqData;

    BOQDelegate boqItemDelgate;
    Ui::WorkSheetWidget *ui;
    StorageManager *pStorageManager;
    BOQGenerator *pBOQGenerator;
    BOQTableModel *pBOQTableModel;
    QCompleter *pItemCompleter, *pCategoryCompleter;

    // action variables of the table context menu
    QAction *pAddRowAboveAct ;
    QAction *pAddRowBelowAct;
    QAction *pRemoveRowAct;
    QAction *pCutAct;
    QAction *pPasteAct;

    void creatContextMenu();
    void addBOQItem(BOQItem &boqItem);
    void addBOQItem(BOQTableItem &boqTableItem );
    void showError(const QString errorMsg);
    void setupCompleters();
    void setupBOQTable();
    int getActiveRow();
    QVector<QTextLength>  getColumnWidthConstraints();
    QTextDocument* createTextDocument();

};

#endif // WORKSHEETWIDGET_H
