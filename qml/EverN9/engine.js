var EDIT_ID = "edit_id"

function onLoadHandler(){

}
function onClickHandler(event){
   /* var style = event.target.getAttribute('style')+";border:solid";
    event.target.setAttribute('style', style);
    event.target.setAttribute("id", EDIT_ID);
    window.qml.editText(event.target.innerHTML);
*/
    window.qml.editText(document.body.innerHTML);

}

function isTextOnly(element){
    var textOnly, child;
    textOnly = true;
    for (child = element.firstChild;
         child;
         child = child.nextSibling
         ) {

        if (child.nodeType == 1) { // 1 == Element
            textOnly = false;
            break;
        }
    }
    return textOnly;
}

function addText(text){
    var div = document.createElement("div");
    var content = document.createTextNode(text);
    div.appendChild(content);
    document.body.appendChild(div);
}

function editText(text){
    var editedElement = document.getElementById(EDIT_ID);

    editedElement.innerHTML = text;
    editedElement.setAttribute("id","");

}

function log(string){
    window.qml.log(string);
}

function handleResourceLoad(hash, guid, filename, fullPath){
    var divs = document.getElementsByTagName("en-media");
    if(divs === null){
        log("no resources");
    }else{
        for (var i = 0; i < divs.length; i++) {
            var div = divs[i];
            if(div.getAttribute("hash").toLowerCase() == hash.toLowerCase()){
                if(isImage(div)){
                    processImage(hash, guid, div, fullPath);
                }else{
                    processAttach(hash, guid, div, filename, fullPath);
                }
            }
        }
    }
}
function processImage(hash, guid, div, fullPath){
    div.innerHTML += "<img src='"+fullPath+"' />";
}
function processAttach(hash, guid, div, filename, fullPath){
    //div.innerHTML += "<a href='"+guid+"/"+guid+"'>"+filename+"</a>";
    div.innerHTML += "<a href='#' onclick=\"window.qml.openFile('"+fullPath+"');\" >"+filename+"</a>";

}
function isImage(div){
    return div.getAttribute("type").indexOf("image") != -1;
}
