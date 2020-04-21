#pragma once
#include <QtPlugin>
#include <QVector>
#include <QVariant>

class QString;
class QIcon;
class TShape;

class TShapeFactory
{
public:
    virtual ~TShapeFactory() {}
    virtual QString TShapeName() const = 0;
    virtual QIcon TShapeIcon() const = 0;
    virtual TShape* CreateTShape() const = 0;
    virtual TShape* CreateTShape(int /*numberofParameters*/, QVector< QVariant > /*parametersList*/) const
    {
        return (CreateTShape() );
    }
    virtual bool IsFlat() = 0;
};

Q_DECLARE_INTERFACE(TShapeFactory, "tonatiuh.TShapeFactory")
