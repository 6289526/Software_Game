const ws = new WebSocket(
    "ws://localhost:" + global.backendPort + "/web/app/events"
);
console.log("Setup web socket:", ws);
var currentPath;
var nowPid;
var ID = -1;
// JSON形式で飛んでくる
// event:   多分string
// ID:  int
// ArtNameInFrontend:   string
ws.onmessage = (message) => {
    let obj = JSON.parse(message.data);

    console.log(obj.event);
    console.log(obj.ID);
    console.log(obj.AtrNameInFrontend);

    switch (obj.event) {
        case "server-join":
            console.log("join!");
            ID = obj.ID;
            break;
        case "pid":
            if (ID === obj.ID) {
                nowPid = obj.AtrNameInFrontend;
                console.log("nowPid:" + nowPid);
            }
            break;
        case "comdone":
            if (ID === obj.ID) {
                currentPath = obj.AtrNameInFront;
                var messages = document.querySelector('ul');
                const li2 = document.createElement('li');
                li2.textContent = '$';
                messages.appendChild(li2);
                // console.log(`${line}`);
                const div = document.getElementsByClassName("mainWindow");
                div.scrollTop = div.scrollHeight;
                const mainWindow = document.getElementById("mainWindow");
                mainWindow.scrollTop = mainWindow.scrollHeight;
            }
            break;
        case "stdout":
            if (ID === obj.ID) {
                console.log(obj.AtrNameInFrontend);
                var messages = document.querySelector('ul');
                for (let line of obj.AtrNameInFrontend.split('\n')) {
                    const li2 = document.createElement('li');
                    li2.textContent = line;
                    messages.appendChild(li2);
                    // console.log(`${line}`);
                    const div = document.getElementsByClassName("mainWindow");
                    div.scrollTop = div.scrollHeight;
                    const mainWindow = document.getElementById("mainWindow");
                    mainWindow.scrollTop = mainWindow.scrollHeight;
                }
            }
            break;
        case "stderr":
            if (ID === obj.ID) {
                console.log(obj.AtrNameInFrontend);
                var messages = document.querySelector('ul');
                for (let line of obj.AtrNameInFrontend.split('\n')) {
                    const li2 = document.createElement('li');
                    li2.textContent = line;
                    messages.appendChild(li2);
                    // console.log(`${line}`);
                    const div = document.getElementsByClassName("mainWindow");
                    div.scrollTop = div.scrollHeight;
                    const mainWindow = document.getElementById("mainWindow");
                    mainWindow.scrollTop = mainWindow.scrollHeight;
                }
            }
            break;
    }
};


function clean() {
    const messages = document.querySelector('ul');
    while (messages.firstChild) {
        messages.removeChild(messages.firstChild);
    }
}



window.addEventListener('DOMContentLoaded', onLoad);

var client_num = 1;
function onLoad() {
    document.querySelector('#btnMake').addEventListener('click', () => {
        ws.send(
            JSON.stringify({
                event: "server",
                ID: ID,
                AtrNameInFrontend: 'cd ../server/build && cmake -DCMAKE_BUILD_TYPE=Release .. && make -j8',
            })
        );
    });
    document.querySelector('#btnRun').addEventListener('click', () => {
        ws.send(
            JSON.stringify({
                event: "server",
                ID: ID,
                AtrNameInFrontend: 'cd ../server/build && ./server' + ' ' + client_num,
            })
        );
    });
    document.querySelector('#btnAdd').addEventListener('click', () => {
        client_num++;
        var messages = document.querySelector('ul');
        const li = document.createElement('li');
        // let { stdout } = await sh(command);
        li.textContent = 'Now NumClient:' + client_num;
        messages.insertBefore(li, messages.firstElementChild);

    });
    document.querySelector('#btnReduce').addEventListener('click', () => {
        client_num--;
        var messages = document.querySelector('ul');
        const li = document.createElement('li');
        // let { stdout } = await sh(command);
        li.textContent = 'Now NumClient:' + client_num;
        messages.insertBefore(li, messages.firstElementChild);

    });
    document.querySelector('#btnKill').addEventListener('click', () => {
        ws.send(
            JSON.stringify({
                event: "server",
                ID: ID,
                AtrNameInFrontend: "kill -9 $(ps -a | grep server | awk '{print $1}')",
            })
        );

    });

    document.querySelector('#btnClean').addEventListener('click', () => {
        clean();

    });
};