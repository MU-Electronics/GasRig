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
        target: PressuriseVsTimeGraph
        onEmit_newPressureGraphData: {
            // Load the new data
            PressuriseVsTimeGraph.updatePressure(chartView.series("Pressure"));
        }
        onEmit_newValveOneGraphData: {
            // Load the new data
            PressuriseVsTimeGraph.updateValveOne(1, chartView.series("Valve One"));
        }
        onEmit_newValveTwoGraphData: {
            // Load the new data
            PressuriseVsTimeGraph.updateValveTwo(2, chartView.series("Valve Two"));
        }
        onEmit_newValveSevenGraphData: {
            // Load the new data
            PressuriseVsTimeGraph.updateValveSeven(7, chartView.series("Valve Seven"));
        }
        onEmit_newValveNineGraphData: {
            // Load the new data
            PressuriseVsTimeGraph.updateValveNine(9, chartView.series("Valve Nine"));
        }
    }

    ChartView {
        id: chartView

        animationOptions: ChartView.AllAnimations
        antialiasing: true
        dropShadowEnabled: true
        backgroundRoundness : 5
        property bool openGL: true

        title: "Vacuum Readings"
        height: 400
        width: parent.width - 10

        CategoryAxis {
            id: valveAxisY
            min: 0
            max: 1
            CategoryRange {
                label: "Closed"
                endValue: 0
            }
            CategoryRange {
                label: "Open"
                endValue: 1
            }
        }

        ValueAxis {
           id: axisY
           min: PressuriseVsTimeGraph.graphMinY
           max: PressuriseVsTimeGraph.graphMaxY
           titleText: "Pressure (Bar)"
           alignment:  Qt.AlignLeft;
        }

        DateTimeAxis {
            id: axisX
            min: PressuriseVsTimeGraph.graphMinX
            max: PressuriseVsTimeGraph.graphMaxX
            tickCount: 10
            titleText: "Time (Minute:Second:Millisecond)"
            format: "h:m:s:z" // z = millisecond
            minorGridVisible: true
            labelsAngle: 90
            gridVisible : true
            alignment:  Qt.AlignBottom;
        }



        SplineSeries {
            id: pressure
            name: "Pressure"
            axisX: axisX
            axisY: axisY
            useOpenGL: chartView.openGL
        }

        ScatterSeries {
            id: valveSeven
            name: "Valve Nine"
            axisX: axisX
            axisY: valveAxisY
            useOpenGL: chartView.openGL
            markerSize: 5
            borderWidth: 1
        }

        ScatterSeries {
            id: valveSeven
            name: "Valve Seven"
            axisX: axisX
            axisY: valveAxisY
            useOpenGL: chartView.openGL
            markerSize: 5
            borderWidth: 1
        }

        ScatterSeries {
            id: valveTwo
            name: "Valve Two"
            axisX: axisX
            axisY: valveAxisY
            useOpenGL: chartView.openGL
            markerSize: 5
            borderWidth: 1
        }

        ScatterSeries {
            id: valveOne
            name: "Valve One"
            axisX: axisX
            axisY: valveAxisY
            useOpenGL: chartView.openGL
            markerSize: 5
            borderWidth: 1
        }
    }

}
