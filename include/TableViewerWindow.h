#ifndef TABLEVIEWERWINDOW_H
#define TABLEVIEWERWINDOW_H

#include <QMainWindow>
#include <qtableview.h>
#include "Table.h"
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QSqlField>
#include <string>
#include <QStandardItemModel>
#include <fmt/format.h>
#include "TableViewerToolbar.h"
#include "TableViewerMenubar.h"
#include <QFileDialog>

#include "TableColumnDataDock.h"
#include "TableDataDock.h"
#include "TableManager.h"

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
    void sendDrawTable(Table* i_table);
    void itemSelected(Table* i_table, int i_row, int i_column);
    void columnSelected(Table* i_table, int i_column);

public slots:

    void selectItem(int i_row, int i_column);

    void newColumnTriggered();

    void newRowTriggered();

    void editItemDisplay(std::string i_value);

    void editItemWorth(float i_value);

    void editColumnName(std::string i_value);

    void editColumnImportance(float i_value);

    void newTriggered();
    void openTriggered();
    void saveTriggered();
    void saveAsTriggered();
    void closeTriggered();
    void quitTriggered();

    void changeColumnType(Type i_type);

private:
    bool _m_fileSaved = true;
    QString _m_filePath;
    Ui::TableViewerWindow *ui;
    Table *_m_data;
    TableViewerMenubar *_m_menubar;
    TableViewerToolbar *_m_toolbar;
    TableManager *_m_table;
    TableDataDock *_m_itemDock;
    TableColumnDataDock *_m_columnDock;

};

#endif // TABLEVIEWERWINDOW_H
