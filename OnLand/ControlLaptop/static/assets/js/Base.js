socket = io('/telemetry-subscribers');
socket.on('connect', () => {
    console.log("Connection: " + socket.connected);
    socket.join('connection_updates');
});