import QtQuick 2.2
import QtQuick.Controls 2.1
import QtQuick.Window 2.2

import QtCharts 2.2


Item
{
    // Hold the select value for backing pump mode
    property bool bootup: false

    width: parent.width
    height: parent.height

    // Redraw diagram on valve update
    Connections {
        target: SystemStatusManager
        onEmit_valveStateChanged: {
        }
        onEmit_pressureSensorChanged: {
        }
        onEmit_vacuumStateChanged: {

        }
        onEmit_flowControllerStateChanged: {
        }
    }

    ChartView {
        title: "Vacuum Readings"
        antialiasing: true
        height: 400
        width: parent.width - 10

        LineSeries {
            id: vacuumSeries
            name: "Vacuum"

            axisX: DateTimeAxis {
                format: "h:MM:ss"
                tickCount: 5
                titleText: "Time"
            }
            axisY: ValueAxis {
                titleText: "Pressure (mBar)"
                min: 0
                max: 10
            }

            XYPoint { x: 1498478338216; y: 2 }
            XYPoint { x: 1498478338684; y: 3 }

        }
    }

}
