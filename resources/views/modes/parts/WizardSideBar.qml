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

     // Button actions on pressed
     property var editParamsPressed: (function(){ console.log('Edit params pressed') });
     property var startPressed: (function(){ console.log('Start was pressed') });
     property var cancelPressed: (function(){ console.log('Cancel was pressed') });
     property var nextPressed: (function(){ console.log('Next stage was pressed') });

     WizardSideButton {
        text: "Edit Parameters"
        onClicked: rootSideBar.editParamsPressed()
        backgroundInit: Material.color(Material.Blue, Material.Shade400)
        backgroundPressed: Material.color(Material.Blue, Material.Shade600)
        textColorInit: "#ffffff"
        textColorPressed: "#ffffff"
        enabled: (rootSideBar.stage === 0 || rootSideBar.stage === 2 || rootSideBar.stage === 3) ? true : false
    }

    WizardSideButton{
        text: (rootSideBar.stage === 1 || rootSideBar.stage === 2 || rootSideBar.stage === 3) ? qsTr('Restart') : qsTr('Start')
        onClicked: rootSideBar.startPressed()
        backgroundInit: Material.color(Material.Green, Material.Shade400)
        backgroundPressed: Material.color(Material.Green, Material.Shade600)
        textColorInit: "#ffffff"
        textColorPressed: "#ffffff"
        enabled: (rootSideBar.stage === 0 || rootSideBar.stage === 2 || rootSideBar.stage === 3) ? true : false
    }

    WizardSideButton{
        text: "Cancel Mode"
        onClicked: rootSideBar.cancelPressed()
        backgroundInit: Material.color(Material.Red, Material.Shade400)
        backgroundPressed: Material.color(Material.Red, Material.Shade600)
        textColorInit: "#ffffff"
        textColorPressed: "#ffffff"
    }

    WizardSideButton{
        text: "Next Stage"
        onClicked: rootSideBar.nextPressed()
        backgroundInit: Material.color(Material.Orange, Material.Shade400)
        backgroundPressed: Material.color(Material.Orange, Material.Shade600)
        textColorInit: "#ffffff"
        textColorPressed: "#ffffff"
        enabled: (rootSideBar.stage === 0 || rootSideBar.stage === 2 || rootSideBar.stage === 3) ? true : false
    }
}
