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

#ifndef RESOURCEDATABROWSER_H
#define RESOURCEDATABROWSER_H

#include "storage/storagemanager.h"

#include <QDialog>

namespace Ui {
    class ResourceDataBrowser;
}

class ResourceDataBrowser : public QDialog
{
    Q_OBJECT

public:
    explicit ResourceDataBrowser(StorageManager &pStorageManagerAct ,QWidget *parent = 0);
    ~ResourceDataBrowser();

private slots:
    void on_closeButton_clicked();

    void on_AddResourceButton_clicked();

    void removeSelectedResource();

    void addNewResource();

    void editSelectedResource();


private:
    QAction *pRemoveResourceAct;
    QAction *pAddResourceAct;
    QAction *pEditResourceAct;

    StorageManager *pStorageManagerAct;
    Ui::ResourceDataBrowser *ui;

    QSqlTableModel *pModel;
    void setupResourceTable();
    void fillUIData();
    void initActions();
};

#endif // RESOURCEDATABROWSER_H
