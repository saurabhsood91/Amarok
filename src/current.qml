import QtQuick 1.0

 ListView {
     width: 500; height: 500
     anchors.fill: parent

     model: currentModel
     delegate: Rectangle {
         height: 25
         width: 100
         Text { text: "Name : " + track + "\n" + "Artist: " + artist }
     }
 }
