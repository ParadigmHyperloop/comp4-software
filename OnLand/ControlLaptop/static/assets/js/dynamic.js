function getStr(_class,_id) {
  return ('.'+_class+' .'+_id+'').toString();}

function setupConfigurationFormSubmit() {
  $('#configuration-form').submit(function handleConfigurationFormSubmission(e) {
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
        debugger;
        console.log('Submitted Properly')
      },
    })
  });
}

$('document').ready(function() {
  setupConfigurationFormSubmit()
});