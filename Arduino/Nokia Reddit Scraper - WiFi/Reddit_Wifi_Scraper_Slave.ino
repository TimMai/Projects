#include <WiFiLink.h>
#include <Wire.h>

/*
  ThingSpeak library is not being used to conserve memory
*/

//WiFi info
char ssid[] = "Campus Lodge"; // your network SSID (name)
int status = WL_IDLE_STATUS;
char server[] = "api.thingspeak.com";

//ThingSpeak params channel - need to read latest data upon startup and then send scraping parameters
unsigned long params_channel_number = 316708;
const char * params_read_key = "OUR7Z8B1RDN7KMLY";
const char * params_write_key = "ZIE5W42CTL5250M2";

//Parameters
String sub = "popular"; //Field1
int num = 0;           //Field2
int id = 0;             //Field3
bool ez_read = true;    //Field4

//ThingSpeak results - need to recieve incoming scraped results
unsigned long results_channel_number = 316731;
const char * results_read_key = "ZEPXT4FHP9OO2ONG";
const char * results_write_key = "SNOB4G4PRCAH6BTZ";

//I2C
String wire_request;
bool take_params = false;
int params_count = 0;


//Results fields - only one string to conserve space. This string will be sent to the master
String field;

bool cont = false;

WiFiClient client;

void setup() {
  //I2C
  Wire.begin(1);

  //Initialize serial and wait for port to open:
  Serial.begin(115200);
  Serial.print("Start\n");

  //This part is commented out because my current WiFi network is not like standard ones (with SSID and Pass)
  /*
    //Check if communication with wifi module has been established
    if (WiFi.status() == WL_NO_WIFI_MODULE_COMM) {
    Serial.println("Communication with WiFi module not established.");
    while (true); // don't continue:
    }

    // attempt to connect to Wifi network:
    while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid);

    // wait 10 seconds for connection:
    delay(10000);
    }*/

  WiFi.begin(ssid);
  delay(10000);
  Serial.println("Connected to wifi");
  IPAddress ip = WiFi.localIP();
  Serial.println("IP: " + ip);

  //Get the ID from the latest params
  get_params();
}

void loop() {
  Wire.onReceive(receiveEvent); //Upon receiving a message from master, call receiveEvent
  Wire.onRequest(requestEvent); //Upon receiving a request from master (where slave must send data back), call requestEvent

  // Would put this in the receiveEvent but then the program does not work
  // I guess you can't call functions within Wire?
  if (params_count == 4) {
    params_count = 0;
    wire_request = "SendOK";
    post_params();
  }

  else if (wire_request == "GetResults") {
    get_results();
    wire_request = "";
    field = "";
  }
}

/*
  Gets data from Params server
  Modifies global variables sub, num, id, ez_read
*/
void get_params() {
  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  if (client.connect(server, 80)) {
    Serial.println("connected to params server - gonna get");
    // Make a HTTP request:
    client.print("GET /channels/" + String(params_channel_number) + "/feeds/last?api_key=" + String(params_read_key) + " HTTP/1.1\r\n");
    client.print("Host: api.thingspeak.com\r\n");
    client.print("Connection: close\r\n\r\n");
    delay(500);
  }

  String json_string = "";
  bool copy = false;
  while (client.available()) {
    char c = client.read();

    if (c == '{')
      copy = true;
    else if (c == '}')
      copy = false;
    if (copy)
      json_string += c;

    Serial.write(c);
  }
  json_string += "}";

  Serial.println();
  Serial.println("Client is no longer available");
  Serial.println(json_string);

  String sub_string;
  String num_string;
  String id_string;
  String ez_read_string;

  //Parse through JSON data
  for (int i = 0; i < json_string.length(); i++) {
    //Look for field1 (size 6 + 3)
    if (json_string.substring(i, 9 + i) == "\"field1\":") {
      while (json_string[9 + i] != ',') {
        sub_string += json_string[9 + i];
        i++;
      }
    }

    if (json_string.substring(i, 9 + i) == "\"field2\":") {
      while (json_string[9 + i] != ',') {
        num_string += json_string[9 + i];
        i++;
      }
    }

    //Look for field4
    if (json_string.substring(i, 9 + i) == "\"field3\":") {
      while (json_string[9 + i] != ',') {
        id_string += json_string[9 + i];
        i++;
      }
    }

    //Look for field4 (size 6 + 3)
    if (json_string.substring(i, 9 + i) == "\"field4\":") {
      while (json_string[9 + i] != '}') {
        ez_read_string += json_string[9 + i];
        i++;
      }
      break;
    }
  }

  //Remove quotation marks
  sub_string = sub_string.substring(1, sub_string.length() - 1);
  num_string = num_string.substring(1, num_string.length() - 1);
  id_string = id_string.substring(1, id_string.length() - 1);
  ez_read_string = ez_read_string.substring(1, ez_read_string.length() - 1);

  Serial.println("sub: " + sub_string);
  Serial.println("num: " + num_string);
  Serial.println("id: " + id_string);
  Serial.println("ez_read: " + ez_read_string);

  id = id_string.toInt();
  id += 1;

  Serial.println("Next id has to be: " + String(id));

  client.stop();
}

/*
  Posts data to Params server
  Data - sub, num, id, ez_read
*/
void post_params() {
  String params = "api_key=";
  params += String(params_write_key);
  params += "&field1=";
  params += sub;
  params += "&field2=";
  params += String(num);
  params += "&field3=";
  params += String(id);
  params += "&field4=";
  if (ez_read)
    params += "true";
  else
    params += "false";

  if (client.connect(server, 80)) {
    Serial.println("connected to params server - gonna post");
    client.println("POST /update HTTP/1.1");
    client.println("Host: api.thingspeak.com");
    client.println("Content-Length: " + String(params.length()));
    client.println();
    client.println(params);
    delay(500);
  }

  client.stop();
  delay(5000);
}

