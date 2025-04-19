import QtQuick
import SDL

Window {
  width: 640
  height: 480
  visible: true
  title: qsTr("QtQuickSDL Test")

  SDLItem {
    id: sdlItem
    anchors.fill: parent
    renderer: qsdlrenderer
  }

  Timer {
    running: true
    repeat: true
    interval: 100
    onTriggered: sdlItem.renderer.render()
  }

  Shortcut {
    sequence: "Ctrl+H"
    onActivated: sdlItem.renderer.visibleWindow = !sdlItem.renderer.visibleWindow
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
