import QtQuick 2.0
import QtQuick.Controls 2.1

Item {
    id: vacuumTab

    // Define vars for interface state
    property bool vacuumstate: false



    Button{
        id: "testingVacuum"
        objectName: "testingVacuum"

        text: "Vacuum"
        onClicked: {
            vacuumstate = !vacuumstate
            TestingManager.requestVacuum(vacuumstate);
        }
    }
}
