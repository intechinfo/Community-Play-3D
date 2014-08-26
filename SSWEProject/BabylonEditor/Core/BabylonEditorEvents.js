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

/// Global Events container
var BabylonEditorEvents = BabylonEditorEvents || {}

/// Scene Events
BabylonEditorEvents.SceneEvents = BabylonEditorEvents.SceneEvents || {}
BabylonEditorEvents.SceneEvents.ObjectPicked = 0;
BabylonEditorEvents.SceneEvents.ObjectAdded = 1;
BabylonEditorEvents.SceneEvents.ObjectRemoved = 2;

/*
/// Add other events here...
*/

/// Event type
var BabylonEditorEventType = BabylonEditorEventType || {}
BabylonEditorEventType.SceneEvent = 0;

/// Event Structures
/// --------------------------------------------------------------------------------------------------
var BabylonEditorEvent = BabylonEditorEvent || {}
BabylonEditorEvent.EventType = null;

/// Scene Event
BabylonEditorEvent.SceneEvent = BabylonEditorEvent.SceneEvent || {};
BabylonEditorEvent.SceneEvent.Type = -1;
BabylonEditorEvent.SceneEvent.UserData = BabylonEditorEvent.SceneEvent.UserData || {};
/// --------------------------------------------------------------------------------------------------
