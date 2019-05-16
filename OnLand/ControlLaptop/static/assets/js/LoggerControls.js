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
