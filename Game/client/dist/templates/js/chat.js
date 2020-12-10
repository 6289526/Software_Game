$(function () {

  var socket = null;
  
  var messages = $("#messages");

  $("#Forwardbox").submit(function () {
    var msgBox = $("#Forwardbox textarea");
    if (!msgBox.val()) return false;
    if (!socket) {
      alert("Error: There is no socket connection.");
      return false;
    }

    socket.send(msgBox.val());
    msgBox.val("F");
    return false;

  });
  $("#Leftbox").submit(function () {
    var msgBox = $("#Leftbox textarea");
    if (!msgBox.val()) return false;
    if (!socket) {
      alert("Error: There is no socket connection.");
      return false;
    }

    socket.send(msgBox.val());
    msgBox.val("L");
    return false;

  });
  $("#Rightbox").submit(function () {
    var msgBox = $("#Rightbox textarea");
    if (!msgBox.val()) return false;
    if (!socket) {
      alert("Error: There is no socket connection.");
      return false;
    }

    socket.send(msgBox.val());
    msgBox.val("R");
    return false;

  });
  $("#Putbox").submit(function () {
    var msgBox = $("#Putbox textarea");
    if (!msgBox.val()) return false;
    if (!socket) {
      alert("Error: There is no socket connection.");
      return false;
    }

    socket.send(msgBox.val());
    msgBox.val("P");
    return false;

  });

  if (!window["WebSocket"]) {
    alert("Error: Your browser does not support web sockets.")
  } else {
    socket = new WebSocket("ws://{{.Host}}/room");
    socket.onclose = function () {
      alert("Connection has been closed.");
    }
    socket.onmessage = function (e) {
      messages.append($("<li>").text(e.data));
    }
  }

});
