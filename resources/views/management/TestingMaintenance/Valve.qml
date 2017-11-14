import QtQuick 2.5
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.1
import QtQml.Models 2.2

import Fluid.Layouts 1.0 as FluidLayouts
import Fluid.Controls 1.0 as FluidControls
import Fluid.Material 1.0 as FluidMaterial
import Fluid.Core 1.0 as FluidCore

import "../../parts"
import "ValveComponents"

Item {
    id: valveTab

    width: parent.width - 10



    Flickable {

        width: parent.width
        height: window.height - 180
        contentWidth: parent.width
        contentHeight: purgeSystemFunction.height + pulsingValveControl.height + valveStateControl.height + /*outputHighPressure.height +*/ exhuastSystem.height + setHighPressure.height + 50;

        SetHighPressureContainer{
            id: setHighPressure
            width: parent.width - 10
        }

        ExhaustSystemContainer{
            id: exhuastSystem
            anchors.top: setHighPressure.bottom
            anchors.topMargin: 15
        }

        /**
         * Not requires for first release
         *
            OutputHighPressureContainer{
                id: outputHighPressure
                anchors.top: exhuastSystem.bottom
                anchors.topMargin: 15
            }
         *
         */

        PurgeSystemContainer{
            id: purgeSystemFunction
            // anchors.top: outputHighPressure.bottom
            anchors.top: exhuastSystem.bottom
            anchors.topMargin: 15
        }

        PulseValveContainer{
            id: pulsingValveControl
            anchors.top: purgeSystemFunction.bottom
            anchors.topMargin: 15
        }

        SafeValveContainer{
            id: valveStateControl
            anchors.top: pulsingValveControl.bottom
            anchors.topMargin: 15
        }
    }
}
