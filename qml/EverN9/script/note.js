function handleResource(name, hash, filePath) {
    var elements = document.getElementsByTagName("en-media");
    if (elements) {
        for (var i = 0; i < elements.length; ++i) {
            var element = elements[i];
            if (element.getAttribute("hash") == hash) {
                if (element.getAttribute("type").indexOf("image") != -1)
                    element.innerHTML = "<img alt='" + name + "' src='" + filePath + "'/>";
                else
                    element.innerHTML = "<a href='#' onclick=\"window.Qt.openUrlExternally('" + filePath + "');\">" + name + "</a>";
            }
        }
    }
}
