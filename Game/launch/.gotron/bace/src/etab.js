//const TabGroup = require('electron-tabs') normally but for demo :
const TabGroup = require('electron-tabs');

let tabGroup = new TabGroup({
  newTab: {
    title: 'client',
    src: '../../client/src/index.html',
    webviewAttributes: {
      enableRemoteModule: true,
      nodeIntegration: true,
      webviewTag: true,
      preload: './preload.js',
    },
    visible: true,
    active: true,
  },
});
tabGroup.addTab({
  title: 'server',
  src: '../../server/src/index.html',
  webviewAttributes: {
    enableRemoteModule: true,
    nodeIntegration: true,
    webviewTag: true,
    preload: './preload.js',
  },
  visible: true,
  active: true,
});
tabGroup.addTab({
  title: 'client',
  src: '../../client/src/index.html',
  webviewAttributes: {
    enableRemoteModule: true,
    nodeIntegration: true,
    webviewTag: true,
    preload: './preload.js',
  },
  visible: true,
  active: true,
});