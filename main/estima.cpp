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

#include "ui_mainwindow.h"

#include "gui/aboutdialog.h"
#include "core/worksheetwidget.h"
#include "main/estima.h"
#include "gui/addresource.h"
#include "core/units.h"
#include "gui/resourcedatabrowser.h"
#include "gui/boqviewerdialog.h"
#include "gui/itemviewer.h"
#include "gui/addremoveurc.h"

#include <QFileDialog>
#include <QSettings>





int Estima::sheetCount = 0;

Estima::Estima(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    version = "0.5";
    setWindowTitle(tr("Estima"));
    ui->tabWidget->removeTab(0);
    ui->tabWidget->removeTab(0);
    setCentralWidget(ui->tabWidget);
    ui->tabWidget->setTabsClosable(true);
    connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)),this, SLOT(closeTab(int)) );
    init();
    updateTabRelatedActions();
    readSettings();
}



Estima::~Estima()
{

    delete ui;
    delete pXMLRenderer;
    delete pStorageManager;

}

void Estima::on_actionNew_Project_triggered()
{
    pNewProjForm = new ProjectPropertiesForm(this);
    connect(pNewProjForm, SIGNAL(newProjData(ProjData)), this, SLOT(createNewPoject(ProjData)));
    pNewProjForm->exec();



}



void Estima::on_actionOpen_Project_triggered()
{
    QString filepath = QFileDialog::getOpenFileName(this, "Open Project", QDir::currentPath(),
                                                    "Estima projects (*.xest);; All file (*.*)");
    if(!filepath.isEmpty()){
        bool noError = true;
        BOQData boqData = pStorageManager->loadProject(filepath, &noError);
        if(noError){
            loadProject(boqData, filepath);
        }else{
            QMessageBox::critical(this, tr("Error Loading project"), tr("<p>Estima encounterred an error while loading "
                                               "the project file. The file may be corrupted<p>"));
        }
    }
}

void Estima::addNewSheet(WorkSheetWidget& newWorksheet)
{
    newWorksheet.setTabIndex(sheetCount);
    QString sheetName = QString(tr("Sheet %1")).arg(sheetCount+1);
    ui->tabWidget->insertTab(sheetCount, &newWorksheet, sheetName);
    ui->tabWidget->setCurrentIndex(sheetCount);
    sheetCount++;
    updateTabRelatedActions();
}


void Estima::removeSheet(WorkSheetWidget* worksheet)
{
    ui->tabWidget->removeTab(worksheet->getTabIndex());
    sheetCount--;
}


void Estima::init()
{
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(close()));
    // default xmlrenderer
    pXMLRenderer = new XMLRenderer(this);


//    // setup storageManager for sqlite
//    pStorageManager = new StorageManager(sqliteDBM, *pXMLRenderer, this);
//    // sqlite arguments not needed. for completness only.
//    pStorageManager->createConnection("root","","127.0.0.1","estima");
//    Units::getInstance()->setStorageManager(*pStorageManager);
    // setup storageManager for mysql
    pStorageManager = new StorageManager(mySQLDBM, *pXMLRenderer, this);
    if(!pStorageManager->createConnection("root","","127.0.0.1","estima")){
        // code to configure sql db
        QMessageBox::warning(this,tr("Estima"), tr("Couldn't connect to database")  );
        qDebug("couldn't connect");
    }else{
        Units::getInstance()->setStorageManager(*pStorageManager);
    }

}


void Estima::closeTab(int tabNumber)
{
    int userReply = QMessageBox::question(ui->tabWidget, tr("Close Request"), tr("Do you want save "
           "the project before closing?"), QMessageBox::Yes, QMessageBox::No, QMessageBox::Cancel);
    bool isSucess = false;
    switch(userReply){
    case QMessageBox::Yes:
        isSucess = ((WorkSheetWidget *)ui->tabWidget->widget(tabNumber))->saveProject();
        if(!isSucess){
            break;
        }
    case QMessageBox::No:
        ui->tabWidget->removeTab(tabNumber);
        sheetCount--;
        updateTabRelatedActions();
    }

}

