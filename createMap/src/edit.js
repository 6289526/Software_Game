const fs = remote.require('fs');
const { BrowserWindow, Menu, dialog } = remote;

let inputArea = null;
let inputTxt = null;
let footerArea = null;

let currentPath = '';
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




window.addEventListener('DOMContentLoaded', onLoad);


function onLoad() {

    //メニューバー設置
    const menu = Menu.buildFromTemplate(template);
    Menu.setApplicationMenu(menu);
    inputArea = document.getElementById('input_area');
    inputTxt = document.getElementById('input_txt');
    footerArea = document.getElementById('footer_fixed');

    editor = ace.edit('input_txt');
    // Ace.jsのカラーテーマ
    editor.setTheme('ace/theme/dracula');
    editor.focus();
    editor.gotoLine(1);
    editor.renderer.setShowPrintMargin(false);
    setEditorTheme();

    document.addEventListener('dragover', (event) => {
        event.preventDefault();
    });
    document.addEventListener('drop', (event) => {
        event.preventDefault();
    });

    inputArea.addEventListener('dragover', (event) => {
        event.preventDefault();
    });
    inputArea.addEventListener('dragleave', (event) => {
        event.preventDefault();
    });
    inputArea.addEventListener('dragend', (event) => {
        event.preventDefault();
    });
    inputArea.addEventListener('drop', (event) => {
        event.preventDefault();
        const file = event.dataTransfer.files[0];
        readFile(file.path);
    });
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
        for (var i = 0; i < ArrayData[0][0]; i++) {
            // console.log('cutText' + i + ':' + cutText[i]);
            // つぎに空白で区切る
            let cutCutText = cutText[i].split(/\s+/);
            // console.log('length:' + ArrayData[i].length);
            for (let j = 0; j < ArrayData[i].length; j++) {
                // console.log('cutText:' + cutCutText[j]);
                // 数値に変換して代入
                ArrayData[i][j] = Number(cutCutText[j]);
            }
        }
        // console.log(ArrayData);

        // エディターに表示
        editor.setValue(text.toString(), -1);
        setEditorTheme(path);
        
        // ファイルを読み込んだら再レンダリング
        setMap();
    });
}

function saveFile() {
    // 新しいファイルの時名前を付けて保存
    if (currentPath === '') {
        saveNewFile();
        return;
    }
    // ファイルに書き込み
    const data = editor.getValue();
    writeFile(currentPath, data);
    readFile(currentPath);
    // const win = BrowserWindow.getFocusedWindow();
    // dialog.showMessageBox(win, {
    //     title: 'ファイルの上書き保存を行います。',
    //     type: 'info',
    //     buttons: ['OK', 'Cancel'],
    //     detail: '本当に保存しますか？'
    // }).then(result => {
    //     if (result.response === 0) {
    //         const data = editor.getValue();
    //         writeFile(currentPath, data);
    //     }
    // }).catch(err => {
    //     console.log(err)
    // });
}

function writeFile(path, data) {
    fs.writeFile(path, data, (error) => {
        if (error != null) {
            alert('error : ' + error);
        } else {
            setEditorTheme(path);
        }
    });
}

function saveNewFile() {
    const win = BrowserWindow.getFocusedWindow();
    dialog.showSaveDialog(win, {
        properties: ['openFile'],
        filters: [
            {
                name: 'Documents',
                extensions: ['*']
            }
        ]
    }).then(result => {
        if (!result.canceled) {
            // ファイルに書き込み
            const data = editor.getValue();
            writeFile(currentPath, data);
            readFile(currentPath);
            
        }
    }).catch(err => {
        console.log(err)
    });
}

// 構文チェック機能設定.ファイルの読み込み時などに拡張子からターゲットの言語を変更
function setEditorTheme(fileName = '') {
    const type = fileName.split('.');
    const ext = type[type.length - 1].toLowerCase()

    switch (ext) {
        case 'txt':
            editor.getSession().setMode('ace/mode/plain_text');
            break;
        case 'py':
            editor.getSession().setMode('ace/mode/python');
            break;
        case 'rb':
            editor.getSession().setMode('ace/mode/ruby');
            break;
        case 'c':
        case 'cpp':
        case 'h':
            editor.getSession().setMode('ace/mode/c_cpp');
            break
        case 'html':
            editor.getSession().setMode('ace/mode/html');
            break;
        case 'js':
            editor.getSession().setMode('ace/mode/javascript');
            break;
        case 'md':
            editor.getSession().setMode('ace/mode/markdown');
            break;
        default:
            editor.getSession().setMode('ace/mode/plain_text');
            break;
    }

}