import QtQuick 2.0
import QtQuick.Controls 2.1

Item {
    id: vacuumTab

    Button{
        id: "testingVacuum"
        objectName: "testingVacuum"
        text: "Vacuum"
        onClicked: {
            TestingManager.vacuumOn();
        }
    }
}
