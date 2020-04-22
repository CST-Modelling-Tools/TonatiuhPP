#pragma once

#include <QFileSystemModel>


class FilesModel : public QFileSystemModel
{

public:
     FilesModel(QObject *parent = 0);

protected:
     int columnCount( const QModelIndex& parent = QModelIndex() ) const;

};
