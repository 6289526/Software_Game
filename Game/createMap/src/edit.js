const fs = remote.require('fs');
const { BrowserWindow, Menu, dialog } = remote;

let inputArea = null;
let inputTxt = null;
let footerArea = null;

let currentPath = '/home/moko/school/Software_Game/Game/createMap/src/mapdata.csv';
let editor = null;


//メニューバー内容
const template = [
    // { role: 'fileMenu' }
    {
        label: 'ファイル',
        submenu: [
            {
                label: 'ファイルを開く',
                accelerator: 'Ctrl+O',
                click: function () {
                    openLoadFile();
                }
            },
            {
                label: '保存',
                accelerator: 'Ctrl+S',
                click: function () {
                    saveFile();
                }
            },
            {
                label: '名前を付けて保存',
                accelerator: 'Ctrl+Shift+S',
                click: function () {
                    saveNewFile();
                }
            },
            // { 
            //     label: '自動保存',
            //     type: 'checkbox', 
            //     checked: function () {
            //         saveFile();
            //     }
            // }
            ,
            { role: 'quit' }
        ]
    },
    // { role: 'editMenu' }
    {
        label: '編集',
        submenu: [
            // { 
            //     label: 'undo',
            //     accelerator: 'Ctrl+Z',
            //     click: function(){
            //         editor.session.mergeUndoDeltas = true; 
            //         editor.session.insert({row: 0, column:0}, `${currentPath}`);
            //     } 
            // },
            {
                label: 'undo',
                accelerator: 'Ctrl+Z',
                click: function () {
                    editor.undo();
                }
            },
            {
                label: 'redo',
                accelerator: 'Ctrl+Y',
                click: function () {
                    editor.redo();
                }
            },
            { type: 'separator' },
            { role: 'cut' },
            { role: 'copy' },
            { role: 'paste' },
            { role: 'delete' },
            { type: 'separator' },
            { role: 'selectAll' }
        ]
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
            { role: 'togglefullscreen' }
        ]
    },
    // { role: 'windowMenu' }
    {
        label: 'ウィンドウ',
        submenu: [
            { role: 'minimize' },
            { role: 'zoom' }
        ]
    },
    {
        label: 'GitHub',
        submenu: [
            {
                label: 'our repository',
                click: async () => {
                    const { shell, app } = require('electron')
                    await shell.openExternal('https://github.com/6289526/Software_Game')
                }
            }
        ]
    },
    {
        label: 'Twitter',
        submenu: [
            {
                label: 'my account',
                click: async () => {
                    const { shell, app } = require('electron')
                    await shell.openExternal('https://twitter.com/KenjiroHayamiti')
                }
            }
        ]
    }
]




window.addEventListener('load', onLoad);


function onLoad() {
    //メニューバー設置
    const menu = Menu.buildFromTemplate(template);
    Menu.setApplicationMenu(menu);
    // inputArea = document.getElementById('input_area');
    // inputTxt = document.getElementById('input_txt');
    footerArea = document.getElementById('footer_fixed');
    
};



function openLoadFile() {
    const win = BrowserWindow.getFocusedWindow();
    dialog.showOpenDialog(win, {
        properties: ['openFile'],
        filters: [
            {
                name: 'Documents',
                extensions: ['csv']
            }
        ]
    }).then(result => {
        if (!result.canceled) {
            readFile(result.filePaths[0]);
        }
    }).catch(err => {
        console.log(err)
    });


}

function readFile(path) {
    currentPath = path;
    fs.readFile(path, (error, text) => {
        if (error != null) {
            alert('error : ' + error);
            return;
        }
        footerArea.innerHTML = path;

        // 取得したファイル内の文字を配列に突っ込む
        // まず、行ごとに区切る
        let cutText = text.toString().split('\n');
        // console.log(cutText);

        // 一行を区切る
        let cutCutText = cutText[0].split(/\s+/);
        // エディター上での縦幅
        ArrayData[0][0] = Number(cutCutText[0]);
        // エディター上での横幅
        ArrayData[0][1] = Number(cutCutText[1]);

        // マップデータを転写
        for (var i = 1; i < ArrayData[0][1]+1; i++) {
            // console.log('cutText' + i + ':' + cutText[i]);
            // つぎに空白で区切る
            cutCutText = cutText[i].split(/\s+/);
            
            if (ArrayData.length > i) {// 配列の要素数が足りている場合
                // console.log('length:' + ArrayData[i].length);
                for (let j = 0; j < ArrayData[0][0]; j++) {
                    // console.log('cutText:' + cutCutText[j]);
                    // 数値に変換して代入
                    if (Number.isNaN(cutCutText[j])) {
                        ArrayData[i][j] = -1;
                    } else if (isNaN(cutCutText[j])) {
                        ArrayData[i][j] = -1;
                    } else {
                        ArrayData[i][j] = Number(cutCutText[j]);
                    }
                }
            }
            else {//足りない場合
                let subArray = [];
                // console.log('length:' + ArrayData[i].length);
                for (let j = 0; j < ArrayData[0][1]; j++) {
                    if (Number.isNaN(cutCutText[j])) {
                        subArray[j] = -1;
                    } else if (isNaN(cutCutText[j])) {
                        subArray[j] = -1;
                    } else {
                        subArray[j] = Number(cutCutText[j]);
                    }
                }
                ArrayData.push(subArray);
            }
        }

        console.log(ArrayData);

        // ファイルを読み込んだら再レンダリング
        setMap();
    });
}
