podState = 'psBooting';
sensor_ranges = false;

function getSensorRanges() {
    $.get($SCRIPT_ROOT + '/sensor_ranges', function (data) {
        sensor_ranges = JSON.parse(data)
    });
}

function updateRowStatus(row,min,max,actual){
    if(actual > max || actual < min){
        this.addClass()
    }
}

function ParsePodState(state) {
    if (!sensor_ranges) {
        console.log("No sensor Ranges yet")
        return
    }
    if (state !== podState) {
        podState = state
        $(".sensor-row").each(function (index) {
            sensor_name = $(this).attr('id');
            new_min = sensor_ranges[sensor_name][state][0];
            new_max = sensor_ranges[sensor_name][state][1];
            current = $('#' + sensor_name + '-actual').text()
            $('#' + sensor_name + '-min').text(new_min);
            $('#' + sensor_name + '-max').text(new_max);
            updateRowStatus($(this), new_min, new_max, current)
        });
    }
}

socket.on('telemetry', function (data) {
    data = JSON.parse(data);
    console.log(data)
    newPodState = data['podState'];
    ParsePodState(newPodState);
    for (sensor_name in data) {
        if(typeof(data[sensor_name]) == "boolean" || isNaN(data[sensor_name])){
            $("#" + sensor_name + "-actual").text(data[sensor_name])
        }
        else{
            $("#" + sensor_name + "-actual").text(data[sensor_name].toFixed(2))
        }

    }
});


//Run on load
getSensorRanges();

