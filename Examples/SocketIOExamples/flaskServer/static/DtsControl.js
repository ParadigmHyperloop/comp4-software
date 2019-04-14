var socket = io();

socket.on('connect', () => {
console.log(socket.connected);
});

socket.on('pds packet', () => {
console.log("packet!");
});


$("#menu-toggle").click(function(e) {
  e.preventDefault();
  $("#wrapper").toggleClass("toggled");
});

$(".brake-btn").click(function () {
  status = 0;
  btnId = $(this).attr("id");

  if( btnId === "btn-on"){
    console.log("allo")
  }




});