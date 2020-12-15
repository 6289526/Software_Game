const TabGroup = require("electron-tabs");

let tabGroup = new TabGroup({
    title: "New Tab",
    src: "./client.html",
    webviewAttributes: {
        enableRemoteModule: true,
            nodeIntegration: true,
            webviewTag: true,
        preload: "./preload.js"
    },
    visible: true,
    active: true
});

tabGroup.addTab({
    title: "server",
    src: "./server.html",
    webviewAttributes: {
        enableRemoteModule: true,
            nodeIntegration: true,
            webviewTag: true,
        preload: "./preload.js"
    },
    visible: true,
    active: true
});

tabGroup.addTab({
    title: "client",
    src: "./client.html",
    webviewAttributes: {
        enableRemoteModule: true,
            nodeIntegration: true,
            webviewTag: true,
        preload: "./preload.js"
    },
    visible: true,
    active: true
});

