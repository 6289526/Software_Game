const fs = remote.require('fs');
const { BrowserWindow, Menu, dialog } = remote;

let inputArea = null;
let inputTxt = null;
let footerArea = null;

let currentPath = '';
let editor = null;

var ArrayData = [
    [20, 20],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
];

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
        label: 'ヘルプ',
        submenu: [
            {
                label: 'Learn More',
                click: async () => {
                    const { shell, app } = require('electron')
                    await shell.openExternal('https://electronjs.org')
                }
            }
        ]
    }
]




window.addEventListener('DOMContentLoaded', onLoad);
window.addEventListener('click', setMenu);

function setMenu() {
    //メニューバー設置
    const menu = Menu.buildFromTemplate(template);
    Menu.setApplicationMenu(menu);
}

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

    var langTools = ace.require("ace/ext/language_tools");
    editor.setOptions({
        enableBasicAutocompletion: true,
        enableSnippets: false,
        enableLiveAutocompletion: true
    });
    // uses http://rhymebrain.com/api.html
    // var rhymeCompleter = {
    //     getCompletions: function (editor, session, pos, prefix, callback) {
    //         if (prefix.length === 0) { callback(null, []); return }
    //         $.getJSON(
    //             "http://rhymebrain.com/talk?function=getRhymes&word=" + prefix,
    //             function (wordList) {
    //                 // wordList like [{"word":"flow","freq":24,"score":300,"flags":"bc","syllables":"1"}]
    //                 callback(null, wordList.map(function (ea) {
    //                     return { name: ea.word, value: ea.word, score: ea.score, meta: "rhyme" }
    //                 }));
    //             })
    //     }
    // }
    // langTools.addCompleter(rhymeCompleter);
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

    // document.querySelector('#btnLoad').addEventListener('click', () => {
    //     openLoadFile();
    // });
    // document.querySelector('#btnSave').addEventListener('click', () => {
    //     saveFile();
    // });
};



function openLoadFile() {
    const win = BrowserWindow.getFocusedWindow();
    dialog.showOpenDialog(win, {
        properties: ['openFile'],
        filters: [
            {
                name: 'Documents',
                extensions: ['*']
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

        let cutText = text.toString().split('\n');
        console.log(cutText);
        for (var i = 0; i < ArrayData.length; i++) {
            console.log('cutText' + i + ':' + cutText[i]);
            let cutCutText = cutText[i].split(/\s+/);

            // console.log('length:' + ArrayData[i].length);
            for (let j = 0; j < ArrayData[i].length; j++) {
                console.log('cutText:' + cutCutText[j]);
                ArrayData[i][j] = Number(cutCutText[j]);
            }
        }
        console.log(ArrayData);
        editor.setValue(text.toString(), -1);
        setEditorTheme(path);
        // ファイルを読み込んだらレンダリング
        init();
        // メインプロセスにファイル名を送信
        // const aPath = path.split('/');
        // const aPath = currentPath.split('\\');
        // const name = aPath[aPath.length - 1].toLowerCase();
        //メインプロセスのipcMain.on('editor-send')に変数dataを送る
        // ipcRenderer.send('editor-send', name);
    });
}

function saveFile() {
    // 新しいファイルの時名前を付けて保存
    if (currentPath === '') {
        saveNewFile();
        return;
    }


    // const tmp = editor.getValue();
    // writeFile(currentPath, tmp);
    // let text = tmp.split('\n');
    // // ファイルに書き込み
    // var data = text[0] + '\n';
    // for (var i = 1; i < text.length; i++) {
    //     data += text[i] + '\n';
    // }
    // console.log(data);
    const data = editor.getValue();
    // var data = ArryData[0]+'\n';
    // data += ArryData[1]+'\n';
    writeFile(currentPath, data);

    // writeFile(currentPath, data);

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

            const tmp = editor.getValue();
            // writeFile(currentPath, tmp);
            // let text = tmp.toString().split('\n');
            // // ファイルに書き込み
            // var data = text[0] + '\n';
            // for (var i = 1; i < text.length; i++) {
            //     data += text[i] + '\n';
            // }
            // currentPath = result.filePath;
            const data = editor.getValue();
            // var data = ArryData[0]+'\n';
            // data += ArryData[1]+'\n';
            writeFile(currentPath, data);
            readFile(currentPath);
            // メインプロセスにファイル名を送信
            // const aPath = currentPath.split('/');
            // const aPath = currentPath.split('\\');
            // const name = aPath[aPath.length - 1].toLowerCase();
            // //メインプロセスのipcMain.on('editor-send')に変数dataを送る
            // ipcRenderer.send('editor-send', name);
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