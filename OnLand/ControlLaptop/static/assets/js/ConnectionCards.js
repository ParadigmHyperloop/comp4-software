$( document ).ready(function() {
    window.conectionLoader = $("#connected-loader");
    window.noConnectionLoader = $("#no-connection-loader");
    window.telemetryLoader = $("#telemetry-loader");
    window.noTelemetryLoader = $("#no-telemetry-loader");
    window.telemetryCount = $("#telem-received-num");

    socket.emit('join_room','connection_updates');
    socket.emit('join_room','telemetry_updates');
});





function toggleTelemetryIndicator(ping){
    if (ping == 1 && telemetryLoader.css('display') === "none") {
        telemetryLoader.css('display', 'block');
        noTelemetryLoader.css('display', 'none');
    }
    else if (telemetryLoader.css('display') === 'block' && ping == 0) {
        telemetryLoader.css('display', 'none');
        noTelemetryLoader.css('display', 'block');
    }
}

function toggleCommanderIndicator(status){
    if (status == 1 && conectionLoader.css('display') === "none") {
        conectionLoader.css('display', 'block');
        noConnectionLoader.css('display', 'none');
    } else if (conectionLoader.css('display') === 'block' && status == 0) {
        conectionLoader.css('display', 'none');
        noConnectionLoader.css('display', 'block');
    }
}

socket.on('connection_update', function (update_json) {
    const update = JSON.parse(update_json);
    const connection = update['name'];
    status = update['status'];

    if(connection === 'commander'){
        toggleCommanderIndicator(status);
    }
    else if(connection === 'telemetry'){
        toggleTelemetryIndicator(status);
    }
});

socket.on('pod_telemetry', function (data) {
    toggleTelemetryIndicator(1);
    let count = telemetryCount.text();
    count++;
    telemetryCount.text(count);
});
