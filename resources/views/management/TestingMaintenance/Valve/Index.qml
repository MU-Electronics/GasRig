import QtQuick 2.5
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.1
import QtQml.Models 2.2

import Fluid.Layouts 1.0 as FluidLayouts
import Fluid.Controls 1.0 as FluidControls
import Fluid.Material 1.0 as FluidMaterial
import Fluid.Core 1.0 as FluidCore

import "../../../parts"

Item {
    id: valveTab

    width: parent.width



    Flickable {

        width: parent.width
        height: window.height - 180
        contentWidth: parent.width
        contentHeight: purgeSystemFunction.height + pulsingValveControl.height + valveStateControl.height + /*outputHighPressure.height +*/ exhuastSystem.height + setHighPressure.height + 50;

        SetHighPressure{
            id: setHighPressure
            width: parent.width - 10
        }

        ExhaustSystem{
            id: exhuastSystem
            anchors.top: setHighPressure.bottom
            anchors.topMargin: 15
        }

        PurgeSystem{
            id: purgeSystemFunction
            // anchors.top: outputHighPressure.bottom
            anchors.top: exhuastSystem.bottom
            anchors.topMargin: 15
        }

        PulseValve{
            id: pulsingValveControl
            anchors.top: purgeSystemFunction.bottom
            anchors.topMargin: 15
        }

        SafeValve{
            id: valveStateControl
            anchors.top: pulsingValveControl.bottom
            anchors.topMargin: 15
        }
    }
}
