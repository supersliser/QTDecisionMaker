//
// Created by t on 14/05/25.
//

#ifndef TABLEVIEWERMENUBAR_H
#define TABLEVIEWERMENUBAR_H

#include <QMenuBar>

#include "MenuBarEdit.h"
#include "MenuBarFile.h"
#include "MenuBarHelp.h"
#include "MenuBarView.h"


class TableViewerMenubar : public QMenuBar
{
    Q_OBJECT

public:
    TableViewerMenubar(QWidget* parent = nullptr);

    ~TableViewerMenubar() override = default;
    
    MenuBarFile* getFileMenu() const { return _m_fileMenu; }

signals:
    void zoom(float i_newZoomAmount);
    void filterChanged(const QString& filterText);
    void sortChanged(int columnIndex, bool ascending);

private slots:
    void _zoom(float i_newZoomAmount);
    void _filterChanged(const QString& filterText);
    void _sortChanged(int columnIndex, bool ascending);
    void _showFilterDialog();
    void _showSortDialog();

private:
    MenuBarFile* _m_fileMenu;
    MenuBarEdit* _m_editMenu;
    MenuBarView* _m_viewMenu;
    MenuBarHelp* _m_helpMenu;
    QMenu* _m_filterMenu;
    QMenu* _m_sortMenu;
};


#endif //TABLEVIEWERMENUBAR_H
