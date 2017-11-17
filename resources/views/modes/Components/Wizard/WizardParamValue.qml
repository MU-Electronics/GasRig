import QtQuick 2.6

Row{
    id: paramValue

    spacing: 5
    rightPadding: 10

    property var title: 'Parameter: '
    property var value: ''

    Text{
        text: paramValue.title
        font.pointSize: 11
        font.weight: Font.Bold
    }
    Text{
        text: paramValue.value
        font.pointSize: 11
    }
}

