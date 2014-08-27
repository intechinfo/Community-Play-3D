/// <reference path="index.html" />

function BabylonEditor(babylonEditorCore) {
    this.engine = null;

    /// An editor must manage multiple scenes. Then, you'll be able to mange different
    /// worlds in your video game
    this.scenes = new Array();
    this.canvases = new Array();

    this.transformer = null;

    this._core = babylonEditorCore;
    this._core.customUpdates.push(this);

    this._mainToolbar = null;
    this._mditionTool = null;
    this._graphTool = null;

    this._createUI();
};

BabylonEditor.prototype.update = function () {

    this._core.currentScene.render();

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
                content: '<div id="MainEditorEditObject"></div>'
            },
            {
                type: 'main', style: pstyle, content: '<canvas id="renderCanvas"></canvas>', // Canvas HTML content
                tabs: [{ id: 'MainScenes', caption: 'Main Scene' }, { id: 'scene2', caption: 'Test Scene' }],
                /// Create a default tab for the default scene
            },
            {
                type: 'right', size: 300, resizable: true, style: pstyle, minSize: 300,
                content: '<div id="MainGraphTool"></div>'
            },
            {
                type: 'bottom', size: 50, resizable: true, style: pstyle,
                content: '<div id="MainOptionsBar" style="height: 100%"></div>'
            }
        ]
    });

    /// Create Babylon's engine here. Then, we'll be able to manage events like onClick, onResize, etc.
    var canvas = document.getElementById("renderCanvas");
    var scope = this;

    canvas.ondblclick = function (event) {
        scope._core.getPickedMesh(event, true);
    };

    w2ui['Mainlayout'].on('resize', function (target, eventData) {
        scope.engine.resize();
    });

    /// Configure this
    this.canvases.push(canvas);
    this.engine = new BABYLON.Engine(canvas, true);

    /// Create tool bar
    this._mainToolbar = new BabylonEditorMainToolbar(this._core);
    this._mainToolbar._createUI();

    /// Create Left Side bar
    this._editionTool = new BabylonEditorEditionTool(this._core);

    /// Create Right Side bar (Scene Graph)
    this._graphTool = new BabylonEditorGraphTool(this._core);
    this._graphTool._createUI();

    /// Create bottom toolbar
    $('#MainOptionsBar').w2toolbar({
        name: 'MainOptionsBar',
        items: []
    });

    /// Finish configuration and create default camera
    var scene = new BABYLON.Scene(this.engine);

    this.scenes.push(scene);
    this._core.currentScene = scene;

    var camera = new BABYLON.FreeCamera("BabylonEditorCamera", new BABYLON.Vector3(0, 5, -10), scene);
    camera.setTarget(new BABYLON.Vector3.Zero());
    camera.attachControl(canvas, false);

    this.transformer = new BabylonEditorTransformer(this.engine, this._core);

    this._graphTool._fillGraph(null, null);
};
