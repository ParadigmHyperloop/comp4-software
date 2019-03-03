function getStr(_class,_id) {
  return ('.'+_class+' .'+_id+'').toString();}

function setupConfigurationFormSubmit() {
  $('#configuration-form').on('submit' , function handleConfigurationFormSubmission(e) {
    console.log("SUBMISSION");
    e.preventDefault();
    debugger;
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
        debugger;
        console.log('Submitted Properly')
      },
      error: function handleErrorFormSubmission(data) {
        debugger;
        console.log(data);
      }
    })
  });
}

$(document).ready(function() {
  debugger;
  setupConfigurationFormSubmit();
});