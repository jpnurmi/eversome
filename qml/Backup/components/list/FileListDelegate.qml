import QtQuick 1.0
import com.nokia.meego 1.0
import "../../constants.js" as Constants
import Qt.labs.folderlistmodel 1.0

DefaultDelegate{
    property FolderListModel model;
    property EnchancedListView listView;
    height: fileName != "." ? Constants.FILE_LIST_VIEW_ITEM_HEIGHT : 0
    function getImage(){
        if(model.isFolder(index)){
            if(fileName == ".."){
                return "../../images/icon-back.png";
            }
            return "../../images/icon-folder.png";
        }else{
            if(
                    fileName.indexOf("jpg") != -1 || fileName.indexOf("png") != -1 || fileName.indexOf("jpeg") != -1
                    || fileName.indexOf("gif") != -1

                    ){
                return filePath;
            }
            return "../../images/icon-file.png"
        }

        return "";
    }

    Image{
        id:thumb
        anchors.left: parent.left
        anchors.margins: 6
        source:getImage()
        width:Constants.FILE_LIST_VIEW_ITEM_HEIGHT-12
        height:Constants.FILE_LIST_VIEW_ITEM_HEIGHT-12

    }

    Label {
        anchors.left: thumb.right
        anchors.leftMargin: 12
        anchors.verticalCenter: parent.verticalCenter
        text: fileName != "." && fileName != ".." ? "<strong>"+fileName+"</stong>" : ""
        width:parent.width-24-thumb.width
        wrapMode: Text.WordWrap
    }
}
