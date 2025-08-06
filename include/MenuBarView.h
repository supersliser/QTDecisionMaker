//
// Created by: T on 19/07/2025
//

#ifndef MENUBARVIEW_H
#define MENUBARVIEW_H

#include <QMenu>

class MenuBarView : public QMenu
{
    Q_OBJECT

public:
    MenuBarView(QWidget* parent = nullptr);
    ~MenuBarView() override = default;

signals:
    void zoom(float i_newZoomAmount);
    void filterChanged(const QString& filterText);
    void sortChanged(int columnIndex, bool ascending);

private slots:
    void _zoom25();
    void _zoom50();
    void _zoom100();
    void _zoom200();
    void _zoomin();
    void _zoomout();
    void _showFilterDialog();
    void _showSortDialog();
private:
    float _m_currentZoom = 100.0f;
};
#endif
