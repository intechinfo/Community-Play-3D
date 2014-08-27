
function BabylonGraphTool(babylonEditorCore) {
    this._core = babylonEditorCore;
    this._core.eventReceivers.push(this);

    this._GraphRootName = 'babylon_editor_root_0';
};

BabylonGraphTool.prototype.onEvent = function (event) {

    if (event.EventType == BabylonEditorEventType.SceneEvent) {

        if (event.SceneEvent.Type == BabylonEditorEvents.SceneEvents.ObjectAdded
            || event.SceneEvent.Type == BabylonEditorEvents.SceneEvents.ObjectRemoved) {

            var element = (event.SceneEvent.UserData.mesh.parent == null) ? null : event.SceneEvent.UserData.mesh.parent.id;

            /// An object was added
            if (event.SceneEvent.Type == BabylonEditorEvents.SceneEvents.ObjectAdded) {
                if (event.SceneEvent.UserData.mesh != null)
                    this._modifyElement(event.SceneEvent.UserData.mesh, element, false);
            }

            /// An object was removed
            if (event.SceneEvent.Type == BabylonEditorEvents.SceneEvents.ObjectRemoved) {
                if (event.SceneEvent.UserData.mesh != null)
                    this._modifyElement(event.SceneEvent.UserData.mesh, element, true);
            }
        }

        /// An object was picked
        if (event.SceneEvent.Type == BabylonEditorEvents.SceneEvents.ObjectPicked) {
            var mesh = event.SceneEvent.UserData.mesh;
            if (mesh != null) {
                w2ui['MainGraph'].select(mesh.id);
                var element = w2ui['MainGraph'].get(mesh.id);
                while (element.parent != null) {
                    element.expanded = true;
                    element = element.parent;
                }
                w2ui['MainGraph'].refresh();
            } else {
                w2ui['MainGraph'].select(this._GraphRootName);
            }
        }
    }

}

BabylonGraphTool.prototype._fillGraph = function (mesh, element) {
    var children = null;

    /// Set root as the root node of the side bar
    /// if the element isn't specified
    if (element == null) {
        BabylonEditorUtils.clearSideBar(w2ui['MainGraph']);
        w2ui['MainGraph'].add([{ id: this._GraphRootName, text: 'Root', img: 'icon-position' }]);
        root = this._GraphRootName;
    }

    /// Same as element
    if (mesh == null)
        children = this._core.currentScene.meshes;
    else
        children = mesh.getChildren();

    if (root == this._GraphRootName)
        w2ui['MainGraph'].get(root).expanded = true;

    /// If children then fill the side bar recursively
    if (children != null) {
        for (var i = 0; i < children.length; i++) {

            var id = '' + children[i].name;
            w2ui['MainGraph'].add(root,
                [
                    { id: children[i].id, text: children[i].name, img: 'icon-mesh' }
                ]
            );
            this._fillGraph(children[i], 'test' + i);

        }
    }

}

BabylonGraphTool.prototype._modifyElement = function (mesh, element, remove) {
    if (mesh == null)
        return null;

    if (element == null) {
        element = this._GraphRootName;
    }

    var graph = w2ui['MainGraph'];
    if (!remove)
        graph.add(element, [{ id: mesh.id, text: mesh.name, img: 'icon-mesh' }]);
    else {
        element = graph.get(element);
        graph.remove(element);
    }
}

BabylonGraphTool.prototype._createUI = function () {

    w2ui['Mainlayout'].content('right', $().w2sidebar({
        name: 'MainGraph',
        img: null,
        keyboard: false,
        nodes: [
            { id: this._GraphRootName, text: 'Root', img: 'icon-position' }
        ]
    }));

}
