
void connectToWiFi(const char* ssid, const char* password) {
  //Connect to WiFi Network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to WiFi");
  Serial.println("...");
  WiFi.begin(ssid, password);
  int retries = 0;
  while ((WiFi.status() != WL_CONNECTED) && (retries < 15)) {
    retries++;
    delay(500);
    Serial.print(".");
  }
  if (retries > 14) {
    Serial.println(F("WiFi connection FAILED"));
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println(F("WiFi connected!"));
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  }
  Serial.println(F("WiFi-Setup ready"));
}
