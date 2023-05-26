import machine, time
from time import sleep
from machine import Pin
import _thread
 
class Ultrassonico:
    """
    Driver to use the untrasonic sensor HC-SR04.
    The sensor range is between 2cm and 4m.
    The timeouts received listening to echo pin are converted to OSError('Out of range')
    """
    pulse_time = 0
    # echo_timeout_us is based in chip range limit (400cm)
    #def __init__(self, trigger_pin, echo_pin, echo_timeout_us=500*2*30):
    def __init__(self, porta):
        """
        trigger_pin: Output pin to send pulses
        echo_pin: Readonly pin to measure the distance. The pin should be protected with 1k resistor
        echo_timeout_us: Timeout in microseconds to listen to echo pin. 
        By default is based in sensor limit range (4m)
        """
        echo_timeout_us=500*2*30
        self.echo_timeout_us = echo_timeout_us
        # Init trigger pin (out)
        if(porta == 1):
            #trigger 32
            #echo 34
            trigger_pin = 32
            echo_pin = 34
        elif(porta == 2):
            #trigger 32
            #echo 34
            trigger_pin = 14
            echo_pin = 35
        elif(porta == 3):
            #trigger 32
            #echo 34
            trigger_pin = 15
            echo_pin = 33
        elif(porta == 4):
            #trigger 32
            #echo 34
            trigger_pin = 17
            echo_pin = 16
        else:
                raise OSError('Porta invalida')

        self.trigger = Pin(trigger_pin, mode=Pin.OUT, pull=None)
        self.trigger.value(0)

        # Init echo pin (in)
        self.echo = Pin(echo_pin, mode=Pin.IN, pull=None)
        _thread.start_new_thread(self._leitura_thread, ())

    def _leitura_thread(self):
         while True:
            self.trigger.value(0) # Stabilize the sensor
            time.sleep_us(5)
            self.trigger.value(1)
            # Send a 10us pulse.
            time.sleep_us(10)
            self.trigger.value(0)
            try:
                self.pulse_time = machine.time_pulse_us(self.echo, 1, self.echo_timeout_us)
            except OSError as ex:
                if ex.args[0] == 110: # 110 = ETIMEDOUT
                    self.pulse_time = 0
            sleep(0.25)
             
 
    def _send_pulse_and_wait(self):
        """
        Send the pulse to trigger and listen on echo pin.
        We use the method `machine.time_pulse_us()` to get the microseconds until the echo is received.
        """
        self.trigger.value(0) # Stabilize the sensor
        time.sleep_us(5)
        self.trigger.value(1)
        # Send a 10us pulse.
        time.sleep_us(10)
        self.trigger.value(0)
        try:
            pulse_time = machine.time_pulse_us(self.echo, 1, self.echo_timeout_us)
            return pulse_time
        except OSError as ex:
            if ex.args[0] == 110: # 110 = ETIMEDOUT
                raise OSError('Out of range')
            raise ex
 
    def distancia(self):
        """
        Get the distance in milimeters without floating point operations.
        """
        pulse_time = self.pulse_time#self._send_pulse_and_wait()
 
        # To calculate the distance we get the pulse_time and divide it by 2 
        # (the pulse walk the distance twice) and by 29.1 becasue
        # the sound speed on air (343.2 m/s), that It's equivalent to
        # 0.34320 mm/us that is 1mm each 2.91us
        # pulse_time // 2 // 2.91 -> pulse_time // 5.82 -> pulse_time * 100 // 582 
        mm = pulse_time * 100 // 582
        return mm
