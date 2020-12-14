
window.addEventListener('DOMContentLoaded', onLoad);
function onLoad() {
  document.querySelector('#btnName').addEventListener('click', () => {
    // var socket = null;

    var msgBox = $("#namebox textarea");

    $("#namebox").submit(function () {

      if (!msgBox.val()) return false;
      if (!socket) {
        alert("Error: There is no socket connection.");
      }

      socket.send(msgBox.val());
      // msgBox.val("");
      $('#nameBox').fadeOut();
      $('#defaultCanvas0').fadeIn();
      console.log(deviceMode);
      deviceMode = "gameControl";

    });
  });

}
