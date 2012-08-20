import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.0
import com.evernote.types 1.0
import "../list"
import "../../constants.js" as Constants

SelectionDialog {
    property string notebookGUID;

    Component.onCompleted: {
        if(model.count === 0){
            Cache.fillWithNotebooks();
        }
    }

    Connections{
        target:Cache
        onNotebookFired:{
            model.append(notebook);
        }
    }

    titleText: qsTr("__select_notebooks__")
    model: ListModel{}

    function setSelectedItem(notebookGUID){
        for(var i=0;i<model.count;i++){
            var current = model.get(i)
            console.log(current.guid);
            console.log(notebookGUID);
            if(notebookGUID === current.guid){
                selectedIndex = i;
                console.log(current.name +","+selectedIndex);
                return;
            }
        }
        return;
    }
    function getSelectedItem(){
        return model.get(selectedIndex);
    }
}
