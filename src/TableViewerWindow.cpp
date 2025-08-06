#include <iostream>
#include "TableViewerWindow.h"
#include "./ui_tableviewerwindow.h"
#include "FileSystemManager.h"
#include "PreferencesManager.h"
#include "PreferencesWindow.h"
#include "MenuBarFile.h"
#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>
#include <QFile>

TableViewerWindow::TableViewerWindow(QWidget* parent)
    : QMainWindow(parent)
      , ui(new Ui::TableViewerWindow)
{
    ui->setupUi(this);
    
    // Initialize preferences manager
    _m_preferencesManager = new PreferencesManager(this);
    
    _m_data = new Table(true);

    _m_toolbar = new TableViewerToolbar(this);
    ui->TableContainer->addWidget(_m_toolbar);
    connect(_m_toolbar, &TableViewerToolbar::newRow, this, &TableViewerWindow::newRowTriggered);
    connect(_m_toolbar, &TableViewerToolbar::newColumn, this, &TableViewerWindow::newColumnTriggered);

    _m_menubar = new TableViewerMenubar(this);
    connect(_m_menubar, &TableViewerMenubar::zoom, this, &TableViewerWindow::changeZoom);
    connect(_m_menubar, &TableViewerMenubar::filterChanged, this, &TableViewerWindow::filterTable);
    connect(_m_menubar, &TableViewerMenubar::sortChanged, this, &TableViewerWindow::sortTable);
    
    // Connect preferences manager to file menu for recent files
    auto* fileMenu = _m_menubar->getFileMenu();
    if (fileMenu)
    {
        fileMenu->setPreferencesManager(_m_preferencesManager);
    }

    _m_table = new TableManager(this);
    ui->TableContainer->addWidget(_m_table);
    connect(_m_table, &TableManager::selectItem, this, &TableViewerWindow::selectItem);
    connect(_m_table, &TableManager::columnReordered, this, &TableViewerWindow::reorderColumnsByDrag);
    connect(_m_table, &TableManager::rowReordered, this, &TableViewerWindow::reorderRowsByDrag);
    connect(this, &TableViewerWindow::sendDrawTable, _m_table, &TableManager::drawTable);
    connect(this, &TableViewerWindow::sendDrawTable, this, &TableViewerWindow::actionOccured);

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

    _m_undoStack = std::stack<Table>();
    _m_undoStack.push(_m_table);
    _m_redoStack = std::stack<Table>();

    // Connect preference change signals
    connect(_m_preferencesManager, &PreferencesManager::backgroundColorChanged,
            this, &TableViewerWindow::onBackgroundColorChanged);
    connect(_m_preferencesManager, &PreferencesManager::textColorChanged,
            this, &TableViewerWindow::onTextColorChanged);
    connect(_m_preferencesManager, &PreferencesManager::defaultZoomChanged,
            this, &TableViewerWindow::onDefaultZoomChanged);

    // Apply current color theme
    applyColorTheme();

    emit sendDrawTable(_m_data);
}

void TableViewerWindow::actionOccured(Table* i_table)
{
    if (!_m_undoing)
    {
        _m_undoStack.push(*i_table);
    }
    _m_undoing = false;
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
    openFileInternal(fileName);
}

