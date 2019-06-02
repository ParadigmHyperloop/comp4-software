$(document).ready(function () {

    $('#logging-off-btn').click(function () {
        socket.emit('logger_control', JSON.stringify({'command':'stop'}));
    });

    $('#logging-on-btn').click(function () {
        socket.emit('logger_control', JSON.stringify({'command':'start'}));
    });

    socket.emit('join_room','logger_feedback_updates');

});

socket.on ('logger_feedback', function (data) {
    feedback = JSON.parse(data);
    status = feedback['feedback'];
    console.log(status);
    if(status === 'started'){
        showNotification('Logging Session Started', 'Success');
    }
    else if(status === 'stopped'){
        filename = feedback['filename'];
        showNotification('Session Ended - FileName: '+ filename, 'Success');
    }
});

