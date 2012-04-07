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

#ifndef PROJECTPROPERTIESFORM_H
#define PROJECTPROPERTIESFORM_H

#include "core/estimaEnums.h"
#include "core/CoreStructures.h"
#include <QDialog>


namespace Ui {
    class ProjectPropertiesForm;
}

class ProjectPropertiesForm : public QDialog
{
    Q_OBJECT

public:
    explicit ProjectPropertiesForm(QWidget *parent = 0);
    ~ProjectPropertiesForm();

private slots:
    void on_cancelButton_clicked();

    void on_createProjButton_clicked();

signals:
    void  newProjData(ProjData projData);
private:
    Ui::ProjectPropertiesForm *ui;
    bool isValidInput();
};

#endif // PROJECTPROPERTIESFORM_H
