#pragma once

#include <QDialog>

#include "ui_selectsurfacedialog.h"

class SceneFilter;
//class QItemSelectionModel;
class SceneModel;


class SelectSurfaceDialog: public QDialog, private Ui::SelectSurfaceDialog
{
    Q_OBJECT

public:
    SelectSurfaceDialog(SceneModel& model, bool enableLight = true, QWidget* parent = 0 );
    ~SelectSurfaceDialog();

    void SetShapeTypeFilters(QVector<QString> filters);

    QString GetSelectedSurfaceURL() const;

public slots:
    void accept();

private:
    bool m_isLightVisible;
    SceneModel* m_model;
    SceneFilter* m_filter;
};
