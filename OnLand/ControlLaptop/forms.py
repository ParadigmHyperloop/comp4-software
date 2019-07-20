from flask_wtf import FlaskForm
from wtforms import StringField, SubmitField, BooleanField, IntegerField
from wtforms.validators import DataRequired, NumberRange


class OverridesForm(FlaskForm):
    lp_1 = BooleanField("N0 : Low Pressure 1")
    lp_2 = BooleanField("N1 : Low Pressure 2")
    lp_3 = BooleanField("N2 : Low Pressure 3")
    lp_4 = BooleanField("N3 : Regulated Pressure")
    hp = BooleanField("N4 : High Pressure")
    hp_temp = BooleanField("N5 : Tank Temperature")
    bms_values = BooleanField("C2: BMS Values")
    inverter_values = BooleanField("Inverter Values")
    brake_node_heartbeat = BooleanField("C0 : Brake Node Heartbeat")
    lvdc_node_heartbeat = BooleanField("C1 : LVDC Node Heartbeat")
    enclosure_node_heartbeat = BooleanField("C4 : Enclosure Node Heartbeat")
    enclosure_pressure = BooleanField("N6 : Enclosure Pressure")
    enclosure_temperature = BooleanField("N7 : Enclosure Temperature")
    cooling_pressure = BooleanField("N8 : Cooling Pressure")
    cooling_temperature = BooleanField("N9 : Cooling Temperature")
    navigation_heartbeat = BooleanField("C5 : Navigation Node Heartbeat")
    node_states = BooleanField("Check Node States")


class PodConfigurationForm(FlaskForm):
    retrieval_timeout = StringField('Retrieval Timeout (s)',
                                    validators=[DataRequired("Timeout")])
    telemetry_port = StringField('PDS Telemetry Port',
                                 validators=[DataRequired("PDS Port")])
    command_port = StringField('PDS Command Port',
                               validators=[DataRequired("PDS Port")])
    heartbeat_timout = StringField('HeartBeat Timeout (s)',
                                   validators=[DataRequired("HeartBeat Timeout")])
    pod_ip = StringField('Pod Address (IP)',
                         validators=[DataRequired("Pod Address")])
    pod_driver = BooleanField('Pod Driver: ')
    submit = SubmitField('Send Configuration')


class FlightProfileForm(FlaskForm):
    max_flight_time = IntegerField('Maximum Flight Time (s)')
    motor_speed = IntegerField('Motor Torque (N/m)')
    start_speed = IntegerField('Start Torque', default=0)
    acceleration_time = IntegerField('Ramp Time', default=0)
    flight_distance = IntegerField('Flight Distance (m)')
    taxi = BooleanField("Taxi", default=False)
    tube_pressure = IntegerField(' Tube Pressure ', default=0)
    max_velocity = IntegerField('Max Velocity', default=0)
    brake_distance = IntegerField('Braking Distance', default=0)




def validate_configuration_values(configuration_form):
    # TODO: implement once we know specifics of configuration
    return {
        'error': ['Error Messages'],
        'all_values_valid': True
    }


