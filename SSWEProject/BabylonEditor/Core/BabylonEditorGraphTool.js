/// <reference path="./../index.html" />

function BabylonEditorGraphTool(babylonEditorCore) {
    this._core = babylonEditorCore;
    this._core.eventReceivers.push(this);

    this._graphRootName = 'babylon_editor_root_0';
};

BabylonEditorGraphTool.prototype.onEvent = function (event) {

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
                w2ui['MainGraph'].select(this._graphRootName);
            }
        }

        /// An object changed
        if (event.SceneEvent.Type == BabylonEditorEvents.SceneEvents.ObjectChanged) {
            var object = event.SceneEvent.UserData.object;
            if (object != null) {
                var element = w2ui['MainGraph'].get(object.id);
                element.text = element.data.name;
                if (object.parent != null)
                    element.parent = w2ui['MainGraph'].get(object.parent.id);
                w2ui['MainGraph'].refresh();
            }
        }
    }

}

BabylonEditorGraphTool.prototype._getObjectIcon = function (object) {
    var icon = '';

    if (object instanceof BABYLON.Mesh) icon = 'icon-mesh';
    else if (object instanceof BABYLON.Light) icon = 'icon-add-light';

    return icon;
}

BabylonEditorGraphTool.prototype._fillGraph = function (object, element) {
    var children = null;

    /// Set root as the root node of the side bar
    /// if the element isn't specified
    if (element == null) {
        BabylonEditorUtils.clearSideBar(w2ui['MainGraph']);
        w2ui['MainGraph'].add([{ id: this._graphRootName, text: 'Root', img: 'icon-position', data: null }]);
        root = this._graphRootName;
    }

    /// Same as element
    if (object == null) {
        children = this._core.currentScene.meshes;
        children.push.apply(children, this._core.currentScene.lights);
    } else
        children = object.getDescendants();

    if (root == this._graphRootName)
        w2ui['MainGraph'].get(root).expanded = true;

    /// If children then fill the side bar recursively
    if (children != null) {
        for (var i = 0; i < children.length; i++) {
            
            var object = children[i];
            var icon = this._getObjectIcon(object);

            w2ui['MainGraph'].add(root,
                [
                    { id: object.id, text: object.name, img: icon, data: object }
                ]
            );
            this._fillGraph(object, 'test' + i);

        }
    }

}

BabylonEditorGraphTool.prototype._modifyElement = function (object, element, remove) {
    if (object == null)
        return null;

    if (element == null) {
        element = this._graphRootName;
    }

    var graph = w2ui['MainGraph'];
    if (!remove) {
        var icon = this._getObjectIcon(object);
        graph.add(element, [{ id: object.id, text: object.name, img: icon, data: object }]);
    } else {
        element = graph.get(element);
        graph.remove(element);
    }

}

BabylonEditorGraphTool.prototype._createUI = function () {
    var scope = this;

    w2ui['Mainlayout'].content('right', $().w2sidebar({
        name: 'MainGraph',
        img: null,
        keyboard: false,

        nodes: [
            { id: this._graphRootName, text: 'Root', img: 'icon-position', data: null }
        ],

        onClick: function (event) {
            var ev = BabylonEditorEvent;
            ev.EventType = BabylonEditorEventType.SceneEvent;
            ev.SceneEvent.Type = BabylonEditorEvents.SceneEvents.ObjectPicked;
            ev.SceneEvent.UserData = { mesh: event.object.data };
            scope._core.sendEvent(ev);
        }

    }));

}
