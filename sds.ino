SDS011Data getSDS011data() {
  SDS011Data data;

  PmResult pm = sensor.readPm();
  if (pm.isOk()) {
    data.pm25 = pm.pm25;
    data.pm10 = pm.pm10;
    data.valid = true;
    Serial.println(pm.toString());

  } else {
    Serial.print("Could not read values from sensor, reason: ");
    Serial.println(pm.statusToString());
    data.valid = false;
  }

  return data;
}
