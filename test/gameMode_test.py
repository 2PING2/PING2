""" Ce fichier permet de tester les méthodes de la classe GameMode """
import unittest
import sys
import os
import time
from unittest.mock import MagicMock
from datetime import datetime

# Ajoutez le chemin vers le dossier `src` au sys.path
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), "../src")))

# Importer les classes nécessaires
from classes.gameMode import RedLightGreenLight
from classes.input.input import Input, PlayerInput, BeamSwitch, LinearActuatorInput, GameController3button, OtherInput
from classes.output.output import Output, PlayerOutput, Led, LinearActuatorOutput
from classes.ledStrip import LedStrip, PlayerLedStrip
from classes.config import GPIO_PIN, NUMBER_OF_LEDS, FREQUENCY, DMA_CHANNEL, BRIGHTNESS, PLAYER_OFFSETS




class TestRedLightGreenLight(unittest.TestCase):

    def setUp(self):
        """
        Prépare les données d'entrée (Input) et de sortie (Output) pour les tests.
        """
        self.game_mode = RedLightGreenLight()

        # Configuration des joueurs (entrées)
        player_input_1 = PlayerInput(
            BeamSwitch(isBeamSwitchOn=True),
            LinearActuatorInput(leftLimit=0, rightLimit=100, currentPose=0),
            GameController3button(newAction=True),
            id_player=0
        )
        self.input_data = Input(
            ListPlayerInput=[player_input_1],
            OtherInput=OtherInput(ballPose=(10, 20))
        )

        # Configuration des joueurs (sorties)
        player_output_1 = PlayerOutput(
            PlayerLedStrip(LedStrip(GPIO_PIN, NUMBER_OF_LEDS, FREQUENCY, DMA_CHANNEL, BRIGHTNESS), PLAYER_OFFSETS[1]),
            LinearActuatorOutput(move_to_right=False, move_to_leftLimit=False)
        )
        self.game_mode.output_data.ListPlayerOutput.append(player_output_1)
        

    def test_can_move_during_green_light(self):
        current_time = self.game_mode.time_init + 1  # Feu vert
        self.assertTrue(self.game_mode.can_move(current_time))

    def test_cannot_move_during_red_light(self):
        current_time = self.game_mode.time_init + self.game_mode.duration_green_light + 1  # Feu rouge
        self.assertFalse(self.game_mode.can_move(current_time))

    def test_check_action_green_light(self):
        player = self.input_data.ListPlayerInput[0]
        player.GameController.newAction = False

        current_time = self.game_mode.time_init + 1  # Lumière verte
        self.game_mode.check_action(player, current_time)

        output = self.game_mode.output_data.ListPlayerOutput[0]
        self.assertFalse(output.LinearActuatorOutput.move_to_right)
        self.assertEqual(output.Led.color, None)

    def test_check_action_red_light(self):
        player = self.input_data.ListPlayerInput[0]
        player.GameController.newAction = True

        current_time = self.game_mode.time_init + self.game_mode.duration_green_light + 1  # Lumière rouge
        self.game_mode.check_action(player, current_time)

        output = self.game_mode.output_data.ListPlayerOutput[0]
        self.assertTrue(output.LinearActuatorOutput.move_to_leftLimit)
        self.assertEqual(output.Led.color, "orange")

    def test_victory_condition(self):
        player = self.input_data.ListPlayerInput[0]
        player.LinearActuatorInput.currentPose = player.LinearActuatorInput.rightLimit

        self.assertTrue(self.game_mode.check_victory(player))
        output = self.game_mode.output_data.ListPlayerOutput[0]
        self.assertEqual(output.Led.color, "yellow")

    """"tests de la fonction run avec plusieurs joueurs"""
    def test_run(self):
        player_input_2 = PlayerInput(
            BeamSwitch(isBeamSwitchOn=True),
            LinearActuatorInput(leftLimit=0, rightLimit=100, currentPose=0),
            GameController3button(newAction=False),
            id_player=1
        )
        self.input_data.ListPlayerInput.append(player_input_2)

        # Configuration des joueurs (sorties)
        player_output_2 = PlayerOutput(
            Led(color=None, intensity=0),
            LinearActuatorOutput(move_to_right=False, move_to_leftLimit=False)
        )
        self.game_mode.output_data.ListPlayerOutput.append(player_output_2)

        # Exécution de la méthode run
        output_data = self.game_mode.run(self.input_data)

        # Vérification des sorties
        # Joueur 1
        self.assertTrue(output_data.ListPlayerOutput[0].LinearActuatorOutput.move_to_right)
        self.assertEqual(output_data.ListPlayerOutput[0].Led.color, "green")
        # Joueur 2
        self.assertFalse(output_data.ListPlayerOutput[1].LinearActuatorOutput.move_to_right)
        self.assertEqual(output_data.ListPlayerOutput[1].Led.color, "green")
    
    def test_long_run(self):
        """Simule un test prolongé où les actions des manettes changent et le jeu évolue au fil du temps."""
        

        # Ajout d'un deuxième joueur
        player_input_2 = PlayerInput(
            BeamSwitch(isBeamSwitchOn=True),
            LinearActuatorInput(leftLimit=0, rightLimit=100, currentPose=0),
            GameController3button(newAction=False),
            id_player=1
        )
        self.input_data.ListPlayerInput.append(player_input_2)

        # Configuration des joueurs (sorties)
        player_output_2 = PlayerOutput(
            Led(color=None, intensity=0),
            LinearActuatorOutput(move_to_right=False, move_to_leftLimit=False)
        )
        self.game_mode.output_data.ListPlayerOutput.append(player_output_2)

        # Simulation sur une longue période
        simulation_duration = 6  # secondes
        start_time = time.time()
        current_time = start_time

        while current_time - start_time < simulation_duration:
            # Simuler les actions des joueurs
            for idx, player_input in enumerate(self.input_data.ListPlayerInput):
                # Alterner entre `newAction=True` et `newAction=False`
                player_input.GameController.newAction = (int(current_time - start_time) % 2 == 0)  # Changer toutes les 2 secondes

                # Avancer les positions pour simuler le déplacement
                if player_input.GameController.newAction:
                    player_input.LinearActuatorInput.currentPose += 10

            # Exécuter une itération du jeu
            self.game_mode.run(self.input_data)
            # Afficher les inputs et outputs toutes les secondes
            
            print(f"Time: {datetime.fromtimestamp(current_time).strftime('%H:%M:%S')}")
            print(f"Etats Manette Player 1: {self.input_data.ListPlayerInput[0].GameController.newAction}")
            print(f"Etats Manette Player 2: {self.input_data.ListPlayerInput[1].GameController.newAction}")
            print(f"Etats LED Player 1: {self.game_mode.output_data.ListPlayerOutput[0].Led.color} ")
            print(f"Etats LinearActuatorOutput move_to_right Player 1: {self.game_mode.output_data.ListPlayerOutput[0].LinearActuatorOutput.move_to_right}")
            print(f"Etats LinearActuatorOutput move_to_leftLimit Player 1: {self.game_mode.output_data.ListPlayerOutput[0].LinearActuatorOutput.move_to_leftLimit}")
            print(f"Etats LED Player 2: {self.game_mode.output_data.ListPlayerOutput[1].Led.color}")
            print(f"Etats LinearActuatorOutput move_to_right Player 2: {self.game_mode.output_data.ListPlayerOutput[1].LinearActuatorOutput.move_to_right}")
            print(f"Etats LinearActuatorOutput move_to_leftLimit Player 2: {self.game_mode.output_data.ListPlayerOutput[1].LinearActuatorOutput.move_to_leftLimit}")

            time.sleep(1)
            # Avancer dans le temps
            current_time = time.time()



if __name__ == '__main__':
    unittest.main()