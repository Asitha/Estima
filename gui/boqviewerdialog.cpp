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

#include "boqviewerdialog.h"
#include "ui_boqviewerdialog.h"

#include <QMessageBox>
#include <QPrinter>
#include <QFileDialog>

BOQViewerDialog::BOQViewerDialog(QTextDocument *pDocument, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BOQViewerDialog)
{
    ui->setupUi(this);
    this->pDocument = pDocument;
    setText(pDocument);

}

BOQViewerDialog::~BOQViewerDialog()
{
    delete ui;
}

void BOQViewerDialog::setText(QTextDocument *document)
{

    ui->textEdit->setDocument(document);
}

void BOQViewerDialog::on_cancelButton_clicked()
{
    this->close();
}

void BOQViewerDialog::on_saveAsPDFButton_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save As PDF"), QDir::currentPath());
    if(!filePath.isEmpty()){
       QPrinter printer(QPrinter::ScreenResolution);
       printer.setOutputFileName(filePath);
       printer.setOutputFormat(QPrinter::PdfFormat);
       printer.setPaperSize(QPrinter::A4);
       pDocument->print(&printer);
    }
}
