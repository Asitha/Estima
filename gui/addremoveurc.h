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

#ifndef ADDREMOVEURC_H
#define ADDREMOVEURC_H

#include "storage/storagemanager.h"
#include "core/boqgenerator.h"
#include "core/units.h"

#include <QDialog>
#include <QtCore>
#include <QtGui>



namespace Ui {
    class AddRemoveURC;
}

class AddRemoveURC : public QDialog
{
    Q_OBJECT

public:
    explicit AddRemoveURC(StorageManager *storageManager,BOQGenerator *boqGenerator, QWidget *parent = 0);
    ~AddRemoveURC();
    bool fillData(Item item);

private slots:
    void on_AddRsrcPushButton_clicked();

    void on_URCEditButtonBox_accepted();

    void on_URCEditButtonBox_rejected();


    void on_itemlineEdit_editingFinished();

    void on_rsrcNamelineEdit_editingFinished();

    void on_itemlineEdit_textEdited(const QString &arg1);

    void on_rsrcNamelineEdit_textEdited(const QString &arg1);

    void removeRow();

private:
    QAction *removeResourceAct;

    StorageManager *storageManager;
    BOQGenerator *boqGenerator;
    Ui::AddRemoveURC *ui;
    Units *units;
    URCData urcData;

    QCompleter *itemCompleter;
    QCompleter *resourceCompleter;
    QSqlQueryModel itemModel;
    QSqlQueryModel resourceModel;

    Item item;
    int firstBlankRow;

    bool addToURCTable(ResourceURC rsrcData);
    void fillUIData();
    void setupCompleters();
    void setupResourceTable();
    int rowToAdd(QString name, int duplicityChkCol = 0);
    void clearTable();
    QList<ResourceURC> getTableData();
    void ShowItemData();
    void createContextMenu();
};

#endif // ADDREMOVEURC_H
