// language=JQuery-CSS

const enablePodStates = $('#enable-manual-pod-states');
const disablePodStates = $('#disable-manual-pod-states');
const manualPodStates = $(".pod-state-btn");

const enableBrakeNodeStates = $(`#enable-manual-brake-node-states`);
const disableBrakeNodeStates = $('#disable-manual-brake-node-states');
const manualBrakeNodeStates = $('.brake-node-state-btn');

const enableLvdcNodeStates = $('#enable-manual-lvdc-node-states');
const disableLvdcNodeStates = $('#disable-manual-lvdc-node-states');
const manualLvdcNodeStates = $('.lvdc-node-state-btn');

///////// POD STATES ////////

enablePodStates.click(function () {
    manualPodStates.removeAttr('disabled').removeClass('active');;
    /*todo send command to set automatic transitions to 1*/
});

disablePodStates.click(function () {
    manualPodStates.attr('disabled','disabled');
    //todo send command to set automatic transitions to 0
});

manualPodStates.click(function () {
   const btn = $(this);
   if (isDisabled($(this))){
       return null;
   }
   const state = btn.data('state');
   //todo finish
});

///////// BRAKE NODE STATES ////////

enableBrakeNodeStates.click(function () {
    manualBrakeNodeStates.removeAttr('disabled').removeClass('active');

});

disableBrakeNodeStates.click(function () {
    manualBrakeNodeStates.attr('disabled','disabled');
    let command = {};
    command['target'] = 'brake node';
    command['state'] = 'bnsNone';
    $("#solenoid-controls").removeClass("in").css("height","")

    socket.emit('command', JSON.stringify(command))
});

manualBrakeNodeStates.click(function () {
    const btn = $(this);
    // Handel leaving solenoid control state
    if(btn.data('state') !== 'bnsSolenoidControl'){
        $("#solenoid-controls").removeClass("in").css("height","")
    }
    if (isDisabled($(this))){
       return null;
    }
    const state = btn.data('state')
    let command = {};
    command['target'] = 'brake_node';
    command['state'] = state;
    socket.emit('manual_state_command', JSON.stringify(command));
});

$('#send-solenoid-configuration').click(function () {
    let configuration = [0,0,0,0];
    const solenoid_btns = $('.solenoid-state-btn.active');
    solenoid_btns.each(function () {
        let index = parseInt($(this).data('solenoid-index'), 10);
        configuration[index]=1;
    });

    let command = {};
    command['target'] = 'brake_node';
    command['configuration'] = configuration
    socket.emit('manual_configuration_command', JSON.stringify(command))
});





///////// LVDC NODE STATES ////////

enableLvdcNodeStates.click(function () {
    manualLvdcNodeStates.removeAttr('disabled').removeClass('active')
});

disableLvdcNodeStates.click(function () {
    manualLvdcNodeStates.attr('disabled','disabled');
    //todo send command to set manual brake node field to none
});


function isDisabled(jqueryObj){
    if(jqueryObj.attr('disabled') == 'disabled'){
        return true;
    }
    else{
        return false;
    }
}

