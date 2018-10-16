import QtQuick 2.9
import QtQml.Models 2.3

Item {
    id: root

    width: parent.width
    height: 400

    ListModel {
        id: functionlist
        ListElement {
            text: "Set valve 1"
        }
        ListElement {
            text: "Set pressure"
        }
        ListElement {
            text: "Set valve 4"
        }
    }
}
