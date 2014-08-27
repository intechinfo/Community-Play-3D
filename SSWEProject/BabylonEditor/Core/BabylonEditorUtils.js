/// <reference path="./../index.html" />

/*

*/

var BabylonEditorUtils = BabylonEditorUtils || {}

/* UI utils */
BabylonEditorUtils.clearSideBar = function (sideBar) {
    var toRemove = [];
    for (var i=0; i < sideBar.nodes.length; i++) {
        toRemove.push(sideBar.nodes[i].id);
    }
    sideBar.remove.apply(sideBar, toRemove);
}

BabylonEditorUtils.fillFormsWithScope = function(forms, scope) {
    for (var i = 0; i < forms.length; i++) {
        var ui = w2ui[forms[i]];
        if (ui != null)
            ui.scope = scope;
    }
}

/* Parsers */
BabylonEditorUtils.toFloat = function (string) {
    return parseFloat(string.replace(',', '.'));
}

/* Core utils */
/// To Add

/* Events utils */
BabylonEditorUtils.sendEventObjectAdded = function (object, core) {
    var event = BabylonEditorEvent;
    event.EventType = BabylonEditorEventType.SceneEvent;
    event.SceneEvent.Type = BabylonEditorEvents.SceneEvents.ObjectAdded;
    event.SceneEvent.UserData = { mesh: object };
    core.sendEvent(event);
}

BabylonEditorUtils.sendEventObjectRemoved = function (object, core) {
    var event = BabylonEditorEvent;
    event.EventType = BabylonEditorEventType.SceneEvent;
    event.SceneEvent.Type = BabylonEditorEvents.SceneEvents.ObjectRemoved;
    event.SceneEvent.UserData = { mesh: object };
    core.sendEvent(event);
}
