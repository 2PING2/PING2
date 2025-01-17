from pingpy.debug import logger

import pygame
import time

class SpeakerOutput:
    def __init__(self):
        self.audioPiste = None
        pygame.mixer.init()

    def play(self, audio_file):
        self.audioPiste = audio_file
        try:
            pygame.mixer.music.load(audio_file)
            pygame.mixer.music.play()
            while pygame.mixer.music.get_busy():
                pygame.time.Clock().tick(10)
            self.lastPlayedAudio = audio_file
        except FileNotFoundError:
            logger.write_in_log("ERROR", "gameMode", "cycle", "Audio file missing:{}".format(audio_file))

    def duration(self, audio_file):
        return pygame.mixer.Sound(audio_file).get_length()