void TableViewerWindow::openRecentFile(const QString& filePath)
{
    // Check if file exists
    if (!QFile::exists(filePath))
    {
        QMessageBox::warning(this, tr("File Not Found"),
                           tr("The file '%1' no longer exists.").arg(filePath));
        return;
    }
    
    openFileInternal(filePath);
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
            
            // Add to recent files
            _m_preferencesManager->addRecentFile(_m_filePath);
            
            // Update recent files menu
            auto* fileMenu = _m_menubar->getFileMenu();
            if (fileMenu)
            {
                fileMenu->updateRecentFiles();
            }
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
        
        // Add to recent files
        _m_preferencesManager->addRecentFile(fileName);
        
        // Update recent files menu
        auto* fileMenu = _m_menubar->getFileMenu();
        if (fileMenu)
        {
            fileMenu->updateRecentFiles();
        }
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

void TableViewerWindow::undoTriggered()
{
    if (!_m_undoStack.empty())
    {
        _m_undoing = true;
        _m_redoStack.push(_m_undoStack.top());
        _m_undoStack.pop();
        *_m_data = _m_undoStack.top();
        emit sendDrawTable(_m_data);
    }
}
void TableViewerWindow::redoTriggered()
{
    if (!_m_redoStack.empty())
    {
        _m_undoing = true;
        *_m_data = _m_redoStack.top();
        _m_undoStack.push(_m_data);
        _m_redoStack.pop();
        emit sendDrawTable(_m_data);
    }
}

void TableViewerWindow::cutTriggered()
{
    // natively managed by the clipboard
    emit sendDrawTable(_m_data);
}
void TableViewerWindow::copyTriggered()
{
    // natively managed by the clipboard
}
void TableViewerWindow::pasteTriggered()
{
    // natively managed by the clipboard
    emit sendDrawTable(_m_data);
}
void TableViewerWindow::preferencesTriggered()
{
    PreferencesWindow prefsWindow(_m_preferencesManager, this);
    if (prefsWindow.exec() == QDialog::Accepted)
    {
        // Update UI based on new preferences
        auto* fileMenu = _m_menubar->getFileMenu();
        if (fileMenu)
        {
            fileMenu->updateRecentFiles();
        }
        
        // The color and zoom changes will be handled automatically by the signals
        // from PreferencesManager, but we call applyColorTheme() to ensure immediate update
        applyColorTheme();
    }
}

void TableViewerWindow::openFileInternal(const QString& filePath)
{
    try
    {
        *_m_data = FileSystemManager::readFile(filePath);
        emit sendDrawTable(_m_data);
        _m_fileSaved = true;
        _m_filePath = filePath;
        
        // Add to recent files
        _m_preferencesManager->addRecentFile(filePath);
        
        // Update recent files menu
        auto* fileMenu = _m_menubar->getFileMenu();
        if (fileMenu)
        {
            fileMenu->updateRecentFiles();
        }
    }
    catch (...)
    {
        QMessageBox::warning(this, tr("Error Opening File"),
                           tr("Failed to open file '%1'.").arg(filePath));
    }
}
void TableViewerWindow::findTriggered()
{
    if (_m_findLineEdit == nullptr)
    {
        _m_findLineEdit = new QLineEdit(this);
        connect(_m_findLineEdit, &QLineEdit::textEdited, _m_table, &TableManager::findTriggered);
        ui->TableContainer->addWidget(_m_findLineEdit);
    }
    else
    {
        ui->TableContainer->removeWidget(_m_findLineEdit);
        disconnect(_m_findLineEdit, &QLineEdit::textEdited, _m_table, &TableManager::findTriggered);
        delete _m_findLineEdit;
    }
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

void TableViewerWindow::changeZoom(float i_newZoom)
{
    _m_table->zoomChanged(i_newZoom);
}

void TableViewerWindow::filterTable(const QString& filterText)
{
    // Use the new filterTable method instead of findTriggered
    _m_table->filterTable(filterText);
}

void TableViewerWindow::sortTable(int columnIndex, bool ascending)
{
    if (!_m_data) return;
    
    // Use the new sort method in Table class
    _m_data->sortByColumn(columnIndex, ascending);
    _m_fileSaved = false;
    
    // Redraw the table to show the sorted data
    emit sendDrawTable(_m_data);
}

void TableViewerWindow::reorderColumns()
{
    // Use the reordering functionality in Table class
    if (_m_data) {
        _m_data->reorderColumnsByDisplayIndex();
        _m_fileSaved = false;
        emit sendDrawTable(_m_data);
    }
}

void TableViewerWindow::reorderRows()
{
    // Use the reordering functionality in Table class
    if (_m_data) {
        _m_data->reorderRowsByDisplayIndex();
        _m_fileSaved = false;
        emit sendDrawTable(_m_data);
    }
}

void TableViewerWindow::reorderColumnsByDrag(int fromIndex, int toIndex)
{
    if (!_m_data || fromIndex == toIndex) return;
    
    // Implement actual column reordering by swapping columns
    if (fromIndex >= 0 && fromIndex < static_cast<int>(_m_data->headingCount()) &&
        toIndex >= 0 && toIndex < static_cast<int>(_m_data->headingCount())) {
        
        // Update display indices for reordering
        _m_data->heading(fromIndex)->setDisplayIndex(toIndex);
        _m_data->heading(toIndex)->setDisplayIndex(fromIndex);
        
        _m_data->reorderColumnsByDisplayIndex();
        _m_fileSaved = false;
        emit sendDrawTable(_m_data);
    }
}

void TableViewerWindow::reorderRowsByDrag(int fromIndex, int toIndex)
{
    if (!_m_data || fromIndex == toIndex) return;
    
    // Implement actual row reordering by swapping rows
    if (fromIndex >= 0 && fromIndex < static_cast<int>(_m_data->rowCount()) &&
        toIndex >= 0 && toIndex < static_cast<int>(_m_data->rowCount())) {
        
        // Update display indices for reordering
        _m_data->row(fromIndex)->setDisplayIndex(toIndex);
        _m_data->row(toIndex)->setDisplayIndex(fromIndex);
        
        _m_data->reorderRowsByDisplayIndex();
        _m_fileSaved = false;
        emit sendDrawTable(_m_data);
    }
}


// Help menu slot implementations
void TableViewerWindow::searchTriggered()
{
    // Reuse the existing find functionality for search
    findTriggered();
}

void TableViewerWindow::reportBugTriggered()
{
    QDesktopServices::openUrl(QUrl("https://github.com/supersliser/QTDecisionMaker/issues/new"));
}

void TableViewerWindow::viewSourceTriggered()
{
    QDesktopServices::openUrl(QUrl("https://github.com/supersliser/QTDecisionMaker"));
}

void TableViewerWindow::openForumsTriggered()
{
    QDesktopServices::openUrl(QUrl("https://github.com/supersliser/QTDecisionMaker/discussions"));
}

void TableViewerWindow::documentationTriggered()
{
    QDesktopServices::openUrl(QUrl("https://github.com/supersliser/QTDecisionMaker/wiki"));
}

void TableViewerWindow::onBackgroundColorChanged(const QColor& color)
{
    applyColorTheme();
}

void TableViewerWindow::onTextColorChanged(const QColor& color)
{
    applyColorTheme();
}

void TableViewerWindow::onDefaultZoomChanged(float zoom)
{
    changeZoom(zoom);
}

void TableViewerWindow::applyColorTheme()
{
    QColor backgroundColor = _m_preferencesManager->getBackgroundColor();
    QColor textColor = _m_preferencesManager->getTextColor();
    
    // Create a palette with the custom colors
    QPalette customPalette = this->palette();
    customPalette.setColor(QPalette::Window, backgroundColor);
    customPalette.setColor(QPalette::WindowText, textColor);
    customPalette.setColor(QPalette::Base, backgroundColor.lighter(110)); // Slightly lighter for input fields
    customPalette.setColor(QPalette::AlternateBase, backgroundColor.lighter(120));
    customPalette.setColor(QPalette::Text, textColor);
    customPalette.setColor(QPalette::Button, backgroundColor);
    customPalette.setColor(QPalette::ButtonText, textColor);
    
    // Apply to the main window
    this->setPalette(customPalette);
    
    // Apply to major UI components
    if (_m_table)
    {
        _m_table->setPalette(customPalette);
    }
    if (_m_itemDock)
    {
        _m_itemDock->setPalette(customPalette);
    }
    if (_m_columnDock)
    {
        _m_columnDock->setPalette(customPalette);
    }
    if (_m_toolbar)
    {
        _m_toolbar->setPalette(customPalette);
    }
    
    // Force update of all child widgets
    this->update();
}
