#include <iostream>
#include "tableviewerwindow.h"
#include "./ui_tableviewerwindow.h"
#include "FileSystemManager.h"

TableViewerWindow::TableViewerWindow(QWidget* parent)
    : QMainWindow(parent)
      , ui(new Ui::TableViewerWindow)
{
    ui->setupUi(this);
    data = new Table(true);

    toolbar = new tableViewerToolbar(this);
    // ui->TableContainer->setDirection(QBoxLayout::BottomToTop);
    ui->TableContainer->addWidget(toolbar);
    connect(toolbar, &tableViewerToolbar::newRow, this, &TableViewerWindow::newRowTriggered);
    connect(toolbar, &tableViewerToolbar::newColumn, this, &TableViewerWindow::newColumnTriggered);

    menubar = new tableViewerMenubar(this);

    table = new tableManager(this);
    ui->TableContainer->addWidget(table);
    connect(table, &tableManager::selectItem, this, &TableViewerWindow::selectItem);
    connect(this, &TableViewerWindow::sendDrawTable, table, &tableManager::drawTable);

    itemDock = new TableDataDock(ui->verticalFrame);
    ui->verticalFrame->layout()->addWidget(itemDock);
    connect(itemDock, &TableDataDock::displayValueChanged, this, &TableViewerWindow::editItemDisplay);
    connect(itemDock, &TableDataDock::worthValueChanged, this, &TableViewerWindow::editItemWorth);
    connect(table, &tableManager::itemEdited, itemDock, &TableDataDock::displayValueUpdated);
    connect(this, &TableViewerWindow::itemSelected, itemDock, &TableDataDock::setItem);

    columnDock = new TableColumnDataDock(ui->verticalFrame);
    ui->verticalFrame->layout()->addWidget(columnDock);
    connect(columnDock, &TableColumnDataDock::displayValueChanged, this, &TableViewerWindow::editColumnName);
    connect(columnDock, &TableColumnDataDock::worthValueChanged, this, &TableViewerWindow::editColumnImportance);
    connect(columnDock, &TableColumnDataDock::typeChanged, this, &TableViewerWindow::changeColumnType);
    connect(this, &TableViewerWindow::columnSelected, columnDock, &TableColumnDataDock::setItem);

    // connect(ui->DataTable, &QTableWidget::currentCellChanged, this, &TableViewerWindow::selectItem);
    // connect(ui->DisplayData, &QLineEdit::editingFinished, this, &TableViewerWindow::editItemDisplay);
    // connect(ui->WorthData, &QDoubleSpinBox::valueChanged, this, &TableViewerWindow::editItemWorth);
    // connect(ui->columnName, &QLineEdit::editingFinished, this, &TableViewerWindow::editColumnName);
    // connect(ui->columnImportance, &QDoubleSpinBox::valueChanged, this, &TableViewerWindow::editColumnImportance);
    //
    // for (int i = 0; i < 7; i++)
    // {
    //     ui->TypeDropDown->addItem(tr(DataType::createDataType((Type)(i))->getName().data()));
    // }
    //
    // connect(ui->TypeDropDown, &QComboBox::activated, this, &TableViewerWindow::changeColumnType);

    emit sendDrawTable(data);
}

TableViewerWindow::~TableViewerWindow()
{
    delete ui;
}

void TableViewerWindow::selectItem(int row, int column)
{
    emit itemSelected(data, row, column);
    emit columnSelected(data, column);
}

void TableViewerWindow::newColumnTriggered()
{
    data->addHeading(Column("Unnamed Column"));
    fileSaved = false;

    emit sendDrawTable(data);
}

void TableViewerWindow::newRowTriggered()
{
    data->addRow(Row("Unnamed Item"));
    fileSaved = false;

    emit sendDrawTable(data);
}

void TableViewerWindow::editItemDisplay(std::string value)
{
    Item* i = data->item(table->selectedColumn() - 1, table->selectedRow());
    if (i == nullptr) { return; }
    i->displayValue = value;
    fileSaved = false;
    Column* c = data->heading(table->selectedColumn() - 1);
    c->testAutoSetType(i->displayValue);
    columnDock->setType(c->type().getType());
    emit sendDrawTable(data);
}

void TableViewerWindow::editItemWorth(float value)
{
    Item* i = data->item(table->selectedColumn() - 1, table->selectedRow());
    if (i == nullptr) { return; }
    i->worthValue = value;
    fileSaved = false;
    emit sendDrawTable(data);
}

void TableViewerWindow::editColumnName(std::string value)
{
    data->heading(table->selectedColumn() - 1)->setName(value);
    fileSaved = false;
    emit sendDrawTable(data);
}

void TableViewerWindow::editColumnImportance(float value)
{
    if (table->selectedColumn() < 0) { return;}
    data->heading(table->selectedColumn() - 1)->setImportance(value);
    fileSaved = false;
    emit sendDrawTable(data);
}

void TableViewerWindow::newTriggered()
{
    delete data;
    data = new Table(false);
    emit sendDrawTable(data);
    fileSaved = false;
    filePath.clear();
}

void TableViewerWindow::openTriggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "/home/", tr("Decision File (*.dec)"));
    if (fileName.isEmpty())
    {
        return;
    }
    *data = FileSystemManager::readFile(fileName);
    emit sendDrawTable(data);
    fileSaved = true;
    filePath = fileName;
}

void TableViewerWindow::saveTriggered()
{
    if (filePath.isEmpty())
    {
        saveAsTriggered();
    }
    else
    {
        if (FileSystemManager::writeFile(filePath, data))
        {
            fileSaved = true;
        }
        else
        {
            std::cerr << "Failed to save file." << std::endl;
        }
    }
}

void TableViewerWindow::saveAsTriggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "/home/", tr("Decision File (*.dec)"));
    if (fileName.isEmpty())
    {
        return;
    }
    if (FileSystemManager::writeFile(fileName, data))
    {
        fileSaved = true;
        filePath = fileName;
    }
    else
    {
        std::cerr << "Failed to save file." << std::endl;
    }
}

void TableViewerWindow::closeTriggered()
{
    // Implement close file logic here
}

void TableViewerWindow::quitTriggered()
{
    close();
}

void TableViewerWindow::changeColumnType(Type type)
{
    if (table->selectedColumn() == 0)
    {
        columnDock->setType(NAME);
        return;
    }
    if (table->selectedColumn() + 1 == data->headingCount())
    {
        columnDock->setType(Type::NUM);
        return;
    }
    data->heading(table->selectedColumn())->setType(*DataType::createDataType(type));
    columnDock->setType(data->heading(table->selectedColumn())->type().getType());
    fileSaved = false;
    emit sendDrawTable(data);
}
