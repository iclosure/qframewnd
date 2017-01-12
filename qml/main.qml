import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls.Material 2.0
import QtQuick.Controls.Styles.Flat 1.0 as Flat
import QtQuick.Controls 1.4 as T0
import QtQuick.Window 2.2
import Qt.labs.settings 1.0
import QtQuick.XmlListModel 2.0
import com.smartsoft.model 1.0
import "quick3d"

ApplicationWindow {
    id: window
    visible: true
    width: Screen.width * 2 / 3
    height: Screen.height * 2 / 3
    title: MyObject1.objectName
    flags: Qt.Window
           | Qt.FramelessWindowHint
           | Qt.WindowMinMaxButtonsHint
           | Qt.WindowCloseButtonHint
           | Qt.WindowFullscreenButtonHint

    Settings {
        id: settings
        property string style: "Default"
    }

    function caption(pos) {
        var item = layout1.childAt(pos.x, pos.y)
        if (item && !item.enabled) {
            return true
        } else {
            return false
        }
    }

    header: ToolBar {
        id: headerBar
        objectName: "toolBar"

        RowLayout {
            id: layout1
            objectName: "layout1"
            anchors.fill: parent

            ToolButton {
                id: button1
                objectName: "button1"
                text: qsTr("\u25C0 %1").arg(Qt.application.name)
                onClicked: {
                    console.log("button clicked!");
                }
            }

            Label {
                id: caption
                objectName: "caption"
                Layout.fillWidth: true
                Layout.fillHeight: true
                enabled: false
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                text: "------Welcome to Qt------"
                color: Material.background
            }

            ToolButton {
                id: buttonMenu
                objectName: "buttonMenu"
                contentItem: Image {
                    fillMode: Image.Pad
                    horizontalAlignment: Image.AlignHCenter
                    verticalAlignment: Image.AlignVCenter
                    source: "qrc:/images/menu@2x.png"
                }
                onClicked: optionsMenu.open()

                Menu {
                    id: optionsMenu
                    x: parent.width - width
                    y: headerBar.height
                    transformOrigin: Menu.TopRight

                    MenuItem {
                        text: "Settings"
                        onTriggered: {
                            settingsDialog.open()
                        }
                    }
                    MenuItem {
                        text: "About"
                        onTriggered: aboutDialog.open()
                    }
                    MenuItem {
                        text: "Exit"
                        onTriggered: window.close()
                    }
                }
            }
        }
    }

    footer: TabBar {
        id: tabBar
        currentIndex: swipeView.currentIndex
        TabButton {
            text: qsTr("First")
        }
        TabButton {
            text: qsTr("Second")
        }
        TabButton {
            text: qsTr("Thrid")
        }
        TabButton {
            text: qsTr("Fourth")
        }
        TabButton {
            text: qsTr("Fifth")
        }
    }

    onClosing: {
        console.log("closing")
        //close.accepted = false
    }

    ListModel {
        id: myModel
        objectName: "myModel"
    }

    ListModel {
        id: fruitModel
        ListElement {
            name: "Apple"
            cost: 2.45
            attributes: [
                ListElement { description: "Core" },
                ListElement { description: "Deciduous" }
            ]
        }
        ListElement {
            name: "Orange"
            cost: 3.25
            attributes: [
                ListElement { description: "Citrus" }
            ]
        }
        ListElement {
            name: "Banana"
            cost: 1.95
            attributes: [
                ListElement { description: "Tropical" },
                ListElement { description: "Seedless" }
            ]
        }

    }

    SwipeView {
        id: swipeView
        anchors.fill: parent
        currentIndex: tabBar.currentIndex

        Page {
            id: page3d

            Quick3D {
                id: quick3d
                anchors.fill: parent
            }
        }

        Page1 {
            id: page1
            comboBox.width: 200
            comboBox.height: 35
            comboBox.textRole: "name"
            comboBox.model: fruitModel
        }

        Page2 {
        }

        Page3 {
        }

        Page {
            RowLayout {
                anchors.centerIn: parent
                spacing: 10

                //ScrollIndicator.vertical: ScrollIndicator {}

                T0.GroupBox {
                    title: "Button"
                    checkable: false//settingsData.checks
                    flat: false//!settingsData.frames
                    Layout.fillWidth: true
                    GridLayout {
                        columns: 6
                        T0.Button {
                            id: button
                            text: "Normal"
                        }
                        T0.Button {
                            text: "Default"
                            isDefault: true
                        }
                        Repeater {
                            model: 50

                            T0.Button {
                                text: "Checkable"
                                checkable: true
                            }
                        }

                        T0.Button {
                            text: "Menu"
                            menu: T0.Menu {
                                T0.MenuItem { text: "Normal"; shortcut: "Ctrl+N" }
                                T0.MenuSeparator { }
                                T0.MenuItem { text: "Checkable 1"; checkable: true; checked: true }
                                T0.MenuItem { text: "Checkable 2"; checkable: true; checked: true }
                                T0.MenuSeparator { }
                            }
                            visible: Qt.application.supportsMultipleWindows
                        }
                    }
                }
            }
        }

        Page {
            Column {
                anchors.fill: parent

                ListView {
                    width: 180; height: 300
                    model: feedModel
                    delegate: Column {
                        Text { text: title + " (" + type + ")"; font.bold: wanted }
                        Text { text: first_author }
                        Text { text: year }
                    }
                }

                ProgressBar {
                    id: progressBar1
                    width: 600
                    to: 100
                    value: feedModel.progress
                    onValueChanged: {
                        MyObject1.setTitle(value.toFixed(2, 6, '0'))
                    }
                }

                Timer {
                    running: true
                    interval: 500
                    triggeredOnStart: true
                    repeat: true
                    onTriggered: {
                        progressBar1.value = Math.random()  * 100
                    }
                }
            }
        }
    }

    XmlListModel {
        id: feedModel
        source: "qrc:/test1.xml"
        // XmlRole queries will be made on <book> elements
        query: "/catalog/book"

        // query the book title
        XmlRole { name: "title"; query: "title/string()" }

        // query the book's year
        XmlRole { name: "year"; query: "year/number()" }

        // query the book's type (the '@' indicates 'type' is an attribute, not an element)
        XmlRole { name: "type"; query: "@type/string()" }

        // query the book's first listed author (note in XPath the first index is 1, not 0)
        XmlRole { name: "first_author"; query: "author[1]/string()" }

        // query the wanted attribute as a boolean
        XmlRole { name: "wanted"; query: "boolean(@wanted)" }
    }


    Popup {
        id: settingsDialog
        x: Math.round((window.width - width) / 2)
        y: Math.round(window.height / 6)
        width: Math.round(Math.min(window.width, window.height) / 3 * 2)
        modal: true
        focus: true
        /*title: "Settings"

        standardButtons: Dialog.Ok | Dialog.Cancel
        onAccepted: {
            settings.style = styleBox.displayText
            settingsDialog.close()
        }
        onRejected: {
            styleBox.currentIndex = styleBox.styleIndex
            settingsDialog.close()
        }*/

        contentItem: ColumnLayout {
            id: settingsColumn
            spacing: 20

            RowLayout {
                spacing: 10

                Label {
                    text: "Style:"
                }

                ComboBox {
                    id: styleBox
                    property int styleIndex: -1
                    model: ["Default", "Material", "Universal"]
                    Component.onCompleted: {
                        styleIndex = find(settings.style, Qt.MatchFixedString)
                        if (styleIndex !== -1)
                            currentIndex = styleIndex
                    }
                    Layout.fillWidth: true
                }
            }

            Label {
                text: "Restart required"
                color: "#e41e25"
                opacity: styleBox.currentIndex !== styleBox.styleIndex ? 1.0 : 0.0
                horizontalAlignment: Label.AlignHCenter
                verticalAlignment: Label.AlignVCenter
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
        }
    }

    Popup {
        id: aboutDialog
        modal: true
        focus: true
        //title: "About"
        x: (window.width - width) / 2
        y: window.height / 6
        width: Math.min(window.width, window.height) / 3 * 2
        contentHeight: aboutColumn.height

        Column {
            id: aboutColumn
            spacing: 20

            Label {
                width: aboutDialog.availableWidth
                text: "The Qt Quick Controls 2 module delivers the next generation user interface controls based on Qt Quick."
                wrapMode: Label.Wrap
                font.pixelSize: 12
            }

            Label {
                width: aboutDialog.availableWidth
                text: "In comparison to the desktop-oriented Qt Quick Controls 1, Qt Quick Controls 2 "
                      + "are an order of magnitude simpler, lighter and faster, and are primarily targeted "
                      + "towards embedded and mobile platforms."
                wrapMode: Label.Wrap
                font.pixelSize: 12
            }
        }
    }
}
