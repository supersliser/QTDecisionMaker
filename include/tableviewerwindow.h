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

#include "TableColumnDataDock.h"
#include "TableDataDock.h"
#include "tableManager.h"

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

signals:
    void sendDrawTable(Table* table);
    void itemSelected(Table* table, int row, int column);
    void columnSelected(Table* table, int column);

public slots:

    void selectItem(int row, int column);

    void newColumnTriggered();

    void newRowTriggered();

    void editItemDisplay(std::string value);

    void editItemWorth(float value);

    void editColumnName(std::string value);

    void editColumnImportance(float value);

    void newTriggered();
    void openTriggered();
    void saveTriggered();
    void saveAsTriggered();
    void closeTriggered();
    void quitTriggered();

    void changeColumnType(Type type);

private:
    bool fileSaved = true;
    QString filePath;
    Ui::TableViewerWindow *ui;
    Table *data;
    tableViewerMenubar *menubar;
    tableViewerToolbar *toolbar;
    tableManager *table;
    TableDataDock *itemDock;
    TableColumnDataDock *columnDock;

};

#endif // TABLEVIEWERWINDOW_H
