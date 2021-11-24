/**
    ESP8266 InfluxDbV2: InfluxData

    Purpose: Holds the data of a single measurement.

    @see
   https://docs.influxdata.com/influxdb/v1.5/concepts/glossary/#measurement

    @author David G. Simmons Forked from:
    @author Tobias Schürg
*/

class InfluxDataV2 {
 public:
  InfluxDataV2(String bucket) : _bucket(bucket) {}

  void addTag(String key, String value) { _tags += "," + key + "=" + value; }
  void addValue(String key, float value) {
    _values = (_values == "") ? (" ") : (_values += ",");
    _values += key + "=" + String(value);
  }

  String toString() const { return _bucket + _tags + _values; }

 private:
  String _bucket;
  String _org;
  String _tags;
  String _values;
};
