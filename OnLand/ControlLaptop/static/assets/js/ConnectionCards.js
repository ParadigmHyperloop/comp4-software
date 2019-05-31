$( document ).ready(function() {
    conectionLoader = $("#connected-loader");
    noConnectionLoader = $("#no-connection-loader");
    telemetryLoader = $("#telemetry-loader");
    noTelemetryLoader = $("#no-telemetry-loader");
    telemetryCount = $("#telem-received-num");
});

function showNotification(message, status){
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

function toggleTelemetryIndicator(ping){
    if (ping === 1 && telemetryLoader.css('display') === "none") {
        telemetryLoader.css('display', 'block');
        noTelemetryLoader.css('display', 'none');
    }
    else if (telemetryLoader.css('display') === 'block' && ping === 0) {
        telemetryLoader.css('display', 'none');
        noTelemetryLoader.css('display', 'block');
    }
}

function toggleCommanderIndicator(status){
    if (status === 1 && conectionLoader.css('display') === "none") {
        conectionLoader.css('display', 'block');
        noConnectionLoader.css('display', 'none');
    } else if (conectionLoader.css('display') === 'block' && status === 0) {
        conectionLoader.css('display', 'none');
        noConnectionLoader.css('display', 'block');
    }
}

socket.on('connection_updates', function (update_json) {
    update = JSON.parse(update_json);
    connection = update['name'];
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
    count = telemetryCount.text();
    count++;
    telemetryCount.text(count);
});
