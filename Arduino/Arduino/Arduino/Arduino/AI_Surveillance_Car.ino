#include <WiFi.h>
#include <WebServer.h>

// ==================================================
// WIFI
// ==================================================

const char* ssid = "WIFI NAME";
const char* password = "WIFI PASSWORD";

// ==================================================
// L298N MOTOR PINS
// ==================================================

#define IN1 25
#define IN2 26
#define IN3 27
#define IN4 14

// ==================================================
// WEB SERVER
// ==================================================

WebServer server(80);

// ==================================================
// STOP
// ==================================================

void stopMotors() {

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  Serial.println("STOP");
}

// ==================================================
// FORWARD
// ==================================================

void moveForward() {

  // Left motor
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  // Right motor
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  Serial.println("FORWARD");
}

// ==================================================
// BACKWARD
// ==================================================

void moveBackward() {

  // Left motor
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  // Right motor
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  Serial.println("BACKWARD");
}

// ==================================================
// LEFT
// ==================================================

void turnLeft() {

  // Left motor backward
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  // Right motor forward
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  Serial.println("LEFT");
}

// ==================================================
// RIGHT
// ==================================================

void turnRight() {

  // Left motor forward
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  // Right motor backward
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  Serial.println("RIGHT");
}

// ==================================================
// CONTROL COMMAND
// ==================================================

void handleControl() {

  if (!server.hasArg("move")) {

    server.send(
      400,
      "text/plain",
      "Missing move command"
    );

    return;
  }

  String command = server.arg("move");

  Serial.print("Received command: ");
  Serial.println(command);

  if (command == "F") {

    moveForward();

  }

  else if (command == "B") {

    moveBackward();

  }

  else if (command == "L") {

    turnLeft();

  }

  else if (command == "R") {

    turnRight();

  }

  else if (command == "S") {

    stopMotors();

  }

  else {

    stopMotors();

    server.send(
      400,
      "text/plain",
      "Invalid command"
    );

    return;
  }

  server.send(
    200,
    "text/plain",
    "OK"
  );
}

// ==================================================
// TEST WEB PAGE
// ==================================================

void handleRoot() {

  String html = R"rawliteral(

<!DOCTYPE html>

<html>

<head>

<meta name="viewport"
content="width=device-width, initial-scale=1">

<title>AI Surveillance Car</title>

<style>

body {
  background: #111;
  color: white;
  text-align: center;
  font-family: Arial;
}

button {
  width: 120px;
  height: 65px;
  margin: 8px;
  font-size: 18px;
  border-radius: 10px;
}

</style>

</head>

<body>

<h1>AI Surveillance Car</h1>

<h2>Motor Test</h2>

<button onclick="move('F')">
FORWARD
</button>

<br>

<button onclick="move('L')">
LEFT
</button>

<button onclick="move('S')">
STOP
</button>

<button onclick="move('R')">
RIGHT
</button>

<br>

<button onclick="move('B')">
BACKWARD
</button>

<script>

function move(command) {

  fetch("/control?move=" + command)

  .then(response => response.text())

  .then(data => console.log(data));

}

</script>

</body>

</html>

)rawliteral";

  server.send(
    200,
    "text/html",
    html
  );
}

// ==================================================
// SETUP
// ==================================================

void setup() {

  Serial.begin(115200);

  delay(1000);

  Serial.println();
  Serial.println("==============================");
  Serial.println("AI SURVEILLANCE CAR");
  Serial.println("==============================");

  // Motor pins

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Make sure robot is stopped
  stopMotors();

  // =================================================
  // WIFI
  // =================================================

  WiFi.begin(
    ssid,
    password
  );

  Serial.print(
    "Connecting to WiFi"
  );

  while (
    WiFi.status() != WL_CONNECTED
  ) {

    delay(500);

    Serial.print(".");
  }

  Serial.println();

  Serial.println(
    "WiFi connected!"
  );

  Serial.print(
    "ESP32 IP address: "
  );

  Serial.println(
    WiFi.localIP()
  );

  // =================================================
  // SERVER
  // =================================================

  server.on(
    "/",
    handleRoot
  );

  server.on(
    "/control",
    handleControl
  );

  server.begin();

  Serial.println(
    "Web server started!"
  );

  Serial.println();

  Serial.println(
    "Commands:"
  );

  Serial.println(
    "F = FORWARD"
  );

  Serial.println(
    "B = BACKWARD"
  );

  Serial.println(
    "L = LEFT"
  );

  Serial.println(
    "R = RIGHT"
  );

  Serial.println(
    "S = STOP"
  );

  Serial.println();

  Serial.print(
    "Open in browser: http://"
  );

  Serial.println(
    WiFi.localIP()
  );
}

// ==================================================
// LOOP
// ==================================================

void loop() {

  server.handleClient();

  delay(2);
}
