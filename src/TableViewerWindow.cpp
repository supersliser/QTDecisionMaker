#include <iostream>
#include "TableViewerWindow.h"
#include "./ui_tableviewerwindow.h"
#include "FileSystemManager.h"

TableViewerWindow::TableViewerWindow(QWidget* parent)
    : QMainWindow(parent)
      , ui(new Ui::TableViewerWindow)
{
    ui->setupUi(this);
    _m_data = new Table(true);

    _m_toolbar = new TableViewerToolbar(this);
    ui->TableContainer->addWidget(_m_toolbar);
    connect(_m_toolbar, &TableViewerToolbar::newRow, this, &TableViewerWindow::newRowTriggered);
    connect(_m_toolbar, &TableViewerToolbar::newColumn, this, &TableViewerWindow::newColumnTriggered);

    _m_menubar = new TableViewerMenubar(this);

    _m_table = new TableManager(this);
    ui->TableContainer->addWidget(_m_table);
    connect(_m_table, &TableManager::selectItem, this, &TableViewerWindow::selectItem);
    connect(this, &TableViewerWindow::sendDrawTable, _m_table, &TableManager::drawTable);

    _m_itemDock = new TableDataDock(ui->verticalFrame);
    ui->verticalFrame->layout()->addWidget(_m_itemDock);
    connect(_m_itemDock, &TableDataDock::displayValueChanged, this, &TableViewerWindow::editItemDisplay);
    connect(_m_itemDock, &TableDataDock::worthValueChanged, this, &TableViewerWindow::editItemWorth);
    connect(_m_table, &TableManager::itemEdited, _m_itemDock, &TableDataDock::displayValueUpdated);
    connect(this, &TableViewerWindow::itemSelected, _m_itemDock, &TableDataDock::setItem);

    _m_columnDock = new TableColumnDataDock(ui->verticalFrame);
    ui->verticalFrame->layout()->addWidget(_m_columnDock);
    connect(_m_columnDock, &TableColumnDataDock::displayValueChanged, this, &TableViewerWindow::editColumnName);
    connect(_m_columnDock, &TableColumnDataDock::worthValueChanged, this, &TableViewerWindow::editColumnImportance);
    connect(_m_columnDock, &TableColumnDataDock::typeChanged, this, &TableViewerWindow::changeColumnType);
    connect(this, &TableViewerWindow::columnSelected, _m_columnDock, &TableColumnDataDock::setItem);

    emit sendDrawTable(_m_data);
}

TableViewerWindow::~TableViewerWindow()
{
    delete ui;
}

void TableViewerWindow::selectItem(int i_row, int i_column)
{
    emit itemSelected(_m_data, i_row, i_column);
    emit columnSelected(_m_data, i_column);
}

void TableViewerWindow::newColumnTriggered()
{
    _m_data->addHeading(Column("Unnamed Column"));
    _m_fileSaved = false;

    emit sendDrawTable(_m_data);
}

void TableViewerWindow::newRowTriggered()
{
    _m_data->addRow(Row("Unnamed Item"));
    _m_fileSaved = false;

    emit sendDrawTable(_m_data);
}

void TableViewerWindow::editItemDisplay(std::string i_value)
{
    Item* i = _m_data->item(_m_table->selectedColumn() - 1, _m_table->selectedRow());
    if (i == nullptr) { return; }
    i->displayValue = i_value;
    _m_fileSaved = false;
    Column* c = _m_data->heading(_m_table->selectedColumn() - 1);
    c->testAutoSetType(i->displayValue);
    _m_columnDock->setType(c->type().type());
    emit sendDrawTable(_m_data);
}

void TableViewerWindow::editItemWorth(float i_value)
{
    Item* i = _m_data->item(_m_table->selectedColumn() - 1, _m_table->selectedRow());
    if (i == nullptr) { return; }
    i->worthValue = i_value;
    _m_fileSaved = false;
    emit sendDrawTable(_m_data);
}

void TableViewerWindow::editColumnName(std::string i_value)
{
    _m_data->heading(_m_table->selectedColumn() - 1)->setName(i_value);
    _m_fileSaved = false;
    emit sendDrawTable(_m_data);
}

void TableViewerWindow::editColumnImportance(float i_value)
{
    if (_m_table->selectedColumn() < 0) { return;}
    _m_data->heading(_m_table->selectedColumn() - 1)->setImportance(i_value);
    _m_fileSaved = false;
    emit sendDrawTable(_m_data);
}

void TableViewerWindow::newTriggered()
{
    delete _m_data;
    _m_data = new Table(false);
    emit sendDrawTable(_m_data);
    _m_fileSaved = false;
    _m_filePath.clear();
}

void TableViewerWindow::openTriggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "/home/", tr("Decision File (*.dec)"));
    if (fileName.isEmpty())
    {
        return;
    }
    *_m_data = FileSystemManager::readFile(fileName);
    emit sendDrawTable(_m_data);
    _m_fileSaved = true;
    _m_filePath = fileName;
}

void TableViewerWindow::saveTriggered()
{
    if (_m_filePath.isEmpty())
    {
        saveAsTriggered();
    }
    else
    {
        if (FileSystemManager::writeFile(_m_filePath, _m_data))
        {
            _m_fileSaved = true;
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
    if (FileSystemManager::writeFile(fileName, _m_data))
    {
        _m_fileSaved = true;
        _m_filePath = fileName;
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

void TableViewerWindow::changeColumnType(Type i_type)
{
    if (_m_table->selectedColumn() == 0)
    {
        _m_columnDock->setType(NAME);
        return;
    }
    if (_m_table->selectedColumn() + 1 == _m_data->headingCount())
    {
        _m_columnDock->setType(Type::NUM);
        return;
    }
    _m_data->heading(_m_table->selectedColumn())->setType(*DataType::createDataType(i_type));
    _m_columnDock->setType(_m_data->heading(_m_table->selectedColumn())->type().type());
    _m_fileSaved = false;
    emit sendDrawTable(_m_data);
}
