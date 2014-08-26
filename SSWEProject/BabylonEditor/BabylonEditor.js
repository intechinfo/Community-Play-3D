
function BabylonEditor(babylonEditorCore) {
    this.engine = null;

    /// An editor must manage multiple scenes. Then, you'll be able to mange different
    /// worlds in your video game
    this.scenes = [];
    this.canvases = [];

    this._core = babylonEditorCore;

    this._EditionTool = null;

    this._createUI();
};

BabylonEditor.prototype.update = function () {

    for (var i = 0; i < this.scenes.length; i++) {
        if (this.scenes[i])
        this.scenes[i].render();
    }
}

BabylonEditor.prototype._createUI = function() {

    /// Create Layouts in one shot
    var pstyle = 'background-color: #F5F6F7; border: 1px solid #dfdfdf; padding: 5px;';
    $('#Mainlayout').w2layout({
        name: 'Mainlayout',
        panels: [
            {
                type: 'top', size: 50, resizable: true, style: pstyle,
                content: '<div id="MainToolBar" style="height: 100%"></div>', // Tool bar HTML content
            },
            {
                type: 'left', size: 350, resizable: true, style: pstyle, minSize: 350,
                content: '<div id="MainEditMesh"></div>'
            },
            {
                type: 'main', style: pstyle, content: '<canvas id="renderCanvas"></canvas>', // Canvas HTML content
                tabs: [{ id: 'MainScenes', caption: 'Main Scene' }, { id: 'scene2', caption: 'Test Scene' }],
                /// Create a default tab for the default scene
            },
            { type: 'right', size: 200, resizable: true, style: pstyle },
            {
                type: 'bottom', size: 50, resizable: true, style: pstyle,
                content: '<div id="MainOptionsBar" style="height: 100%"></div>'
            }
        ]
    });

    /// Create Babylon's engine here. Then, we'll be able to manage events like onResizing, etc.
    var canvas = document.getElementById("renderCanvas");
    var scope = this;
    canvas.addEventListener("click", function (evt) {
        scope._core.getPickedMesh(evt, true);
    });

    this.canvases.push(canvas);
    this.engine = new BABYLON.Engine(canvas, true);

    /// Create tool bar
    this.createMainToolBar();

    /// Create Left Side bar
    this._EditionTool = new BabylonEditionTool(this._core);

    /// Create Right Side bar (Scene Graph)
    w2ui['Mainlayout'].content('right', $().w2sidebar({
        name: 'MainGraph',
        img: null,
        nodes: [
            {
                id: 'MainGraphRoot', text: 'Root', expanded: true, group: true, nodes: []
            },
        ],
    }));

    /// Create bottom toolbar
    $('#MainOptionsBar').w2toolbar({
        name: 'MainOptionsBar',
        items: [

        ]
    });

    /// Configure events...
    w2ui['Mainlayout'].get('main', false).onResizing = this.engine.resize();

    /// To test canvases
    this.scenes.push(this._core.createDefaultScene(canvas, this.engine));
};

BabylonEditor.prototype.createMainToolBar = function () {
    /// Create tool bar
    $('#MainToolBar').w2toolbar({
        name: 'MainToolBar',
        items: [
            {
                type: 'menu', id: 'MainEdit', caption: 'Edit', img: 'icon-edit', checked: false,
                items: [
                    { text: 'Edit Materials...' },
                    { text: 'Test' },
                    { type: 'break' },
                    { text: 'Test' },
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
            { type: 'button', id: 'MainToolBarAddMesh', caption: 'Add Mesh...', img: 'icon-add-mesh', checked: false },
            { type: 'break' },
            { type: 'button', id: 'MainToolBarPosition', caption: '', img: 'icon-position', checked: false },
            { type: 'button', id: 'MainToolBarRotation', caption: '', img: 'icon-rotation', checked: false },
            { type: 'button', id: 'MainToolBarScale', caption: '', img: 'icon-scale', checked: false }
        ]
    });
};
