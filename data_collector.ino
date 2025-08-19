
void sendData() {
  Serial.println("Sending data...");

  String jsonObj = MY_JSON_OBJECT; // ADD YOU JSON OBJECT YOU WANT TO SEND
  String myUrl = ""; // e.g., "/api/data"

  Serial.println(jsonObj);
  httpClient.beginRequest();
  httpClient.post(myUrl);
  String token = getValidAccessToken();
  httpClient.sendHeader("Authorization", "Bearer " + token);
  httpClient.sendHeader("Content-Type", "application/json");
  httpClient.sendHeader("Content-Length", jsonObj.length());
  httpClient.beginBody();
  httpClient.print(jsonObj);
  httpClient.endRequest();
  int statusCode = httpClient.responseStatusCode();
  String response = httpClient.responseBody();

  Serial.print("Status code: ");
  Serial.println(statusCode);
  Serial.print("Response: ");
  Serial.println(response);
  Serial.println();
}


