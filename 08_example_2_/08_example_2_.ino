#define LED_PIN 7

int period_us = 1000;  // 기본 주기 (1ms)
int duty = 0;          // duty (%)
int step = 1;          // duty 변화 방향 (+1 또는 -1)

// 자동 period 전환용
unsigned long lastChange = 0;
int periodStage = 0;  // 0: 10ms, 1: 1ms, 2: 0.1ms

void set_period(int period) {
  if (period < 100) period = 100;
  if (period > 10000) period = 10000;
  period_us = period;
}

void set_duty(int d) {
  if (d < 0) d = 0;
  if (d > 100) d = 100;
  duty = d;
}

void pwmWrite() {
  int on_time = period_us * duty / 100;
  int off_time = period_us - on_time;

  digitalWrite(LED_PIN, HIGH);
  delayMicroseconds(on_time);

  digitalWrite(LED_PIN, LOW);
  delayMicroseconds(off_time);
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600); 

  set_period(10000);  // 시작은 10ms
}

void loop() {
  // PWM 한 주기 출력
  pwmWrite();

  // duty 값 업데이트 (삼각파)
  duty += step;
  if (duty >= 100 || duty <= 0) step = -step;

  // Serial Plotter에 출력
  Serial.println(duty);

  // 20초마다 period 전환 (10ms → 1ms → 0.1ms 순환)
  if (millis() - lastChange >= 20000) {
    lastChange = millis();
    periodStage = (periodStage + 1) % 3;
    if (periodStage == 0) set_period(10000); // 10ms
    if (periodStage == 1) set_period(1000);  // 1ms
    if (periodStage == 2) set_period(100);   // 0.1ms
  }
}
