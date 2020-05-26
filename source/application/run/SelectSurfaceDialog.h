#pragma once

#include <QDialog>

namespace Ui {
class SelectSurfaceDialog;
}

class SceneFilter;
class SceneModel;


class SelectSurfaceDialog: public QDialog
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
    Ui::SelectSurfaceDialog* ui;
    bool m_isLightVisible;
    SceneModel* m_model;
    SceneFilter* m_filter;
};
