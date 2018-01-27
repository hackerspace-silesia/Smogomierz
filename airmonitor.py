#!/usr/bin/env python
import json, requests

sensor_model = "PMS7003"
#Wspolrzedne miernika zanieczyszczen. Do sprawdzenia na http://mapa.airmonitor.pl
lat = 50.2639
long = 18.9957

data = '{"lat": "' + str(lat) + '", ' \
        '"long": "'+ str(long) + '", ' \
        '"pressure": ' + str(float('%.2f' % pressure_values_avg)) + ', ' \
        '"temperature": ' + str(float('%.2f' % temp_values_avg)) + ', ' \
        '"humidity": ' + str(float('%.2f' % humidity_values_avg)) + ', ' \
        '"pm1": ' + str(float(pm1_values_avg)) + ', ' \
        '"pm25": ' + str(float(pm25_values_avg)) + ', ' \
        '"pm10":' + str(float(pm10_values_avg)) + ', ' \
        '"sensor": "' + str(sensor_model) + '"}'

url = ('http://api.airmonitor.pl:5000/api')
resp = requests.post(url,
                     timeout=10,
                     data=json.dumps(data),
                     headers={"Content-Type": "application/json"})
resp.status_code
