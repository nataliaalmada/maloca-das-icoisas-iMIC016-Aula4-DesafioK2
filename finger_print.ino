#include <Adafruit_Fingerprint.h>
#include <HardwareSerial.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>

const char * ssid = "Macuxi Digital"; // Substitua pelo seu SSID
const char * password = "@Macuxi#ufrr35anos"; // Substitua pela sua senha

const char* root_ca = \
"-----BEGIN CERTIFICATE-----\n" \
"MIIF8TCCA9mgAwIBAgIUaNfJfXos2nJvWHan6NDQEWJZOnkwDQYJKoZIhvcNAQEL\n" \
"BQAwgYcxCzAJBgNVBAYTAkJSMRAwDgYDVQQIDAdSb3JhaW1hMRIwEAYDVQQHDAlC\n" \
"b2EgVmlzdGExDzANBgNVBAoMBk1hbG9jYTEQMA4GA1UECwwHaUNvaXNhczELMAkG\n" \
"A1UEAwwCT2kxIjAgBgkqhkiG9w0BCQEWE2V4ZW1wbG9AZXhlbXBsby5jb20wHhcN\n" \
"MjUwMzIyMTMyODE0WhcNMjYwMzIyMTMyODE0WjCBhzELMAkGA1UEBhMCQlIxEDAO\n" \
"BgNVBAgMB1JvcmFpbWExEjAQBgNVBAcMCUJvYSBWaXN0YTEPMA0GA1UECgwGTWFs\n" \
"b2NhMRAwDgYDVQQLDAdpQ29pc2FzMQswCQYDVQQDDAJPaTEiMCAGCSqGSIb3DQEJ\n" \
"ARYTZXhlbXBsb0BleGVtcGxvLmNvbTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCC\n" \
"AgoCggIBALaRnvrIvxEsbt6kWoVHJ7eBI6U/Pib/+kthLijWV1EmDLClnvSUXqki\n" \
"q2l9iKFM7D3lZbhlMX3btj8t5esu+nshuD6x+Sdz6B/rnCgLmX7jbKoDbr4cJ+os\n" \
"ofw168On7eQo3lfiHFshcVE8toqK1d2RVmgeRakLVXFfFNl8JkeXcxStJVOINL4Y\n" \
"TDppRe2y+Gi1xrZcZhM1O8Q1EcC9pfDciKJSUuY17CbYjxyxvXDIpesm3HJ7Iyzk\n" \
"1ubOO9xrWhmm/YEz3q7IrZLMr/FoWfzTi8qTVEH0PzAgItcdQ2+zi58OvKhCf+rV\n" \
"mW9Aq3M9D+g87E9C0MQBN6vBKLioQvfzIVvzzpeEupgmcHMh5U4JkKMeadzj/Rd5\n" \
"F/7lVwU4Gr0CHAAEtMy9AS1IVREp6iaSvUxo4gOw4xYcei3hQGr3nZKZpPqamjjk\n" \
"/yx96SO0sOoG6tBoOpCOBaRCX0wrHzRKDetQ/zl8vswXw4iAMy98+8Zf6GE9hLCJ\n" \
"XCnncCyFCGIz0p9ynPfdbufrrmLnzc6K9e4m9UMJBOTpQYb+bTotSFUHvGNJEqiQ\n" \
"Rrgib4K3A+bZaqz1Y6nu3I5ie+PQZBnUHL6goCSmZWhgN5DEhAzYek9t6jCAYbmZ\n" \
"Ocd+8opjX6S5w2glWJzcQlTKNjml3g5FTeut/bYf7r2hNu3zqBDlAgMBAAGjUzBR\n" \
"MB0GA1UdDgQWBBS+tDmOSFHyE3xgiB95lTGnsguteDAfBgNVHSMEGDAWgBS+tDmO\n" \
"SFHyE3xgiB95lTGnsguteDAPBgNVHRMBAf8EBTADAQH/MA0GCSqGSIb3DQEBCwUA\n" \
"A4ICAQBVgIGcJCm8jLosgf7CkMpe7yKpQk5Y2xvGkcAb05hNu5WaFPfmdiY+w03S\n" \
"FKPFi8ooDumaVLTSyv2VkUuVmLMSbBno5+qdeZiKzO/dIpHhHcqnuyvBRGiXXcc7\n" \
"ESM/HvZ1Yoq7Pfm5/GM/wE83QilbUfua+zKt2upkaLTLsZEL9gAdS8e75rEEpiiG\n" \
"o0gMbCx9HJs2RA75uEHilTKOT6fQ36I43D5nmMRR+kF/EHofPU6kEgPFgQhyNvK+\n" \
"Zrqh1ByLwrk1mWR5JaIMMRD1JbzogNqvq0LvzrWQ0ufD4EyFVSWTZtJ1PTG4Atyz\n" \
"6unog9B12FlR09M/Kvot45s+2jXAuTP3YprMzl8xpsssfroXp1iezoPGeiHeRq+p\n" \
"OlsDGFE7m4x7OQ1hTYzT1BmIQxIZ5PcWqPjpowpTaf/oBwG8xChJimBCDFmyCcYm\n" \
"QwyB1QYuAs657MYdXUrdA5hgOf6WFfHWBtRbup8rStyc8us5QVPPusdMTatughBj\n" \
"ZMEY99hmUvdqneBsHO1gIs5y30/X8xtz+s1cvgegWW1b21anzwngsPrbXGiCAJfW\n" \
"PY+4yMLfFJ82slnIvAGFd5gEouxjqmgNqiUpvrMWv0qYQobDCj4nAiWt5rQ56Hr7\n" \
"JrVaxv8C94/BQtDAjnHdcxSw68BNiX2y75hvH+MCngUoeK3Hlg==" \
"-----END CERTIFICATE-----\n";

