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

function setupConfigurationFormSubmit() {
  $('#configuration-form').on('submit' , function handleConfigurationFormSubmission(e) {
    console.log("SUBMISSION");
    e.preventDefault();
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
    })
  });
}

$(document).ready(function() {
  setupConfigurationFormSubmit();
});