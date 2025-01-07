#include <Arduino.h>

// Paramètres du balayage
const int32_t MIN_VELOCITY = 1000;     // Vitesse minimale (pas/s)
const int32_t MAX_VELOCITY = 100000;    // Vitesse maximale (pas/s)
const int32_t VELOCITY_STEP = 1000;    // Pas d'incrémentation de la vitesse

const uint8_t MIN_CURRENT = 60;        // Courant minimum (%)
const uint8_t MAX_CURRENT = 100;       // Courant maximum (%)
const uint8_t CURRENT_STEP = 5;       // Pas d'incrémentation du courant

const unsigned long STABILIZATION_DURATION = 20000; // Temps de stabilisation avant le début de l'échantillonnage (us)
const unsigned long SAMPLE_DURATION = 500000;        // Durée d'échantillonnage pour chaque condition (us)
const unsigned long SAMPLE_INTERVAL = 1000;           // Intervalle entre chaque mesures (us)

#include <TMC2209.h>

HardwareSerial &serial_stream = Serial2;
TMC2209 stepper_driver;

uint8_t run_current = MIN_CURRENT;
int32_t velocity = MIN_VELOCITY;
uint16_t stall_guard = 0;
int64_t stabilization_start = 0;
int64_t sample_start = 0;
int64_t chunk_start = 0;

void setup()
{
  Serial.begin(115200);

  stepper_driver.setup(serial_stream);

  stepper_driver.setRunCurrent(run_current);
  stepper_driver.setMicrostepsPerStepPowerOfTwo(8);
  stepper_driver.enable();
  stepper_driver.moveAtVelocity(velocity);

  delayMicroseconds(STABILIZATION_DURATION);

  // Envoyer le header une seule fois au début
  Serial.println("HEADER:time<int64>,velocity<int32>,run_current<uint8>,stall_guard<uint16>");
}

void loop()
{
  int64_t current_time = esp_timer_get_time();

  if (current_time - stabilization_start < STABILIZATION_DURATION)
  {
    chunk_start = current_time;
    return;
  }

  if (current_time - sample_start < SAMPLE_INTERVAL)
    return;

  sample_start = current_time;

  stall_guard = stepper_driver.getStallGuardResult();

  // Envoyer les données sous forme binaire
  Serial.write((uint8_t *)&current_time, sizeof(current_time)); // Temps (8 octets)
  Serial.write((uint8_t *)&velocity, sizeof(velocity)); // Vitesse (4 octets)
  Serial.write((uint8_t *)&run_current, sizeof(run_current)); // Courant (1 octet)
  Serial.write((uint8_t *)&stall_guard, sizeof(stall_guard)); // StallGuard (2 octets)
  Serial.println(); // Saut de ligne

  if (current_time - chunk_start < SAMPLE_DURATION + STABILIZATION_DURATION)
    return;

  stabilization_start = current_time;

  if (run_current < MAX_CURRENT)
  {
    run_current += CURRENT_STEP;
    stepper_driver.setRunCurrent(run_current);
    return;
  }

  run_current = MIN_CURRENT;
  stepper_driver.setRunCurrent(run_current);

  if (velocity < MAX_VELOCITY)
  {
    velocity += VELOCITY_STEP;
    stepper_driver.moveAtVelocity(velocity);
    return;
  }

  Serial.println("END_OF_TRANSMISSION"); // Marquer la fin de la transmission
  while (true) {
    delay(1000);
  }

}