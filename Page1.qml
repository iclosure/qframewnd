import QtQuick 2.7
import QtQuick.Controls.Universal 2.0
import com.smartsoft.model 1.0

Page1Form {
    id: page1

    button1.onClicked: {
        console.log("Button Pressed. Entered text: " + textField1.text);

        MyObject1.setTitle(textField1.text)
    }

    Connections {
        target: MyObject1
        onTitleChanged: {
            textField2.text = text
        }
    }

    comboBox.onCurrentTextChanged: {
        MyObject1.setTitle(comboBox.currentText);
    }

    checkBox1.onCheckedChanged: {
        MyObject1.setTitle(checkBox1.checked)
    }
}
