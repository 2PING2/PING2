# mock_rpi_ws281x.py

from pingpy.debug import logger

logger.write_in_log("INFO", "rpi_ws281xMock", "__init__", "rpi_ws281x mock loaded")

class PixelStrip:
    def __init__(self, num_pixels, pin, freq_hz, dma_channel, invert=False, brightness=255):
        self.num_pixels = num_pixels
        self.pin = pin
        self.freq_hz = freq_hz
        self.dma_channel = dma_channel
        self.invert = invert
        self.brightness = brightness
        self.pixels = [0] * num_pixels  # Initialise les pixels à la couleur "éteinte" (0)

    def begin(self):
        print("Simulating PixelStrip.begin()")

    def setPixelColor(self, index, color):
        if index < 0 or index >= self.num_pixels:
            raise ValueError("Pixel index out of range")
        self.pixels[index] = color  # Simule l'attribution de la couleur au pixel

    def show(self):
        pass
        # print("Simulating PixelStrip.show()")
        # print("Current Pixel Colors:", self.pixels)

    def setBrightness(self, brightness):
        if brightness < 0 or brightness > 255:
            raise ValueError("Brightness out of range")
        self.brightness = brightness
        print(f"Simulating setting brightness to {brightness}")
    
    def numPixels(self):
        return self.num_pixels

class Color:
    def __init__(self, r, g, b):
        self.r = r
        self.g = g
        self.b = b

    def __repr__(self):
        return f"Color({self.r}, {self.g}, {self.b})"

    def __eq__(self, other):
        return self.r == other.r and self.g == other.g and self.b == other.b