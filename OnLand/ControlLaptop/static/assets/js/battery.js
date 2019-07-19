$( document ).ready(function() {
    socket.emit('join_room', 'telemetry_updates');
});


socket.on('pod_telemetry', function (data) {
    let value, value_cell, value_name, current_state;
    data = JSON.parse(data);

    cells = data['hvBatteryCellVoltages']

    for (let id = 0; id <= 191; id++) {
        temp = cells[id]
        document.getElementById(id).innerHTML = temp

        if (temp > 175) {
            $(`.${id}`).addClass("danger")
            $(`.${id}`).removeClass("success")
        }
        else {
            $(`.${id}`).addClass("success")
            $(`.${id}`).removeClass("danger")
        }
    }
});   