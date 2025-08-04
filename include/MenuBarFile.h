//
// Created by t on 14/05/25.
//

#ifndef MENUBARFILECLASS_H
#define MENUBARFILECLASS_H
#include <QMenu>
#include <QAction>

class PreferencesManager;

class MenuBarFile : public QMenu
{
    Q_OBJECT

public:
    MenuBarFile(QWidget* parent = nullptr);
    ~MenuBarFile() override = default;
    
    void setPreferencesManager(PreferencesManager* prefsManager);
    void updateRecentFiles();

private slots:
    void openRecentFile();

private:
    void setupRecentFilesMenu();
    
    PreferencesManager* _preferencesManager;
    QMenu* _recentFilesMenu;
    QAction* _recentFilesSeparator;
    QList<QAction*> _recentFileActions;
    
    static const int MAX_RECENT_FILES = 10;
};


#endif //MENUBARFILECLASS_H
