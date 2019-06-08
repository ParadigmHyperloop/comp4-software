socket = io();
socket.on('connect', () => {
    console.log("Connection: " + socket.connected);
    socket.emit('join_room','notification_updates');
});



socket.on ('frontend_notification', function (data) {
    const notification = JSON.parse(data);
    const message = notification['message'];
    const status = notification['status'];
    showNotification(message, status);
});

showNotification('Logging Session Started', 'Success');

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