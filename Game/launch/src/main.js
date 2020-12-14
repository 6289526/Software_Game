const { app, Menu, BrowserWindow } = require('electron');
const path = require('path');
const url = require('url');

let mainWindow;
//メニューバー内容
const template = [
    // { role: 'fileMenu' }
    {
        label: 'ファイル',
        submenu: [{ role: 'quit' }],
    },
    // { role: 'viewMenu' }
    {
        label: '表示',
        submenu: [
            { role: 'reload' },
            { role: 'forcereload' },
            { role: 'toggledevtools' },
            { type: 'separator' },
            { role: 'resetzoom' },
            { role: 'zoomin' },
            { role: 'zoomout' },
            { type: 'separator' },
            { role: 'togglefullscreen' },
        ],
    },
    // { role: 'windowMenu' }
    {
        label: 'ウィンドウ',
        submenu: [{ role: 'minimize' }],
    },
    {
        label: 'GitHub',
        submenu: [
            {
                label: 'our repository',
                click: async () => {
                    const { shell, app } = require('electron');
                    await shell.openExternal('https://github.com/6289526/Software_Game');
                },
            },
        ],
    },
    {
        label: 'ヘルプ',
        submenu: [
            {
                label: 'Learn More',
                click: async () => {
                    const { shell, app } = require('electron');
                    await shell.openExternal('https://electronjs.org');
                },
            },
        ],
    },
];


function createWindow() {
    mainWindow = new BrowserWindow({
        width: 1200,
        height: 600,
        webPreferences: {
            enableRemoteModule: true,
            nodeIntegration: true,
            webviewTag: true,
            preload: path.join(`${__dirname}/preload.js`), // <- 追加
        },
        icon: `${__dirname}/editor/my2.ico`
    });

    mainWindow.loadURL(url.format({
        pathname: path.join(__dirname, 'index.html'),
        protocol: 'file:',
        slashes: true
    }));

    // 開発ツールを有効化
    // mainWindow.webContents.openDevTools();

    //メニューバー設置
    const menu = Menu.buildFromTemplate(template);
    Menu.setApplicationMenu(menu);

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