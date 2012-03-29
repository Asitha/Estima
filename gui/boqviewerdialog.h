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

#ifndef BOQVIEWERDIALOG_H
#define BOQVIEWERDIALOG_H

#include "core/boqtablemodel.h"

#include <QTextDocument>
#include <QDialog>

namespace Ui {
    class BOQViewerDialog;
}

class BOQViewerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BOQViewerDialog(QTextDocument *document, QWidget *parent = 0);
    ~BOQViewerDialog();
    void setText(QTextDocument *document);

private slots:
    void on_cancelButton_clicked();

    void on_saveAsPDFButton_clicked();

private:
    QTextDocument *pDocument;
    Ui::BOQViewerDialog *ui;

};

#endif // BOQVIEWERDIALOG_H
