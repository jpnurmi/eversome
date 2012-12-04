function handleResource(fileName, hash, filePath, thumbnail)
{
    var elements = document.getElementsByTagName("en-media");
    if (elements && elements.length > 0) {
        for (var i = 0; i < elements.length; ++i) {
            if (elements[i].getAttribute("hash") == hash) {
                var child = elements[i].lastChild;
                if (child && child.getAttribute("id") == "resource")
                    elements[i].removeChild(child);
                var resource = createResource(elements[i].getAttribute("type"), fileName, hash, filePath, thumbnail);
                if (resource)
                    elements[i].appendChild(resource);
            }
        }
    }
}

function createResource(type, fileName, hash, filePath, thumbnail)
{
    var image = document.createElement("img");
    image.setAttribute("class", "resource");
    image.setAttribute("alt", fileName);
    image.setAttribute("src", thumbnail);

    var anchor = document.createElement("a");
    image.setAttribute("class", "resource");
    anchor.setAttribute("href", "#");
    anchor.setAttribute("onclick", "window.qml.openUrlExternally('" + filePath + "');");
    anchor.innerHTML = fileName;

    var div = document.createElement("div");
    div.setAttribute("class", "resource");
    div.setAttribute("onclick", "window.qml.openUrlExternally('" + filePath + "');");
    div.appendChild(image);
    div.appendChild(anchor);
    return div;
}
