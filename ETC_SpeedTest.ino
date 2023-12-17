#include <ESP8266Firebase.h>
#include <ESP8266WiFi.h>
#include <NTPtimeESP.h>

#define _SSID "Iphone"          
#define _PASSWORD "20012001"      
#define REFERENCE_URL "https://fir-etc-b3fca-default-rtdb.firebaseio.com/"  

Firebase firebase(REFERENCE_URL);

NTPtime NTPch("ch.pool.ntp.org");   
strDateTime dateTime;

int button1 = 5;
int button2 = 4;
int button1State;
int button2State;

int sensor_distance = 10; // 10m

void setup()
{
  Serial.begin(9600);
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(1000);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to: ");
  Serial.println(_SSID);
  WiFi.begin(_SSID, _PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("-");
  }

  Serial.println("");
  Serial.println("WiFi Connected");

  // Print the IP address
  Serial.print("IP Address: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}
void loop() {
  int sensor_end = 0;
  button1State = digitalRead(button1);
  while(button1State == 1) {
    button1State = digitalRead(button1);
    delay(10);
  }
  unsigned long start_time = millis();
  button2State = digitalRead(button2);
  while(button2State == 1) {
    button2State = digitalRead(button2);
    delay(10);
  }
  unsigned long end_time = millis();
  float elapsedTime = (end_time - start_time) / 1000.0;
  float vehicle_speed_in_kmph = 10 * 3.6 / elapsedTime;
  Serial.println(vehicle_speed_in_kmph, 5);  

  dateTime = NTPch.getNTPtime(7.0, 0);
  while (dateTime.valid == 0) {
    Serial.println("dang lay du lieu thoi gian");
    dateTime = NTPch.getNTPtime(7.0, 0);
    delay(100);
  }
    NTPch.printDateTime(dateTime);

    byte actualHour = dateTime.hour;      // Gio
    byte actualMinute = dateTime.minute;  // Phut
    byte actualsecond = dateTime.second;  // Giay
    int actualyear = dateTime.year;       // Nam
    byte actualMonth = dateTime.month;    // Thang
    byte actualday =dateTime.day;         // Ngay
    byte actualdayofWeek = dateTime.dayofWeek;

    char date_data[20] = {0};
    sprintf(date_data, "%d/%d/%d", actualday, actualMonth, actualyear);
    firebase.pushString("data/date", date_data);
    char time_data[20] = {0};
    sprintf(time_data, "%d:%d:%d", actualHour, actualMinute, actualsecond);
    firebase.pushString("data/time", time_data);
    firebase.pushFloat("data/speed", vehicle_speed_in_kmph);
  
}
