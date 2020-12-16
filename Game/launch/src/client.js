const childProcess = require('child_process');
const { exec } = require('child_process');

/**
 * Execute simple Shell command (async wrapper).
 * @param {String} cmd
 * @return {Object} { stdout: String, stderr: String }
 */
async function sh(cmd) {
    return new Promise(function (resolve, reject) {
        exec(cmd, (err, stdout, stderr) => {
            if (err) {
                reject(err);
            } else {
                resolve({ stdout, stderr });
            }
        });
    });
}

// var Shell = WScript.CreateObject("WScript.Shell");
// Shell.Run("command here");

async function main(command) {
    var messages = document.querySelector('ul');
    const li = document.createElement('li');
    // let { stdout } = await sh(command);
    li.textContent = command;
    messages.insertBefore(li, messages.firstElementChild);
    const spawn = childProcess.spawn(command, { shell: true });
    spawn.stdout.on('data', (data) => {
        console.log('STDOUT', data.toString());
        // messages.append($("<li>").text(command));
        for (let line of data.toString().split('\n')) {
            const li2 = document.createElement('li');
            li2.textContent = line;
            messages.insertBefore(li2, messages.firstElementChild);
            console.log(`${line}`);
        }
    });
    spawn.stderr.on('data', (data) => {
        console.log('STDERR', data.toString());
        for (let line of data.toString().split('\n')) {
            const li2 = document.createElement('li');
            li2.textContent = line;
            messages.insertBefore(li2, messages.firstElementChild);
            console.log(`${line}`);
        }
    });
    spawn.on('close', (code) => {
        console.log('CODE', code);
        for (let line of data.toString().split('\n')) {
            const li2 = document.createElement('li');
            li2.textContent = line;
            messages.insertBefore(li2, messages.firstElementChild);
            console.log(`${line}`);
        }
    });
}

async function kill() {
    let { stdout } = await sh('ps -a | grep client');
    console.log(stdout);
    let pros = stdout.split(' ');
    var i = 0;
    for (i = 0; i < pros.length; i++) {
        if (pros[i] != '') {
            break;
        }
    }
    console.log(pros[i]);
    main('kill -9' + ' ' + pros[i]);
    
}

function clean() {
    const messages = document.querySelector('ul');
    while (messages.firstChild) {
        messages.removeChild(messages.firstChild);
    }
}


window.addEventListener('DOMContentLoaded', onLoad);

function onLoad() {
    document.querySelector('#btnLoad').addEventListener('click', () => {
        main('cd ../client/build && cmake -DCMAKE_BUILD_TYPE=Release .. && make -j8');
    });
    document.querySelector('#btnSave').addEventListener('click', () => {
        main('cd ../client/build && ./client');
    });
    document.querySelector('#btnKill').addEventListener('click', () => {
        kill();
        
    });
    document.querySelector('#btnClean').addEventListener('click', () => {
        clean();
        
    });
};