#ifndef TABLEVIEWERWINDOW_H
#define TABLEVIEWERWINDOW_H

#include <QMainWindow>
#include <qtableview.h>
#include "table.h"
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QSqlField>
#include <string>
#include <QStandardItemModel>

QT_BEGIN_NAMESPACE
namespace Ui {
class TableViewerWindow;
}
QT_END_NAMESPACE

class TableViewerWindow : public QMainWindow
{
    Q_OBJECT

public:
    TableViewerWindow(QWidget *parent = nullptr);
    ~TableViewerWindow();

    void addRow();
    void addColumn();
    void drawTable();
private:
    Ui::TableViewerWindow *ui;
    QTableView * getTableDisplay();
    Table* data;
    QSqlTableModel *model;
};
#endif // TABLEVIEWERWINDOW_H
