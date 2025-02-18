"""
This file is part of the PING² project.
Copyright (c) 2024 PING² Team

This code is licensed under the Attribution-NonCommercial-NoDerivatives 4.0 International (CC BY-NC-ND 4.0).
You may share this file as long as you credit the original author.

RESTRICTIONS:
- Commercial use is prohibited.
- No modifications or adaptations are allowed.
- See the full license at: https://creativecommons.org/licenses/by-nc-nd/4.0/

For inquiries, contact us at: projet.ping2@gmail.com
"""


####################################
# UICorner SETTINGS
####################################
RESET_DELAY_AFTER_BUTTON_PRESS = 6
SHORT_PRESS_DELAY = 0.5
LONG_PRESS_DELAY = 2.5

####################################
# SERIAL SETTINGS
####################################
BAUD_RATE = 115200
UI_CORNER_BAUD_RATE = 9600

TIMEOUT = 0.2  # Timeout for the serial communication (in seconds)
RETRY_ATTEMPTS = 3  # Number of attempts to reset a port in case of a failed connection
RETRY_DELAY = 2  # Delay between each reset attempt (in seconds)

ports = {
    "UICorner": "/dev/UICorner",
    "Player" : ["/dev/player1", "/dev/player2", "/dev/player3", "/dev/player4"],
    "ESP32": "/dev/ESP32"
}
PORT_ESP32 = ports["ESP32"]

####################################
# CONTROLLER TYPES KEYS
####################################
CONTROLLER_TYPE_3BUTTONS = "3buttons"
CONTROLLER_TYPE_1BUTTON_1JOYSTICK = "1button_1joystick"

####################################
# GIT UPDATE
####################################
ROOT_PATH = '/home/pi/Documents'  # Root path of the project
import os
os.environ["SDL_AUDIODRIVER"] = "alsa"
GIT_CLONE_PATH = os.path.join(ROOT_PATH, "PING2")# Github repository path
GIT_BRANCH = 'dev'  # Branch to check for updates
# Update with the correct files (all files to check)
ESP_FIRMWARE_PATH = "esp32/.pio/build/esp32dev/firmware.bin"
ESP_BOOTLOADER_PATH = "esp32/.pio/build/esp32dev/bootloader.bin"
ESP_PARTITION_PATH = "esp32/.pio/build/esp32dev/partitions.bin"


FILE_AND_FOLDER_TO_CHECK = ["raspberry/src", ESP_FIRMWARE_PATH, ESP_BOOTLOADER_PATH, ESP_PARTITION_PATH]  # Files and folders to check for updates

HOTSPOT_TIMEOUT = 300  # Timeout for the hotspot setup in seconds
CHECK_WIFI_DELAY = 5  # Delay between each Wi-Fi check in seconds

DEBUG_PRINT_IN_TERMINAL = True  # Print debug messages in the monitor

HTML_PATH = 'index.html' # Update with the correct path
CSS_PATH = 'styles.css'  # Update with the correct path

####################################
# SERIAL KEYS FOR CONTROLLER
####################################
SEP_KEY = '/'
RESET_KEY = 'reset'
MODE_KEY = 'mode'
MODE_PB_KEY = 'mode_pb'
VOLUME_KEY = 'volume'
LIGHT_KEY = 'light'
LEVEL_KEY = 'level'

LEFT_BUTTON_KEY = "left"
RIGHT_BUTTON_KEY = "right"
SHOOT_BUTTON_KEY = "shoot"
INCREMENT_KEY = "increment"
DECREMENT_KEY = "decrement"
PUSH_KEY = "push"
RELEASE_KEY = "release"

####################################
# SERIAL KEYS FOR ESP32
####################################
KEY_SEP = '/'
PARAM_BEGIN_SEP = '{'
PARAM_END_SEP = '}'
LINE_SEP = '\n'
PLAYER_KEY = ("P")
MOVE_TO_KEY = ("MT")
CURRENT_POSITION_KEY = ("CP")
CURRENT_SPEED_KEY = ("CS")
CURRENT_ACCELERATION_KEY = ("CA")

SET_KEY = ("S")
MAX_SPEED_KEY = ("MS")
MAX_ACCELERATION_KEY = ("MA")
CALIBRATION_KEY = ("C")
RIGHT_LIMIT_KEY = ("RL")
LEFT_LIMIT_KEY = ("LL")
SOL_SATE_KEY = ("SS")
STOP_KEY = ("S")
BUSY_KEY = ("B")

MOVE_TO_LEFT_LIMIT_KEY = (MOVE_TO_KEY+LEFT_LIMIT_KEY)
MOVE_TO_RIGHT_LIMIT_KEY = (MOVE_TO_KEY+RIGHT_LIMIT_KEY)
SET_MAX_SPEED_KEY = (SET_KEY+MAX_SPEED_KEY)
SET_MAX_ACCELERATION_KEY = (SET_KEY+MAX_ACCELERATION_KEY)
SET_SOL_STATE_KEY = (SET_KEY+SOL_SATE_KEY)

