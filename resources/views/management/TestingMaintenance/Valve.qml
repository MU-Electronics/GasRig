import QtQuick 2.5
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.1
import QtQml.Models 2.2

import Fluid.Layouts 1.0 as FluidLayouts
import Fluid.Controls 1.0 as FluidControls
import Fluid.Material 1.0 as FluidMaterial
import Fluid.Core 1.0 as FluidCore

import "../../parts"

Item {
     id: valveTab

     // Define vars for interface state
     property var valveStatuses: [false, false, 0, 0, 0, 0, 0, 0, 0]


     width: parent.width - 10


     /**
      * Set the valve status
      */
     FluidControls.Card
     {
         id: pumpControl

         width: parent.width-10

         height: 170

         padding: 5


         AlertBox
         {
             id: valveAlert
             height: 30
             width: parent.width
             type: "Warning"
             textContent: "Be carefully there is no automated protection in this mode!"
         }

         Item
         {
              id: valveLayoutContainer

              width: parent.width
              height: 100

              anchors.top: valveAlert.bottom
              anchors.topMargin: 20

              FluidLayouts.AutomaticGrid
              {
                  id: valveLayout


                  property var noInColoum: 5
                  property var noInRow: 2

                  width: parent.width

                  cellWidth: parent.width / noInColoum
                  cellHeight: 100

                  anchors.fill: parent

                  model: ListModel {
                      id: valvesList
                      ListElement { number: "1"; name: "Valve 1" }
                      ListElement { number: "2"; name: "Valve 2" }
                      ListElement { number: "3"; name: "Valve 3" }
                      ListElement { number: "4"; name: "Valve 4" }
                      ListElement { number: "5"; name: "Valve 5" }
                      ListElement { number: "6"; name: "Valve 6" }
                      ListElement { number: "7"; name: "Valve 7" }
                      ListElement { number: "8"; name: "Valve 8" }
                      ListElement { number: "9"; name: "Valve 9" }
                  }
                  delegate: Rectangle
                  {
                      width: valveLayout.cellWidth - 10
                      height: valveLayoutContainer.height / 2

                      Button
                      {
                          text: model.name
                          objectName: "valveButton"
                          Material.background: if(ConnectionStatusManager.valveState[model.number] === "1"){ Material.color(Material.Green, Material.Shade500) }else{ Material.color(Material.Grey, Material.Shade300) }
                          Material.foreground: if(ConnectionStatusManager.valveState[model.number] === "1"){ Material.color(Material.Grey, Material.Shade100) }else{ Material.color(Material.Grey, Material.Shade800) }
                          onClicked:
                          {
                              var toggle = false;
                              if(ConnectionStatusManager.valveState[model.number] == 0)
                                  toggle = true;

                              console.log(ConnectionStatusManager.valveState[model.number])

                              // Set vac pump
                              TestingManager.requestValveState(model.number, toggle);
                          }
                      }
                  }
              }
         }
     }
}
