function showSuccessConfigurationUpdate(from, align){
	$.notify({
		icon: "pe-7s-gift",
		message: "Configuration <b>Updated Successfully</b>"

	},{
	  type: 'success',
		timer: 4000,
		placement: {
			from: from,
			align: align
		}
	});
}

function validConfigFormValues() {
  let formHasError = true;
  // Ip Address Validation
  if (!(/^(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$/.test($('#pod-ip').val())))
  {
    formHasError = false;
    $('#pod-ip').parent().addClass('has-error');
  }
  if (isNaN($('#retrieval-timout').val())) { // Validated Retrieval timeout
    formHasError = false;
    $('#retrieval-timout').parent().addClass('has-error');
  }
  if (isNaN($('#max-flight-time').val())) { // Validated max-flight time
    formHasError = false;
    $('#max-flight-time').parent().addClass('has-error');
  }
  if (isNaN($('#motor-speed').val())) { // Validated motor speed
    formHasError = false;
    $('#motor-speed').parent().addClass('has-error');
  }
  if (isNaN($('#telemetry-port').val()) || $('#telemetry-port').val().length !== 4) { // Validated Telemetry port
    formHasError = false;
    $('#telemetry-port').parent().addClass('has-error');
  }
  if (isNaN($('#command-port').val()) || $('#command-port').val().length !== 4) { // Validated command port
    formHasError = false;
    $('#command-port').parent().addClass('has-error');
  }
  if (isNaN($('#flight-length').val())) { // Validated flight Length
    formHasError = false;
    $('#flight-length').parent().addClass('has-error');
  }
  if (isNaN($('#heartbeat-timeout').val())) { // Validated flight Length
    formHasError = false;
    $('#heartbeat-timeout').parent().addClass('has-error');
  }
  return formHasError;
}

function setupConfigurationFormSubmit() {
  $('#configuration-form').on('submit' , function handleConfigurationFormSubmission(e) {
    console.log("SUBMISSION");
    e.preventDefault();
    if (validConfigFormValues())
    {
      $.ajax({
      type: "POST",
      beforeSend: function(xhr, settings) {
        if (!/^(GET|HEAD|OPTIONS|TRACE)$/i.test(settings.type) && !this.crossDomain) {
          xhr.setRequestHeader("X-CSRFToken", "{{ form.csrf_token._value() }}")
        }
      },
      url: '/submit_configuration',
      data: $('#configuration-form').serialize(),
      success: function handleSucceessFormSubmission(data) {
        showSuccessConfigurationUpdate('top','right');
        console.log('Submission, Success')
      },
      error: function handleErrorFormSubmission(data) {
        console.log("FAILED");
      }
    });
    }
  });
}

function setupConfigurationForm()
{
  $('#pod-driver').bootstrapToggle({
    on: 'Motor',
    off: 'Simulation'
  });

  $('.js-config-form-input').on('focusin',function handleFormFocus()
  {
    $(this).parent().removeClass('has-error');
  });
}

$(document).ready(function() {
  setupConfigurationForm();
  setupConfigurationFormSubmit();
});