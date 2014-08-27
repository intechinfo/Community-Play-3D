/// <reference path="./../index.html" />

function BabylonEditorEditionTool(babylonEditorCore) {
    this._core = babylonEditorCore;
    this._core.eventReceivers.push(this);
    this._core.customUpdates.push(this);

    this.object = null;

    this._forms = [
        'MainEditObjectGeneral',
        'MainEditObjectTransform',
        'MainEditObjectOptions'
    ];

};

BabylonEditorEditionTool.prototype.onEvent = function (event) {

    if (event.EventType == BabylonEditorEventType.SceneEvent) {
        if (event.SceneEvent.Type == BabylonEditorEvents.SceneEvents.ObjectPicked) {

            if (this.object != null && this.object instanceof BABYLON.Mesh) {
                this.object.showBoundingBox = false;
                this.object.showSubMeshesBoundingBox = false;
            }

            this.object = event.SceneEvent.UserData.mesh;
            /// Test here for the moment...
            this._core.transformer.setNodeToTransform(this.object);

            if (this.object != null) {

                if (this.object instanceof BABYLON.Mesh) {
                    this.object.showBoundingBox = true;
                    this.object.showSubMeshesBoundingBox = true;
                }

                this._clearUI();
                this._createUI();
            } else {
                this._clearUI();
            }

        }
    }

}

BabylonEditorEditionTool.prototype.update = function () {
    if (this.object != null) {

    }
}

BabylonEditorEditionTool.prototype._clearUI = function () {
    /*for (var i = 0; i < this._forms.length; i++) {
        var ui = w2ui[this._forms[i]];
        if (ui)
            ui.destroy();
    }*/
    BabylonEditorUICreator.clearUI(this._forms);
}

BabylonEditorEditionTool.prototype._onChange = function (event) {
    var scope = w2ui['MainEditObjectTransform'].scope;

    var general = w2ui['MainEditObjectGeneral'];
    var transform = w2ui['MainEditObjectTransform'];
    var options = w2ui['MainEditObjectOptions'];
    
    /// General 
    scope.object.name = general.get('MainEditObjectName').el.value;
    scope.object.setEnabled(general.get('MainEditObjectEnabled').el.checked);

    /// Transforms
    scope.object.position = new BABYLON.Vector3(
        BabylonEditorUtils.toFloat(transform.get('MainEditObjectTransformPositionX').el.value),
        BabylonEditorUtils.toFloat(transform.get('MainEditObjectTransformPositionY').el.value),
        BabylonEditorUtils.toFloat(transform.get('MainEditObjectTransformPositionZ').el.value)
    );

    if (scope.object instanceof BABYLON.Mesh) {
        scope.object.rotation = new BABYLON.Vector3(
            BabylonEditorUtils.toFloat(transform.get('MainEditMeshTransformRotationX').el.value),
            BabylonEditorUtils.toFloat(transform.get('MainEditMeshTransformRotationY').el.value),
            BabylonEditorUtils.toFloat(transform.get('MainEditMeshTransformRotationZ').el.value)
        );

        scope.object.scaling = new BABYLON.Vector3(
            BabylonEditorUtils.toFloat(transform.get('MainEditMeshTransformScaleX').el.value),
            BabylonEditorUtils.toFloat(transform.get('MainEditMeshTransformScaleY').el.value),
            BabylonEditorUtils.toFloat(transform.get('MainEditMeshTransformScaleZ').el.value)
        );
    }

    /// Options
    if (scope.object instanceof BABYLON.Mesh) {
        scope.object.isVisible = options.get('MainEditMeshOptionsVisible').el.checked;
        scope.object.infiniteDistance = options.get('MainEditMeshOptionsInfiniteDistance').el.checked;
        scope.object.checkCollisions = options.get('MainEditMeshOptionsCheckCollisions').el.checked;
    }

    /// Reset focus
    $(this).focus();

    /// Send event because object changed
    var event = BabylonEditorEvent;
    event.EventType = BabylonEditorEventType.SceneEvent;
    event.SceneEvent.Type = BabylonEditorEvents.SceneEvents.ObjectChanged;
    event.SceneEvent.UserData = { object: scope.object };
    scope._core.sendEvent(event);
}

