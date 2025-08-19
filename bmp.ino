
BMP280Data getBMP280Data() {
  BMP280Data data;
  data.temp = bmp.readTemperature();
  data.pressure = bmp.readPressure() / 100.0;
  return data;
}
