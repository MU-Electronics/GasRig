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

    // Redraw diagram on pressure update
    Connections {
        target: VacuumVsTimeGraph
        onEmit_newVacuumGraphData: {
            // Load the new data
            VacuumVsTimeGraph.update(chartView.series("Vacuum"));
        }
    }


    // Redraw diagram on valve update
    Connections {
        target: ValvesVsTimeGraph
        onEmit_newValveOneGraphData: {
            // Load the new data
            ValvesVsTimeGraph.update(1, chartView.series("V1"));
        }
        onEmit_newValveTwoGraphData: {
            // Load the new data
            ValvesVsTimeGraph.update(2, chartView.series("V2"));
        }
        onEmit_newValveThreeGraphData: {
            // Load the new data
            ValvesVsTimeGraph.update(3, chartView.series("V3"));
        }
        onEmit_newValveFourGraphData: {
            // Load the new data
            ValvesVsTimeGraph.update(4, chartView.series("V4"));
        }
        onEmit_newValveFiveGraphData: {
            // Load the new data
            ValvesVsTimeGraph.update(5, chartView.series("V5"));
        }
        onEmit_newValveSixGraphData: {
            // Load the new data
            ValvesVsTimeGraph.update(6, chartView.series("V6"));
        }
        onEmit_newValveSevenGraphData: {
            // Load the new data
            ValvesVsTimeGraph.update(7, chartView.series("V7"));
        }
        onEmit_newValveEightGraphData: {
            // Load the new data
            ValvesVsTimeGraph.update(8, chartView.series("V8"));
        }
        onEmit_newValveNineGraphData: {
            // Load the new data
            ValvesVsTimeGraph.update(9, chartView.series("V9"));
        }
    }


    Component.onCompleted: {
        // Reload the data
        VacuumVsTimeGraph.update(chartView.series("Vacuum"));
        ValvesVsTimeGraph.update(1, chartView.series("V1"));
        ValvesVsTimeGraph.update(2, chartView.series("V2"));
        ValvesVsTimeGraph.update(3, chartView.series("V3"));
        ValvesVsTimeGraph.update(4, chartView.series("V4"));
        ValvesVsTimeGraph.update(5, chartView.series("V5"));
        ValvesVsTimeGraph.update(6, chartView.series("V6"));
        ValvesVsTimeGraph.update(7, chartView.series("V7"));
        ValvesVsTimeGraph.update(8, chartView.series("V8"));
        ValvesVsTimeGraph.update(9, chartView.series("V9"));
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
        title: "Vacuum Vs Time with valve statuses"

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

                // If zoom is greater than 1
                if (chartView.zoomFactor > 1)
                {
                    // Update X position
                    chartView.scrollLeft(touch1.x - touch1.previousX);
                    chartView.xDeviation = chartView.xDeviation + (touch1.x - touch1.previousX);

                    // Update Y position
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

            // Cursor shape
            cursorShape: Qt.PointingHandCursor

            // Update mnouse postion
            onPositionChanged: {
                // If zoom is 1 then no panning is allowed
                if(chartView.zoomFactor > 1)
                {
                    // If mouse is pressed update graph
                    if (pressed)
                    {
                        // Show grabbed hand
                        cursorShape = Qt.ClosedHandCursor;

                        // State screen touched
                        parent.touched = true;

                        // Update X position
                        chartView.scrollLeft(mouse.x - previousPoints.x);
                        chartView.xDeviation = chartView.xDeviation + (mouse.x - previousPoints.x);

                        // Update Y position
                        chartView.scrollUp(mouse.y - previousPoints.y);
                        chartView.yDeviation = chartView.yDeviation + (mouse.y - previousPoints.y);
                    }
                    else
                    {
                        // Show grabbed hand
                        cursorShape = Qt.PointingHandCursor;
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
            max: 7
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
                label: "V3 Open"
                endValue: 1.8
            }
            CategoryRange {
                label: "V4 Open"
                endValue: 2.2
            }
            CategoryRange {
                label: "V5 Open"
                endValue: 2.6
            }
            CategoryRange {
                label: "V6 Open"
                endValue: 3
            }
            CategoryRange {
                label: "V7 Open"
                endValue: 3.4
            }
            CategoryRange {
                label: "V8 Open"
                endValue: 3.8
            }
            CategoryRange {
                label: "V9 Open"
                endValue: 4.2
            }
        }

        // X axis for pressure
        // ValueAxis {
        LogValueAxis {
           id: axisY
           min: VacuumVsTimeGraph.graphMinY
           max: VacuumVsTimeGraph.graphMaxY
           titleText: "Vacuum (mBar)"
           minorGridVisible: true
           //base: 10
           // minorTickCount: 10
        }

        // Y axis for time
        DateTimeAxis {
            id: axisX
            min: VacuumVsTimeGraph.graphMinX
            max: VacuumVsTimeGraph.graphMaxX
            tickCount: 10
            titleText: "Time (Minute:Second:Millisecond)"
            format: "h:m:s:z"
            minorGridVisible: true
            labelsAngle: 90
            gridVisible : true
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

        // Valve two status series
        ScatterSeries {
            id: valveThree
            name: "V3"
            axisX: axisX
            axisYRight: valveAxisY
            markerSize: 8
            borderWidth: 1
            borderColor: valveThree.color

            // Use opengl for performance
            // useOpenGL: chartView.openGL
        }


        // Valve status series
        ScatterSeries  {
            id: valveFour
            name: "V4"
            axisX: axisX
            markerSize: 8
            borderWidth: 1
            borderColor: valveFour.color
            axisYRight: valveAxisY

            // Use opengl for performance
            // useOpenGL: chartView.openGL
        }

        // Valve status series
        ScatterSeries  {
            id: valveFive
            name: "V5"
            axisX: axisX
            markerSize: 8
            borderWidth: 1
            borderColor: valveFive.color
            axisYRight: valveAxisY

            // Use opengl for performance
            // useOpenGL: chartView.openGL
        }

        // Valve seven status series
        ScatterSeries {
            id: valveSix
            name: "V6"
            axisX: axisX
            axisYRight: valveAxisY
            markerSize: 8
            borderWidth: 1
            borderColor: valveSix.color

            // Use opengl for performance
            // useOpenGL: chartView.openGL
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

        // Valve seven status series
        ScatterSeries {
            id: valveEight
            name: "V8"
            axisX: axisX
            axisYRight: valveAxisY
            markerSize: 8
            borderWidth: 1
            borderColor: valveEight.color

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

        // Pressure readings series
        LineSeries {
            id: pressure
            name: "Vacuum"
            axisX: axisX
            axisY: axisY

            // Use opengl for performance
            // useOpenGL: chartView.openGL
        }
    }


    /**
     * Date time goes below graph (Qt bug) and leaves text, so just cover it up for now
     */
    Rectangle{
        id: cover
        anchors.top: chartView.bottom
        anchors.topMargin: - 12
        color: Material.color(Material.Grey, Material.Shade200)
        width: parent.width
        height: 15
    }


    // Control buttons for zoom in, out and reset
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
