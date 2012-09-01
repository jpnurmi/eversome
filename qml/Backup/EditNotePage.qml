import QtQuick 1.1
import QtWebKit 1.0
import com.nokia.meego 1.0
import com.evernote.types 1.0
import com.nokia.extras 1.0
import "constants.js" as Constants
import "components"
import "components/dialogs"
import "components/list"
import "components/dialogs/noteedit"

Page {
    id: editNotePage
    property alias title: txtTitle.text
    property Note targetNote;
    property string selectedNotebookGUID;
    property variant selectedTagGUIDs;
    tools:commonTools

    Connections{
        target: Session
        onNoteContentDownloaded:{
            txtText.text = Cache.getNoteContent(note);
        }
        onNoteLoadStarted:{
            targetNote = note;
            title = targetNote.title;

            selectedNotebookGUID = targetNote.notebookGUID;
            ddNotebooks.selectedText = Cache.getNotebook(note).name

            selectedTagGUIDs = targetNote.tagGuids;
            tagDialog.setSelectedIndexes(selectedTagGUIDs);
            ddTags.selectedText = tagDialog.getSelectedTagNamesString();
        }

        onNoteLoadFinished:{
        }

        onNoteLoadError:{
        }
        onResourceDownloaded:{

        }
    }

    Header{
        id:header
        statusText: ""
        ToolButton{
            id: btnSave
            text: qsTr("__save__")
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            anchors.margins: Constants.DEFAILT_MARGIN
            onClicked:{

            }
        }
        ToolButton{
            id: btnClose
            text: qsTr("__close__")
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            anchors.margins: Constants.DEFAILT_MARGIN
            onClicked: {
                pageStack.pop();
            }
        }
    }

    TagSelectionDialog{
        id:tagDialog
        onAccepted: {
            selectedTagGUIDs = tagDialog.getSelectedTagGUIDs();
            ddTags.selectedText = tagDialog.getSelectedTagNamesString();
        }
    }
    NotebookSelectionDialog{
        id:notebookDialog

        onAccepted: {
            selectedNotebookGUID = notebookDialog.getSelectedItem().guid;
            ddNotebooks.selectedText = notebookDialog.getSelectedItem().name;
        }
    }
    AddTextDialog{
        id:addTextDialog
        onAccepted:{
            if(acceptButtonText == qsTr("__add__")){
                webView.evaluateJavaScript("addText('" +text+ "');");
            }else{
                webView.evaluateJavaScript("editText('" +text+ "');");
            }
        }
    }
    FileSelectionDialog{
        id:attachDialog
    }

    Column {

        width:parent.width - 30
        height: parent.height-header.height-commonTools.height
        anchors.top: header.bottom
        anchors.topMargin: Constants.DEFAILT_MARGIN;
        anchors.bottom: commonTools.top
        anchors.horizontalCenter: parent.horizontalCenter
        id:clmContent
        Flickable{
            width:parent.width
            height:parent.height
            contentWidth: parent.width
            contentHeight: txtTitle.height+Constants.DEFAILT_MARGIN+txtText.height+Constants.DEFAILT_MARGIN+ddNotebooks.height+Constants.DEFAILT_MARGIN+ddTags.height
            anchors.margins: 15

            TextField{
                id: txtTitle;
                placeholderText: qsTr("__title__")
                width: parent.width
                anchors.left: parent.left
                text: ""
            }

            DropDownRow{
                id:ddNotebooks
                anchors.top: txtTitle.bottom
                anchors.topMargin: Constants.DEFAILT_MARGIN
                title: qsTr("__notebook__")
                onClicked: {
                    notebookDialog.setSelectedItem(selectedNotebookGUID)
                    notebookDialog.open();
                }
            }
            DropDownRow{
                id:ddTags
                anchors.top: ddNotebooks.bottom
                anchors.topMargin: Constants.DEFAILT_MARGIN
                title: qsTr("__tags__")
                selectedText: ""
                onClicked: {
                    tagDialog.open();
                }
            }
            TextArea{
                    id:txtText
                    text:"some shit"

                    textFormat: TextEdit.RichText
                    width: parent.width
                    anchors.top: ddTags.bottom
                    anchors.left: parent.left
                    anchors.right: parent.right

            }


            ToolBarLayout {
                id: commonTools
                visible: true
                Rectangle{
                    color:"green"
                    anchors.fill: parent;
                }

                ToolIcon{
                    id: icnAddText
                    anchors.left: parent.left
                    platformIconId: "icon-m-toolbar-edit"
                    onClicked: {
                        editOrAddText("", false);
                    }
                    width: 100
                }
                ToolIcon{
                    id: icnAddAttachement
                    platformIconId: "icon-m-toolbar-attachment"
                    anchors.left: icnAddText.right
                    onClicked: {
                        attachDialog.open()
                    }
                }
                ToolButton{
                    id: icnAddLink
                    text: qsTr("__link__")
                    anchors.left: icnAddAttachement.right
                }
            }

        }
    }
    function clear(){
        title = "";
    }
    function editOrAddText(text, isEdit){
        addTextDialog.clear();
        if(isEdit){
            addTextDialog.text = text;
            addTextDialog.acceptButtonText = qsTr("__save__");
        }else{
            addTextDialog.text = "";
            addTextDialog.acceptButtonText = qsTr("__add__");
        }
        addTextDialog.open();
    }
}
