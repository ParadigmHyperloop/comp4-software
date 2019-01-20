//intermediary method used
function getStr(_class,_id) {
    return ('.'+_class+' .'+_id+'').toString();}


//pings Flask backend to update Sensor Overview table
function sensorRefresh() {
    $.getJSON("/_sensorsRefresh", function (data) {
        for (i = 0; i < data.length; i++) {
            // for (j=0;j<data[i].length;j++) {
            var sensor = data[i].last_id_field;
            if (sensor === $(getStr(sensor, "name")).text()) {
                $(getStr(sensor, "current")).html(data[i].last_val);
                $(getStr(sensor, "min")).html(data[i].min_val);
                $(getStr(sensor, "max")).html(data[i].max_val);
            }
        }
    });
   setTimeout(arguments.callee, 2500);}


//adds IDs and Classes to every row/col of the sensor table, to allow for differentiation and automatic
// refresh
function sensorNaming() {
    $("#sensorTab").find(">tbody > tr").each(function () {
        $(this).addClass($(this).text());

        var temp = $(this).children();
        temp[0].setAttribute('class','name');
        temp[1].setAttribute('class','status');
        temp[2].setAttribute('class','min');
        temp[3].setAttribute('class','current');
        temp[4].setAttribute('class','max');
    });}