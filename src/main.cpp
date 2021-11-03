#include <Arduino.h>
#include <Arduino_JSON.h>

void setup() {
  Serial.begin(9600);		//initialisation
  pinMode(D13, OUTPUT);
  digitalWrite(D13, LOW);
}

void loop() {
  String reception = "";
  while (!Serial.available());
  while (Serial.available()) { 	//tant que des données sont en attente
    char c = Serial.read(); 	// lecture
    reception += String(c); 	//on ajoute à l'objet réception
    //delay(10); 		//petite attente
  }
  reception.trim(); 	//on enlève le superflu en début et fin de chaîne
  JSONVar jsonRecu = JSON.parse(reception);
  JSONVar jsonReponse;


  double val1 = double(jsonRecu["Rz"]);
  if(val1 == 10){
    digitalWrite(D13, HIGH);
  }
  else{
    digitalWrite(D13, LOW);
  }
  jsonReponse["statut"] = true;
  jsonReponse["test"] = "message test";
  jsonReponse["AngleZx2"] = val1 * 2;
  jsonReponse["ValCapteur2"] = 123.456;
  jsonReponse["time"] = millis();

  delay(100);
  Serial.println(jsonReponse);
}