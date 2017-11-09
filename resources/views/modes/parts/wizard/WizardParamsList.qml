import QtQuick 2.0

import Fluid.Core 1.0 as FluidCore
import Fluid.Layouts 1.0 as FluidLayouts


FluidLayouts.AutomaticGrid
{
    id: paramsLayoutGrid

    property var noInColoum: 2
    property var noInRow: 2

    width: parent.width - 10
    height: height.width

    cellWidth: 180
    cellHeight: 20

    model: ListModel {
        ListElement { title: qsTr(""); value: "" }
    }

    delegate: WizardParamValue{
        title: model.title
        value: model.value
        width: 180
        height: 20
    }
}
