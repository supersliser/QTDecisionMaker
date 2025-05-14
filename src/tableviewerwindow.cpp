#include <iostream>
#include "tableviewerwindow.h"
#include "./ui_tableviewerwindow.h"

TableViewerWindow::TableViewerWindow(QWidget* parent)
    : QMainWindow(parent)
      , ui(new Ui::TableViewerWindow)
{
    ui->setupUi(this);
    data = new Table(true);

    tableViewerToolbar *toolbar = new tableViewerToolbar(this);
    ui->TableContainer->setDirection(QBoxLayout::BottomToTop);
    ui->TableContainer->addWidget(toolbar);
    connect(toolbar, &tableViewerToolbar::newRow, this, &TableViewerWindow::newRowTriggered);
    connect(toolbar, &tableViewerToolbar::newColumn, this, &TableViewerWindow::newColumnTriggered);

    menubar = new tableViewerMenubar(this);

    connect(ui->DataTable, &QTableWidget::currentCellChanged, this, &TableViewerWindow::selectItem);
    connect(ui->DisplayData, &QLineEdit::editingFinished, this, &TableViewerWindow::editItemDisplay);
    connect(ui->WorthData, &QDoubleSpinBox::valueChanged, this, &TableViewerWindow::editItemWorth);
    connect(ui->columnName, &QLineEdit::editingFinished, this, &TableViewerWindow::editColumnName);
    connect(ui->columnImportance, &QDoubleSpinBox::valueChanged, this, &TableViewerWindow::editColumnImportance);
}

TableViewerWindow::~TableViewerWindow()
{
    delete ui;
}

void TableViewerWindow::setColumnHeader(int i_column, QString i_name)
{
    ui->DataTable->setHorizontalHeaderItem(i_column, new QTableWidgetItem(i_name));
}

void TableViewerWindow::setItem(int i_row, int i_column, QString i_name)
{
    ui->DataTable->setItem(i_row, i_column, new QTableWidgetItem(i_name));
}

void TableViewerWindow::setRowHeader(int i_row, QString i_name)
{
    ui->DataTable->setVerticalHeaderItem(i_row, new QTableWidgetItem(i_name));
}


void TableViewerWindow::drawTable()
{
    auto disp = ui->DataTable;


    // Set column headers
    disp->setColumnCount(data->headingCount() + 1);
    for (int column = 0; column < data->headingCount(); column++)
    {
        setColumnHeader(column, tr(data->heading(column)->name().data()));
    }
    setColumnHeader(data->headingCount(), tr("Total Value"));

    // Populate rows
    data->calculateAllTotals();
    disp->setRowCount(data->rowCount());
    for (int row = 0; row < data->rowCount(); row++)
    {
        setItem(row, 0, tr(data->row(row)->name().data()));
        for (int column = 1; column < data->headingCount(); column++)
        {
            setItem(row, column, tr(data->item(column, row)->displayValue.data()));
        }
        auto d = data->row(row)->totalValue();
        setItem(row, data->headingCount(), fmt::format("{:.2f}", d).c_str());
    }
    disp->show();
}

void TableViewerWindow::selectItem(int row, int column, int prev_row, int prev_column)
{
    Item* i;
    if (column == 0)
    {
        i = new Item();
        i->displayValue = data->row(row)->name();
        i->worthValue = 0;
    }
    else
    {
        i = data->item(column, row);
    }
    // Get the data from the model
    if (i == nullptr)
    {
        return;
    }

    // Do something with the data (e.g., display it in a message box)
    ui->DisplayData->setText(QString(i->displayValue.c_str()));
    ui->WorthData->setValue(i->worthValue);
    ui->WorthData->setEnabled(column != 0);
    if (ui->ItemDetailsDock->isHidden())
    {
        ui->ItemDetailsDock->setFloating(false);
    }
    ui->ItemDetailsDock->show();

    if (column != 0)
    {
        auto c = data->heading(column);
        ui->columnName->setText(c->name().c_str());
        ui->columnImportance->setValue(c->importance());
        ui->columnImportance->setEnabled(column != 0);
        ui->ColumnDetailsDock->show();
    }

    if (column == 0)
    {
        delete i;
    }
}

void TableViewerWindow::newColumnTriggered()
{
    data->addHeading(Column("Unnamed Column"));
    drawTable();
}

void TableViewerWindow::newRowTriggered()
{
    data->addRow(Row("Unnamed Item"));
    drawTable();
}

void TableViewerWindow::editItemDisplay()
{
    if (ui->DataTable->currentColumn() == 0)
    {
        data->row(ui->DataTable->currentRow())->setName(ui->DisplayData->text().toStdString());
    }
    Item* i = data->item(ui->DataTable->currentColumn(), ui->DataTable->currentRow());
    if (i == nullptr)
    {
        return;
    }
    i->displayValue = ui->DisplayData->text().toStdString();
    drawTable();
}

void TableViewerWindow::editItemWorth()
{
    Item* i = data->item(ui->DataTable->currentColumn(), ui->DataTable->currentRow());
    if (i == nullptr)
    {
        return;
    }
    i->worthValue = ui->WorthData->value();
    drawTable();
}

void TableViewerWindow::editColumnName()
{
    data->heading(ui->DataTable->currentColumn())->setName(ui->columnName->text().toStdString());
    drawTable();
}

void TableViewerWindow::editColumnImportance()
{
    data->heading(ui->DataTable->currentColumn())->setImportance(ui->columnImportance->value());
    drawTable();
}

 void TableViewerWindow::newTriggered()
 {
    std::cout << "New file triggered" << std::endl;
    delete data;
    data = new Table(false);
    drawTable();
 }

void TableViewerWindow::openTriggered(bool checked)
{
    Q_UNUSED(checked);
    std::cout << "Open file triggered" << std::endl;
    // Implement open file logic here
}

void TableViewerWindow::saveTriggered(bool checked)
{
    Q_UNUSED(checked);
    std::cout << "Save file triggered" << std::endl;
    // Implement save file logic here
}

void TableViewerWindow::saveAsTriggered(bool checked)
{
    Q_UNUSED(checked);
    std::cout << "Save As file triggered" << std::endl;
    // Implement save as file logic here
}

void TableViewerWindow::closeTriggered(bool checked)
{
    Q_UNUSED(checked);
    std::cout << "Close file triggered" << std::endl;
    // Implement close file logic here
}

void TableViewerWindow::quitTriggered(bool checked)
{
    Q_UNUSED(checked);
    std::cout << "Quit file triggered" << std::endl;
    // Implement quit file logic here
}