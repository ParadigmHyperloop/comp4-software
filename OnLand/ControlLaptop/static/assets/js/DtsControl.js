socket = io();

socket.on('connect', () => {
    console.log("Connection: " + socket.connected);
});

socket.on('pds packet', () => {
    console.log("packet!");
});

socket.on('ping', function (ping) {
    console.log(ping);

    conectionLoader = $("#connected-loader");
    noConnectionLoader = $("#no-connection-loader");
    if (ping == 1 && conectionLoader.css('display') == "none") {
        console.log("here");
        conectionLoader.css('display', 'block');
        noConnectionLoader.css('display', 'none');
    } else if (conectionLoader.css('display') == 'block' && ping == 0) {
        conectionLoader.css('display', 'none');
        noConnectionLoader.css('display', 'block');
    }
});



/*
BUTTON CLICKS
We assign button clicks once the page finishes loading so
we can assure the jquery doesnt search before the html is
done loading.
 */
$(document).ready(function () {
    $('#menu-toggle').click(function (e) {
        e.preventDefault();
        $("#wrapper").toggleClass("toggled");
    });


    $('.brake-btn').click(function () {
        console.log("Asdf");
        status = '0';
        btnId = $(this).attr("id");
        console.log(btnId);
        if (btnId === "on-btn") {
            status = '1'
        }
        console.log(status);
        socket.emit('command', status)
    });

    $('.loader-inner').loaders()
});

