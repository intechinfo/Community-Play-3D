
function BabylonEditionTool(babylonEditorCore) {
    this._core = babylonEditorCore;
    this._core.eventReceivers.push(this);
    this._core.customUpdates.push(this);

    this.mesh = null;

    this._forms = [
        'MainEditMeshTransform',
        'MainEditMeshOptions'
    ];
};

BabylonEditionTool.prototype.onEvent = function (event) {

    if (event.EventType == BabylonEditorEventType.SceneEvent) {
        if (event.SceneEvent.Type == BabylonEditorEvents.SceneEvents.ObjectPicked) {

            if (this.mesh != null) {
                this.mesh.showBoundingBox = false;
                this.mesh.showSubMeshesBoundingBox = false;
            }

            this.mesh = event.SceneEvent.UserData.mesh;
            if (this.mesh != null) {
                this.mesh.showBoundingBox = true;
                this.mesh.showSubMeshesBoundingBox = true;

                this._clearUI();
                this._createUI();
            } else {
                this._clearUI();
            }

        }
    }

    console.log(event);
}

BabylonEditionTool.prototype.update = function () {
    if (this.mesh != null) {

    }
}

BabylonEditionTool.prototype._clearUI = function () {
    for (var i = 0; i < this._forms.length; i++) {
        var ui = w2ui[this._forms[i]];
        if (ui)
            w2ui[this._forms[i]].destroy();
    }
}

BabylonEditionTool.prototype._createUI = function () {

    $('#MainEditMesh').empty();

    $('#MainEditMesh').append('<div id="MainEditMeshTransform"></div>');
    $('#MainEditMeshTransform').after('<div id="MainEditMeshOptions"></div>');

    /// -----------------------------------------------------------------------------------------------------
    /// Transforms
    $('#MainEditMeshTransform').w2form({
        name: 'MainEditMeshTransform',
        header: 'Transforms',
        fields: [
            /// Position
            {
                name: 'MainEditMeshTransformPositionX', type: 'float',
                html:
                    {
                        text: '<img src="UI/position.png"></img>',
                        caption: 'Position :'
                    }
            },
            { name: 'MainEditMeshTransformPositionY', type: 'float', html: { caption: ' ' } },
            { name: 'MainEditMeshTransformPositionZ', type: 'float', html: { caption: ' ' } },
            /// Rotation
            {
                name: 'MainEditMeshTransformRotationX', type: 'float',
                html:
                    {
                        text: '<img src="UI/rotation.png"></img>',
                        caption: 'Rotation :'
                    }
            },
            { name: 'MainEditMeshTransformRotationY', type: 'float', html: {caption:' '} },
            { name: 'MainEditMeshTransformRotationZ', type: 'float', html: { caption: ' ' } },
            /// Scale
            {
                name: 'MainEditMeshTransformScaleX', type: 'float',
                html:
                    {
                        text: '<img src="UI/scale.png"></img>',
                        caption: 'Scale :'
                    }
            },
            { name: 'MainEditMeshTransformScaleY', type: 'float', html: { caption: ' ' } },
            { name: 'MainEditMeshTransformScaleZ', type: 'float', html: { caption: ' ' } },
        ],
    });

    /// -----------------------------------------------------------------------------------------------------

    /// -----------------------------------------------------------------------------------------------------
    /// Options
    $('#MainEditMeshOptions').w2form({
        name: 'MainEditMeshOptions',
        header: 'Options',
        fields: [
            { name: 'MainEditMeshOptionsVisible', type: 'checkbox', html: { caption: 'Visible :' } },
            { name: 'MainEditMeshOptionsInfiniteDistance', type: 'checkbox', html: { caption: 'Infinite Distance :' } },
            { name: 'MainEditMeshOptionsCheckCollisions', type: 'checkbox', html: { caption: 'Check Collisions :' } },

        ]
    });
    /// -----------------------------------------------------------------------------------------------------


}
