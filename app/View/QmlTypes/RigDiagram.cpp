#include "RigDiagram.h"

// Include external libs
#include <QMap>


namespace App { namespace View { namespace QmlTypes
{
    RigDiagram::RigDiagram(QQuickItem *parent)
        :   QQuickItem(parent)
    {

    }

    RigDiagram::~RigDiagram()
    {

    }


    /**
     * Synchronise the state of the QML scene with the rendering scene graph
     *
     * @brief RigDiagram::updatePaintNode
     * @param oldNode
     * @param nodeData
     * @return
     */
    QSGNode *RigDiagram::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *nodeData)
    {
        // See
        // http://doc.qt.io/qt-5/qtquick-scenegraph-customgeometry-example.html
        // http://doc.qt.io/qt-5/qsggeometrynode.html
        // http://doc.qt.io/qt-5/qquickitem.html#updatePaintNode
    }



}}}

