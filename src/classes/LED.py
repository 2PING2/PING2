class Bandeau_LED:
    def __init__(self, num_leds):
        self.num_leds = num_leds
        self.leds = [LED() for _ in range(num_leds)]

