// language=JQuery-CSS

enablePodStates = $('#enable-manual-pod-states')
disablePodStates = $('#disable-manual-pod-states')
manualPodStates = $(".pod-state-btn")

enableBrakeNodeStates = $(`#enable-manual-brake-node-states`)
disableBrakeNodeStates = $('#disable-manual-brake-node-states')
manualBrakeNodeStates = $('.brake-node-state-btn')

enableLvdcNodeStates = $('#enable-manual-lvdc-node-states');
disableLvdcNodeStates = $('#disable-manual-lvdc-node-states');
manualLvdcNodeStates = $('.lvdc-node-state-btn');

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
    btn = $(this);
   if (isDisabled($(this))){
       return null;
   }
   var state = btn.data('state');
   socket.emit('manual')
});

///////// BRAKE NODE STATES ////////

enableBrakeNodeStates.click(function () {
    manualBrakeNodeStates.removeAttr('disabled').removeClass('active');

});

disableBrakeNodeStates.click(function () {
    manualBrakeNodeStates.attr('disabled','disabled');
    //todo send command to set manual brake node field to none
});

manualBrakeNodeStates.click(function () {
    // Handel leaving solenoid control state
    if($(this).data('state') != 'bnsSolenoidControl'){
        $("#solenoid-controls").removeClass("in").css("height","")
    }
    //todo send command to set manual brake node field to none
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

