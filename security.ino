
const char* keycloak_url = "/realms/MY_REALM/protocol/openid-connect/token";
const int keycloak_port = 8080; // e.g., 8080 or 443 for HTTPS
const char* keycloak_client_id = "MY_CLIENT_ID";
const char* keycloak_client_secret = "MY_CLIENT_SECRET";
const char* keycloak_username = "MY_USERNAME";
const char* keycloak_user_password = "MY_PASSWORD";

String accessToken;
String refreshToken;
unsigned long tokenExpiresAt = 0;
HttpClient http(wifiClient, serverAddress, keycloak_port);

bool getAccesstoken(const String& grantType) {
  http.stop();  
  String body = "grant_type=" + grantType + "&client_id=" + String(keycloak_client_id) + "&client_secret=" + String(keycloak_client_secret);
  if (grantType == "password") {
    body += "&username=" + String(keycloak_username) + "&password=" + String(keycloak_user_password);
  } else if (grantType == "refresh_token") {
    body += "&refresh_token=" + String(refreshToken);
  }

  http.beginRequest();
  http.post(keycloak_url);
  http.sendHeader("Content-Type", "application/x-www-form-urlencoded");
  http.sendHeader("Content-Length", body.length());
  http.beginBody();
  http.print(body);
  http.endRequest();

  int statusCode = http.responseStatusCode();
  String response = http.responseBody();

  if (statusCode != 200) {
    Serial.println("Token fetch failed. Status: " + String(statusCode));
    return false;
  }

  // Parse JSON
  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, response);
  if (error) {
    Serial.println("JSON parse failed.");
    return false;
  }

  accessToken = doc["access_token"].as<String>();
  refreshToken = doc["refresh_token"].as<String>();
  int expiresIn = doc["expires_in"].as<int>();
  tokenExpiresAt = millis() + (expiresIn - 10) * 1000UL;
  Serial.println("Access token acquired.");
  return true;
}

String getValidAccessToken() {
  if (accessToken == "" || millis() >= tokenExpiresAt) {
    Serial.println("Access token expired or missing. Refreshing...");
    if (refreshToken != "") {
      if (!getAccesstoken("refresh_token")) {
        Serial.println("Refresh failed, retrying password grant...");
        if (!getAccesstoken("password")) {
          Serial.println("Both refresh and password grants failed!");
          return "";
        }
      }
    } else {
      if (!getAccesstoken("password")) {
        Serial.println("Initial token fetch failed!");
        return "";
      }
    }
  }
  return accessToken;
}
