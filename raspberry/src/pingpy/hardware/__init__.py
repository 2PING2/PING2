from .autoSwitch import AutoSwitch
from .ledStrip import LedStrip
from pingpy.config.config import LED_STRIP_PIN, NUMBER_OF_LEDS, FREQUENCY, DMA_CHANNEL, MAX_BRIGHTNESS

ledStrip = LedStrip(LED_STRIP_PIN, NUMBER_OF_LEDS, FREQUENCY, DMA_CHANNEL, MAX_BRIGHTNESS)
