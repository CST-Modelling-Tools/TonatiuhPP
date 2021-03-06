#include <Inventor/SoNodeKitPath.h>
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSelection.h>
#include <Inventor/VRMLnodes/SoVRMLBackground.h>

#include <QApplication>
#include <QString>

#include "Document.h"
#include "kernel/scene/TSceneKit.h"
#include "application/view/GraphicRoot.h"

/*!
 * Creates a new document object.
 */
Document::Document():
    m_scene(0)
{
    New();
}

/*!
 * Initializes the document with a empty scene.
 */
void Document::New()
{
    if (m_scene) ClearScene();
    m_scene = new TSceneKit;
    m_scene->ref();
    m_isModified = false;
}

/*!
 * Sets the scene form \a fileName to the document.
 */
bool Document::ReadFile(const QString& fileName)
{
    if (fileName.isEmpty())
        return false;

    SoInput input;

    if (!input.openFile(fileName.toLatin1().data()))
    {
        QString message = QString("Cannot open file %1.").arg(fileName);
        emit Warning(message);
        return false;
    }

    if (!input.isValidFile())
    {
        QString message = QString("Error reading file %1.").arg(fileName);
        emit Warning(message);
        return false;
    }

    SoSeparator* separator = SoDB::readAll(&input);
    input.closeFile();

    if (!separator)
    {
        QString message = QString("Error reading file %1.").arg(fileName);
        emit Warning(message);
        return false;
    }

    TSceneKit* scene = dynamic_cast<TSceneKit*>(separator->getChild(0));
    if (!scene) return false;

    QString version = ((SoSFString*)scene->getField("version"))->getValue().getString();
    if (version != "2020") {
        QString message = QString("Version %1 is not compatible.").arg(version);
        emit Warning(message);
        return false;
    }

    if (m_scene) ClearScene();
    m_scene = scene;
    m_scene->ref();
    m_isModified = false;
    return true;
}

/*!
 * Writes the document scene to a file with the given \a fileName.
 *
 * Returns true if the scene was successfully written; otherwise returns false.
 */
bool Document::WriteFile(const QString& fileName)
{
    SoWriteAction action;
    if (!action.getOutput()->openFile(fileName.toLatin1().constData() ) )
    {
        QString message = QString("Cannot open file %1.").arg(fileName);
        emit Warning(message);
        return false;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);
    action.getOutput()->setBinary(false);
    action.getOutput()->setFloatPrecision(6);
    // by default %.8g for floats and %.16lg for double

    if (fileName.endsWith(".tnh") || fileName.endsWith(".tnhpp")) // normal
        action.apply(m_scene);
    else if (fileName.endsWith(".tnhd")) // debug
        action.apply(m_scene->m_graphicRoot->getRoot());

    action.getOutput()->closeFile();
    QApplication::restoreOverrideCursor();

    m_isModified = false;
    return true;
}

/*!
 * Clears the scene of the document.
 */
void Document::ClearScene()
{
    while (m_scene->getRefCount() > 1) //? >=
        m_scene->unref();
}

