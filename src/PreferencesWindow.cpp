//
// Created for QTDecisionMaker preferences dialog
//

#include "../include/PreferencesWindow.h"
#include "../include/PreferencesManager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTabWidget>
#include <QLabel>
#include <QPushButton>
#include <QDoubleSpinBox>
#include <QListWidget>
#include <QColorDialog>
#include <QGroupBox>
#include <QFormLayout>
#include <QMessageBox>

PreferencesWindow::PreferencesWindow(PreferencesManager* prefsManager, QWidget* parent)
    : QDialog(parent)
    , _prefsManager(prefsManager)
    , _currentBackgroundColor(prefsManager->getBackgroundColor())
    , _currentTextColor(prefsManager->getTextColor())
    , _currentZoom(prefsManager->getDefaultZoom())
{
    setWindowTitle(tr("Preferences"));
    setModal(true);
    setMinimumSize(500, 400);
    
    setupUi();
    updateColorButtons();
    updateRecentFilesList();
}

void PreferencesWindow::setupUi()
{
    auto* mainLayout = new QVBoxLayout(this);
    
    _tabWidget = new QTabWidget(this);
    mainLayout->addWidget(_tabWidget);
    
    setupGeneralTab();
    setupAppearanceTab();
    setupRecentFilesTab();
    
    // Button layout
    auto* buttonLayout = new QHBoxLayout();
    
    _resetButton = new QPushButton(tr("Reset to Defaults"), this);
    _cancelButton = new QPushButton(tr("Cancel"), this);
    _okButton = new QPushButton(tr("OK"), this);
    
    _okButton->setDefault(true);
    
    buttonLayout->addWidget(_resetButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(_cancelButton);
    buttonLayout->addWidget(_okButton);
    
    mainLayout->addLayout(buttonLayout);
    
    // Connect buttons
    connect(_resetButton, &QPushButton::clicked, this, &PreferencesWindow::onResetToDefaults);
    connect(_cancelButton, &QPushButton::clicked, this, &PreferencesWindow::onCancelClicked);
    connect(_okButton, &QPushButton::clicked, this, &PreferencesWindow::onOkClicked);
}

void PreferencesWindow::setupGeneralTab()
{
    _generalTab = new QWidget();
    _tabWidget->addTab(_generalTab, tr("General"));
    
    auto* layout = new QVBoxLayout(_generalTab);
    
    // Zoom settings group
    auto* zoomGroup = new QGroupBox(tr("Zoom Settings"), _generalTab);
    auto* zoomLayout = new QFormLayout(zoomGroup);
    
    _zoomSpinBox = new QDoubleSpinBox(zoomGroup);
    _zoomSpinBox->setRange(0.5, 3.0);
    _zoomSpinBox->setSingleStep(0.1);
    _zoomSpinBox->setDecimals(1);
    _zoomSpinBox->setSuffix("x");
    _zoomSpinBox->setValue(_currentZoom);
    
    zoomLayout->addRow(tr("Default Zoom Level:"), _zoomSpinBox);
    
    layout->addWidget(zoomGroup);
    layout->addStretch();
    
    connect(_zoomSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &PreferencesWindow::onZoomChanged);
}

void PreferencesWindow::setupAppearanceTab()
{
    _appearanceTab = new QWidget();
    _tabWidget->addTab(_appearanceTab, tr("Appearance"));
    
    auto* layout = new QVBoxLayout(_appearanceTab);
    
    // Color settings group
    auto* colorGroup = new QGroupBox(tr("Color Settings"), _appearanceTab);
    auto* colorLayout = new QFormLayout(colorGroup);
    
    _backgroundColorButton = new QPushButton(tr("Choose Color"), colorGroup);
    _textColorButton = new QPushButton(tr("Choose Color"), colorGroup);
    
    colorLayout->addRow(tr("Background Color:"), _backgroundColorButton);
    colorLayout->addRow(tr("Text Color:"), _textColorButton);
    
    layout->addWidget(colorGroup);
    layout->addStretch();
    
    connect(_backgroundColorButton, &QPushButton::clicked, this, &PreferencesWindow::onBackgroundColorClicked);
    connect(_textColorButton, &QPushButton::clicked, this, &PreferencesWindow::onTextColorClicked);
}

void PreferencesWindow::setupRecentFilesTab()
{
    _recentFilesTab = new QWidget();
    _tabWidget->addTab(_recentFilesTab, tr("Recent Files"));
    
    auto* layout = new QVBoxLayout(_recentFilesTab);
    
    auto* label = new QLabel(tr("Recently opened files:"), _recentFilesTab);
    layout->addWidget(label);
    
    _recentFilesList = new QListWidget(_recentFilesTab);
    layout->addWidget(_recentFilesList);
    
    auto* buttonLayout = new QHBoxLayout();
    _clearRecentButton = new QPushButton(tr("Clear Recent Files"), _recentFilesTab);
    buttonLayout->addStretch();
    buttonLayout->addWidget(_clearRecentButton);
    
    layout->addLayout(buttonLayout);
    
    connect(_clearRecentButton, &QPushButton::clicked, this, &PreferencesWindow::onClearRecentFiles);
}

void PreferencesWindow::updateColorButtons()
{
    // Update background color button
    QString bgStyle = QString("QPushButton { background-color: %1; }").arg(_currentBackgroundColor.name());
    _backgroundColorButton->setStyleSheet(bgStyle);
    
    // Update text color button
    QString textStyle = QString("QPushButton { background-color: %1; }").arg(_currentTextColor.name());
    _textColorButton->setStyleSheet(textStyle);
}

void PreferencesWindow::updateRecentFilesList()
{
    _recentFilesList->clear();
    QStringList recentFiles = _prefsManager->getRecentFiles();
    for (const QString& file : recentFiles)
    {
        _recentFilesList->addItem(file);
    }
}

void PreferencesWindow::onBackgroundColorClicked()
{
    QColor color = QColorDialog::getColor(_currentBackgroundColor, this, tr("Choose Background Color"));
    if (color.isValid())
    {
        _currentBackgroundColor = color;
        updateColorButtons();
    }
}

void PreferencesWindow::onTextColorClicked()
{
    QColor color = QColorDialog::getColor(_currentTextColor, this, tr("Choose Text Color"));
    if (color.isValid())
    {
        _currentTextColor = color;
        updateColorButtons();
    }
}

void PreferencesWindow::onZoomChanged(double value)
{
    _currentZoom = static_cast<float>(value);
}

void PreferencesWindow::onClearRecentFiles()
{
    auto reply = QMessageBox::question(this, tr("Clear Recent Files"),
                                     tr("Are you sure you want to clear all recent files?"),
                                     QMessageBox::Yes | QMessageBox::No,
                                     QMessageBox::No);
    
    if (reply == QMessageBox::Yes)
    {
        _prefsManager->clearRecentFiles();
        updateRecentFilesList();
    }
}

void PreferencesWindow::onResetToDefaults()
{
    auto reply = QMessageBox::question(this, tr("Reset to Defaults"),
                                     tr("Are you sure you want to reset all preferences to their default values?"),
                                     QMessageBox::Yes | QMessageBox::No,
                                     QMessageBox::No);
    
    if (reply == QMessageBox::Yes)
    {
        _prefsManager->resetToDefaults();
        
        // Update local state
        _currentBackgroundColor = _prefsManager->getBackgroundColor();
        _currentTextColor = _prefsManager->getTextColor();
        _currentZoom = _prefsManager->getDefaultZoom();
        
        // Update UI
        _zoomSpinBox->setValue(_currentZoom);
        updateColorButtons();
        updateRecentFilesList();
    }
}

void PreferencesWindow::onOkClicked()
{
    // Save current settings to preferences manager
    _prefsManager->setBackgroundColor(_currentBackgroundColor);
    _prefsManager->setTextColor(_currentTextColor);
    _prefsManager->setDefaultZoom(_currentZoom);
    
    accept();
}

void PreferencesWindow::onCancelClicked()
{
    reject();
}