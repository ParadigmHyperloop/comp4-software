podState = 'psBooting';
sensor_ranges = false;

socket.emit('join_room','telemetry_updates');

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
    data = JSON.parse(data);
    console.log(data)
    const newPodState = 'psStandby';
    ParsePodState(newPodState);
    for (let sensor_name in data) {
        if(!data.hasOwnProperty(sensor_name)){
            continue;
        }
        debugger;
        let value_cell = $("#" + sensor_name + "-actual");
        if(typeof(data[sensor_name]) == "boolean" || isNaN(data[sensor_name])){
            value_cell.text(data[sensor_name])
        }
        else{
            let min = $("#" + sensor_name + "-min").text();
            let max = $("#" + sensor_name + "-max").text();
            let row = $("#" + sensor_name);
            value_cell.text(data[sensor_name].toFixed(2));
            updateRowStatus(row,min,max,data[sensor_name]);
        }
    }
});


//Run on load
getSensorRanges();