ASK_KEY = ("A")
ASK_CURRENT_POSITION_KEY = (ASK_KEY+CURRENT_POSITION_KEY)
ASK_CURRENT_SPEED_KEY = (ASK_KEY+CURRENT_SPEED_KEY)
ASK_CURRENT_ACCELERATION_KEY = (ASK_KEY+CURRENT_ACCELERATION_KEY)
ASK_MAX_SPEED_KEY = (ASK_KEY+MAX_SPEED_KEY)
ASK_CALIBRATED = (ASK_KEY+CALIBRATION_KEY)
ASK_RIGHT_LIMIT_KEY = (ASK_KEY+RIGHT_LIMIT_KEY)
ASK_LEFT_LIMIT_KEY = (ASK_KEY+LEFT_LIMIT_KEY)
ASK_SOL_STATE_KEY = (ASK_KEY+SOL_SATE_KEY)

ASK_STATUS_SETTINGS = ("ask_status_settings")

STATUS_LED_KEY = ("status_led")
STATUS_LED_ON = ("on")
STATUS_LED_OFF = ("off")
STATUS_LED_BLINK = ("blink")
STATUS_LED_FADEOUT = ("fadeOut")
####################################
# LED STRIP SETTINGS
####################################
LED_STRIP_PIN = 10
NUMBER_OF_LEDS = 176
FREQUENCY = 800000
DMA_CHANNEL = 10
MAX_BRIGHTNESS = 0.05
PLAYER_LED_STRIP_OFFSETS = {
    1: (132, 176),
    2: (88, 132),
    3: (44, 88),
    4: (0, 44)
} 

GREEN = (0, 255, 0)
RED = (255, 0, 0)
ORANGE = (255, 165, 0)
YELLOW = (255, 255, 0)
BLUE = (30, 30, 255)
PURPLE = (159, 19, 187)
WHITE = (255, 255, 255)

####################################
# AUTO SWITCH / LED SETTINGS 
####################################
AUTO_PIN = {"switch" : [5,23,24,16], "led" : [6,27,21,20]}


####################################
# AUDIO SETTINGS
####################################
MAX_VOLUME = 1.00
PATH_AUDIO = ("raspberry/src/pingpy/audio/")

PATH_AUDIO_BEGIN_GAME = PATH_AUDIO + "C_est_partie.wav"
PATH_AUDIO_GAGNE = PATH_AUDIO + "a_gagne.wav"
PATH_AUDIO_PLAYER_BLEU = PATH_AUDIO + "Le_joueur_bleu.wav"
PATH_AUDIO_PLAYER_ROUGE = PATH_AUDIO + "Le_joueur_rouge.wav"
PATH_AUDIO_PLAYER_JAUNE = PATH_AUDIO + "Le_joueur_jaune.wav"
PATH_AUDIO_PLAYER_VERT = PATH_AUDIO + "Le_joueur_vert.wav"

# redLightGreenLight
PATH_AUDIO_123SOLEIL = PATH_AUDIO + "redLightGreenLight/"
PATH_AUDIO_123SOLEIL_INTRO = PATH_AUDIO_123SOLEIL + "Intro_redLightGreenLight.wav"
PATH_AUDIO_123SOLEIL_123 = PATH_AUDIO_123SOLEIL + "123.wav"
PATH_AUDIO_123SOLEIL_SOLEIL = PATH_AUDIO_123SOLEIL + "Soleil.wav"

# light tracker
PATH_AUDIO_LIGHT_TRACKER = PATH_AUDIO + "lightTracker/"
PATH_AUDIO_LIGHT_TRACKER_INTRO = PATH_AUDIO_LIGHT_TRACKER + "Intro_LightTracker.wav"
PATH_AUDIO_LIGHT_TRACKER_BEGIN_ROUND = PATH_AUDIO_LIGHT_TRACKER + "Top_depart.wav"
PATH_AUDIO_LIGHT_TRACKER_RED_PLAYER_WIN_ROUND = PATH_AUDIO_LIGHT_TRACKER + "Joueur_rouge_manche.wav"
PATH_AUDIO_LIGHT_TRACKER_BLUE_PLAYER_WIN_ROUND = PATH_AUDIO_LIGHT_TRACKER + "Joueur_bleu_manche.wav"
PATH_AUDIO_LIGHT_TRACKER_YELLOW_PLAYER_WIN_ROUND = PATH_AUDIO_LIGHT_TRACKER + "Joueur_jaune_manche.wav"
PATH_AUDIO_LIGHT_TRACKER_GREEN_PLAYER_WIN_ROUND = PATH_AUDIO_LIGHT_TRACKER + "Joueur_vert_manche.wav"

# sand box
PATH_AUDIO_SANDBOX = PATH_AUDIO + "sandBox/"
PATH_AUDIO_SANDBOX_INTRO = PATH_AUDIO_SANDBOX + "Intro_sandBox.wav"

####################################
# redLightGreddLight SETTINGS
####################################
MAX_REACTION_TIME = 1.5