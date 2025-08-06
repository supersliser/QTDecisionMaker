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
    void reorderColumns();
    void reorderRows();

private slots:
    void _zoom(float i_newZoomAmount);
    void _filterChanged(const QString& filterText);
    void _sortChanged(int columnIndex, bool ascending);
    void _reorderColumns();
    void _reorderRows();

private:
    MenuBarFile* _m_fileMenu;
    MenuBarEdit* _m_editMenu;
    MenuBarView* _m_viewMenu;
    MenuBarHelp* _m_helpMenu;
};


#endif //TABLEVIEWERMENUBAR_H
