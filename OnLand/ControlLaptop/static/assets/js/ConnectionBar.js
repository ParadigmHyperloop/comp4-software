$( document ).ready(function() {
    conectionLoader = $("#connected-loader");
    noConnectionLoader = $("#no-connection-loader");
    telemetryLoader = $("#telemetry-loader");
    noTelemetryLoader = $("#no-telemetry-loader");
    telemetryCount = $("#telem-received-num");
});

function showDtsNotifications(message, status){
    debugger;
	$.notify({
		icon: "pe-7s-next-2",
		message: `<b>${message}</b>`

	},{
	  type: status,
		timer: 3500,
		placement: {
			from: 'top',
			align: 'right'
		}
	});
}

function toggleTelemetryLoader(ping){
    if (ping == 1 && telemetryLoader.css('display') == "none") {
        telemetryLoader.css('display', 'block');
        noTelemetryLoader.css('display', 'none');
    }
    else if (telemetryLoader.css('display') == 'block' && ping == 0) {
        telemetryLoader.css('display', 'none');
        noTelemetryLoader.css('display', 'block');
    }
}

socket.on('telemetry_connection', function (ping) {
    toggleTelemetryLoader(ping);
});

socket.on('ping', function (ping) {
    if (ping == 1 && conectionLoader.css('display') == "none") {
        conectionLoader.css('display', 'block');
        noConnectionLoader.css('display', 'none');
    } else if (conectionLoader.css('display') == 'block' && ping == 0) {
        conectionLoader.css('display', 'none');
        noConnectionLoader.css('display', 'block');
    }
});

socket.on('telemetry', function (data) {
    toggleTelemetryLoader(1);
    count = telemetryCount.text();
    count++;
    telemetryCount.text(count);
});

socket.on ('logging_session_ended', function (data) {
    console.log(data);
    showDtsNotifications('Session Ended - FileName: '+ data, 'Success');
});

socket.on ('logging_session_started', function (data) {
    debugger;
    console.log(data);
    showDtsNotifications('Logging Session Started', 'Success');
});
