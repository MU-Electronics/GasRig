#pragma once

#include <QtQuick/QQuickItem>
#include <QQmlApplicationEngine>

// Include macros
#include "../../Utilities/Properties.h"

// Include settings container
#include "../../Settings/Container.h"

// Experiment Engine with state machine methods
#include "../../Experiment/Engine.h"

namespace App { namespace View { namespace QmlTypes
{

    class RigDiagram : public QQuickItem
    {
        Q_OBJECT

        public:
            RigDiagram(QQuickItem *parent = 0);
            ~RigDiagram();

            QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *nodeData);

        signals:


        public slots:


        private:

    };
}}}


