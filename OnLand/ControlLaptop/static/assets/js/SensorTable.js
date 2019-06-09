let podState = '';
let sensor_ranges = false;
let tableSensors = [];

$( document ).ready(function() {
    socket.emit('join_room','telemetry_updates');
    $("#sensorTable").find("tr").each(function(){ tableSensors.push(this.id); });
    console.log(tableSensors);
});

function getSensorRanges() {
    $.get($SCRIPT_ROOT + '/sensor_ranges', function (data) {
        sensor_ranges = JSON.parse(data)
    });
}

function updateRowStatus(row,min,max,actual){
    if(actual > max || actual < min){
        row.addClass('danger');
        row.removeClass('success');
    }
    else{
        row.addClass('success');
        row.removeClass('danger');
    }
}

function ParsePodState(state) {
    let new_min;
    let new_max;
    let success;
    let sensor_name;
    if (!sensor_ranges) {
        getSensorRanges();
        console.log("No sensor Ranges yet")
        return
    }
    if (state !== podState) {
        podState = state;
        $(".sensor-row").each(function (index) {
             sensor_name = $(this).attr('id');
             success = true;
            try {
                new_min = sensor_ranges[sensor_name][state][0];
                new_max = sensor_ranges[sensor_name][state][1];
            }
            catch(error) {
              console.error("Error getting max and min values for " + sensor_name + "in state " + podState);
              success = false;
            }
            if(success){
                $('#' + sensor_name + '-min').text(new_min);
                $('#' + sensor_name + '-max').text(new_max);
            }
        });
    }
}


socket.on('pod_telemetry', function (data) {
    let value, sensor_cell, sensor_row, sensor_name, min, max;
    data = JSON.parse(data);

    const newPodState = data['podState'];
    ParsePodState(newPodState);

    const length = tableSensors.length;
    for (let index = 0; index < length; index++) {
        sensor_name = tableSensors[index];
        if(!data.hasOwnProperty(sensor_name)){
            continue;
        }
        value = data[sensor_name];
        sensor_cell = $("#" + sensor_name + "-actual");
        sensor_row =  $("#" + sensor_name);
        if(typeof(data[sensor_name]) == "boolean" || isNaN(data[sensor_name])){
            sensor_cell.text(data[sensor_name])
        }
        else{
            min = $("#" + sensor_name + "-min").text();
            max = $("#" + sensor_name + "-max").text();
            sensor_cell.text(value.toFixed(2));
            updateRowStatus(sensor_row,min,max,data[sensor_name]);
        }
    }
});






//Run on load
getSensorRanges();

