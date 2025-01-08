""" Ce fichier correspond à la classe du haut-parleur"""
import pyttsx3

class Speaker:
    def __init__(self, volume, mode, difficulty):
        pass
    
    def __str__(self):
        return "Speaker"
    
    # fonction qui permet au haut-parleur de dire un message
    def say(self, message):
        pyttsx3.speak(message)
        pass