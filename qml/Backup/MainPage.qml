import QtQuick 1.1
import com.nokia.meego 1.0

import com.nokia.extras 1.0
import "components/list"
import "components/dialogs"

Page {
    id: mainPage
    tools: commonTools
    Header {
        id: header;
    }

    EditNotePage{
        id:editNotePage
    }
    FileSelectionDialog{
        id:fsd
        onFileSelected:{
            console.log("filename:"+fileName+" filePath: "+filePath);
        }
    }

    function setStatusText(){
        if(Session.isSyncInProgress()){
            return;
        }
        header.statusText =  qsTr("__title__")
    }

    InfoBanner{
        id: networkErrorBanner
        x: 0
        y: parent.height-height
        z: 10
        text: qsTr("__basic_network_error__");
    }
    Connections{
        target: Cache
        onClearNotes:{
            listModel.clear();
        }
        onNoteAdded:{
            listModel.append(note);
        }
    }


    BlockingDialog{
        id:block
    }


    Connections{
        target: Session
        onSyncStarted:
        {
            header.statusText =  qsTr("__syncing__").arg(percent)
            header.showProgress();
            icnSync.enabled = false;
        }
        onNoteContentDownloaded:{
            var notCached = listModel.get(list.currentIndex);
            listModel.set(list.currentIndex, notCached);
            console.log(notCached.dateCreated);
        }

        onSyncFinished:
        {
            header.hideProgress();
            setStatusText();
            icnSync.enabled = true;
        }
        onSyncFailed:{
            header.hideProgress();
            setStatusText();
            icnSync.enabled = true;
            networkErrorBanner.show();
        }
        onLogoutStarted:{
            block.show();
        }
        onLogoutFinished:{
            block.close();
            var loginPage = Qt.resolvedUrl("LoginPage.qml")
            pageStack.pop();
            pageStack.push(loginPage);

        }
    }
    ListModel {
        id: listModel
    }

    SectionScroller{
        listView: list
    }
    EnchancedListView {
        id: list
        anchors.top:header.bottom
        width:parent.width
        height:parent.height-header.height
        model: listModel
        delegate: NoteListDelegate{
           width: mainPage.width;
           height: 185
           onItemClicked: {
               var viewPage = Qt.resolvedUrl("NoteView.qml")
               pageStack.push(viewPage, {note: Cache.getNote(index)})
               Session.getNoteContentAsync(Cache.getNote(index));
           }
           onLongItemClicked: {
               var editPage = Qt.resolvedUrl("EditNotePage.qml")
               pageStack.push(editPage, {note: Cache.getNote(index)})
               Session.getNoteContentAsync(Cache.getNote(index));
           }

        }
        section {
            property:"dateCreated"
            criteria: ViewSection.FullString
            delegate: Item{
                y: -30
                width: list.width
                height: 30
                Text{
                    id:sectionName
                    font.pixelSize: 18
                    font.weight: Font.Bold
                    color:"gray"
                    text:section
                    anchors.right: parent.right
                    anchors.rightMargin: 30
                    anchors.verticalCenter: parent.verticalCenter
                }

                Image{
                    source: "images/header_bottom.png"
                    anchors.left: parent.left
                    anchors.leftMargin: 8
                    anchors.verticalCenter: parent.verticalCenter
                    width:list.width-8-30-sectionName.width-30

                }
            }

        }
    }
    ToolBarLayout {
        id: commonTools
        visible: true
        Rectangle{
            color:"green"
            anchors.fill: parent;
        }
        ToolIcon {
            id: icnViewMenu
            platformIconId: "toolbar-view-menu"
            anchors.right: (parent === undefined) ? undefined : parent.right
            onClicked: (myMenu.status == DialogStatus.Closed) ? myMenu.open() : myMenu.close()
        }

        ToolIcon{
            id: icnSync
            platformIconId: "toolbar-refresh"
            anchors.right: icnViewMenu.left
            onClicked: {  Session.syncAsync() }
        }
        ToolIcon{
            id: icnAddNew
            platformIconId: "icon-m-toolbar-add"
            anchors.left: parent.left
            onClicked: {
                editNotePage.clear();
                pageStack.push(editNotePage);
            }
        }
    }


    Menu {
        id: myMenu
        visualParent: pageStack
        MenuLayout {

            MenuItem {
                id: miLogout
                text: qsTr("__logout__")
                onClicked: {
                    Session.logoutAsync();
                }
            }

        }
    }
}
