from flask_wtf import FlaskForm
from wtforms import StringField, SubmitField, BooleanField
from wtforms.validators import DataRequired


class FlightConfigurationForm(FlaskForm):
    retrieval_timeout = StringField('Retrieval Timeout (s)',
                                    validators=[DataRequired("Air Resistance")])
    max_flight_time = StringField('Max Flight Time (s)',
                                  validators=[DataRequired("Motor Speed")])
    motor_speed = StringField('Max Flight Time(units)',
                              validators=[DataRequired("Motor Speed")])
    telemetry_port = StringField('PDS Telemetry Port',
                                 validators=[DataRequired("PDS Port")])
    command_port = StringField('PDS Command Port',
                               validators=[DataRequired("PDS Port")])
    flight_Length = StringField('Flight Length (s)',
                                validators=[DataRequired("Expected FLight Length")])
    heartbeat_timout = StringField('HeartBeat Timeout (s)',
                                   validators=[DataRequired("HeartBeat Timeout")])
    pod_ip = StringField('Pod Address (IP)',
                         validators=[DataRequired("Pod Address")])
    pod_driver = BooleanField('Pod Driver: ')
    submit = SubmitField('Send Configuration')


def validate_configuration_values(configuration_form):
    # TODO: implement once we know specifics of configuration
    return {
        'error': ['Error Messages'],
        'all_values_valid': True
    }


