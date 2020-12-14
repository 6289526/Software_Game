// メインプロセスの起点
import { app, BrowserWindow, Menu } from 'electron';
import path from 'path';
const { ipcMain } = require('electron');
// const remote = require('electron').remote;
const url = require('url');
// セキュアな Electron の構成
// 参考: https://qiita.com/pochman/items/64b34e9827866664d436

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

const createWindow = (): void => {
  // レンダープロセスとなる、ウィンドウオブジェクトを作成する。
  const win = new BrowserWindow({
    width: 1200,
    height: 600,
    webPreferences: {
      enableRemoteModule: true,
      nodeIntegration: true,
      webviewTag: true,
      preload: path.join(`${__dirname}/preload.js`), // <- 追加
    },
    icon: `${__dirname}/editor/my2.ico`,
  });

  // 読み込む index.html。
  // tsc でコンパイルするので、出力先の dist の相対パスで指定する。
  win.loadURL(
    url.format({
      pathname: path.join(__dirname, '/index.html'),
      protocol: 'file:',
      slashes: true,
    }),
  );
  // 開発者ツールを起動する
  // win.webContents.openDevTools();

  //メニューバー設置
  const menu = Menu.buildFromTemplate(template);
  Menu.setApplicationMenu(menu);
};

// Electronの起動準備が終わったら、ウィンドウを作成する。
app.whenReady().then(createWindow);

// すべての ウィンドウ が閉じたときの処理
app.on('window-all-closed', () => {
  // macOS 以外では、メインプロセスを停止する
  // macOS では、ウインドウが閉じてもメインプロセスは停止せず
  // ドックから再度ウインドウが表示されるようにする。
  if (process.platform !== 'darwin') {
    app.quit();
  }
});

app.on('activate', () => {
  // macOS では、ウインドウが閉じてもメインプロセスは停止せず
  // ドックから再度ウインドウが表示されるようにする。
  if (BrowserWindow.getAllWindows().length === 0) {
    createWindow();
  }
});
