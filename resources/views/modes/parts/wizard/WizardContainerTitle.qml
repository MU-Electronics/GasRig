import QtQuick 2.6

Row{
    spacing: 10

    property var title: 'Parameters'
    property var titleSmall: 'Edit Parameters on the right'

    Text{
        text: parent.title
        font.pointSize: 18
    }
    Text{
        text: parent.titleSmall
        font.pointSize: 12
        topPadding: 5.5
    }
}

