#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

const char *ssid = "Vincent COMP";
const char *password = "vincentl116";
const char *host = "api.spotify.com";
const int httpsPort = 443;
const String AUTH_CODE = "ZTBjOGU2MzBiMDE3NGIxMmE2Yzg4NTRjMTMzN2Y5OTg6ZmZjMWYwNTY2NmRjNDYxZjkwM2VmM2YzMGVhYzlmMDk=";
const String REF_TOKEN = "AQAKPooz3cmcWRXc6mjk73ZmZwXnXP5JYrBQIjjDKdi1E0KcfLRb89DUITI_tTaW0IRmb5cdijHn-KXdzVgeZBVDH_CeqqfQ8QvXtslQJ2GI1csPRprMq4J_ScmREJ3lQ80";

WiFiClientSecure client;


String refreshToken(String AUTH_CODE, String REF_TOKEN)
{
  const int capacity = 512;
  StaticJsonDocument<capacity> doc;
  const char *host = "accounts.spotify.com";
  String url = "/api/token";
  String body = "grant_type=refresh_token&refresh_token=" + REF_TOKEN;
  String res = "";
  String ACC_TOKEN;
  if (!client.connect(host, httpsPort))
  {
    Serial.println("connection failed");
    return "";
  }
  Serial.println(body.length());
  client.print(String("POST ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Authorization: Basic " + AUTH_CODE + "\r\n" +
               "Content-Type: application/x-www-form-urlencoded\r\n" +
               "Content-Length: " + body.length() + "\r\n" +
               "Connection: close\r\n\r\n");
  client.print(body);
  Serial.println("refreshToken request sent");
  while (client.connected())
  {
    String line = client.readStringUntil('\n');
    if (line == "\r")
    {
      Serial.println("headers received");

      break;
    }
  }
  while (client.available())
  {
    char c = client.read();
    res = res + c;
    Serial.write(c);
  }
  Serial.println();
  DeserializationError err = deserializeJson(doc, res);
  if (err)
  {
    Serial.print(F("deserializeJson() failed with code "));
    Serial.println(err.c_str());
    return "";
  }
  else
  {
    ACC_TOKEN = doc["access_token"].as<String>();
    Serial.println(ACC_TOKEN);
  }
  client.stop();
  return ACC_TOKEN;
}

String getCurrentSong(String ACC_TOKEN)
{
  const char *host = "api.spotify.com";
  String url = "/v1/me/player/currently-playing";
  String res = "";
  String songId;
  StaticJsonDocument<128> filter;
  filter["item"]["id"] = true;
  const int capacity = 128;
  StaticJsonDocument<capacity> doc;
  if (!client.connect(host, httpsPort))
  {
    Serial.println("connection failed");
    return "";
  }
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Authorization: Bearer " + ACC_TOKEN + "\r\n" +
               "Connection: close\r\n\r\n");
  while (client.connected())
  {
    String line = client.readStringUntil('\n');
    if (line == "\r")
    {
      Serial.println("headers received");

      break;
    }
  }
  while (client.available())
  {
    char c = client.read();
    res = res + c;
    Serial.write(c);
  }
  Serial.println();
  DeserializationError err = deserializeJson(doc, res, DeserializationOption::Filter(filter));
  if (err)
  {
    Serial.print(F("deserializeJson() failed with code "));
    Serial.println(err.c_str());
    return "";
  }
  else
  {
    songId = doc["item"]["id"].as<String>();
    Serial.println(songId);
  }
  client.stop();
  return songId;
}

bool saveCurrentSong(String ACC_TOKEN,String songId){
  const char *host = "api.spotify.com";
  String url = "/v1/me/tracks?ids=" + songId;
  String res = "";
   if (!client.connect(host, httpsPort))
  {
    Serial.println("connection failed");
    return "";
  }
  client.print(String("PUT ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Authorization: Bearer " + ACC_TOKEN + "\r\n" +
               "Content-Length: 0\r\n" +
               "Connection: close\r\n\r\n");
  while (client.connected())
  {
    String line = client.readStringUntil('\n');
    if (line == "\r")
    {
      Serial.println("headers received");

      break;
    }
  }
  while (client.available())
  {
    char c = client.read();
    res = res + c;
    Serial.write(c);
  }
  Serial.println();
  client.stop();
  if(res == "")
  { return true;}
  else
  {
    return false;
  }
  
 
}

void setup()
{
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();
  client.setInsecure();
  WiFi.begin(ssid, password);

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());

  while (!client.connect(host, httpsPort))
  {
    Serial.println("connection failed");
    delay(500);
  }
  String ACC_TOKEN = refreshToken(AUTH_CODE, REF_TOKEN);
  if (ACC_TOKEN == "")
  {
    Serial.println("Didnt get Access Token");
    return;
  }
  String songId = getCurrentSong(ACC_TOKEN);
  if (songId == "")
  {
    Serial.println("Didnt get Current Song Name");
    return;
  }
 if(saveCurrentSong(ACC_TOKEN,songId))
 {
   Serial.println("Song Saved to library");
 }
 else
 {
   Serial.println("Didnt save the song");
   
 }
 

}

void loop()
{
}
