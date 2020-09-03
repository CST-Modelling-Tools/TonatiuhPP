#pragma once

#include <QDialog>

namespace Ui {
class SelectSurfaceDialog;
}

class SceneFilter;
class SceneTreeModel;


class SelectSurfaceDialog: public QDialog
{
    Q_OBJECT

public:
    SelectSurfaceDialog(SceneTreeModel& model, bool enableLight = true, QWidget* parent = 0);
    ~SelectSurfaceDialog();

    void SetShapeTypeFilters(QStringList filters);

    QString GetSelectedSurfaceURL() const;

public slots:
    void accept();

private:
    Ui::SelectSurfaceDialog* ui;
    bool m_isLightVisible;
    SceneTreeModel* m_model;
    SceneFilter* m_filter;
};
