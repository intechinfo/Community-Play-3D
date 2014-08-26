/* 

Core class that handles core functions,
custom events management...

*/

function BabylonEditorCore() {

    /// Array that contains all the instances that need to be updated.
    /// Just declare "update" in your prototype
    this.customUpdates = [];

    /// Array that containts all the instances that need to receive
    /// events from other classes and from other events.
    /// See example in getPickedMesh();
    /// Each 
    this.eventReceivers = [];

    this.currentScene = null;
}

BabylonEditorCore.prototype.createDefaultScene = function (canvas, engine) {

    this.currentScene = new BABYLON.Scene(engine);

    var camera = new BABYLON.FreeCamera("camera1", new BABYLON.Vector3(0, 5, -10), this.currentScene);
    camera.setTarget(new BABYLON.Vector3.Zero());
    camera.attachControl(canvas, false);

    var light = new BABYLON.DirectionalLight("dir01", new BABYLON.Vector3(-1, -2, -1), this.currentScene);
    light.position = new BABYLON.Vector3(10, 10, 0);

    var shadows = new BABYLON.ShadowGenerator(1024, light);

    var sphere = BABYLON.Mesh.CreateSphere("sphere1", 16, 2, this.currentScene);
    sphere.position.y = 1;
    shadows.getShadowMap().renderList.push(sphere);

    var ground = BABYLON.Mesh.CreateGround("ground1", 6, 6, 2, this.currentScene);
    ground.receiveShadows = true;

    return this.currentScene;
};

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

    var result = this.currentScene.pick(event.clientX, event.clientY);

    if (sendEvent === undefined) {
        return result;
    } else {
        var event = BabylonEditorEvent;
        event.EventType = BabylonEditorEventType.SceneEvent;
        event.SceneEvent.Type = BabylonEditorEvents.ObjectPicked;
        event.SceneEvent.UserData = { mesh: result.pickedMesh };
        this.sendEvent(event);
    }
}
