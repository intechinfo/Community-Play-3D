/*

*/

var BabylonEditorUtils = BabylonEditorUtils || {}

BabylonEditorUtils.clearSideBar = function (sideBar) {
    var toRemove = [];
    for (var i=0; i < sideBar.nodes.length; i++) {
        toRemove.push(sideBar.nodes[i].id);
    }
    sideBar.remove.apply(sideBar, toRemove);
}
