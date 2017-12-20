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


    /*Component.onCompleted: {
        // Reload the data
        PressuriseVsTimeGraph.updatePressure(chartView.series("Pressure"));
        PressuriseVsTimeGraph.updateValve(1, chartView.series("Valve One"));
        PressuriseVsTimeGraph.updateValve(2, chartView.series("Valve Two"));
        PressuriseVsTimeGraph.updateValve(7, chartView.series("Valve Seven"));
        PressuriseVsTimeGraph.updateValve(7, chartView.series("Valve Seven"));
    }*/


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
        height: 800 // height.height - 10
        width: parent.width - 10

        // Controls
        property real zoomFactor: 1.0
        property int xDeviation: 0
        property int yDeviation: 0

        MultiPointTouchArea {
            anchors.fill: parent
            touchPoints: [
                           TouchPoint {
                             id: touch1
                           }
                         ]
            onTouchUpdated: {
                if (chartView.zoomFactor > 1) {
                    chartView.scrollLeft(touch1.x - touch1.previousX);
                    chartView.xDeviation = chartView.xDeviation + (touch1.x - touch1.previousX);

                    chartView.scrollUp(touch1.y - touch1.previousY);
                    chartView.yDeviation = chartView.yDeviation + (touch1.y - touch1.previousY);
                }
            }
        }

        // X axis for valve status
        CategoryAxis {
            id: valveAxisY
            min: -0.5
            max: 5
            labelsPosition: CategoryAxis.AxisLabelsPositionOnValue
            titleText: "Valve Status"
            CategoryRange {
                label: "Closed"
                endValue: 0
            }
            CategoryRange {
                label: "V1 Open"
                endValue: 1
            }
            CategoryRange {
                label: "V2 Open"
                endValue: 1.4
            }
            CategoryRange {
                label: "V7 Open"
                endValue: 1.8
            }
            CategoryRange {
                label: "V9 Open"
                endValue: 2.2
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

        // Pressure readings series
         LineSeries {
            id: pressure
            name: "Pressure"
            axisX: axisX
            axisY: axisY

            // Use opengl for performance
            // useOpenGL: chartView.openGL
        }
    }


    Row
    {
        anchors.top: chartView.bottom
        anchors.topMargin: 20
        spacing: 20
        Button {
            id: zoomInButton
            text: "Zoom In"
            onClicked: {
                console.log("ZoomIn pressed");
                chartView.zoomFactor = chartView.zoomFactor * 2.0;
                chartView.zoom(2.0)

            }
        }

        Button {
            id: zoomOutButton
            anchors.top: chartView.bottom
            text: "Zoom out"
            onClicked: {
                chartView.zoomFactor = chartView.zoomFactor * 0.5;
                var zoomDiff = 1 / chartView.zoomFactor;
                var xMove = chartView.xDeviation * zoomDiff;
                var yMove = chartView.yDeviation * zoomDiff;
                chartView.xDeviation = chartView.xDeviation - xMove;
                chartView.xDeviation = chartView.xDeviation * 0.5;
                chartView.yDeviation = chartView.yDeviation - yMove;
                chartView.yDeviation = chartView.yDeviation * 0.5;

                if (chartView.zoomFactor >= 1.0) {
                    chartView.scrollRight(xMove);
                    chartView.scrollDown(yMove);
                    chartView.zoom(0.5);
                    if (chartView.xDeviation == 0) {
                        // Becouse of rounding error, reset the zoom
                        chartView.zoomReset();
                    }
                } else {
                    chartView.zoomFactor = 1.0;
                }
            }
        }

        Button {
            id: zoomResetButton
            anchors.top: chartView.bottom
            text: "Zoom out"
            onClicked: {
                console.log("ZoomReset pressed, xDeviation " + chartView.xDeviation);
                chartView.scrollLeft(chartView.xDeviation * -1);
                chartView.zoomReset();
                chartView.xDeviation = 0;
            }
        }
    }

}