BabylonEditorEditionTool.prototype._createUI = function () {

    $('#MainEditorEditObject').empty();

    /// Create divs for forms
    BabylonEditorUICreator.createDivsForForms(this._forms, 'MainEditorEditObject');

    /// -----------------------------------------------------------------------------------------------------
    /// General
    var fields = new Array();

    BabylonEditorUICreator.extendFields(fields, [
        BabylonEditorUICreator.createField('MainEditObjectName', 'text', 'Name :'),
        BabylonEditorUICreator.createField('MainEditObjectEnabled', 'checkbox', 'Enabled :'),
    ]);

    var MainEditObjectGeneral = BabylonEditorUICreator.createForm('MainEditObjectGeneral', 'General', fields, this._onChange);

    /// Fill fields
    BabylonEditorUICreator.extendRecord(MainEditObjectGeneral.name, {
        MainEditObjectName: this.object.name,
        MainEditObjectEnabled: this.object.isEnabled()
    });

    /// -----------------------------------------------------------------------------------------------------

    /// -----------------------------------------------------------------------------------------------------
    /// Transforms

    fields = new Array();
    BabylonEditorUICreator.extendFields(fields, [
        /// Position
        BabylonEditorUICreator.createField('MainEditObjectTransformPositionX', 'float', 'Position :', 3, '<img src="UI/position.png"></img>'),
        BabylonEditorUICreator.createField('MainEditObjectTransformPositionY', 'float', ' ', 3),
        BabylonEditorUICreator.createField('MainEditObjectTransformPositionZ', 'float', ' ', 3),
    ]);

    /// If mesh
    if (this.object instanceof BABYLON.Mesh) {
        BabylonEditorUICreator.extendFields(fields, [
            /// Rotation
            BabylonEditorUICreator.createField('MainEditMeshTransformRotationX', 'float', 'Rotation :', 3, '<img src="UI/rotation.png"></img>'),
            BabylonEditorUICreator.createField('MainEditMeshTransformRotationY', 'float', ' ', 3),
            BabylonEditorUICreator.createField('MainEditMeshTransformRotationZ', 'float', ' ', 3),
            /// Scale
            BabylonEditorUICreator.createField('MainEditMeshTransformScaleX', 'float', 'Scaling :', 3, '<img src="UI/scale.png"></img>'),
            BabylonEditorUICreator.createField('MainEditMeshTransformScaleY', 'float', ' ', 3),
            BabylonEditorUICreator.createField('MainEditMeshTransformScaleZ', 'float', ' ', 3)
        ]);
    }

    var MainEditObjectTransform = BabylonEditorUICreator.createForm('MainEditObjectTransform',
                                                                    'Transforms', fields, this._onChange);

    /// Fill fields
    BabylonEditorUICreator.extendRecord(MainEditObjectTransform.name, {
        MainEditObjectTransformPositionX: this.object.position.x,
        MainEditObjectTransformPositionY: this.object.position.y,
        MainEditObjectTransformPositionZ: this.object.position.z,
    });

    if (this.object instanceof BABYLON.Mesh) {
        BabylonEditorUICreator.extendRecord(MainEditObjectTransform.name, {
            MainEditMeshTransformRotationX: this.object.rotation.x,
            MainEditMeshTransformRotationY: this.object.rotation.y,
            MainEditMeshTransformRotationZ: this.object.rotation.z,

            MainEditMeshTransformScaleX: this.object.scaling.x,
            MainEditMeshTransformScaleY: this.object.scaling.y,
            MainEditMeshTransformScaleZ: this.object.scaling.z
        });
    }

    /// -----------------------------------------------------------------------------------------------------

    /// -----------------------------------------------------------------------------------------------------
    /// Options
    fields = new Array();
    if (this.object instanceof BABYLON.Mesh) {
        BabylonEditorUICreator.extendFields(fields, [
            BabylonEditorUICreator.createField('MainEditMeshOptionsVisible', 'checkbox', 'Visible :', 6),
            BabylonEditorUICreator.createField('MainEditMeshOptionsInfiniteDistance', 'checkbox', 'Infinite Distance :', 6),
            BabylonEditorUICreator.createField('MainEditMeshOptionsCheckCollisions', 'checkbox', 'Check Collisions :', 6)
        ]);
    }

    var MainEditObjectOptions = BabylonEditorUICreator.createForm('MainEditObjectOptions',
                                                                  'Options', fields, this._onChange);

    /// Configure fields
    if (this.object instanceof BABYLON.Mesh) {
        BabylonEditorUICreator.extendRecord(MainEditObjectOptions.name, {
            MainEditMeshOptionsVisible: this.object.isVisible,
            MainEditMeshOptionsInfiniteDistance: this.object.infiniteDistance,
            MainEditMeshOptionsCheckCollisions: this.object.checkCollisions
        });
    }
    /// -----------------------------------------------------------------------------------------------------

    /// Finish
    BabylonEditorUtils.fillFormsWithScope(this._forms, this);
}
