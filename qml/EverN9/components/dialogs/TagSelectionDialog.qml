import QtQuick 1.1
import com.nokia.meego 1.0
import "../list"
import com.nokia.extras 1.0
import "../../constants.js" as Constants

MultiSelectionDialog {
    Component.onCompleted: {
        if(model.count === 0){
            Cache.fillWithTags();
        }
    }

    Connections{
        target:Cache
        onTagFired:{
            model.append(tag);
        }
    }

    titleText: qsTr("__select_tags__")
    //selectedIndexes: [1, 3, 5]
    model: ListModel{}
    acceptButtonText: qsTr("__ok__")

    function setSelectedIndexes(tagGuids){
        var k = 0;
        var test = [];
        for(var i =0;i<model.count;i++){
            var tag = model.get(i);
            for(var j=0;j<tagGuids.length;j++){
                if(tag.guid === tagGuids[j]){
                    test[k] = i;
                    console.log("selected index:"+i);
                    k++;
                }
            }
        }
        selectedIndexes = test;
        console.log(selectedIndexes);
    }

    function getSelectedTagGUIDs(){
        console.log("getSelected");
        var result = [];
        for(var i=0;i<selectedIndexes.length;i++){
            result[i] = model.get(selectedIndexes[i]).guid;
            console.log(result[i]);
        }
        return result;
    }
    function getSelectedTagNamesString(){
        var result = "";
        for(var i=0;i<selectedIndexes.length;i++){
            result += model.get(selectedIndexes[i]).name + (i !== selectedIndexes.length-1 ?  "," : "")
        }
        return result;
    }
}