//Get results from results server
void get_results() {
  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  if (client.connect(server, 80)) {
    Serial.println("connected to results server - gonna get");
    // Make a HTTP request:
    client.print("GET /channels/" + String(results_channel_number) + "/feeds/last?api_key=" + String(results_read_key) + " HTTP/1.1\r\n");
    client.print("Host: api.thingspeak.com\r\n");
    client.print("Connection: close\r\n\r\n");
    delay(500);
  }

  int phase = 0;
  int pulse = 0;
  String json_string; //Will hold the data that will be sent
  char end_char;      //Phase will increment upon finding this endchar - either , or }. End char denotes separations between the fields in the JSON data
  bool pulse_start = false;

  while (client.available()) {
    char c = client.read();

    //When the JSON data is presented
    if (c == '{') {
      while (client.available()) {
        if (pulse == num + 1)
          break;

        //Data will be parsed as it is being received char by char
        String target = "\"field";
        target += String(phase + 1);
        target += "\":";
        c = client.read();
        json_string += c;
        Serial.write(c);

        //If a pulse has been started and can be sent (i.e. in the middle of a phase)
        if (pulse_start == true && c != end_char && json_string.length() == 71) {
          Serial.println("\nSending: " + json_string);
          Serial.println("\nPhase: " + String(phase));
          Serial.println("\nPulse: " + String(pulse));
          field = json_string.substring(0, 28);
          Wire.onRequest(requestEvent);
          delay(2500);
          field = json_string.substring(28, 56);
          Wire.onRequest(requestEvent);
          delay(2500);
          field = json_string.substring(56, 71);
          Wire.onRequest(requestEvent);
          delay(2500);
          pulse++;
          json_string = "";
          field = "";
        }

        //If the zeroth phase has been completed. First phase is different from the rest because a "OK" or "NO" will be sent depending on if the Params ID matches the Results ID
        else if (pulse_start == true && c == end_char && phase == 0) {
          pulse_start = false;
          json_string = json_string.substring(0, json_string.length() - 2);
          Serial.println("\nPhase0: " + json_string);
          if (json_string.toInt() == id) {
            Serial.println("OK");
            json_string = "OK";
            field = json_string;
            json_string = "";

            Wire.onRequest(requestEvent);
            delay(5000);
            field = "";
            phase++;
            pulse++;
          }
          else {
            json_string = "NO";
            Serial.println("NO");
            field = json_string;
            json_string = "";

            Wire.onRequest(requestEvent);
            field = "";
            delay(5000);
            client.stop();
            return;
          }
        }

        //If the end of a non-zero phase has been reached
        else if (pulse_start == true && c == end_char && json_string.length() == 71 && phase != 0) {
          pulse_start = false;
          Serial.println("Sending: " + json_string);
          Serial.println("\nPhase: " + String(phase));
          Serial.println("\nPulse: " + String(pulse));
          field = json_string.substring(0, 28);
          Wire.onRequest(requestEvent);
          delay(2500);
          field = json_string.substring(28, 56);
          Wire.onRequest(requestEvent);
          delay(2500);
          field = json_string.substring(56, 71);
          Wire.onRequest(requestEvent);
          delay(2500);
          json_string = "";
          field = "";
          phase++;
          pulse++;
        }

        //If a pulse has been started but the data is not actually the results (extra JSON data)
        //Extra part at the end of the elif-statement is to deal with commas that are part of the title
        else if (pulse_start == true && c == end_char && json_string.length() != 71 && phase != 0 && json_string[json_string.length() - 2] == '"') {
          pulse_start = false;
          json_string = "";
          phase++;
          //pulse++;
        }

        //Looks for when the results data will appear in the JSON data
        //Only want the data past the field
        if (pulse_start == false && json_string.substring(json_string.length() - 10, json_string.length() - 1) == target) {
          json_string = "";
          pulse_start = true;
          if (pulse != num)
            end_char = ',';
          else
            end_char = '}';
        }
      }
    }

    if (pulse == num)
      break;

    Serial.write(c);
  }

  Serial.println();
  Serial.println("Client is no longer available");
  wire_request = "";
  field = "";
  id++;

  client.stop();
}

//Deal with master messages
void receiveEvent() {
  while (Wire.available()) {
    char c = Wire.read();
    wire_request += c;
  }

  Serial.println("Request: " + wire_request);

  //Master is going to send params
  if (wire_request == "SendParams") {
    wire_request = "";
    take_params = true;
  }

  //Take in the sent params
  else if (take_params) {
    params_count++;
    switch (params_count) {
      case 1:
        sub = wire_request;
        wire_request = "";
        break;
      case 2:
        num = wire_request.toInt();
        wire_request = "";
        break;
      case 3:
        id = wire_request.toInt();
        wire_request = "";
        break;
      case 4:
        if (wire_request == "true")
          ez_read = true;
        else
          ez_read = false;
        take_params = false;
        params_count = 0;
        break;
      case 5:
        break;
    }
  }
}

//Deal with master requests
void requestEvent() {
  String output;

  //Master requests ID
  if (wire_request == "RequestID") {
    //Here would get data from params
    output = String(id);
    while (output.length() < 3)
      output += " ";
    Wire.write(output.c_str());
    wire_request = "";
    return;
  }

  //Master wants the OK that the params have been sent
  else if (wire_request == "SendOK") {
    output = "OK";
    Serial.println("Sent the OK");
    Wire.write(output.c_str());
    wire_request = "";
    return;
  }

  //Master wants the results
  else if (wire_request == "GetResults") {
    Wire.write(field.c_str());
    cont = true;
    return;
  }
}

