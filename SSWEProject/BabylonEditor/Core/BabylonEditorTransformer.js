/// <reference path="./../index.html" />

/* 

Transformer class. It contains a custom scene
with arrows etc. to place, rotation or scale objects.

*/

function BabylonEditorTransformer(engine, core) {

    this._engine = engine;
    this._core = core;
    core.customUpdates.push(this);
    core.eventReceivers.push(this);

    this._scene = new BABYLON.Scene(engine);
    this._scene.autoClear = false;

    this._nodeToTransform = null;

    this._positionArrowX = null;
    this._positionArrowY = null;
    this._positionArrowZ = null;

    this._createTransformers();
}

BabylonEditorTransformer.prototype.update = function () {
    this._scene.activeCamera = this._core.currentScene.activeCamera;

    /// Update transformers scales
    var distance = BABYLON.Vector3.Distance(this._scene.activeCamera.position, this._positionArrowX.position) * 0.03;
    this._positionArrowX.scaling = new BABYLON.Vector3(distance, distance, distance);

    /// Update transformers
    this._positionArrowY.position.y = this._positionArrowX.getBoundingInfo().boundingBox.extendSize.y;
    this._positionArrowY.position.z = -this._positionArrowX.getBoundingInfo().boundingBox.extendSize.y;
    this._positionArrowZ.position.y = this._positionArrowX.getBoundingInfo().boundingBox.extendSize.y;
    this._positionArrowZ.position.x = -this._positionArrowX.getBoundingInfo().boundingBox.extendSize.y;

    this._scene.render();
}

BabylonEditorTransformer.prototype.onEvent = function (event) {

}

BabylonEditorTransformer.prototype.setNodeToTransform = function (node) {
    this._nodeToTransform = node;
    
    this._positionArrowX.parent = node;
    this._positionArrowX.setEnabled(node == null ? false : true);
}

BabylonEditorTransformer.prototype._createTransformers = function () {
    var materialTransformerX = new BABYLON.StandardMaterial("BabylonEditorTransformerXMaterial", this._scene);
    var materialTransformerY = new BABYLON.StandardMaterial("BabylonEditorTransformerYMaterial", this._scene);
    var materialTransformerZ = new BABYLON.StandardMaterial("BabylonEditorTransformerZMaterial", this._scene);

    materialTransformerX.emissiveColor = new BABYLON.Color3(0, 1, 0);
    materialTransformerY.emissiveColor = new BABYLON.Color3(1, 0, 0);
    materialTransformerZ.emissiveColor = new BABYLON.Color3(0, 0, 1);

    this._positionArrowX = BABYLON.Mesh.CreateCylinder("x", 7, 0.2, 0.2, 15, this._scene);
    this._positionArrowX.rotation.x = Math.PI / 2.0;
    this._positionArrowX.material = materialTransformerX;

    this._positionArrowY = BABYLON.Mesh.CreateCylinder("y", 7, 0.2, 0.2, 15, this._scene);
    this._positionArrowY.material = materialTransformerY;
    this._positionArrowY.rotation.x = -(Math.PI / 2.0);
    this._positionArrowY.parent = this._positionArrowX;

    this._positionArrowZ = BABYLON.Mesh.CreateCylinder("z", 7, 0.2, 0.2, 15, this._scene);
    this._positionArrowZ.material = materialTransformerZ;
    this._positionArrowZ.rotation.z = -(Math.PI / 2.0);
    this._positionArrowZ.parent = this._positionArrowX;

    /// Finish
    this._positionArrowX.setEnabled(false);
}
