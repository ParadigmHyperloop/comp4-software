socket = io('/controls-interface');

socket.on('connect', () => {
    console.log("Connection: " + socket.connected);
});