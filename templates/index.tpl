{% args generator %}
<!doctype html>
<meta charset="UTF-8" />
<title>Smogomierz by Hackerspace</title>
<table>
    <thead><tr>
        <th>TIME</th>
        <th>PM1.0 <small>[µg/m³]</small></th>
        <th>PM2.5 <small>[µg/m³]</small></th>
        <th>PM10.0 <small>[µg/m³]</small></th>
        <th>Temperature <small>[°C]</small></th>
        <th>Pressure <small>[hPa]</small></th>
        <th>Humidity <small>[%]</small></th>
    </tr></thead>
    <tbody>
    {% for obj in generator %}
    <tr>
        <td>{{ obj.get('date') }}</td>
        <td>{{ obj.get('pm1') }}</td>
        <td>{{ obj.get('pm25') }}</td>
        <td>{{ obj.get('pm10') }}</td>
        <td>{{ obj.get('temperature') }}</td>
        <td>{{ obj.get('pressure') }}</td>
        <td>{{ obj.get('humidity') }}</td>
    </tr>
    {% endfor %}
    </tbody>
</table> 
