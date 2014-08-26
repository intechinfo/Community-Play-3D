
function BabylonEditionTool(babylonEditorCore) {
    this._core = babylonEditorCore;
    this._core.eventReceivers.push(this);

    this.mesh = null;
};

BabylonEditionTool.prototype.onEvent = function (event) {

    if (event.EventType == BabylonEditorEventType.SceneEvent) {
        if (event.SceneEvent.Type == BabylonEditorEvents.ObjectPicked) {
            this.mesh = event.SceneEvent.UserData.mesh;
            this._createUI();
        }
    }

    console.log(event);

}

BabylonEditionTool.prototype._createUI = function () {

    $('#MainEditMesh').w2form({
        name: 'myForm',
        fields: [
            /// Position
            {
                name: 'Position :', type: 'float',
                html:
                    {
                        text: '<img src="UI/position.png"></img>'
                    }
            },
            { name: '', type: 'float' },
            { name: '', type: 'float' },
            /// Rotation
            {
                name: 'Rotation :', type: 'float',
                html:
                    {
                        text: '<img src="UI/rotation.png"></img>'
                    }
            },
            { name: '', type: 'float' },
            { name: '', type: 'float' },
            /// Scale
            {
                name: 'Scale :', type: 'float',
                html:
                    {
                        text: '<img src="UI/scale.png"></img>'
                    }
            },
            { name: '', type: 'float' },
            { name: '', type: 'float' },
        ],
        actions: {
            reset: function () { /*this.reset();*/ },
            apply: function () { /*this.apply();*/ }
        }
    });

    /*w2ui['Mainlayout'].content('left',
        BabylonEditorUtils.createField('Transform', 'icon-position') +
        BabylonEditionTool.createEditBox('Position')
    );*/
}
