import QtQuick 1.1
import com.nokia.meego 1.0

import com.nokia.extras 1.0
import Qt.labs.folderlistmodel 1.0
import "../../constants.js" as Constants
import "../list"
import "../"


Sheet {
    signal fileSelected(string fileName, string filePath);
    property string currentFileName;
    property string currentFilePath;

    id: dialog
    acceptButtonText: qsTr("__select__")
    rejectButtonText: qsTr("__cancel__")

    ErrorBanner{
        id:error
        text: qsTr("__select_right_file__")
    }

    property alias rootFolder: folderModel.folder;

    content:

        EnchancedListView {
        id: list
        FolderListModel {
            id: folderModel
            showDotAndDotDot:true
        }
        model: folderModel
        delegate:
            FileListDelegate{
            model: folderModel
            listView: list
            id:fileDelegate
            width:dialog.width
            onItemClicked: {
                if(fileName == "."){
                    folderModel.folder = folderModel.parentFolder;
                    currentFileName = fileName;
                    currentFilePath = filePath;
                }else{
                    folderModel.folder = filePath;
                    currentFileName = fileName;
                    currentFilePath = filePath;
                }
            }
        }
    }

    onAccepted: {
        if(folderModel.isFolder(list.currentIndex)){
            error.show();
            return;
        }
        fileSelected(currentFileName, currentFilePath);
        close();
    }
}
