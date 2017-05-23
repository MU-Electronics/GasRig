import QtQuick 2.0
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1

Item {
    id: vacuumTab

    // Define vars for interface state
    property bool vacuumstate: false



    Button
    {
        id: "testingVacuum"
        text: "Vacuum On / Off"
        Material.accent: if(vacuumstate) { Material.Teal } else { Material.Orange }
        onClicked:
        {
            // Save state
            vacuumstate = !vacuumstate

            // Set vac pump
            TestingManager.requestVacuum(vacuumstate);
        }
    }
}
