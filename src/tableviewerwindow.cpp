#include "tableviewerwindow.h"
#include "./ui_tableviewerwindow.h"

TableViewerWindow::TableViewerWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TableViewerWindow)
{
    ui->setupUi(this);
    data = new Table(true);
    setupToolBar();
    // model = new QSqlTableModel;
    // model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    connect(ui->DataTable, &QTableWidget::currentCellChanged, this, &TableViewerWindow::selectItem);
    connect(ui->DisplayData, &QLineEdit::editingFinished, this, &TableViewerWindow::editItemDisplay);
    connect(ui->WorthData, &QDoubleSpinBox::valueChanged, this, &TableViewerWindow::editItemWorth);
    connect(ui->columnName, &QLineEdit::editingFinished, this, &TableViewerWindow::editColumnName);
    connect(ui->columnImportance, &QDoubleSpinBox::valueChanged, this, &TableViewerWindow::editColumnImportance);

    //connect File menu actions
    connect(ui->menubar->actions()[0], &QAction::triggered, this, &TableViewerWindow::newTriggered);
    connect(ui->menubar->actions()[1], &QAction::triggered, this, &TableViewerWindow::openTriggered);
    connect(ui->menubar->actions()[3], &QAction::triggered, this, &TableViewerWindow::saveTriggered);
    connect(ui->menubar->actions()[4], &QAction::triggered, this, &TableViewerWindow::saveAsTriggered);
    connect(ui->menubar->actions()[5], &QAction::triggered, this, &TableViewerWindow::closeTriggered);
    connect(ui->menubar->actions()[6], &QAction::triggered, this, &TableViewerWindow::quitTriggered);
}

void TableViewerWindow::setupToolBar()
{
    // Create actions for adding rows and columns
    QAction* addRowAction = new QAction(tr("Add Row"), this);
    QAction* addColumnAction = new QAction(tr("Add Column"), this);

    // Connect actions to slots
    connect(addRowAction, &QAction::triggered, this, &TableViewerWindow::newRowTriggered);
    connect(addColumnAction, &QAction::triggered, this, &TableViewerWindow::newColumnTriggered);

    // Add actions to the toolbar
    auto mainToolbar = new QToolBar();
    ui->TableContainer->addWidget(mainToolbar);
    mainToolbar->addAction(addRowAction);
    mainToolbar->addAction(addColumnAction);
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


void TableViewerWindow::drawTable() {
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
        for (int row = 0; row < data->rowCount(); row++) {
            setItem(row, 0, tr(data->row(row)->name().data()));
            for (int column = 1; column < data->headingCount(); column++) {
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
    if (i == nullptr) {
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

void TableViewerWindow::newColumnTriggered(bool checked)
{
    data->addHeading(Column("Unnamed Column"));
    drawTable();
}
void TableViewerWindow::newRowTriggered(bool checked)
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
    if (i == nullptr) {
        return;
    }
    i->displayValue = ui->DisplayData->text().toStdString();
    drawTable();
}

void TableViewerWindow::editItemWorth()
{
    Item* i = data->item(ui->DataTable->currentColumn(), ui->DataTable->currentRow());
    if (i == nullptr) {
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