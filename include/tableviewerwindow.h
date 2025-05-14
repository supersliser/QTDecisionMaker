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
#include <fmt/format.h>
#include "tableViewerToolbar.h"
#include "tableViewerMenubar.h"

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
public slots:
    void selectItem(int row, int column, int prev_row, int prev_column);
    void newColumnTriggered();
    void newRowTriggered();
    void editItemDisplay();
    void editItemWorth();
    void editColumnName();
    void editColumnImportance();

    // void newTriggered(bool checked);
    // void openTriggered(bool checked);
    // void saveTriggered(bool checked);
    // void saveAsTriggered(bool checked);
    // void closeTriggered(bool checked);
    // void quitTriggered(bool checked);

private:
    Ui::TableViewerWindow *ui;
    Table* data;
    // QSqlTableModel *model;

    void setColumnHeader(int i_column, QString i_name);
    void setRowHeader(int i_row, QString i_name);
    void setItem(int i_row, int i_column, QString i_name);
};
#endif // TABLEVIEWERWINDOW_H
