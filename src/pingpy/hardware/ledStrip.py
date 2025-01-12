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
from config.config import MAX_BRIGTHNESS
from pingpy.debug.logFile import*
log = LogFile()

''' LedStrip class useful for the management of the LED strip. '''
class LedStrip:
    def __init__(self, LED_STRIP_PIN, NUMBER_OF_LEDS, FREQUENCY, DMA_CHANNEL, BRIGHTNESS=255):
        """Init the LED strip.""" 
        self.strip = PixelStrip(NUMBER_OF_LEDS, LED_STRIP_PIN, FREQUENCY, DMA_CHANNEL, invert=False, brightness=BRIGHTNESS)
     
    def setup(self):
        """Config the LED strip."""
        try:
            self.strip.begin()
            log.write_in_log("INFO", "ledStrip", "__init__", "LED strip initialized")
        except Exception as e:
            log.write_in_log("ERROR", "ledStrip", "__init__", f"Failed to initialize LED strip: {e}")
                         
    def setLedStrip(self, color, OFFSET_MIN, OFFSET_MAX):
        """Set the LED strip between OFFSET_MIN and OFFSET_MAX to a color."""
        if OFFSET_MIN < 0 or OFFSET_MAX > self.strip.numPixels() or OFFSET_MIN >= OFFSET_MAX:
            log.write_in_log("ERROR", "LedStrip", "setLedStrip", "Invalid offset")
            return
        if not isinstance(color, int) or color < 0:
            log.write_in_log("ERROR", "LedStrip", "setLedStrip", f"Invalid color value: {color}")
            return
       
        for i in range(OFFSET_MIN, OFFSET_MAX):
            self.strip.setPixelColor(i, color)
        self.strip.show()
        
    def setBrightness(self, brightness):
        """Set the brightness of the LED strip."""
        if not isinstance(brightness, int) or brightness < 0 or brightness > MAX_BRIGTHNESS:
            log.write_in_log("ERROR", "LedStrip", "setBrightness", f"Invalid brightness value: {brightness}")
            return
        self.strip.setBrightness(brightness)
        self.strip.show()
    
    def clear(self):
        '''Clear the LED strip'''
        self.setLedStrip(Color(0, 0, 0), 0, self.strip.numPixels())

    def onLedStrip(self, color):
        """Turn on the LED strip."""
        self.setLedStrip(color, 0, self.strip.numPixels())

class PlayerLedStrip:    
    def __init__(self, ledStrip, minAndMax):
        """Init the player LED strip."""
        self.ledStrip = ledStrip
        self.min, self.max = minAndMax
        
    def onPlayer(self, color):
        """Turn on all the LEDs."""
        self.ledStrip.setLedStrip(color, self.min, self.max)
        
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