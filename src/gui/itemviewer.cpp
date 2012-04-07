#include "itemviewer.h"
#include "ui_itemviewer.h"

ItemViewer::ItemViewer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ItemViewer)
{
    ui->setupUi(this);
    setWindowTitle(tr("Item Browser"));
    tableModel = new QSqlTableModel(this);
    tableModel->setTable("item");
    tableModel->select();
    tableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    ui->tableView->setModel(tableModel);
    connect(ui->submitButton, SIGNAL(clicked()), tableModel, SLOT(submitAll()));
    connect(ui->revertButton, SIGNAL(clicked()), tableModel, SLOT(revertAll()));
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(close()));

    ui->tableView->resizeRowsToContents();
    ui->tableView->resizeColumnsToContents();
}

ItemViewer::~ItemViewer()
{
    delete ui;
}

