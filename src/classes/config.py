
####################################
# RASPBERY PI SETTINGS
####################################
MAX_VOLUME = 100


####################################
# SERIAL SETTINGS
####################################
BAUD_RATE = 115200
TIMEOUT = 1
RETRY_ATTEMPTS = 3  # Number of attempts to reset a port in case of a failed connection
RETRY_DELAY = 2  # Delay between each reset attempt (in seconds)

ports = {
    "UICorner": "/dev/UICorner",
    "Player1": "/dev/Player1",
    "Player2": "/dev/Player2",
    "Player3": "/dev/Player3",
    "Player4": "/dev/Player4",
    "ESP32": "/dev/ESP32"
}

####################################
# LED STRIP SETTINGS
####################################
LED_STRIP_PIN = 10
NUMBER_OF_LEDS = 176
FREQUENCY = 800000
DMA_CHANNEL = 10
MAX_BRIGTHNESS = 200
PLAYER_OFFSETS = {
    1: (132, 176),
    2: (88, 132),
    3: (44, 88),
    4: (0, 44)
} 

GREEN = (0, 255, 0)
RED = (255, 0, 0)
ORANGE = (255, 165, 0)
YELLOW = (255, 255, 0)


####################################
# AUTO SWITCH / LED SETTINGS 
####################################
AUTO_SWITCH1_PIN = 5
AUTO_LED1_PIN = 6
AUTO_SWITCH2_PIN = 17
AUTO_LED2_PIN = 27
AUTO_SWITCH3_PIN = 26
AUTO_LED3_PIN = 21
AUTO_SWITCH4_PIN = 16
AUTO_LED4_PIN = 20




