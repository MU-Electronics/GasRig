import QtQuick 2.0
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.0

Column
{
    id: rootSideBar

     width: parent.width
     spacing: 11.9

     // Current stage of process
     property var stage: 0

     // Button ids
     property alias editButton: editButton
     property alias startButton: startButton;
     property alias cancelButton: cancelButton;
     property alias nextButton: nextButton;

     WizardSideButton {
         id: editButton
        text: "Edit Parameters"
        backgroundInit: Material.color(Material.Blue, Material.Shade400)
        backgroundPressed: Material.color(Material.Blue, Material.Shade600)
        textColorInit: "#ffffff"
        textColorPressed: "#ffffff"
        enabled: (rootSideBar.stage === 0 || rootSideBar.stage === 2 || rootSideBar.stage === 3) ? true : false
    }

    WizardSideButton{
        id: startButton
        text: (rootSideBar.stage === 1 || rootSideBar.stage === 2 || rootSideBar.stage === 3) ? qsTr('Restart') : qsTr('Start')
        backgroundInit: Material.color(Material.Green, Material.Shade400)
        backgroundPressed: Material.color(Material.Green, Material.Shade600)
        textColorInit: "#ffffff"
        textColorPressed: "#ffffff"
        enabled: (rootSideBar.stage === 0 || rootSideBar.stage === 2 || rootSideBar.stage === 3) ? true : false
    }

    WizardSideButton{
        id: cancelButton
        text: "Cancel Mode"
        backgroundInit: Material.color(Material.Red, Material.Shade400)
        backgroundPressed: Material.color(Material.Red, Material.Shade600)
        textColorInit: "#ffffff"
        textColorPressed: "#ffffff"
    }

    WizardSideButton{
        id: nextButton
        text: "Next Stage"
        backgroundInit: Material.color(Material.Orange, Material.Shade400)
        backgroundPressed: Material.color(Material.Orange, Material.Shade600)
        textColorInit: "#ffffff"
        textColorPressed: "#ffffff"
        enabled: (rootSideBar.stage === 0 || rootSideBar.stage === 2 || rootSideBar.stage === 3) ? true : false
    }
}
