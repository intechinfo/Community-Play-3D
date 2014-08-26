/* 

File defining events structures

Build an event (example with a picked mesh) :
var event = BabylonEditorEvent;
event.EventType = BabylonEditorEventType.SceneEvent;
event.SceneEvent.Type = BabylonEditorEvents.ObjectPicked;
event.SceneEvent.UserData = { mesh: result.pickedMesh };
babylonCoreInstance.sendEvent(event);

Work the received event :
MyTool.prototype.onEvent = function(event) {
    if (event.EventType == BabylonEditorEventType.SceneEvent) {
        if (event.SceneEvent.Type == BabylonEditorEvents.ObjectPicked) {
            console.log('event successfuly analyzed')
        }
    }
}

*/

var BabylonEditorEvents = BabylonEditorEvents || {}
BabylonEditorEvents.ObjectPicked = 0;
/// Add other events here...

var BabylonEditorEventType = BabylonEditorEventType || {}
BabylonEditorEventType.SceneEvent = 0;

/// Event Structure
/// --------------------------------------------------------------------------------------------------
var BabylonEditorEvent = BabylonEditorEvent || {}
BabylonEditorEvent.EventType = null;

/// Scene Event
BabylonEditorEvent.SceneEvent = BabylonEditorEvent.SceneEvent || {};
BabylonEditorEvent.SceneEvent.Type = -1;
BabylonEditorEvent.SceneEvent.UserData = BabylonEditorEvent.SceneEvent.UserData || {};
/// --------------------------------------------------------------------------------------------------
