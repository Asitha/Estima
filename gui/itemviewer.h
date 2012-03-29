#ifndef ITEMVIEWER_H
#define ITEMVIEWER_H

#include <QDialog>
#include <QSqlTableModel>

namespace Ui {
    class ItemViewer;
}

class ItemViewer : public QDialog
{
    Q_OBJECT

public:
    explicit ItemViewer(QWidget *parent = 0);
    ~ItemViewer();

private:
    Ui::ItemViewer *ui;
    QSqlTableModel *tableModel;


};

#endif // ITEMVIEWER_H
