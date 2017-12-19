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
            PressuriseVsTimeGraph.updateValve(1, chartView.series("Valve One"));
        }
        onEmit_newValveTwoGraphData: {
            // Load the new data
            PressuriseVsTimeGraph.updateValve(2, chartView.series("Valve Two"));
        }
        onEmit_newValveSevenGraphData: {
            // Load the new data
            PressuriseVsTimeGraph.updateValve(7, chartView.series("Valve Seven"));
        }
        onEmit_newValveNineGraphData: {
            // Load the new data
            PressuriseVsTimeGraph.updateValve(9, chartView.series("Valve Nine"));
        }
    }

    ChartView {
        id: chartView

        // Slower perfromance
        // animationOptions: ChartView.AllAnimations
        // dropShadowEnabled: true

        // Medium performance
        // animationOptions: ChartView.GridAxisAnimations

        // Fastest performance
        animationOptions: ChartView.NoAnimation

        // Enable opengl where possiable
        property bool openGL: true

        // Looks of ChartView
        antialiasing: true
        backgroundRoundness : 5

        // Title of graph
        title: "Pressure Vs Time with valve statuses"

        // Size of graph
        height: height.height - 10
        width: parent.width - 10

        // X axis for valve status
        CategoryAxis {
            id: valveAxisY
            min: -0.5
            max: 1.5
            labelsPosition: CategoryAxis.AxisLabelsPositionOnValue
            titleText: "Valve Status"
            CategoryRange {
                label: "Closed"
                endValue: 0
            }
            CategoryRange {
                label: "Open"
                endValue: 1
            }
        }

        // X axis for pressure
        ValueAxis {
           id: axisY
           min: PressuriseVsTimeGraph.graphMinY
           max: PressuriseVsTimeGraph.graphMaxY
           titleText: "Pressure (Bar)"
        }

        // Y axis for time
        DateTimeAxis {
            id: axisX
            min: PressuriseVsTimeGraph.graphMinX
            max: PressuriseVsTimeGraph.graphMaxX
            tickCount: 10
            titleText: "Time (Minute:Second:Millisecond)"
            format: "h:m:s:z"
            minorGridVisible: true
            labelsAngle: 90
            gridVisible : true
        }

        // Pressure readings series
        // SplineSeries {
         LineSeries {
            id: pressure
            name: "Pressure"
            axisX: axisX
            axisY: axisY

            // Use opengl for performance
            // useOpenGL: chartView.openGL
        }

        // Valve status series
        ScatterSeries  {
            id: valveNine
            name: "Valve Nine"
            axisX: axisX
            markerSize: 8
            borderWidth: 1
            borderColor: valveNine.color
            axisYRight: valveAxisY

            // Use opengl for performance
            // useOpenGL: chartView.openGL
        }

        // Valve seven status series
        ScatterSeries {
            id: valveSeven
            name: "Valve Seven"
            axisX: axisX
            axisYRight: valveAxisY
            markerSize: 8
            borderWidth: 1
            borderColor: valveSeven.color

            // Use opengl for performance
            // useOpenGL: chartView.openGL
        }

        // Valve two status series
        ScatterSeries {
            id: valveTwo
            name: "Valve Two"
            axisX: axisX
            axisYRight: valveAxisY
            markerSize: 8
            borderWidth: 1
            borderColor: valveTwo.color

            // Use opengl for performance
            // useOpenGL: chartView.openGL
        }

        // Valve one status series
        ScatterSeries {
            id: valveOne
            name: "Valve One"
            axisX: axisX
            axisYRight: valveAxisY
            markerSize: 8
            borderWidth: 1
            borderColor: valveOne.color

            // Use opengl for performance
            // useOpenGL: chartView.openGL
        }
    }

}
