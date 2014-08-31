﻿/// <reference path="./../index.html" />

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

    /// Scene Event
    if (event.EventType == BabylonEditorEventType.SceneEvent) {

        /// Object picked
        if (event.SceneEvent.Type == BabylonEditorEvents.SceneEvents.ObjectPicked) {

            if (this.object != null && this.object instanceof BABYLON.Mesh) {
                this.object.showBoundingBox = false;
                this.object.showSubMeshesBoundingBox = false;
            }

            this.object = event.SceneEvent.UserData.mesh;

            /// Test here for the moment...
            this._core.transformer.setNodeToTransform(this.object);
            /// Tested here for the moment...

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

    else /// UI Event

    if (event.EventType == BabylonEditorEventType.UIEvent) {
        if (event.UIEvent.Type == BabylonEditorEvents.UIEvents.FormChanged) {

            if (this._forms.indexOf(event.UIEvent.Caller) > -1) {
                this._onChange();
            }

        }
    }

}

BabylonEditorEditionTool.prototype.update = function () {
    if (this.object != null) {

    }
}

BabylonEditorEditionTool.prototype._clearUI = function () {
    BabylonEditorUICreator.clearUI(this._forms);
}

BabylonEditorEditionTool.prototype._onChange = function () {
    var general = BabylonEditorUICreator.Form.getElements('MainEditObjectGeneral');
    var transform = BabylonEditorUICreator.Form.getElements('MainEditObjectTransform');
    var options = BabylonEditorUICreator.Form.getElements('MainEditObjectOptions');
    
    var scope = general.scope;

    /// General 
    scope.object.name = general.fields['MainEditObjectName'].value;
    scope.object.setEnabled(general.fields['MainEditObjectEnabled'].checked);

    /// Transforms
    scope.object.position = new BABYLON.Vector3(
        BabylonEditorUtils.toFloat(transform.fields['MainEditObjectTransformPositionX'].value),
        BabylonEditorUtils.toFloat(transform.fields['MainEditObjectTransformPositionY'].value),
        BabylonEditorUtils.toFloat(transform.fields['MainEditObjectTransformPositionZ'].value)
    );

    if (scope.object instanceof BABYLON.Mesh) {
        scope.object.rotation = new BABYLON.Vector3(
            BabylonEditorUtils.toFloat(transform.fields['MainEditMeshTransformRotationX'].value),
            BabylonEditorUtils.toFloat(transform.fields['MainEditMeshTransformRotationY'].value),
            BabylonEditorUtils.toFloat(transform.fields['MainEditMeshTransformRotationZ'].value)
        );

        scope.object.scaling = new BABYLON.Vector3(
            BabylonEditorUtils.toFloat(transform.fields['MainEditMeshTransformScaleX'].value),
            BabylonEditorUtils.toFloat(transform.fields['MainEditMeshTransformScaleY'].value),
            BabylonEditorUtils.toFloat(transform.fields['MainEditMeshTransformScaleZ'].value)
        );
    }

    /// Options
    if (scope.object instanceof BABYLON.Mesh) {
        scope.object.isVisible = options.fields['MainEditMeshOptionsVisible'].checked;
        scope.object.infiniteDistance = options.fields['MainEditMeshOptionsInfiniteDistance'].checked;
        scope.object.checkCollisions = options.fields['MainEditMeshOptionsCheckCollisions'].checked;
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
    BabylonEditorUICreator.Form.createDivsForForms(this._forms, 'MainEditorEditObject');

    /// -----------------------------------------------------------------------------------------------------
    /// General
    var fields = new Array();

    BabylonEditorUICreator.Form.extendFields(fields, [
        BabylonEditorUICreator.Form.createField('MainEditObjectName', 'text', 'Name :'),
        BabylonEditorUICreator.Form.createField('MainEditObjectEnabled', 'checkbox', 'Enabled :'),
    ]);

    var MainEditObjectGeneral = BabylonEditorUICreator.Form.createForm('MainEditObjectGeneral', 'General', fields);

    /// Fill fields
    BabylonEditorUICreator.Form.extendRecord(MainEditObjectGeneral.name, {
        MainEditObjectName: this.object.name,
        MainEditObjectEnabled: this.object.isEnabled()
    });

    /// -----------------------------------------------------------------------------------------------------

    /// -----------------------------------------------------------------------------------------------------
    /// Transforms

    fields = new Array();
    BabylonEditorUICreator.Form.extendFields(fields, [
        /// Position
        BabylonEditorUICreator.Form.createField('MainEditObjectTransformPositionX', 'float', 'Position :', 3, '<img src="UI/position.png"></img>'),
        BabylonEditorUICreator.Form.createField('MainEditObjectTransformPositionY', 'float', ' ', 3),
        BabylonEditorUICreator.Form.createField('MainEditObjectTransformPositionZ', 'float', ' ', 3),
    ]);

    /// If mesh
    if (this.object instanceof BABYLON.Mesh) {
        BabylonEditorUICreator.Form.extendFields(fields, [
            /// Rotation
            BabylonEditorUICreator.Form.createField('MainEditMeshTransformRotationX', 'float', 'Rotation :', 3, '<img src="UI/rotation.png"></img>'),
            BabylonEditorUICreator.Form.createField('MainEditMeshTransformRotationY', 'float', ' ', 3),
            BabylonEditorUICreator.Form.createField('MainEditMeshTransformRotationZ', 'float', ' ', 3),
            /// Scale
            BabylonEditorUICreator.Form.createField('MainEditMeshTransformScaleX', 'float', 'Scaling :', 3, '<img src="UI/scale.png"></img>'),
            BabylonEditorUICreator.Form.createField('MainEditMeshTransformScaleY', 'float', ' ', 3),
            BabylonEditorUICreator.Form.createField('MainEditMeshTransformScaleZ', 'float', ' ', 3)
        ]);
    }

    var MainEditObjectTransform = BabylonEditorUICreator.Form.createForm('MainEditObjectTransform',
                                                                         'Transforms', fields);

    /// Fill fields
    BabylonEditorUICreator.Form.extendRecord(MainEditObjectTransform.name, {
        MainEditObjectTransformPositionX: this.object.position.x,
        MainEditObjectTransformPositionY: this.object.position.y,
        MainEditObjectTransformPositionZ: this.object.position.z,
    });

    if (this.object instanceof BABYLON.Mesh) {
        BabylonEditorUICreator.Form.extendRecord(MainEditObjectTransform.name, {
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
        BabylonEditorUICreator.Form.extendFields(fields, [
            BabylonEditorUICreator.Form.createField('MainEditMeshOptionsVisible', 'checkbox', 'Visible :', 6),
            BabylonEditorUICreator.Form.createField('MainEditMeshOptionsInfiniteDistance', 'checkbox', 'Infinite Distance :', 6),
            BabylonEditorUICreator.Form.createField('MainEditMeshOptionsCheckCollisions', 'checkbox', 'Check Collisions :', 6)
        ]);
    }

    var MainEditObjectOptions = BabylonEditorUICreator.Form.createForm('MainEditObjectOptions',
                                                                       'Options', fields);

    /// Configure fields
    if (this.object instanceof BABYLON.Mesh) {
        BabylonEditorUICreator.Form.extendRecord(MainEditObjectOptions.name, {
            MainEditMeshOptionsVisible: this.object.isVisible,
            MainEditMeshOptionsInfiniteDistance: this.object.infiniteDistance,
            MainEditMeshOptionsCheckCollisions: this.object.checkCollisions
        });
    }
    /// -----------------------------------------------------------------------------------------------------

    /// Finish
    BabylonEditorUICreator.Form.fillFormsWithScope(this._forms, this);
}
