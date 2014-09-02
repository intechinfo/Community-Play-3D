﻿/// <reference path="index.html" />

/*
Babylon Editor's UI creator.
Contains functions to create UI elements for the editor.

Please use this creator to create elements, it can clean your code
and can abstract the creation of GUI elements if the UI library must change
or must be replaced

This creator uses the jQuery and w2ui frameworks
*/

/// FIXME: Work directly with references instead of names (like Sidebar)
/// FIXME: Rename Sidebar as "Graph", because it is a graph...

var BabylonEditorUICreator = BabylonEditorUICreator || {};

/* General */
/// Clears elements stored into an array. Must implement it
/// Because some UI libraries can implement "delete()" instead of
/// "destroy()"
BabylonEditorUICreator.clearUI = function (elements) {
    if (!(elements instanceof Array)) return;
    if (elements.length == 0) return;

    for (var i = 0; i < elements.length; i++) {
        var ui = w2ui[elements[i]];
        if (ui)
            ui.destroy();
    }
}

/// Configure an event
/// element and event are string
BabylonEditorUICreator.addEvent = function(element, event, callback) {
    w2ui[element].on(event, callback);
}

/* Toolbars */
BabylonEditorUICreator.Toolbar = BabylonEditorUICreator.Toolbar || {};

/// Creates a toolbar and returns its reference
BabylonEditorUICreator.Toolbar.createToolbar = function (name, items, scope) {

    /// Configure events with scope on onClick event
    /// Creates the toolbar
    var toolbar = $('#' + name).w2toolbar({
        name: name,
        items: items,
        scope: scope,
        onClick: function (event) {
            /// Send the click event to event receivers
            var ev = BabylonEditorEvent;
            ev.EventType = BabylonEditorEventType.UIEvent;
            ev.UIEvent.Type = BabylonEditorEvents.UIEvents.ToolbarSelected;
            ev.UIEvent.Caller = event.target;
            scope._core.sendEvent(ev);
        }
    });

    return toolbar;
}

/// Creates a menu to use with toolbars
BabylonEditorUICreator.Toolbar.createMenu = function (type, id, caption, img, checked, items) {
    return { type: type, id: id, caption: caption, img: img, checked: checked, items: items == null ? [] : items };
}

/// Creates an item to use with toolbars
BabylonEditorUICreator.Toolbar.createItem = function (type, id, text, icon) {
    return { id: id, type: type, text: text, icon: icon };
}

/// Extends the items
BabylonEditorUICreator.Toolbar.extendItems = function (items, itemsToAdd) {
    /// With w2ui, simply concat arrays
    items.push.apply(items, itemsToAdd);

    return items;
}

/// Call if the menu's item was clicked. It automatically determine if
/// the item is now checked or not
BabylonEditorUICreator.Toolbar.setAutoItemChecked = function (menu, item) {
    var ui = w2ui[menu];
    var checked = ui.get(item).checked;

    if (!checked)
        ui.check(item);
    else
        ui.uncheck(item);
}

/// Set the item checked or not
/// check : boolean
BabylonEditorUICreator.Toolbar.setItemChecked = function (menu, item, check) {
    var ui = w2ui[menu];

    if (check)
        ui.check(item);
    else
        ui.uncheck(item);
}

/// Returns if the item is checked or not
BabylonEditorUICreator.Toolbar.isItemChecked = function (menu, item) {
    return w2ui[menu].get(item).checked;
}

/* Layouts */
BabylonEditorUICreator.Layout = BabylonEditorUICreator.Layout || {};

/// Creates a layout and returns its reference
BabylonEditorUICreator.Layout.createLayout = function (name, panels, scope) {
    var layouts = $('#' + name).w2layout({
        name: name,
        panels: panels
    });

    return layouts;
}

/// Creates a panel
/// A panel must contain a HTML content
/// A panel must contain tabs
BabylonEditorUICreator.Layout.createPanel = function (type, size, resizable, style, content, minSize, tabs) {
    var panel = {
        type: type, size: size, resizable: resizable, style: style, content: content,
        minSize: minSize == null ? 50 : minSize,
        tabs: tabs
    };

    return panel;
}

/// Extends the panels
BabylonEditorUICreator.Layout.extendPanels = function (panels, panelsToAdd) {
    /// With w2ui, just concat arrays
    panels.push.apply(panels, panelsToAdd);

    return panels;
}

/// Creates a tab
BabylonEditorUICreator.Layout.createTab = function (id, caption) {
    return { id: id, caption: caption };
}

/* Forms */
BabylonEditorUICreator.Form = BabylonEditorUICreator.Form || {};

/// Creates a form and returns its reference
BabylonEditorUICreator.Form.createForm = function (name, header, fields, scope) {
    var form = $('#' + name).w2form({
        name: name,
        header: header,
        fields: fields,
        scope: scope,
        onChange: function (event) {
            /// Send the FormChanged event to the event receivers
            var event = BabylonEditorEvent;
            event.EventType = BabylonEditorEventType.UIEvent;
            event.UIEvent.Type = BabylonEditorEvents.UIEvents.FormChanged;
            event.UIEvent.Caller = name;
            this.scope._core.sendEvent(event);
        }
    });

    return form;
};

