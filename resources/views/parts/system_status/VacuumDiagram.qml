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
        id: chartView

        //animationOptions: ChartView.animationDuration

        title: "Vacuum Readings"
        antialiasing: true
        height: 400
        width: parent.width - 10


        // Temp later change to signal
        Timer {
           id: refreshTimer
           interval: 1 / 60 * 1000 // 60 Hz
           running: true
           repeat: true
           onTriggered: {
               SystemStatusManager.pressureGraphUpdate(chartView.series(0));
           }
        }

        ValueAxis {
           id: axisY1
           min: 0
           max: 2
        }

        ValueAxis {
            id: axisX
            min: 0
            max: SystemStatusManager.graphMaxX
        }

        LineSeries {
            id: lineSeries1
            name: "signal 1"
            axisX: axisX
            axisY: axisY1
            // useOpenGL: chartView.openGL
        }


//        LineSeries {
//            id: vacuumSeries
//            name: "Vacuum"

//            axisX: DateTimeAxis {
//                format: "h:MM:ss"
//                tickCount: 5
//                titleText: "Time"
//            }
//            axisY: ValueAxis {
//                titleText: "Pressure (mBar)"
//                min: 0
//                max: 10
//            }

//            XYPoint { x: 1498478338216; y: 2 }
//            XYPoint { x: 1498478338684; y: 3 }

//        }
    }

}
