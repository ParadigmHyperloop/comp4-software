var socket = io();

socket.on('connect', () => {
console.log(socket.connected)
});

socket.on('pds packet', () => {
console.log("packet!")
});

