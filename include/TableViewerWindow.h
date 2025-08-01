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
#include <stack>

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

    void undoTriggered();
    void actionOccured(Table* i_table);
    void redoTriggered();
    void cutTriggered();
    void copyTriggered();
    void pasteTriggered();
    void preferencesTriggered();
    void findTriggered();

    void changeColumnType(Type i_type);

    void changeZoom(float i_newZoom);

    // Help menu slots
    void searchTriggered();
    void reportBugTriggered();
    void viewSourceTriggered();
    void openForumsTriggered();
    void documentationTriggered();

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

    std::stack<Table> _m_undoStack;
    std::stack<Table> _m_redoStack;
    bool _m_undoing = false;

    QLineEdit* _m_findLineEdit = nullptr;
};

#endif // TABLEVIEWERWINDOW_H
