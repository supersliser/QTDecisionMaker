#ifndef TABLEVIEWERWINDOW_H
#define TABLEVIEWERWINDOW_H

#include <QMainWindow>

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

private:
    Ui::TableViewerWindow *ui;
};
#endif // TABLEVIEWERWINDOW_H
