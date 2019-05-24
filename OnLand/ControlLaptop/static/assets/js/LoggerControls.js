$(document).ready(function () {
    $('#logging-off-btn').click(function () {
        socket.emit('end_logging_session');
        console.log("Logging ended");
    });
    $('#logging-on-btn').click(function () {
        socket.emit('start_logging_session');
        console.log("Logging Started");
    });
});

socket.on ('logging_session_ended', function (data) {
    console.log(data);
    showDtsNotifications('Session Ended - FileName: '+ data, 'Success');
});

socket.on ('logging_session_started', function (data) {
    console.log(data);
    showDtsNotifications('Logging Session Started', 'Success');
});
