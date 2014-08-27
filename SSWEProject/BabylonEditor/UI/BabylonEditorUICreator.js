/*
Babylon Editor's UI creator.
Contains functions to create UI elements for the editor.

Please use this creator to create elements, it can clean your code
and can abstract the creation of GUI elements if the UI library must change
or must be replaced
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

/* Forms */
BabylonEditorUICreator.createForm = function (name, header, fields, onChange) {
    var form = $('#' + name).w2form({
        name: name,
        header: header,
        fields: fields,
        onChange: onChange
    });

    return form;
};

BabylonEditorUICreator.createDivsForForms = function (forms, parentDiv) {
    if (!(forms instanceof Array)) return;
    if (forms.length == 0) return;

    var currentForm = parentDiv;
    $('#' + currentForm).append('<div id="' + forms[0] + '"></div>');
    for (var i = 0; i < forms.length; i++) {
        $('#' + currentForm).after('<div id="' + forms[i] + '"></div>');
        currentForm = forms[i];
    }
}

BabylonEditorUICreator.createField = function (name, type, caption, span, text) {
    if (span == null) span = 6;

    return { name: name, type: type, html: { caption: caption, span: span, text: text } };
};

BabylonEditorUICreator.extendFields = function (fields, fieldsToAdd) {
    fields.push.apply(fields, fieldsToAdd);

    return fields;
}

BabylonEditorUICreator.extendRecord = function (form, recordToAdd) {
    var record = $.extend(w2ui[form].record, recordToAdd);

    return record;
}
