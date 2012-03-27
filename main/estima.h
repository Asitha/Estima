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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "core/worksheetwidget.h"
#include "storage/storagemanager.h"
#include "storage/dbmanager.h"
#include "storage/item.h"
#include "storage/sqlitedbmanager.h"
#include "storage/mysqlconnector.h"
#include "gui/projectpropertiesform.h"

#include <QMainWindow>


namespace Ui {
    class MainWindow;
}

class Estima : public QMainWindow
{
    Q_OBJECT

public:
    explicit Estima(QWidget *parent = 0);
    ~Estima();
    void addNewSheet(WorkSheetWidget& newWorksheet);
    void removeSheet(WorkSheetWidget *worksheet);

private slots:
    void on_actionNew_Project_triggered();

    void on_actionAbout_Estima_triggered();

    void on_actionOpen_Project_triggered();

    void updateTabRelatedActions();


    void closeTab(int i);

    void createNewPoject(ProjData projData);

    void on_actionAbout_Qt_triggered();

    void on_actionResources_triggered();

    void on_actionPrint_triggered();

    void on_actionSave_triggered();

    void closeEvent(QCloseEvent *event);

    void on_actionSave_as_triggered();

    void on_actionView_triggered();

private:
    // members
    void init();
    inline void loadProject(BOQData boqData, const QString &filepath);
    void writeSettings();
    void readSettings();

    // variables
    Ui::MainWindow *ui;
    WorkSheetWidget *pCurrentSheet;
    XMLRenderer *pXMLRenderer;
    SQLiteDBManager sqliteDBM;
    MySQLConnector mySQLDBM;
    static int sheetCount;
    ProjectPropertiesForm *pNewProjForm ;
    QString version;

    StorageManager *pStorageManager;
    QDialog *pDlg;
    QList<WorkSheetWidget> workSpaces;
};

#endif // MAINWINDOW_H
