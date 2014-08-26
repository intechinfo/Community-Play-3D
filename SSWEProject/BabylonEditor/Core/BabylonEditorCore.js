﻿/* 

Core class that handles core functions,
custom events management...

*/

function BabylonEditorCore() {

    /// Array that contains all the instances that need to be updated.
    /// Just declare "update" in your prototype
    this.customUpdates = new Array();

    /// Array that containts all the instances that need to receive
    /// events from other classes and from other events.
    /// See example in getPickedMesh();
    /// Each 
    this.eventReceivers = new Array();

    this.currentScene = null;
}

BabylonEditorCore.prototype.update = function () {
    for (var i = 0; i < this.customUpdates.length; i++) {
        this.customUpdates[i].update();
    }
}

BabylonEditorCore.prototype.sendEvent = function (event) {
    for (var i = 0; i < this.eventReceivers.length; i++) {
        this.eventReceivers[i].onEvent(event);
    }
}

BabylonEditorCore.prototype.getPickedMesh = function (event, sendEvent) {
    if (!this.currentScene) return null;

    var result = this.currentScene.pick(event.layerX, event.layerY);

    if (sendEvent === undefined) {
        return result;
    } else {
        var event = BabylonEditorEvent;
        event.EventType = BabylonEditorEventType.SceneEvent;
        event.SceneEvent.Type = BabylonEditorEvents.SceneEvents.ObjectPicked;
        event.SceneEvent.UserData = { mesh: result.pickedMesh };
        this.sendEvent(event);
    }
}
