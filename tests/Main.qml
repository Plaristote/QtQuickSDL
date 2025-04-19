import QtQuick
import SDL

Window {
  width: 640
  height: 480
  visible: true
  title: qsTr("QtQuickSDL Tilemap Test")

  SDLItem {
    anchors.fill: parent
    renderer: game.renderer
  }

  Timer {
    running: true
    repeat: true
    interval: 100
    onTriggered: game.renderer.render()
  }

  Shortcut {
    sequence: "Ctrl+H"
    onActivated: game.renderer.visibleWindow = !game.renderer.visibleWindow
  }

  Shortcut {
    sequence: "Up"
    onActivated: game.cameraOffsetY += 10
  }

  Shortcut {
    sequence: "Down"
    onActivated: game.cameraOffsetY -= 10
  }

  Shortcut {
    sequence: "Left"
    onActivated: game.cameraOffsetX += 10
  }

  Shortcut {
    sequence: "Right"
    onActivated: game.cameraOffsetX -= 10
  }
}
