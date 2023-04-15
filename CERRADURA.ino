#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Servo.h>

int estado = 0;
Servo servo11;
int kn = 6;
int bloqtime = 900000;
int erroneo = 0;
const byte FILAS = 4;
const byte COLUMNAS = 4;


bool ledState = LOW; 
const unsigned long parpadeoInterval = 1000; 
unsigned long lastparpadeoTime = 0; 
bool espera = true; 


char keys[FILAS][COLUMNAS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte pinesFilas[FILAS] = {5, 4, 3, 2};
byte pinesColumnas[COLUMNAS] = {A3, A2, A1, A0};

Keypad teclado = Keypad(makeKeymap(keys), pinesFilas, pinesColumnas, FILAS, COLUMNAS);
char TECLA;
char CLAVE[5];
char USER_CLAVE_1[5] = "1234";
char USER_CLAVE_2[5] = "8756";
char CLAVE_MAESTRA[5] = "3335";
byte INDICE = 0;
/////////////////////////
LiquidCrystal_I2C lcd(0x27, 16, 2); 
//LEDS

/* LED pins */
const byte VERDE = 51;
const byte ROJO = 50;
const byte AZUL = 52;
//


//
void setup() {
  lcd.begin();
  lcd.backlight();
  lcd.print("Bienvenido");
  delay("2000");
  servo11.attach(11, 660, 1400);
  servo11.write(150);
  pinMode(VERDE, OUTPUT);
  pinMode(AZUL, OUTPUT);
  pinMode(ROJO, OUTPUT);
  limpia();
  ////
  lcd.setCursor(0, 0);
  lcd.print("# para cambiar");
  lcd.setCursor(0, 1);
  lcd.print("contrasena");
  delay(1500);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Para abrir solo");
  lcd.setCursor(0, 1);
  lcd.print("Ingresa la pass");
  delay(1500);
  lcd.clear();
}


void loop() {
  TECLA = teclado.getKey();
 
    if (millis() - lastparpadeoTime >= parpadeoInterval) {
    lastparpadeoTime = millis();
    ledState = !ledState;
    digitalWrite(AZUL, ledState);
  }
  /// bandera para mensaje antes de presionar alguna tecla
  if (espera && !TECLA) {
     lcd.setCursor(5, 0);
    lcd.print("[____]");
    lcd.setCursor(0, 1);
    lcd.print("'#' para cambiar");
  }

  if (TECLA) {
    if (espera) {
      lcd.clear();
      espera = false; // Flag set to false to prevent showing "Esperando" message anymore
    }
    lcd.setCursor(5, 0);
    lcd.print("[____]");
    lcd.setCursor(kn, 0);
    lcd.print("*");
    CLAVE[INDICE] = TECLA;
    INDICE++;
    kn++;
    if (kn > 10) kn = 6; 
  }

  if (INDICE == 4) {
    kn = 6;
    if (!strcmp(CLAVE, CLAVE_MAESTRA)) {
      abierto();
    }
    else if (!strcmp(CLAVE, USER_CLAVE_1) || !strcmp(CLAVE, USER_CLAVE_2)) {
      abierto();
    }
    else {
      error();
    }
    INDICE = 0;
  }

  if (TECLA == '#'){
    cambia_clave();
  }
}

void error() {
  lcd.setCursor(0,1);
  lcd.print("Acceso Denegado");
  digitalWrite(ROJO, HIGH);
  delay(3000);
  digitalWrite(ROJO, LOW);
  erroneo++;
  if(erroneo == 3){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Sistema");
    lcd.setCursor(0,1);
    lcd.print("Bloqueado");
    delay(1000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Esperar");
    lcd.setCursor(0,1);
    lcd.print("15 minutos");
    digitalWrite(ROJO, HIGH);
    delay(bloqtime);
    digitalWrite(ROJO, LOW);
    erroneo = 0;
  }
  limpia();
  espera = true;
}

void abierto() {
  lcd.setCursor(0,1);
  lcd.print("Acceso Permitido");
  servo11.write(30);
  digitalWrite(VERDE, HIGH);
  delay(5000);
  digitalWrite(VERDE, LOW);
  servo11.write(150); // mueve el servo a la posición original
  lcd.setCursor(0,1);
  lcd.print("                "); // limpia la línea de la pantalla
  limpia();
  espera = true;
}

void cambia_clave() {
  kn=6;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("CAMBIO DE");
  lcd.setCursor(0,1);
  lcd.print("contrasena");
  delay(1100);
  lcd.clear();
  lcd.print("Ingresar clave");
  lcd.setCursor(0,1);
  lcd.print("maestra:");
  INDICE = 0;
  espera = true;
  while (INDICE <= 3) {
    TECLA = teclado.waitForKey();
    if (TECLA) {
      lcd.clear();
      lcd.setCursor(5, 1);
      lcd.print("[____]");
      lcd.setCursor(kn, 1);
      lcd.print("*");
      CLAVE[INDICE] = TECLA;
      INDICE++;
      kn++; // Incrementa kn por 1
      if (kn > 10) kn = 6; // Si kn supera el límite de columnas, reiniciar en la posición 6
    }
  }
  if (!strcmp(CLAVE, CLAVE_MAESTRA)) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("1) CLAVE 1");
    lcd.setCursor(0,1);
    lcd.print("2) CLAVE 2");
    TECLA = teclado.waitForKey();
    if (TECLA == '1') {
      kn=6;
      espera = true;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("INGRESE NUEVA");
      lcd.setCursor(0,1);
      lcd.print("CONTRASENA 1:");
      INDICE = 0;
      while (INDICE <= 3) {
        TECLA = teclado.waitForKey();
        if (TECLA) {
          espera = true;
          lcd.clear();
          USER_CLAVE_1[INDICE] = TECLA;
          lcd.setCursor(5, 0);
          lcd.print("[____]");
          lcd.setCursor(kn, 0);
          lcd.print("*");
          INDICE++;
          kn++; // Incrementa kn por 1
          if (kn > 10) kn = 6; // Si kn supera el límite de columnas, reiniciar en la posición 6
        }
      }
      lcd.clear();
      lcd.print("CLAVE CAMBIADA");
      delay(3000);
    }
    else if (TECLA == '2') {
      kn=6;
      espera = true;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("INGRESE NUEVA");
      lcd.setCursor(0,1);
      lcd.print("CONTRASENA 2");
      INDICE = 0;
      while (INDICE <= 3) {
        TECLA = teclado.waitForKey();
        if (TECLA) {
          espera = true;
          lcd.clear();
          USER_CLAVE_2[INDICE] = TECLA;
          lcd.setCursor(5, 0);
          lcd.print("[____]");
          lcd.setCursor(kn, 0);
          lcd.print("*");
          INDICE++;
          kn++; // Incrementa kn por 1
          if (kn > 10) kn = 6; 
        }
      }
      lcd.clear();
      espera = true;
      lcd.print("CLAVE CAMBIADA");
      delay(3000);
    }
  }
  else {
    error(); // If the master password is incorrect, display an error
    espera = true;
  }
  kn=6;
  INDICE = 0;
  limpia();
  espera = true;
}

void limpia() {
  for (byte i=0;i<4;i++) {
    CLAVE[i] = 0;
  }
  INDICE = 0;
  lcd.setCursor(0,1);
  lcd.print("                ");
  lcd.setCursor(0,0);
  lcd.print("                ");
}
