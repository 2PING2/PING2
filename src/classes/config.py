
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


####################################
# LED STRIP SETTINGS
####################################
GPIO_PIN = 10
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