String servidorHost = "172.22.68.44"; // URL do servidor
String servidorURL = "https://172.22.68.44:5000/dados"; // URL do servidor

HardwareSerial mySerial(1);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

WiFiClientSecure client;

int ecgPin = 36; // Pino de leitura do ECG ( pode ser outro pino analógico )
int valorECG = 0;

void setup() {
  Serial.begin(115200);
  pinMode ( ecgPin , INPUT ) ;
  mySerial.begin(57600, SERIAL_8N1, 16, 17);

  finger.begin(57600);
  if (!finger.verifyPassword()) {
    Serial.println("Erro: Sensor não encontrado!");
    while (1);
  }

  WiFi.begin(ssid, password);

  // Espera até conectar ao Wi-Fi

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao WiFi...");
  }
  Serial . println (" Conectado ao WiFi !");

  client.setCACert(root_ca);
}

void loop() {
  int id = verificarDigital();
  
  if (id > 0) {
    Serial.print("Usuário autenticado! ID: ");
    Serial.println(id);

    int valorECG = analogRead(ecgPin);
    int heartrate = map(valorECG, 0, 4095, 0, 215);
    String jsonPayload = "{\"ecg\":\"" + String(heartrate) + "\"}";

    enviarDadosHTTPS(jsonPayload);
  } else {
    Serial.println("Usuário não autenticado!");
  }

  delay(2000);
}

// Função para envio seguro via HTTPS
void enviarDadosHTTPS(String payload) {
  HTTPClient https;
  
  if (https.begin(client, servidorURL)) {
    https.addHeader("Content-Type", "application/json");
    
    int httpCode = https.POST(payload);
    if(httpCode > 0) {
      Serial.printf("HTTPS POST: código %d\n", httpCode);
    } else {
      Serial.printf("HTTPS POST falhou: %s\n", https.errorToString(httpCode).c_str());
    }
    https.end();
  }
}

int verificarDigital() {
  int resultado = finger.getImage();
  if (resultado != FINGERPRINT_OK) return -1;

  resultado = finger.image2Tz();
  if (resultado != FINGERPRINT_OK) return -1;

  resultado = finger.fingerFastSearch();
  if (resultado == FINGERPRINT_OK) {
    return finger.fingerID;  // Retorna o ID do usuário autenticado
  }

  return -1;  // Falha na autenticação
}