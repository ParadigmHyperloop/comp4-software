
var sensor_ranges = false;
var table_sensors = [];
var pod_state = ""

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

function updateRowStatus( value_name ,$value_cell, value){
    let min,max;
    min = sensor_ranges[value_name][pod_state][0]
    max = sensor_ranges[value_name][pod_state][1]

    if(!sensor_ranges.hasOwnProperty(value_name)){
        console.log(value_name + " not found in sensor ranges")
        return
    }
    if(value > max || value < min){
        $value_cell.addClass('danger');
        $value_cell.removeClass('success');
    }
    else{
        $value_cell.addClass('success');
        $value_cell.removeClass('danger');
    }
}

function ParsePodState(state) {
    let new_min;
    let new_max;
    let success;
    let sensor_name;
    if (!sensor_ranges) {
        getSensorRanges();
        console.log("No sensor Ranges yet");
        return
    }
    if (state !== pod_state){
        pod_state = state;
        $(".max-min-value").each(function (index) {
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

var telemetry_value_elements = [];
$( document ).ready(function() {
    socket.emit('join_room','telemetry_updates');
    $(".telemetry-value").each(function(){ telemetry_value_elements.push(this); });
});


socket.on('pod_telemetry', function (data) {
    let value, value_cell, value_name, current_state;
    data = JSON.parse(data);
    const length = telemetry_value_elements.length;

    current_state = data['podState'];
    ParsePodState(current_state);

    for (let index = 0; index < length; index++) {

        value_cell = telemetry_value_elements[index];
        value_name = value_cell.id;
        if(!data.hasOwnProperty(value_name)){
            console.log(value_name + " not found in packet")
            continue;
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
        else if($value_cell.hasClass('js-status-field'))
        {
            let currentThreadStatus = $($value_cell.firstChild);
            if (currentThreadStatus.getAttribute('data-thread-status') !== value.toString())
            {
                currentThreadStatus.removeClass("status-success status-warning status-danger");
                switch (value)
                {
                    case 0:
                        currentThreadStatus.addClass('.status-success');
                        currentThreadStatus.text("ACTIVE");
                        break;
                    case 1:
                        currentThreadStatus.addClass('.status-warning');
                        currentThreadStatus.text("BUSY");
                        break;
                    case 2:
                    case 3:
                        currentThreadStatus.addClass('.status-danger');
                        currentThreadStatus.text("DANGER");
                        break;
                }
            }
        }
        else{
            $value_cell.text(value.toFixed(2));
            if($value_cell.hasClass('max-min-value')){
                updateRowStatus(value_name, $value_cell, value);
            }
        }
    }
});




