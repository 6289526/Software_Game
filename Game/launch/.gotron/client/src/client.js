const ws = new WebSocket(
    "ws://localhost:" + global.backendPort + "/web/app/events"
);
console.log("Setup web socket:", ws);
var currentPath;
var nowPid;
const date1 = new Date();
var ID = -1*date1.getMilliseconds();
// This is being called for all incoming messages
ws.onmessage = (message) => {
    let obj = JSON.parse(message.data);

    // event name
    console.log(obj.event);
    console.log(obj.ID);
    console.log(obj.AtrNameInFrontend);
    switch (obj.event) {
        case "client-join":
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
                    console.log(div.scrollTop);
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

function onLoad() {
    
    document.querySelector('#btnMake').addEventListener('click', () => {
        ws.send(
            JSON.stringify({
                event: "client",
                ID: ID,
                AtrNameInFrontend: "cd ../client/build && cmake -DCMAKE_BUILD_TYPE=Release .. && make -j8",
            })
        );
    });
    document.querySelector('#btnRun').addEventListener('click', () => {
        ws.send(
            JSON.stringify({
                event: "client",
                ID: ID,
                AtrNameInFrontend: "cd ../client/build && ./client",
            })
        );
    });
    document.querySelector('#btnKill').addEventListener('click', () => {
        ws.send(
            JSON.stringify({
                event: "client",
                ID: ID,
                AtrNameInFrontend: "kill -9 $(ps -a | grep client | awk '{print $1}')",
            })
        );
    });
    document.querySelector('#btnClean').addEventListener('click', () => {
        clean();

    });
    
};