#include "tableviewerwindow.h"
#include "./ui_tableviewerwindow.h"

TableViewerWindow::TableViewerWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TableViewerWindow)
{
    ui->setupUi(this);
}

TableViewerWindow::~TableViewerWindow()
{
    delete ui;
}
