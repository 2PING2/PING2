from pingpy.debug import logger

import pygame
import time

class SpeakerOutput:
    def __init__(self):
        self.audioPiste = None
        self.isBusy = False
        try:
            pygame.init()
            pygame.mixer.init()
        except Exception as e:
            logger.write_in_log("ERROR", "gameMode", "cycle", "Error in initializing audio:{}".format(e))
        logger.write_in_log("INFO", __name__, "__init__")

    def play(self):
        
        self.isBusy = pygame.mixer.get_busy()
        if self.isBusy:
            return
        if self.audioPiste is None:
            return
        
        try:
            self.isBusy = True
            sound = pygame.mixer.Sound(self.audioPiste)
            sound.play()

        except FileNotFoundError:
            logger.write_in_log("ERROR", "gameMode", "cycle", "Audio file missing:{}".format(self.audioPiste))
        except Exception as e:
            logger.write_in_log("ERROR", "gameMode", "cycle", "Error in playing audio:{}".format(e))
            

    def duration(self, audio_file):
        # try :
        #     return pygame.mixer.Sound(audio_file).get_length()
        # except Exception as e:
        #     logger.write_in_log("ERROR", "gameMode", "cycle", "Audio file missing:{}".format(audio_file))
        #     return 0
        return 0


