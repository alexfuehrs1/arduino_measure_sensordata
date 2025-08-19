
DHT22Data getDHT22data() {
  DHT22Data data;
  data.humidity = dht.readHumidity();
  data.temperature = dht.readTemperature();

  Serial.print("Luftfeuchtigkeit: ");
  Serial.print(data.humidity);
  Serial.println(" %");

  Serial.print("Temperatur: ");
  Serial.print(data.temperature);
  Serial.println(" °C"); 

  return data;
}