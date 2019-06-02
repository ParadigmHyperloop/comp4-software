socket = io();
socket.on('connect', () => {
    console.log("Connection: " + socket.connected);
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