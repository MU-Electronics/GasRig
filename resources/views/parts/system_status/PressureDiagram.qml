import QtQuick 2.7
import QtQuick.Controls 2.1
import QtQuick.Window 2.2

import QtCharts 2.2
import QtQuick.Controls.Material 2.0


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


    Component.onCompleted: {
        // Reload the data
        PressuriseVsTimeGraph.updatePressure(chartView.series("Pressure"));
        PressuriseVsTimeGraph.updateValve(1, chartView.series("Valve One"));
        PressuriseVsTimeGraph.updateValve(2, chartView.series("Valve Two"));
        PressuriseVsTimeGraph.updateValve(7, chartView.series("Valve Seven"));
        PressuriseVsTimeGraph.updateValve(7, chartView.series("Valve Seven"));
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
        height: parent.height - 150
        width: parent.width - 10

        // Controls for zoom
        property real zoomFactor: 1.0
        property int xDeviation: 0
        property int yDeviation: 0
        property bool touched: false

        /**
         * Pan control for touch screen
         */
        MultiPointTouchArea {
            // Do not react to mouse / trackpad (touch screen only)
            mouseEnabled: false

            // Fill full graph
            anchors.fill: parent

            // Create a touch point
            touchPoints: [
                TouchPoint {
                    id: touch1
                }
            ]

            // Update graph on touch to pan
            onTouchUpdated: {
                // State screen touched
                parent.touched = true;

                if (chartView.zoomFactor > 1) {
                    chartView.scrollLeft(touch1.x - touch1.previousX);
                    chartView.xDeviation = chartView.xDeviation + (touch1.x - touch1.previousX);

                    chartView.scrollUp(touch1.y - touch1.previousY);
                    chartView.yDeviation = chartView.yDeviation + (touch1.y - touch1.previousY);
                }
            }
        }

        /**
         * Pan control for mouse / track pad
         */
        MouseArea {
            // Save previous position
            property point previousPoints;

            // Fill the graph
            anchors.fill: parent

            // Update on hover
            hoverEnabled: true

            // Update mnouse postion
            onPositionChanged: {
                // If zoom is 1 then no panning is allowed
                if(chartView.zoomFactor > 1)
                {
                    // If mouse is pressed update graph
                    if (pressed)
                    {
                        // State screen touched
                        parent.touched = true;

                        // Update X position
                        chartView.scrollLeft(mouse.x - previousPoints.x);
                        chartView.xDeviation = chartView.xDeviation + (mouse.x - previousPoints.x);

                        // Update Y position
                        chartView.scrollUp(mouse.y - previousPoints.y);
                        chartView.yDeviation = chartView.yDeviation + (mouse.y - previousPoints.y);
                    }

                    // Save mouse X Y
                    previousPoints.x = mouse.x;
                    previousPoints.y = mouse.y;
                }
            }
        }

        // X axis for valve status
        CategoryAxis {
            id: valveAxisY
            min: -0.5
            max: 6
            labelsPosition: CategoryAxis.AxisLabelsPositionOnValue
            titleText: "Valve Status"
            gridLineColor: Material.color(Material.BlueGrey, Material.Shade300)
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
           minorGridVisible: true
           minorTickCount: 10
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
            name: "V7"
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
            name: "V9"
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
            name: "V2"
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
            name: "V1"
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
        anchors.horizontalCenter: parent.horizontalCenter

        // Zoom in button
        Button {
            id: zoomInButton
            text: "zoom In"
            onClicked: {
                // Set zoom factor
                chartView.zoomFactor = chartView.zoomFactor * 2.0;

                // Zoom in
                chartView.zoom(2.0)

                // No touched
                chartView.touched = false;
            }
        }

        // Zoom out button
        Button {
            id: zoomOutButton
            text: "zoom out"
            onClicked: {
                // Scale the zoom
                chartView.zoomFactor = chartView.zoomFactor * 0.5;
                var zoomDiff = 1 / chartView.zoomFactor;

                // Move graph back to correct postion after zoom
                var xMove = chartView.xDeviation * zoomDiff;
                var yMove = chartView.yDeviation * zoomDiff;

                // Set deviation for new postion
                chartView.xDeviation = chartView.xDeviation - xMove;
                chartView.xDeviation = chartView.xDeviation * 0.5;
                chartView.yDeviation = chartView.yDeviation - yMove;
                chartView.yDeviation = chartView.yDeviation * 0.5;

                // If zoom is more than 1 then perfrom a zoom out
                if (chartView.zoomFactor >= 1.0)
                {
                    // Move X and Y
                    chartView.scrollRight(xMove);
                    chartView.scrollDown(yMove);

                    // Zoom out
                    chartView.zoom(0.5);

                    // If xDeviation is 0 then something went wrong
                    if (chartView.xDeviation == 0 && chartView.touched)
                    {
                        // Becouse of rounding error, reset the zoom
                        chartView.zoomReset();
                    }
                }
                else
                {
                    // Zoom greater than 1
                    chartView.zoomFactor = 1.0;
                }

                // No touched
                chartView.touched = false;
            }
        }

        // Reset zoom button
        Button {
            id: zoomResetButton
            text: "zoom reset"
            onClicked: {
                // Scroll left to start
                chartView.scrollLeft(chartView.xDeviation * -1);

                // Reset the zoom
                chartView.zoomReset();

                // Reset the x deviation
                chartView.xDeviation = 0;

                // No touched
                chartView.touched = false;
            }
        }
    }

}
