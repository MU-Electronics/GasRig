import QtQuick 2.0
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1

Item {
    id: vacuumTab

    // Define vars for interface state
    property bool vacuumstate: false



    Button
    {
        id: "vacuumPumpControl"
        objectName: "vacuumPumpControl"
        text: "Vacuum On / Off"
        Material.background: if(vacuumTab.vacuumstate){ Material.color(Material.Green, Material.Shade500) }else{ Material.color(Material.Grey, Material.Shade300) }
        Material.foreground: if(vacuumTab.vacuumstate){ Material.color(Material.Grey, Material.Shade100) }else{ Material.color(Material.Grey, Material.Shade800) }
        onClicked:
        {
            // Save state
            vacuumstate = !vacuumstate

            // Set vac pump
            TestingManager.requestVacuum(vacuumstate);
        }
    }
}
