/// <reference path="./../index.html" />

function BabylonEditorMainToolbar(babylonEditorCore) {
    this._core = babylonEditorCore;
    this._core.eventReceivers.push(this);
};

BabylonEditorMainToolbar.prototype.onEvent = function (event) {



}

BabylonEditorMainToolbar.prototype._createUI = function () {

    /// Create tool bar
    $('#MainToolBar').w2toolbar({
        name: 'MainToolBar',
        items: [
            {
                type: 'menu', id: 'MainFiles', caption: 'File', img: 'icon-open-file', checked: false,
                items: [
                    { text: 'Open a saved scene...', icon: 'icon-open-file' },
                    { text: 'Save scene...', icon: 'icon-save-file' }
                ]
            },
            {
                type: 'menu', id: 'MainEdit', caption: 'Edit', img: 'icon-edit', checked: false,
                items: [
                    { text: 'Edit Materials...' },
                    { text: 'Edit Material Shaders...' },
                    { type: 'break' },
                    { text: 'Edit Current Scene...' },
                ]
            },
            { type: 'break' },
            {
                type: 'menu', id: 'MainToolBarAddLight', caption: 'Add light', img: 'icon-add-light', checked: false,
                items: [
                    { text: 'Point Light', icon: 'icon-add-light' },
                    { text: 'Spot Light', icon: 'icon-add-light' }
                ]
            },
            {
                type: 'menu', id: 'MainToolBarAddPrimitive', caption: 'Primitives', img: 'icon-primitives', checked: false,
                items: [
                    { text: 'Add Ground', icon: 'icon-add-ground' },
                    { text: 'Add Sphere', icon: 'icon-add-sphere' },
                    { text: 'Add Cube', icon: 'icon-add-cube' },
                    { text: 'Add Billboard', icon: 'icon-add-billboard' }
                ]
            },
            { type: 'button', id: 'MainToolBarAddMesh', caption: 'Add Mesh...', img: 'icon-mesh', checked: false },
            { type: 'break' },
            { type: 'button', id: 'MainToolBarPosition', caption: '', img: 'icon-position', checked: false },
            { type: 'button', id: 'MainToolBarRotation', caption: '', img: 'icon-rotation', checked: false },
            { type: 'button', id: 'MainToolBarScale', caption: '', img: 'icon-scale', checked: false }
        ]
    });

}
