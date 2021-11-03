#include <Arduino.h>
#include <Arduino_JSON.h>

void setup() {
  Serial.begin(9600);		//initialisation
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

  jsonReponse["test"] = "message test";
  jsonReponse["AngleZx2"] = val1 * 2;
  jsonReponse["ValCapteur2"] = 123.456;

  Serial.println(jsonReponse);
}