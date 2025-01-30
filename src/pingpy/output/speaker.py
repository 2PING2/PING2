from pingpy.debug import logger
import os
os.environ["SDL_AUDIODRIVER"] = "alsa"
import pygame

 
class SpeakerOutput:
    def __init__(self):
        self.audioPiste = None
        self.isBusy = False
        self.stop = False
        self.volume = None
        try:
            pygame.mixer.pre_init(44100, -16, 2, 2048)  
            pygame.mixer.init()
        except Exception as e:
            logger.write_in_log("ERROR", __name__ , "Error in initializing audio:{}".format(e))
        logger.write_in_log("INFO", __name__, "__init__")

    def play(self):
        
        if self.stop:
            logger.write_in_log("INFO", __name__, "Stop audio")
            pygame.mixer.music.stop()
            self.stop = False
            
        if self.volume is not None:
            pygame.mixer.music.set_volume(self.volume)
            self.volume = None
            
        self.isBusy = pygame.mixer.get_busy()
        # check if it finishes playing
        if self.isBusy:
            return
        if self.audioPiste is None:
            return
        

        if type(self.audioPiste) is list:
            pisteToPLay = self.audioPiste[0]
            if len(self.audioPiste) > 1 :
                self.audioPiste = self.audioPiste[1:]
            else:
                self.audioPiste = None
        else:
            pisteToPLay = self.audioPiste
            self.audioPiste = None
               
        try:
            self.isBusy = True
            pygame.mixer.music.load(pisteToPLay)
            pygame.mixer.music.play()
            logger.write_in_log("INFO", __name__, "Playing audio:{}".format(pisteToPLay))

        except FileNotFoundError:
            logger.write_in_log("ERROR", __name__, "Audio file missing:{}".format(pisteToPLay))
        except Exception as e:
            logger.write_in_log("ERROR", __name__, "Error in playing audio:{}".format(e))
        
            

    def duration(self, audio_file):
        try :
            ret = pygame.mixer.Sound(audio_file).get_length()
            return ret
        except Exception as e:
            logger.write_in_log("ERROR", __name__, "Audio file missing:{}".format(audio_file))
        return 0


