/// <reference path="index.html" />

/*
Babylon Editor's UI creator.
Contains functions to create UI elements for the editor.

Please use this creator to create elements, it can clean your code
and can abstract the creation of GUI elements if the UI library must change
or must be replaced

This creator uses the jQuery and w2ui frameworks
*/

var BabylonEditorUICreator = BabylonEditorUICreator || {};

/* General */
BabylonEditorUICreator.clearUI = function (elements) {
    if (!(elements instanceof Array)) return;
    if (elements.length == 0) return;

    for (var i = 0; i < elements.length; i++) {
        var ui = w2ui[elements[i]];
        if (ui)
            ui.destroy();
    }
}

BabylonEditorUICreator.addEvent = function(element, event, callback) {
    w2ui[element].on(event, callback);
}

/* Toolbars */
BabylonEditorUICreator.Toolbar = BabylonEditorUICreator.Toolbar || {};

BabylonEditorUICreator.Toolbar.createToolbar = function (name, items, scope) {

    /// Configure events with scope on onClick event
    /// Creates the toolbar
    var toolbar = $('#' + name).w2toolbar({
        name: name,
        items: items,
        scope: scope,
        onClick: function (event) {
            var ev = BabylonEditorEvent;
            ev.EventType = BabylonEditorEventType.UIEvent;
            ev.UIEvent.Type = BabylonEditorEvents.UIEvents.ToolbarSelected;
            ev.UIEvent.Caller = event.target;
            scope._core.sendEvent(ev);
        }
    });

    return toolbar;
}

BabylonEditorUICreator.Toolbar.createMenu = function (type, id, caption, img, checked, items) {
    return { type: type, id: id, caption: caption, img: img, checked: checked, items: items == null ? [] : items };
}

BabylonEditorUICreator.Toolbar.createItem = function (type, id, text, icon) {
    return { id: id, type: type, text: text, icon: icon };
}

BabylonEditorUICreator.Toolbar.extendItems = function (items, itemsToAdd) {
    items.push.apply(items, itemsToAdd);
}

BabylonEditorUICreator.Toolbar.setAutoItemChecked = function (menu, item) {
    var ui = w2ui[menu];
    var checked = ui.get(item).checked;

    if (!checked)
        ui.check(item);
    else
        ui.uncheck(item);
}

BabylonEditorUICreator.Toolbar.setItemChecked = function (menu, item, check) {
    var ui = w2ui[menu];

    if (check)
        ui.check(item);
    else
        ui.uncheck(item);
}

BabylonEditorUICreator.Toolbar.isItemChecked = function (menu, item) {
    return w2ui[menu].get(item).checked;
}

/* Layouts */
BabylonEditorUICreator.Layout = BabylonEditorUICreator.Layout || {};

BabylonEditorUICreator.Layout.createLayout = function (name, panels) {
    var layouts = $('#' + name).w2layout({
        name: name,
        panels: panels
    });

    return layouts;
}

BabylonEditorUICreator.Layout.createPanel = function (type, size, resizable, style, content, minSize, tabs) {
    var panel = {
        type: type, size: size, resizable: resizable, style: style, content: content,
        minSize: minSize == null ? 50 : minSize,
        tabs: tabs
    };

    return panel;
}

BabylonEditorUICreator.Layout.extendPanels = function (panels, panelsToAdd) {
    panels.push.apply(panels, panelsToAdd);

    return panels;
}

BabylonEditorUICreator.Layout.createTab = function (id, caption) {
    return { id: id, caption: caption };
}

/* Forms */
BabylonEditorUICreator.Form = BabylonEditorUICreator.Form || {};

BabylonEditorUICreator.Form.createForm = function (name, header, fields) {
    var form = $('#' + name).w2form({
        name: name,
        header: header,
        fields: fields,
        onChange: function (event) {
            var event = BabylonEditorEvent;
            event.EventType = BabylonEditorEventType.UIEvent;
            event.UIEvent.Type = BabylonEditorEvents.UIEvents.FormChanged;
            event.UIEvent.Caller = name;
            this.scope._core.sendEvent(event);
        }
    });

    return form;
};

