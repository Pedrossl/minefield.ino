#include <Adafruit_NeoPixel.h>
#include <MatrizLed.h>
#include <MD_MAX72xx.h>

// Definições de pinos e constantes
#define PIN_ANALOG_X 0
#define PIN_ANALOG_Y 1

#define QTD_RODA 12
#define PIN_RODA 12
#define BOTAO_A 2
#define BOTAO_B 3
#define BOTAO_C 4
#define BOTAO_D 5
#define PIN_BOTAO_ANALOGICO 8
#define DEBOUNCE_DELAY 1000

#define TIPO_DO_HARDWARE MD_MAX72XX::GENERIC_HW
#define MAX_MODULOS  1  
#define DATA_PIN  10 // DIN
#define CS_PIN    9
#define CLK_PIN   11
#define MAX_PONTOS 100
#define MAX_TAMANHO_PONTO 5
#define NUM_BOMBAS 15
#define PIN_BIPE 13

char ligados[MAX_PONTOS][MAX_TAMANHO_PONTO];
int numPontos = 0;
bool botaoAnalogicoPressionado = false;
bool estadoLEDs[8][8];
bool bombas[8][8];

unsigned long lastButtonPressTimeA = 0;
unsigned long lastButtonPressTimeB = 0;
unsigned long lastButtonPressTimeC = 0;
unsigned long lastButtonPressTimeD = 0;
unsigned long lastButtonPressTimeAnalog = 0;
bool rodarColorido = false;

MD_MAX72XX matriz(TIPO_DO_HARDWARE, DATA_PIN, CLK_PIN, CS_PIN, MAX_MODULOS);
MatrizLed Display8X8;

Adafruit_NeoPixel RODA_LED = Adafruit_NeoPixel (QTD_RODA,
PIN_RODA, NEO_GRB + NEO_KHZ800);

int posicaoAtualLinha = 0;
int posicaoAtualColuna = 0;
int lugaresSegurosMarcados = 0;

void setup() {
  Serial.begin(9600);
  Display8X8.begin(4, 2, 3, 1);
  Display8X8.rotar(false);
  matriz.begin();
  matriz.control(MD_MAX72XX::INTENSITY, MAX_INTENSITY / 2);

  pinMode(BOTAO_A, INPUT_PULLUP);
  pinMode(BOTAO_B, INPUT_PULLUP);
  pinMode(BOTAO_C, INPUT_PULLUP);
  pinMode(BOTAO_D, INPUT_PULLUP);
  pinMode(PIN_BOTAO_ANALOGICO, INPUT_PULLUP);
  pinMode(PIN_BIPE, OUTPUT);

  RODA_LED.begin();
  RODA_LED.setBrightness(75);
  RODA_LED.show();

  inicializarEstados();
  randomizarBombas();
}

void loop() {
  unsigned long currentMillis = millis();

  verificarBotao(BOTAO_A, lastButtonPressTimeA, currentMillis, -1, 0);
  verificarBotao(BOTAO_B, lastButtonPressTimeB, currentMillis, 0, -1);
  verificarBotao(BOTAO_C, lastButtonPressTimeC, currentMillis, 1, 0);
  verificarBotao(BOTAO_D, lastButtonPressTimeD, currentMillis, 0, 1);

  if (digitalRead(PIN_BOTAO_ANALOGICO) == LOW && currentMillis - lastButtonPressTimeAnalog > DEBOUNCE_DELAY) {
    lastButtonPressTimeAnalog = currentMillis;
    botaoAnalogicoPressionado = true;
        lugaresSegurosMarcados++; 
    pintadorDeLed(0,255,0);
    matriz.setPoint(posicaoAtualColuna, posicaoAtualLinha, true);

    if (bombas[posicaoAtualLinha][posicaoAtualColuna]) {
      Serial.println("explodi");
      pintadorDeLed(255,0,0);
      digitalWrite(PIN_BIPE, HIGH);
      delay(1000);
      digitalWrite(PIN_BIPE, LOW);
      matriz.clear();
    } else {
      verificarBombasVizinhas(posicaoAtualLinha, posicaoAtualColuna); 
    }

      estadoLEDs[posicaoAtualLinha][posicaoAtualColuna] = true;

    if (numPontos < MAX_PONTOS) {
      sprintf(ligados[numPontos], "%d,%d", posicaoAtualColuna, posicaoAtualLinha);
      numPontos++;
    }
   if (lugaresSegurosMarcados >= 30) {
      rodarColorido = true; 
    }
      if (rodarColorido) {
    animacaoColorida();
  }
  }
}

