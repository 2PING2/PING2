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

try:
    from rpi_ws281x import PixelStrip, Color
except ImportError:
    from .rpi_ws281xMock import PixelStrip, Color

import time
from pingpy.config.config import MAX_BRIGTHNESS
from pingpy.debug import logger

''' LedStrip class useful for the management of the LED strip. '''
class LedStrip:
    def __init__(self, LED_STRIP_PIN, NUMBER_OF_LEDS, FREQUENCY, DMA_CHANNEL, BRIGHTNESS=50):
        """Init the LED strip.""" 
        self.strip = PixelStrip(NUMBER_OF_LEDS, LED_STRIP_PIN, FREQUENCY, DMA_CHANNEL, invert=False, brightness=BRIGHTNESS)
        logger.write_in_log("INFO", "ledStrip", "__init__", "LED strip created")
     
    def setup(self):
        """Config the LED strip."""
        try:
            result = self.strip.begin()
            # light one all the LEDs
            logger.write_in_log("INFO", "ledStrip", "__init__", f"LED strip setup: {result}")
        except Exception as e:
            logger.write_in_log("ERROR", "ledStrip", "__init__", f"Failed to initialize LED strip: {e}")
                         
    def setLedStrip(self, color, OFFSET_MIN, OFFSET_MAX):
        """Set the LED strip between OFFSET_MIN and OFFSET_MAX to a color."""
        if OFFSET_MIN < 0 or OFFSET_MAX > self.strip.numPixels() or OFFSET_MIN >= OFFSET_MAX:
            logger.write_in_log("ERROR", __name__, f"Invalid offset : {OFFSET_MIN, OFFSET_MAX}")
            return
        # if not isinstance(color, int) or color < 0:
        #     logger.write_in_log("ERROR", __name__, f"Invalid color value: {color}")
        #     return
        try:
            for i in range(OFFSET_MIN, OFFSET_MAX):
                self.strip.setPixelColor(i, color)
            # logger.write_in_log("INFO", __name__, f"LED strip set to {color} between {OFFSET_MIN} and {OFFSET_MAX}")
        except Exception as e:
                logger.write_in_log("ERROR", __name__, "setLedStrip", f"Failed to set LED strip: {e}")
    def show(self):
        self.strip.show()
                   
    def setBrightness(self, brightness):
        """Set the brightness of the LED strip."""
        if not isinstance(brightness, int) or brightness < 0 or brightness > MAX_BRIGTHNESS:
            logger.write_in_log("ERROR", "LedStrip", "setBrightness", f"Invalid brightness value: {brightness}")
            return
        self.strip.setBrightness(brightness)
        self.strip.show()
    
    def clear(self):
        '''Clear the LED strip'''
        self.setLedStrip(Color(0, 0, 0), 0, self.strip.numPixels())

    def onLedStrip(self, r, g, b):
        """Turn on the LED strip."""
        self.setLedStrip(Color(r,g,b), 0, self.strip.numPixels())

class PlayerLedStrip:    
    def __init__(self, ledStrip, minAndMax):
        """Init the player LED strip."""
        self.ledStrip = ledStrip
        self.min, self.max = minAndMax
        self.n_led = self.max - self.min
        self.n_led_per_mm = 144.0 / 1000.0
        self.len_mm = self.n_led / self.n_led_per_mm
        self.min_mm = -self.len_mm / 2
        self.max_mm = self.len_mm / 2
        
    def onPlayer(self, color):
        """Turn on all the LEDs."""
        self.ledStrip.setLedStrip(Color(color[0],color[1],color[2]), self.min, self.max)
    
    def set_mm(self, area_mm, color):
        """Set the player LED strip."""
        # compute the index of min and max
        min_mm, max_mm = area_mm
        min_led = round(self.n_led/2 + min_mm * self.n_led_per_mm)
        max_led = round(self.n_led/2 + max_mm * self.n_led_per_mm)
        # logger.write_in_log("INFO", __name__, f"min_mm {min_mm} -> {min_led} and max_mm {max_mm} -> {max_led}")

        self.set_led_index([min_led, max_led], color)
        
    def set_led_index(self, area_led_index, color):
        min_led, max_led = area_led_index
        min_led += self.min
        max_led += self.min
        
        """Set the player LED strip."""
        if min_led < self.min:
            min_led = self.min
        if max_led > self.max:
            max_led = self.max
        self.ledStrip.setLedStrip(Color(color[0],color[1],color[2]), min_led, max_led)
         
    def clearPlayer(self):
        """Clear all the LEDs."""
        self.onPlayer(Color(0, 0, 0))
    
        
'''
from led_strip import LedStrip, PlayerLedStrip
'''

''' 
# Création d'un objet LedStrip (pour un bandeau LED complet)
led_strip = LedStrip(LED_STRIP_PIN, NUMBER_OF_LEDS, FREQUENCY, DMA_CHANNEL, BRIGHTNESS) 

player1led = PlayerLedStrip(led_strip, PLAYER_OFFSETS[1])

player1led.onPlayer(Color(255, 0, 0))
time.sleep(1)
player1led.clearPlayer()

led_strip.onLedStrip(Color(255, 0, 0))

'''