from flask_wtf import FlaskForm
from wtforms import StringField, SubmitField
from wtforms.validators import DataRequired


class FlightConfigurationForm(FlaskForm):
    air_resistance = StringField('Air Resistance (units)',
                                 validators=[DataRequired("Air Resistance")])
    motor_speed = StringField('Motor Speed (units)',
                              validators=[DataRequired("Motor Speed")])
    tube_pressure = StringField('Tube Pressure (units)',
                                validators=[DataRequired("Tube Pressure")])
    watchdog_timeout = StringField('Watchdog Timeout (ms)',
                                   validators=[DataRequired("Watchdog Timer")])
    pod_ip = StringField('Pod Address (IP)',
                         validators=[DataRequired("Pod Address")])
    submit = SubmitField('Send Configuration')
