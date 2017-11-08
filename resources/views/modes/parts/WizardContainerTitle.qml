import QtQuick 2.6

Row{
    spacing: 10

    property var title: 'Parameters'
    property var titleSmall: 'Edit Parameters on the right'

    Text{
        text: 'Parameters'
        font.pointSize: 20
    }
    Text{
        text: 'Edit Parameters on the right'
        font.pointSize: 14
        topPadding: 5.5
    }
}

