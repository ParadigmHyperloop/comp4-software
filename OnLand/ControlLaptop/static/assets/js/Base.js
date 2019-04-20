socket = io();

socket.on('connect', () => {
    console.log("Connection: " + socket.connected);
});