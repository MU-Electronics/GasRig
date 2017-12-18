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
        onEmit_newGraphData: {
            // Load the new data
            PressuriseVsTimeGraph.updateGraph(chartView.series(0));
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

        ValueAxis {
           id: axisY
           min: PressuriseVsTimeGraph.graphMinY
           max: PressuriseVsTimeGraph.graphMaxY
           titleText: "Pressure (Bar)"
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
        }

        SplineSeries {
            id: lineSeries1
            name: "Pressure"
            axisX: axisX
            axisY: axisY
            useOpenGL: chartView.openGL
        }
    }

}
