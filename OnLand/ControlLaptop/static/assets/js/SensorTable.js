
let sensor_ranges = false;
let table_sensors = [];

$( document ).ready(function() {
    socket.emit('join_room','telemetry_updates');
    $("#sensorTable").find("tr").each(function(){ table_sensors.push(this.id); });
    getSensorRanges();
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
    if (state !== pod_state) {
        pod_state = state;
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


$( document ).ready(function() {
    socket.emit('join_room','telemetry_updates');
    $(".telemetry-value").each(function(){ telemetry_value_elements.push(this); });
});


let telemetry_value_elements = [];
socket.on('pod_telemetry', function (data) {
    let value, value_cell, value_name;
    data = JSON.parse(data);
    console.log(data);
    const length = telemetry_value_elements.length;

    for (let index = 0; index < length; index++) {
        value_cell = telemetry_value_elements[index];
        value_name = value_cell.id;
        if(!data.hasOwnProperty(value_name)){
            console.log(value_name + " not found in packet")
        }
        value = data[value_name];
        let $value_cell = $('#'+value_name);

        if(typeof(data[value_name]) == "boolean" || isNaN(data[value_name])){
            $value_cell.text(value)
        }
        else if($value_cell.hasClass('boolean')){
            value = (value === 1) ? 'True' : 'False';
            $value_cell.text(value)
        }
        else{
            $value_cell.text(value.toFixed(2));
        }
    }
});



/*

socket.on('pod_telemetry', function (data) {
    let value, sensor_cell, sensor_row, sensor_name, min, max;
    data = JSON.parse(data);

    const newPodState = data['podState'];
    ParsePodState(newPodState);

    const length = table_sensors.length;
    for (let index = 0; index < length; index++) {
        sensor_name = table_sensors[index];
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
*/




