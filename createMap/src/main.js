const { app, BrowserWindow } = require('electron');
const path = require('path');
const { ipcMain } = require("electron");
// const remote = require('electron').remote;
const url = require('url');

var ev;
ipcMain.on('async-test', (event, message) => {
    // message には 呼び出し元からのデータ が入っている
    console.log(message)
    ev = event;
    // main から renderer プロセスを呼び出す
    return
})

let mainWindow;



function createWindow() {
    mainWindow = new BrowserWindow({
        width: 700,
        height: 650,
        webPreferences: {
            preload: `${__dirname}/preload.js`,    // preloadを追加
            enableRemoteModule: true,               // warning対策
            nodeIntegration: true,
            webviewTag: true
        },
        
        icon: `${__dirname}/my2.ico`
    });
    mainWindow.loadURL(url.format({
        pathname: path.join(__dirname, 'index.html'),
        protocol: 'file:',
        slashes: true,

    }));


    // 開発ツールを有効化
    // mainWindow.webContents.openDevTools();

    // //メニューバー設置
    // const menu = Menu.buildFromTemplate(template);
    // Menu.setApplicationMenu(menu);

    mainWindow.on('closed', () => {
        mainWindow = null;
    });


}

app.on('ready', createWindow);

app.on('window-all-closed', () => {
    if (process.platform !== 'darwin') {
        app.quit();
    }
});

app.on('activate', () => {
    if (mainWindow === null) {
        createWindow();
    }
});