void inicializarEstados() {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      estadoLEDs[i][j] = false;
      bombas[i][j] = false;
    }
  }
}

void randomizarBombas() {
  for (int i = 0; i < NUM_BOMBAS;) {
    int x = random(8);
    int y = random(8);
    if (!bombas[x][y]) {
      bombas[x][y] = true;
      i++;
    }
  }
}

void pintadorDeLed(int r,int g, int b){
  for (int i = 0; i < QTD_RODA;) {
    RODA_LED.setPixelColor(i,r,g,b);
    RODA_LED.show();
    i++;
  }
}

void verificarBombasVizinhas(int linha, int coluna) {
  int direcoes[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
  for (int i = 0; i < 4; i++) {
    int novaLinha = linha + direcoes[i][0];
    int novaColuna = coluna + direcoes[i][1];

    if (novaLinha >= 0 && novaLinha < 8 && novaColuna >= 0 && novaColuna < 8) {
      if (bombas[novaLinha][novaColuna]) {
        pintadorDeLed(255, 100, 0); 
      }
    }
  }
}

void verificarBotao(int botao, unsigned long &lastButtonPressTime, unsigned long currentMillis, int deltaColuna, int deltaLinha) {
  if (digitalRead(botao) == LOW && currentMillis - lastButtonPressTime > DEBOUNCE_DELAY) {
    lastButtonPressTime = currentMillis;
    Serial.print("Botão pressionado: ");
    pintadorDeLed(0,0,0);
    Serial.println(botao);

    if (bombas[posicaoAtualLinha][posicaoAtualColuna] && !botaoAnalogicoPressionado) {
      bombas[posicaoAtualLinha][posicaoAtualColuna] = true;
      Serial.println("Deixei a bomba Visivel 1");
    }

    if (estadoLEDs[posicaoAtualLinha][posicaoAtualColuna] && !botaoAnalogicoPressionado) {
            Serial.println("Deixei a bomba Visivel 2");

      estadoLEDs[posicaoAtualLinha][posicaoAtualColuna] = true;
      matriz.setPoint(posicaoAtualColuna, posicaoAtualLinha, true);
      botaoAnalogicoPressionado = true;
    } else if (!botaoAnalogicoPressionado) {
            Serial.println("Deixei a bomba Visivel 3");

      matriz.setPoint(posicaoAtualColuna, posicaoAtualLinha, false);
    }

    botaoAnalogicoPressionado = false;
    posicaoAtualColuna = constrain(posicaoAtualColuna + deltaColuna, 0, 7);
    posicaoAtualLinha = constrain(posicaoAtualLinha + deltaLinha, 0, 7);
    matriz.setPoint(posicaoAtualColuna, posicaoAtualLinha, true);
          Serial.println("Deixei a bomba Visivel 4");

  }
}
void animacaoColorida() {
  static int firstPixelHue = 0;
  for (int i = 0; i < RODA_LED.numPixels(); i++) {
    int pixelHue = firstPixelHue + (i * 65536L / RODA_LED.numPixels());
    RODA_LED.setPixelColor(i, RODA_LED.gamma32(RODA_LED.ColorHSV(pixelHue)));
  }
  RODA_LED.show();
  firstPixelHue += 256;
  delay(20);
}