BabylonEditorUICreator.Form.getElements = function (name) {
    var fields = new Array();

    for (var i = 0; i < w2ui[name].fields.length; i++) {
        fields[w2ui[name].fields[i].name] = w2ui[name].fields[i].el;
    }

    var scope = w2ui[name].scope;

    return { fields: fields, scope: scope };
}

BabylonEditorUICreator.Form.createDivsForForms = function (forms, parentDiv) {
    if (!(forms instanceof Array)) return;
    if (forms.length == 0) return;

    var currentForm = parentDiv;
    $('#' + currentForm).append('<div id="' + forms[0] + '"></div>');
    for (var i = 0; i < forms.length; i++) {
        $('#' + currentForm).after('<div id="' + forms[i] + '"></div>');
        currentForm = forms[i];
    }
}

BabylonEditorUICreator.Form.createField = function (name, type, caption, span, text) {
    if (span == null) span = 6;

    return { name: name, type: type, html: { caption: caption, span: span, text: text } };
};

BabylonEditorUICreator.Form.extendFields = function (fields, fieldsToAdd) {
    fields.push.apply(fields, fieldsToAdd);

    return fields;
}

BabylonEditorUICreator.Form.extendRecord = function (form, recordToAdd) {
    var record = $.extend(w2ui[form].record, recordToAdd);

    return record;
}

BabylonEditorUICreator.Form.fillFormsWithScope = function (forms, scope) {
    for (var i = 0; i < forms.length; i++) {
        var ui = w2ui[forms[i]];
        if (ui != null)
            ui.scope = scope;
    }
}

/* Sidebar */
BabylonEditorUICreator.Sidebar = BabylonEditorUICreator.Sidebar || {};

BabylonEditorUICreator.Sidebar.createSideBar = function (name, nodes, scope) {
    var sideBar = $('#' + name).w2sidebar({
        name: name,
        img: null,
        keyboard: false,
        scope: scope,
        nodes: nodes,
        onClick: function (event) {
            var ev = BabylonEditorEvent;
            ev.EventType = BabylonEditorEventType.SceneEvent;
            ev.SceneEvent.Type = BabylonEditorEvents.SceneEvents.ObjectPicked;
            ev.SceneEvent.UserData = { mesh: event.object.data };
            scope._core.sendEvent(ev);
        }

    });

    return sideBar;
}

BabylonEditorUICreator.Sidebar.setSelected = function (sideBar, id) {
    var element = sideBar.get(id);
    while (element.parent != null) {
        element.expanded = true;
        element = element.parent;
    }

    sideBar.select(id);
}

BabylonEditorUICreator.Sidebar.update = function (sideBar) {
    sideBar.refresh();
}

BabylonEditorUICreator.Sidebar.addNodes = function (sideBar, nodes, parent) {
    if (parent == null)
        sideBar.add(nodes);
    else
        sideBar.add(parent, nodes);
}

BabylonEditorUICreator.Sidebar.removeNode = function (sideBar, node) {
    var element = sideBar.get(node);
    if (node)
        sideBar.remove(node);
}

BabylonEditorUICreator.Sidebar.updateNodeFromObject = function (sideBar, object) {
    var element = sideBar.get(object.id);
    element.text = element.data.name;
    if (object.parent != null)
        element.parent = sideBar.get(object.parent.id);
    BabylonEditorUICreator.Sidebar.update(sideBar);
}

BabylonEditorUICreator.Sidebar.setNodeExpanded = function (sideBar, node, expanded) {
    if (expanded)
        sideBar.expand(node);
    else
        sideBar.collapse(node);
}

BabylonEditorUICreator.Sidebar.createNode = function (id, text, img, data) {
    return { id: id, text: text, img: img, data: data };
}

BabylonEditorUICreator.Sidebar.extendNodes = function(nodes, nodesToAdd) {
    return nodes.push.apply(nodes, nodesToAdd);
}
