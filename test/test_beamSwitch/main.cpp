#include <unity.h>
#define EVERYTHING_PUBLIC
#include "BeamSwitch.hpp"
#include "config.h"

BeamSwitch b1(P1_BEAM_R_PIN);
BeamSwitch b2(P2_BEAM_R_PIN);
BeamSwitch b3(P3_BEAM_R_PIN);
BeamSwitch b4(P4_BEAM_R_PIN);

/*
    mesure du temps d'activation : on allume l'émetteur et on mesure le temps que met le récepteur à détecter le signal
    mesure du temps de désactivation : on éteint l'émetteur et on mesure le temps que met le récepteur à ne plus détecter le signal

    protocole de test :
    - initialisation de l'émetteur avec 'emit' à false et on lance la tâche d'émission avec 'setup_common_emitter'
    - initialisation du récepteur avec 'setup' et on lance la tâche de réception
    - on allume l'émetteur en mettant 'emit' à true et en déclenchant le chrono
    - on arrête le chrono quand le récepteur détecte le signal, avec un timeout de 500ms
    - on envoie sur le moniteur série le temps mesuré
    - on éteint l'émetteur en mettant 'emit' à false et en déclenchant le chrono
    - on arrête le chrono quand le récepteur ne détecte plus le signal, avec un timeout de 500ms
*/

void setUp() {
    // Initialisation avant chaque test (laisser vide si inutile)
}

void tearDown() {
    // Nettoyage après chaque test (laisser vide si inutile)
}


int dt[4] = {-1, -1, -1, -1};
unsigned long start = 0, t = 0;

void test_init()
{
    BeamSwitch::setup();

    delay(100);

    TEST_ASSERT_TRUE(true);
}

void test_beamSwitch_emit()
{
    t=0;
    dt[0] = -1;
    dt[1] = -1;
    dt[2] = -1;
    dt[3] = -1;
    start = esp_timer_get_time();
    BeamSwitch::startEmit();
    do
    {
        t = esp_timer_get_time() - start;
        if (b1.getState())
            dt[0] = t;
        if (b2.getState())
            dt[1] = t;
        if (b3.getState())
            dt[2] = t;
        if (b4.getState())
            dt[3] = t;
        
        if (dt[0]>0 && dt[1]>0 && dt[2]>0 && dt[3]>0)
            break;
        
        vTaskDelay(1);
    } while (t < 500000); // 500ms of timeout


    // check for timeout
    TEST_ASSERT_GREATER_THAN(0, dt[0]);
    TEST_ASSERT_GREATER_THAN(0, dt[1]);
    TEST_ASSERT_GREATER_THAN(0, dt[2]);
    TEST_ASSERT_GREATER_THAN(0, dt[3]);
}

void test_beamSwitch_stopEmit()
{
    t=0;
    dt[0] = -1;
    dt[1] = -1;
    dt[2] = -1;
    dt[3] = -1;
    start = esp_timer_get_time();
    BeamSwitch::stopEmit();
    do
    {
        t = esp_timer_get_time()-start;
        if (!b1.getState())
            dt[0] = t;
        if (!b2.getState())
            dt[1] = t;
        if (!b3.getState())
            dt[2] = t;
        if (!b4.getState())
            dt[3] = t;
        if (dt[0]>0 && dt[1]>0 && dt[2]>0 && dt[3]>0)
            break;

        vTaskDelay(1);
    } while (t < 500000); // 500ms of timeout


    // check for timeout
    TEST_ASSERT_GREATER_THAN(0, dt[0]);
    TEST_ASSERT_GREATER_THAN(0, dt[1]);
    TEST_ASSERT_GREATER_THAN(0, dt[2]);
    TEST_ASSERT_GREATER_THAN(0, dt[3]);

}


void setup() 
{
    Serial.begin(115200);
    UNITY_BEGIN();
    RUN_TEST(test_init);
    RUN_TEST(test_beamSwitch_emit);
    RUN_TEST(test_beamSwitch_stopEmit);
    UNITY_END();
    
}

void loop()
{
}