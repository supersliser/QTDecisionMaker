//
// Created for QTDecisionMaker preferences dialog
//

#ifndef PREFERENCESWINDOW_H
#define PREFERENCESWINDOW_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTabWidget>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QListWidget>
#include <QColorDialog>

class PreferencesManager;

class PreferencesWindow : public QDialog
{
    Q_OBJECT

public:
    explicit PreferencesWindow(PreferencesManager* prefsManager, QWidget* parent = nullptr);
    ~PreferencesWindow() override = default;

private slots:
    void onBackgroundColorClicked();
    void onTextColorClicked();
    void onZoomChanged(double value);
    void onClearRecentFiles();
    void onResetToDefaults();
    void onOkClicked();
    void onCancelClicked();

private:
    void setupUi();
    void setupGeneralTab();
    void setupAppearanceTab();
    void setupRecentFilesTab();
    void updateColorButtons();
    void updateRecentFilesList();

    PreferencesManager* _prefsManager;
    
    QTabWidget* _tabWidget;
    
    // General tab
    QWidget* _generalTab;
    QDoubleSpinBox* _zoomSpinBox;
    
    // Appearance tab
    QWidget* _appearanceTab;
    QPushButton* _backgroundColorButton;
    QPushButton* _textColorButton;
    QColor _currentBackgroundColor;
    QColor _currentTextColor;
    
    // Recent files tab
    QWidget* _recentFilesTab;
    QListWidget* _recentFilesList;
    QPushButton* _clearRecentButton;
    
    // Buttons
    QPushButton* _okButton;
    QPushButton* _cancelButton;
    QPushButton* _resetButton;
    
    float _currentZoom;
};

#endif // PREFERENCESWINDOW_H