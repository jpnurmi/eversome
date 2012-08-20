import QtQuick 1.1
import com.nokia.meego 1.0

import com.nokia.extras 1.0
import QtMobility.sensors 1.1
import com.evernote.types 1.0
import QtWebKit 1.0

Page {
    property Note note;
    id: viewPage
    tools: commonTools
    InfoBanner{
        id: networkErrorBanner
        x: 0
        y: parent.height-height
        z: 10

    }

    Rectangle{
        x:0
        y:0
        width:parent.width
        height:parent.height
        color: "white"
    }

    Connections{
        target: EvernoteSession
        onNoteContentDownloaded:{
            webView.url = Cache.getCacheFileName(note);
        }
        onNoteLoadStarted:{
            header.showProgress();
            header.statusText = qsTr("__loading__")
        }

        onNoteLoadFinished:{
            header.hideProgress();

            header.statusText = note.title;
        }

        onNoteLoadError:{
            header.hideProgress();
            networkErrorBanner.text = error;
            networkErrorBanner.show();
        }
        onResourceDownloaded:{
            webView.evaluateJavaScript("handleResourceLoad('"+r.hash+"','"+r.guid+"','"+r.filename+"','"+r.fullPath+"');");
        }
    }
    Header{
        id: header
        statusText: note.title
    }


    Flickable {
        id: flickable
        width:parent.width
        height:viewPage.height - header.height
        contentWidth:webView.width
        contentHeight: webView.height
        anchors.top:header.bottom;

        PinchArea {
            id: pincharea
            property double oldZoom;
            width: webView.width
            height: webView.height
            function calcZoomDelta(zoom, percent) {
                return zoom + Math.log(percent)/Math.log(2)
            }

            onPinchStarted: {
                oldZoom = webView.contentsScale
            }

            onPinchUpdated: {
                var newZoom = calcZoomDelta(oldZoom, pinch.scale);
                if(webView.width > viewPage.width || newZoom > oldZoom){
                    webView.contentsScale = newZoom
                }
            }

            onPinchFinished: {
                var newZoom = calcZoomDelta(oldZoom, pinch.scale);
                if(webView.width > viewPage.width || newZoom > oldZoom){
                    webView.contentsScale = newZoom
                }
            }

            WebView{
                id: webView
                javaScriptWindowObjects: QtObject {
                    WebView.windowObjectName: "qml"
                    function openFile(url) {
                        console.log("qml called from javas:"+url);
                        Qt.openUrlExternally(url);
                    }
                }
                settings.defaultFontSize: 24;
                preferredWidth: viewPage.width
                preferredHeight: viewPage.height-header.height-commonTools.height

                settings.javascriptCanOpenWindows: true
                settings.javascriptEnabled: true;
                settings.pluginsEnabled: true

                contentsScale: 1;
                onDoubleClick: {
                    contentsScale = viewPage.width/webView.width;
                }

            }
        }


    }
    ToolBarLayout {
        id: commonTools
        visible: true
        ToolIcon { iconId: "icon-m-toolbar-back"; onClicked: {
                EvernoteSession.cancelGetNoteContent();
                pageStack.pop();
            }
        }
    }
}
