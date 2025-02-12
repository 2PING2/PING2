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
from pingpy.debug import logger

''' LedStrip class useful for the management of the LED strip. '''
class LedStrip:
    def __init__(self, LED_STRIP_PIN, NUMBER_OF_LEDS, FREQUENCY, DMA_CHANNEL, BRIGHTNESS):
        """Init the LED strip.""" 
        self.strip = PixelStrip(NUMBER_OF_LEDS, LED_STRIP_PIN, FREQUENCY, DMA_CHANNEL, invert=False, brightness=255)
        self.n = NUMBER_OF_LEDS
        self.maxCurrent = 3*255 * NUMBER_OF_LEDS * BRIGHTNESS
        logger.write_in_log("INFO", __name__, "__init__", "LED strip created")
     
    def setup(self):
        """Config the LED strip."""
        try:
            result = self.strip.begin()
            # light one all the LEDs
            logger.write_in_log("INFO", __name__, "__init__", f"LED strip setup")
        except Exception as e:
            logger.write_in_log("ERROR", __name__, "__init__", f"Failed to initialize LED strip: {e}")
                         
    def setLedStrip(self, color, OFFSET_MIN, OFFSET_MAX):
        """Set the LED strip between OFFSET_MIN and OFFSET_MAX to a color."""
        if OFFSET_MIN < 0 or OFFSET_MAX > self.strip.numPixels() or OFFSET_MIN > OFFSET_MAX:
            logger.write_in_log("ERROR", __name__, f"Invalid offset : {OFFSET_MIN, OFFSET_MAX}")
            return
        try:
            
            for i in range(OFFSET_MIN, OFFSET_MAX+1):
                self.strip.setPixelColor(i, color)
        except Exception as e:
                logger.write_in_log("ERROR", __name__, "setLedStrip", f"Failed to set LED strip: {e}")
    def show(self):
        # compute the current needed
        current = 0
        for i in range(self.n):
            color = self.strip.getPixelColor(i)
            red = (color >> 16) & 0xFF
            green = (color >> 8) & 0xFF
            blue = color & 0xFF
            current += red + green + blue
        if current > self.maxCurrent:
            coeff = self.maxCurrent / current
            for i in range(self.n):
                color = self.strip.getPixelColor(i)
                red = (color >> 16) & 0xFF
                green = (color >> 8) & 0xFF
                blue = color & 0xFF
                self.strip.setPixelColor(i, Color(int(red*coeff), int(green*coeff), int(blue*coeff)))
        self.strip.show()
                   
    # def setBrightness(self, input_ptr):
    #     """Set the brightness of the LED strip."""
    #     pass
    
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
        self.brightness = 1.0
        
        
    def onPlayer(self, color):
        """Turn on all the LEDs."""
        self.ledStrip.setLedStrip(Color(color[0],color[1],color[2]), self.min, self.max)
    
    def set_mm(self, area_mm, color):
        """Set the player LED strip."""
        # compute the index of min and max
        min_mm, max_mm = area_mm
        min_led = round(self.n_led/2 + min_mm * self.n_led_per_mm)
        max_led = round(self.n_led/2 + max_mm * self.n_led_per_mm)

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
        self.ledStrip.setLedStrip(Color(int(color[0]*self.brightness),int(color[1]*self.brightness),int(color[2]*self.brightness)), min_led, max_led)
    
    def clearPlayer(self):
        """Clear all the LEDs."""
        self.onPlayer(Color(0, 0, 0))
        
    def set_brightness(self, brightness):
        """Set the brightness of the player LED strip."""
        self.brightness = brightness
