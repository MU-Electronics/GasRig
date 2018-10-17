import QtQuick 2.0
import QtQuick.Controls 2.1
import Fluid.Layouts 1.0 as FluidLayouts
import Fluid.Controls 1.0 as FluidControls
import Fluid.Material 1.0 as FluidMaterial
import QtQuick.Controls.Material 2.0
import Fluid.Core 1.0 as FluidCore
import QtQuick.Layouts 1.0


ColumnLayout
{
    width: parent.width
    height: a.height + b.height + c.height + d.height + e.height + f.height
   //height: parent.height
    RowLayout
    {
        id: a
        SysFunction {
            Layout.fillWidth: true
            display: "Pressurise"
            desc: "Set a pressure of a selected gas from 5 mbar to 95 Bar"
            dropEnabled: true
        }
        SysFunction {
            Layout.fillWidth: true
            display: "Vent"
            desc: "Vent a pressure to exhuast"
            dropEnabled: true
        }
    }
    RowLayout
    {
        id: b
        SysFunction {
            Layout.fillWidth: true
            height: 80
            display: "Purge"
            desc: "Purge the system and or output automatically"
            dropEnabled: true
        }
        SysFunction {
            Layout.fillWidth: true
            height: 80
            display: "Pulse Valve"
            desc: "Pulse a valve x times with a defined duty cycle"
            dropEnabled: true
        }
    }
    RowLayout
    {
        id: c
        SysFunction {
            Layout.fillWidth: true
            height: 80
            display: "Set Valve"
            desc: "Set the state of a valve"
            dropEnabled: true
        }
        SysFunction {
            Layout.fillWidth: true
            height: 80
            display: "Vacumm"
            desc: "Enable the vac station for x minutes with a selected location"
            dropEnabled: true
        }
    }
    RowLayout
    {
        id: d
        SysFunction {
            Layout.fillWidth: true
            height: 80
            display: "Backing Pump"
            desc: "Enable backing pump perminatly"
            dropEnabled: true
        }
        SysFunction {
            Layout.fillWidth: true
            height: 80
            display: "Pumping Gas Type"
            desc: "Define the type of gas the vac station is handling"
            dropEnabled: true
        }
    }
    RowLayout
    {
        id: e
        SysFunction {
            Layout.fillWidth: true
            height: 80
            display: "Flow rate"
            desc: "Set a flow rate for a selected flow controller"
            dropEnabled: true
        }
        SysFunction {
            Layout.fillWidth: true
            height: 80
            display: "Flow output"
            desc: "Set the internal output valve of the flow controller"
            dropEnabled: true
        }
    }
    RowLayout
    {
        id: f
        SysFunction {
            Layout.fillWidth: true
            height: 80
            display: "Flow soft start"
            desc: "Define a soft start value for a selected flow controller"
            dropEnabled: true
        }
        SysFunction {
            Layout.fillWidth: true
            height: 80
            display: "Wait"
            desc: "Wait for a period of time"
            dropEnabled: true
        }
    }
}