/// Returns the HTML elements of the form named 'name'
/// Then, you can access :
///     var els = BabylonEditorUICreator.Form.getElements('MyFormName');
///     var scope = els.scope;
///     scope.object.name = els.fields['MyFormName:name'].value;
///     scope.object.setEnabled(els.fields['MyFormName:enabled'].checked);
/// etc.
BabylonEditorUICreator.Form.getElements = function (name) {
    var fields = new Array();

    for (var i = 0; i < w2ui[name].fields.length; i++) {
        fields[w2ui[name].fields[i].name] = w2ui[name].fields[i].el;
    }

    var scope = w2ui[name].scope;

    return { fields: fields, scope: scope };
}

/// Creates the appropriate divs (<div/>) needed by the forms
/// you want to create (forms)
/// parentDiv = the HTML element that will contain the divs, musn't be null
BabylonEditorUICreator.Form.createDivsForForms = function (forms, parentDiv, clear) {
    if (!(forms instanceof Array)) return;
    if (forms.length == 0) return;

    if (clear)
        $('#' + parentDiv).empty();

    var currentForm = parentDiv;
    $('#' + currentForm).append('<div id="' + forms[0] + '"></div>');
    for (var i = 0; i < forms.length; i++) {
        $('#' + currentForm).after('<div id="' + forms[i] + '"></div>');
        currentForm = forms[i];
    }
}

/// Creates a field to use with forms
BabylonEditorUICreator.Form.createField = function (name, type, caption, span, text) {
    if (span == null)
        span = 6;

    return { name: name, type: type, html: { caption: caption, span: span, text: text } };
};

/// Extends the fields
BabylonEditorUICreator.Form.extendFields = function (fields, fieldsToAdd) {
    /// With w2ui, just concat arrays
    fields.push.apply(fields, fieldsToAdd);

    return fields;
}

/// Extends the recors of the form
BabylonEditorUICreator.Form.extendRecord = function (form, recordToAdd) {
    /// With w2ui, just extend the records using jQuery
    var record = $.extend(w2ui[form].record, recordToAdd);

    return record;
}

/* Sidebar */
BabylonEditorUICreator.Sidebar = BabylonEditorUICreator.Sidebar || {};

/// Creates a side bar and returns its reference
BabylonEditorUICreator.Sidebar.createSideBar = function (name, nodes, scope) {
    var sideBar = $('#' + name).w2sidebar({
        name: name,
        img: null,
        keyboard: false,
        scope: scope,
        nodes: nodes,
        onClick: function (event) {
            /// Send the ObjectPicked event to the event receivers
            /// Must be extern
            var ev = BabylonEditorEvent;
            ev.EventType = BabylonEditorEventType.SceneEvent;
            ev.SceneEvent.Type = BabylonEditorEvents.SceneEvents.ObjectPicked;
            ev.SceneEvent.UserData = { mesh: event.object.data };
            this.scope._core.sendEvent(ev);
        }

    });

    return sideBar;
}

/// Set the selected element in the sidebar
BabylonEditorUICreator.Sidebar.setSelected = function (sideBar, id) {
    /// Expand all parents to have a view
    /// on the selected element
    var element = sideBar.get(id);
    while (element.parent != null) {
        element = element.parent;
        element.expanded = true;
    }

    sideBar.select(id);
}

/// Update side bar if modified and if it is needed
BabylonEditorUICreator.Sidebar.update = function (sideBar) {
    sideBar.refresh();
}

/// Adds nodes to the sidebar
BabylonEditorUICreator.Sidebar.addNodes = function (sideBar, nodes, parent) {
    if (parent == null)
        sideBar.add(nodes);
    else
        sideBar.add(parent, nodes);
}

/// Removed a node from the side bar
BabylonEditorUICreator.Sidebar.removeNode = function (sideBar, node) {
    var element = sideBar.get(node);
    if (node)
        sideBar.remove(node);
}

/// Updates the sidebar's node in function of an object (scene object)
BabylonEditorUICreator.Sidebar.updateNodeFromObject = function (sideBar, object) {
    var element = sideBar.get(object.id);
    element.text = element.data.name;
    if (object.parent != null)
        element.parent = sideBar.get(object.parent.id);
    BabylonEditorUICreator.Sidebar.update(sideBar);
}

/// Sets the node expanded or not
BabylonEditorUICreator.Sidebar.setNodeExpanded = function (sideBar, node, expanded) {
    if (expanded)
        sideBar.expand(node);
    else
        sideBar.collapse(node);
}

/// Creates a node
BabylonEditorUICreator.Sidebar.createNode = function (id, text, img, data) {
    return { id: id, text: text, img: img, data: data };
}

/// Extends the nodes
BabylonEditorUICreator.Sidebar.extendNodes = function (nodes, nodesToAdd) {
    /// With w2ui, just concat arrays
    return nodes.push.apply(nodes, nodesToAdd);
}
