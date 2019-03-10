function sendCommand(command) {
  $.ajax({
    type: "POST",
    url: '/send_command',
    dataType: 'json',
    contentType: 'application/json',
    data: JSON.stringify({
      command: command
    }),
    success: function HANDLE_COMMAND_SEND_SUCCESS (data) {
    },
  })
}

function registerDisconnectFromPodClick() {
  $('#disconnect').on('click', function sendESTOPCOMMAND() {
    $.ajax({
      type: "POST",
      url: '/disconnect_from_pod',
      dataType: 'json',
      contentType: 'application/json',
      data: JSON.stringify({
        command: 'DISCONNECT'
      }),
      success: function HANDLE_DISCONNECT_SUCCESS (data) {
      },
    })
  });
}

function registerESTOPCommandClick()
{
  $('#e-stop').on('click', function sendESTOPCOMMAND() {
    sendCommand('ESTOP');
  });
}

$(document).ready(function() {
  registerESTOPCommandClick();
  registerDisconnectFromPodClick();
});