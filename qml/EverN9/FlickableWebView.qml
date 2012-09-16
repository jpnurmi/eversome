/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtDeclarative module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

/*--------------------------------------------------------
* Module Name : Helium Mobile Browser
* Version : 1.0
*
* Software Name : Helium Mobile Browser
* Version : 1.0
*
* Copyright (c) 2010 - 2011 France Telecom
* This software is distributed under the LGPL v2.1 license,
* the text of which is available at http://www.gnu.org/licenses/lgpl-2.1.html
* or see the "LICENSE.txt" file for more details.
*
*--------------------------------------------------------
* File Name   : FlickableWebView.qml
*
* Created     : 15 June 2011
*
*--------------------------------------------------------
* 1.0 - First open source release
*/

import QtQuick 1.1
import QtWebKit 1.1

Flickable {
    id: flickable

    property alias webView: webView

    width: parent.width
    contentWidth: Math.max(parent.width, webView.width)
    contentHeight: Math.max(parent.height, webView.height)

    onWidthChanged: {
        // Expand (but not above 1:1) if otherwise would be smaller that available width.
        if (width > webView.width*webView.contentsScale && webView.contentsScale < 1.0)
            webView.contentsScale = width / webView.width * webView.contentsScale;
    }

    pressDelay: 200
    onFocusChanged: { if (focus) webView.focus = true; } // Force focus on "webView" when received

    onMovementStarted: webView.renderingEnabled = false
    onMovementEnded: webView.renderingEnabled = true

    PinchArea {
        id: pinchArea
        anchors.fill: parent

        onPinchStarted: {
            webView.renderingEnabled = false;
        }

        onPinchUpdated: {
            webView.doPinchZoom(pinch.scale / pinch.previousScale, pinch.center, pinch.previousCenter)
        }

        onPinchFinished: {
            if (contentX < 0 || contentY < 0) {
                var sc = webView.contentsScale;
                if (webView.contentsScale * webView.width < flickable.width)
                    sc = flickable.width / (webView.width / webView.contentsScale);
                var vx = Math.max(0, contentX) + flickable.width / 2;
                var vy = Math.max(0, contentY) + flickable.height / 2;
                // doZoom will reset renderingEnabled to true
                webView.doZoom(sc, vx, vy);
            } else {
                webView.renderingEnabled = true;
            }
        }

        WebView {
            id: webView
            transformOrigin: Item.TopLeft

            function doZoom(zoom, centerX, centerY)
            {
                if (centerX) {
                    var sc = zoom / contentsScale;
                    scaleAnim.to = zoom;
                    flickVX.from = flickable.contentX;
                    flickVX.to = Math.max(0, Math.min(centerX-flickable.width/2,webView.width*sc-flickable.width))
                    finalX.value = flickVX.to;
                    flickVY.from = flickable.contentY;
                    flickVY.to = Math.max(0, Math.min(centerY-flickable.height/2,webView.height*sc-flickable.height))
                    finalY.value = flickVY.to;
                    quickZoom.start();
                }
            }

            function doPinchZoom(zoom, center, previousCenter)
            {
                var sc = zoom * contentsScale;
                if (sc <= 10) {
                    // calculate contentX and contentY so webview moves along with the pinch
                    flickable.contentX = (center.x * zoom) - (center.x - flickable.contentX) + (previousCenter.x - center.x);
                    flickable.contentY= (center.y * zoom) - (center.y - flickable.contentY) + (previousCenter.y - center.y);
                    contentsScale = sc;
                }
            }

            smooth: false // We don't want smooth scaling, since we only scale during (fast) transitions
            focus: true

            preferredWidth: flickable.width
            preferredHeight: flickable.height

            Keys.onLeftPressed: webView.contentsScale -= 0.1
            Keys.onRightPressed: webView.contentsScale += 0.1
            onAlert: console.log(message)
            onContentsSizeChanged: webView.contentsScale = Math.min(1, flickable.width / contentsSize.width)

            onUrlChanged: {
                // Reset Content to the TopLeft Corner
                flickable.contentX = 0;
                flickable.contentY = 0;
                webView.focus = true;
            }

            onDoubleClick: {
                var zf = 2.0;
                // if zoomed go back to screen width
                if (webView.contentsScale * webView.width > flickable.width) {
                    zf = flickable.width / (webView.width / webView.contentsScale);
                    doZoom(zf, zf * clickX, zf * clickY);
                    // try to do heuristic zoom with maximum 2.5x
                } else if (!heuristicZoom(clickX, clickY, 2.5)) {
                    // if no heuristic zoom found, zoom to 2.0x
                    doZoom(zf, zf * clickX, zf * clickY);
                }
            }

            onZoomTo: doZoom(zoom,centerX,centerY)

            SequentialAnimation {
                id: quickZoom

                PropertyAction {
                    target: webView
                    property: "renderingEnabled"
                    value: false
                }
                ParallelAnimation {
                    NumberAnimation {
                        id: scaleAnim
                        target: webView; property: "contentsScale"
                        // the to property is set before calling
                        duration: 200
                    }
                    NumberAnimation {
                        id: flickVX
                        target: flickable
                        property: "contentX"
                        duration: 200
                        from: 0 // set before calling
                        to: 0 // set before calling
                    }
                    NumberAnimation {
                        id: flickVY
                        target: flickable
                        property: "contentY";
                        duration: 200
                        from: 0 // set before calling
                        to: 0 // set before calling
                    }
                }
                // Have to set the contentXY, since the above 2
                // size changes may have started a correction if
                // contentsScale < 1.0.
                PropertyAction {
                    id: finalX
                    target: flickable
                    property: "contentX"
                    value: 0 // set before calling
                }
                PropertyAction {
                    id: finalY
                    target: flickable
                    property: "contentY"
                    value: 0 // set before calling
                }
                PropertyAction {
                    target: webView
                    property: "renderingEnabled"
                    value: true
                }
            }
        }
    }
}
