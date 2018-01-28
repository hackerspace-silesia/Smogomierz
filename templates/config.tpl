{% args config, updated %}
<!doctype html>
<meta charset="UTF-8" />
<title>Smogomierz by Hackerspace - config</title>
<form method='POST' action='/config'>
    <div class='element'>
        <label for='wifi_essid'>ESSID</label>
        <input type='text' id='wifi_essid' name='wifi_essid' value='{{ config.get('wifi_essid').replace("'", "\\'") }}' />
    </div>
    <div class='element'>
        <label for='wifi_password'>PASSWORD</label>
        <input type='text' id='wifi_password' name='wifi_password' value='{{ config.get('wifi_password').replace("'", "\\'") }}' />
    </div>
    <div class='element'>
        <label for='airmonitor_lat'>LATITUDE</label>
        <input type='number' step='0.0001' id='airmonitor_lat' name='airmonitor_lat' value='{{ config.get('airmonitor_lat') }}' />
    </div>
    <div class='element'>
        <label for='airmonitor_long'>LONGITUDE</label>
        <input type='number' step='0.0001' id='airmonitor_long' name='airmonitor_long' value='{{ config.get('airmonitor_long') }}' />
    </div>
    <div class='element'>
        <label for='save_time_minutes'>COSTAM TYLKO NIE WIEM JAK TO NAZWAC</label>
        <input type='number' id='save_time_minutes' name='save_time_minutes' value='{{ config.get('save_time_minutes') }}' />
    </div>
</form>
