#include "tableviewerwindow.h"
#include "./ui_tableviewerwindow.h"

TableViewerWindow::TableViewerWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TableViewerWindow)
{
    ui->setupUi(this);
    data = new Table();
    model = new QSqlTableModel;
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
}

TableViewerWindow::~TableViewerWindow()
{
    delete ui;
}

QTableView *TableViewerWindow::getTableDisplay() {
    return ui->tableView;
}

void TableViewerWindow::drawTable() {
        auto disp = getTableDisplay();

        // Use QStandardItemModel for manual data population
        QStandardItemModel *model = new QStandardItemModel(this);

        // Set column headers
        model->setColumnCount(data->headingCount() + 1);
        for (int column = 0; column < data->headingCount(); column++) {
            model->setHeaderData(column, Qt::Horizontal, tr(data->heading(column)->name().data()));
        }
        model->setHeaderData(data->headingCount(), Qt::Horizontal, tr("Total Value"));

        // Populate rows
        data->calculateAllTotals();
        model->setRowCount(data->rowCount());
        for (int row = 0; row < data->rowCount(); row++) {
            model->setData(model->index(row, 0), tr(data->row(row)->name().data()));
            for (int column = 1; column < data->headingCount(); column++) {
                model->setData(model->index(row, column + 1), tr(data->item(column, row)->displayValue.data()));
            }
            auto d = data->row(row)->totalValue();
            model->setData(model->index(row, data->headingCount()), d);
        }

        // Set the model to the QTableView
        disp->setModel(model);
        disp->show();

}
