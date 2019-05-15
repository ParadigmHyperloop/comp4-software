/*
BUTTON CLICKS
We assign button clicks once the page finishes loading so
we can assure the jquery doesnt search before the html is
done loading.
 */
$(document).ready(function () {
    $('#menu-toggle').click(function (e) {
        // e.preventDefault();
        $("#wrapper").toggleClass("toggled");
    });
    $('.brake-btn').click(function () {
        console.log("Asdf");
        let status = '0';
        btnId = $(this).attr("id");
        console.log(btnId);
        if (btnId === "on-btn") {
            status = '1';
            socket.emit('start_logging_session');
        } else if (btnId = 'off-btn')
            socket.emit('end_logging_session');
        console.log(status);
        socket.emit('command', status)
    });
    $('.loader-inner').loaders()
});

