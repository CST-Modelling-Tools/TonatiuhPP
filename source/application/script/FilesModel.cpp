#include "FilesModel.h"


FilesModel::FilesModel(QObject* parent):
    QFileSystemModel(parent)
{
    setFilter(QDir::AllDirs | QDir::NoDotAndDotDot | QDir::Files);
    setNameFilterDisables(false);
}

int FilesModel::columnCount(const QModelIndex& /*parent*/) const
{
    return 1;
}
