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

    // connect(ui->DataTable, &QTableWidget::currentCellChanged, this, &TableViewerWindow::selectItem);
    connect(ui->DisplayData, &QLineEdit::editingFinished, this, &TableViewerWindow::editItemDisplay);
    connect(ui->WorthData, &QDoubleSpinBox::valueChanged, this, &TableViewerWindow::editItemWorth);
    connect(ui->columnName, &QLineEdit::editingFinished, this, &TableViewerWindow::editColumnName);
    connect(ui->columnImportance, &QDoubleSpinBox::valueChanged, this, &TableViewerWindow::editColumnImportance);

    for (int i = 0; i < 7; i++)
    {
        ui->TypeDropDown->addItem(tr(DataType::createDataType((Type)(i))->getName().data()));
    }

    connect(ui->TypeDropDown, &QComboBox::currentIndexChanged, this, &TableViewerWindow::changeColumnType);

    emit sendDrawTable(*data);
}

TableViewerWindow::~TableViewerWindow()
{
    delete ui;
}

// void TableViewerWindow::drawTable()
// {
//     auto disp = ui->DataTable;
//
//
//     // Set column headers
//     disp->setColumnCount(data->headingCount() + 1);
//     for (int column = 0; column < data->headingCount(); column++)
//     {
//         setColumnHeader(column, tr(data->heading(column)->name().data()));
//     }
//     setColumnHeader(data->headingCount(), tr("Total Value"));
//
//     // Populate rows
//     data->calculateAllTotals();
//     disp->setRowCount(data->rowCount());
//     for (int row = 0; row < data->rowCount(); row++)
//     {
//         setItem(row, 0, tr(data->row(row)->name().data()));
//         for (int column = 1; column < data->headingCount(); column++)
//         {
//             setItem(row, column, tr(data->item(column, row)->displayValue.data()));
//         }
//         auto d = data->row(row)->totalValue();
//         setItem(row, data->headingCount(), fmt::format("{:.2f}", d).c_str());
//     }
//     disp->show();
// }

void TableViewerWindow::selectItem(int row, int column)
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
        if (column == data->headingCount())
        {
            ui->TypeDropDown->setCurrentIndex(6);
        }
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
        ui->TypeDropDown->setCurrentIndex(c->type().getType());
    }

    if (column == 0)
    {
        ui->TypeDropDown->setCurrentIndex(0);
        delete i;
    }

}

void TableViewerWindow::newColumnTriggered()
{
    data->addHeading(Column("Unnamed Column"));
    fileSaved = false;

    emit sendDrawTable(*data);
}

void TableViewerWindow::newRowTriggered()
{
    data->addRow(Row("Unnamed Item"));
    fileSaved = false;

    emit sendDrawTable(*data);
}

void TableViewerWindow::editItemDisplay()
{
    if (table->currentColumn() == 0)
    {
        data->row(table->currentRow())->setName(ui->DisplayData->text().toStdString());
    }
    Item* i = data->item(table->currentColumn(), table->currentRow());
    if (i == nullptr)
    {
        return;
    }
    i->displayValue = ui->DisplayData->text().toStdString();
    fileSaved = false;
    Column* c = data->heading(table->currentColumn());
    c->testAutoSetType(i->displayValue);
    ui->TypeDropDown->setCurrentIndex(c->type().getType());
    emit sendDrawTable(*data);
}

void TableViewerWindow::editItemWorth()
{
    Item* i = data->item(table->currentColumn(), table->currentRow());
    if (i == nullptr)
    {
        return;
    }
    i->worthValue = ui->WorthData->value();
    fileSaved = false;
    emit sendDrawTable(*data);
}

void TableViewerWindow::editColumnName()
{
    data->heading(table->currentColumn())->setName(ui->columnName->text().toStdString());
    fileSaved = false;
    emit sendDrawTable(*data);
}

void TableViewerWindow::editColumnImportance()
{
    data->heading(table->currentColumn())->setImportance(ui->columnImportance->value());
    fileSaved = false;
        emit sendDrawTable(*data);
}

 void TableViewerWindow::newTriggered()
 {
    delete data;
    data = new Table(false);
    emit sendDrawTable(*data);
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
    emit sendDrawTable(*data);
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

void TableViewerWindow::changeColumnType(int index)
{
    if (index == -1)
    {
        ui->TypeDropDown->clear();
        return;
    }
    if (table->currentColumn() == 0)
    {
        ui->TypeDropDown->setCurrentIndex(0);
        return;
    }
    if (table->currentColumn() == data->headingCount())
    {
        ui->TypeDropDown->setCurrentIndex(6);
        return;
    }
    data->heading(table->currentColumn())->setType(*DataType::createDataType((Type)index));
    fileSaved = false;
    emit sendDrawTable(*data);
}