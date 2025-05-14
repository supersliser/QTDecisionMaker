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
#include <QFileDialog>

QT_BEGIN_NAMESPACE
namespace Ui {
    class TableViewerWindow;
}
QT_END_NAMESPACE

class TableViewerWindow : public QMainWindow {
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

    void newTriggered();
    void openTriggered();
    void saveTriggered();
    void saveAsTriggered();
    void closeTriggered();
    void quitTriggered();

private:
    bool fileSaved = true;
    QString filePath;
    Ui::TableViewerWindow *ui;
    Table *data;
    tableViewerMenubar *menubar;

    void setColumnHeader(int i_column, QString i_name);

    void setRowHeader(int i_row, QString i_name);

    void setItem(int i_row, int i_column, QString i_name);
};

#endif // TABLEVIEWERWINDOW_H
