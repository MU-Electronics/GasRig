import QtQuick 2.6

Row{
    id: paramValue

    spacing: 5

    property var title: 'Parameter: '
    property var value: ''

    Text{
        text: paramValue.title
        font.pointSize: 12
    }
    Text{
        text: paramValue.value
        font.pointSize: 12
    }
}