void Estima::createNewPoject(ProjData projData)
{
    // delete the form before opening the worksheet;
    delete pNewProjForm;

    pCurrentSheet = new WorkSheetWidget(projData, *pStorageManager, this);
    addNewSheet(*pCurrentSheet);
}

void Estima::on_actionAbout_Qt_triggered()
{
    qApp->aboutQt();

}

void Estima::on_actionAbout_Estima_triggered()
{
    QMessageBox::about(this,tr("About Estima"),
        tr("<h2>Estima 0.5 </h2>"
        "<p><b>copyright 2012, Asitha Nanayakkara (daun07@gmail.com)<b><p>"
        "<p>This product is distributed under GNU GPL Licence and can be used by anyone "
        "free of charge.<p>"));
    /*
    AboutDialog about;
    about.exec();*/
}

void Estima::on_actionResources_triggered()
{
    ResourceDataBrowser *browser = new ResourceDataBrowser(pStorageManager, this);
    browser->exec();

}



void Estima::on_actionPrint_triggered()
{
    if(ui->tabWidget->currentIndex() != -1){
        WorkSheetWidget *pWrkSheetWidget = (WorkSheetWidget *)ui->tabWidget->currentWidget();
        pWrkSheetWidget->print();
    }
}

void Estima::on_actionSave_triggered()
{
    if(ui->tabWidget->currentIndex() != -1){
        WorkSheetWidget *pWrkSheetWidget = (WorkSheetWidget *)ui->tabWidget->currentWidget();
        pWrkSheetWidget->saveProject();
    }
}

void Estima::loadProject(BOQData boqData, const QString &filepath )
{
    pCurrentSheet = new WorkSheetWidget(boqData.projectData, *pStorageManager, this);
    pCurrentSheet->setBOQData(boqData.itemList, filepath);
    addNewSheet(*pCurrentSheet);
}

void Estima::closeEvent(QCloseEvent *event){
    int userReply = QMessageBox::question(this, tr("Close Resquest"), tr("Are you sure you want"
                         " to close <i><b>Estima</b></i> ?"),QMessageBox::Yes,  QMessageBox::No );
    if( userReply == QMessageBox::No){
        event->ignore();
    }else {
        writeSettings();
    }

}

void Estima::updateTabRelatedActions()
{
    bool isEnable = true;
    if(ui->tabWidget->count() == 0){
        isEnable = false;
    }
    ui->actionSave->setEnabled(isEnable);
    ui->actionSave_as->setEnabled(isEnable);
    ui->actionPrint->setEnabled(isEnable);
    ui->actionView->setEnabled(isEnable);
}

void Estima::on_actionSave_as_triggered()
{
    if(ui->tabWidget->currentIndex() != -1){
        WorkSheetWidget *pWrkSheetWidget = (WorkSheetWidget *)ui->tabWidget->currentWidget();
        pWrkSheetWidget->saveProjectAs();
    }
}

void Estima::writeSettings()
{
    QSettings settings("Estima Inc.", "Estima" );
    settings.beginGroup("MainWindow");
         settings.setValue("size", size());
         settings.setValue("pos", pos());
         settings.endGroup();
}

void Estima::readSettings()
{
    QSettings settings("Estima Inc.", "Estima" );
    settings.beginGroup("MainWindow");
    resize(settings.value("size", QSize(800, 700)).toSize());
    move(settings.value("pos", QPoint(200, 200)).toPoint());
    settings.endGroup();

}

void Estima::on_actionView_triggered()
{
    if(ui->tabWidget->currentIndex() != -1){
        WorkSheetWidget *pWrkSheetWidget = (WorkSheetWidget *)ui->tabWidget->currentWidget();

        BOQViewerDialog viewer(pWrkSheetWidget->createTextDocument(), this);
        viewer.exec();
    }
}

void Estima::on_actionItems_triggered()
{
    ItemViewer itemViewer;
    itemViewer.exec();
}

void Estima::on_actionURC_Editor_triggered()
{
    BOQGenerator boqGenerator(*pStorageManager);
    AddRemoveURC urcEditor(pStorageManager, &boqGenerator, this);
    urcEditor.exec();
}
