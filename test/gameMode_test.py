"""Ce fichier permet de tester les méthodes de la classe GameMode"""
import unittest
import time
from datetime import datetime
from unittest.mock import MagicMock
from ..src.pingpy.input import RedLightGreenLight
from ..src.pingpy.input import Input, PlayerInput, BeamSwitch, LinearActuatorInput, GameController3button
from ..src.pingpy.output import Output, PlayerOutput, Led, LinearActuatorOutput
from ..src.pingpy.config import GREEN, ORANGE, YELLOW


class TestRedLightGreenLight(unittest.TestCase):

    def setUp(self):
        """
        Prépare les données d'entrée (Input) et de sortie (Output) pour les tests.
        """
        self.game_mode = RedLightGreenLight()
        
        # Simuler les entrées
        self.input_data = Input()
        player_input_1 = PlayerInput(
            BeamSwitch(isBeamSwitchOn=True),
            LinearActuatorInput(leftLimit=0, rightLimit=100, currentPose=0),
            GameController3button(inAction=False),
            id_player=0
        )
        self.input_data.ListPlayerInput = [player_input_1]

        # Simuler les sorties
        self.output_data = Output()
        player_output_1 = PlayerOutput(
            Led(color=None, intensity=0),
            LinearActuatorOutput(moveToRight=False, move_to_leftLimit=False)
        )
        self.output_data.ListPlayerOutput = [player_output_1]

        # Configurer les dépendances mockées
        self.output_data.speaker = MagicMock()
        self.output_data.speaker.duration = MagicMock(return_value=2)
        self.output_data.speaker.audioPiste = None

    def test_setup(self):
        """Tester la méthode setup."""
        self.game_mode.setup(self.input_data, self.output_data)

        self.assertTrue(self.game_mode.initialized)
        self.assertEqual(self.output_data.speaker.audioPiste, r"audio\redLightGreenLight\Intro_123Soleil.wav")
        self.assertTrue(self.output_data.ListPlayerOutput[0].linearActuator.moveTo is not None)

    def test_can_move_during_green_light(self):
        """Tester si les joueurs peuvent bouger pendant le feu vert."""
        self.game_mode.timeInit = time.time()
        self.game_mode.durationGreenLight = 5
        current_time = self.game_mode.timeInit + 1  # Feu vert
        self.assertTrue(self.game_mode.can_move(current_time))

    def test_cannot_move_during_red_light(self):
        """Tester si les joueurs ne peuvent pas bouger pendant le feu rouge."""
        self.game_mode.timeInit = time.time()
        self.game_mode.durationGreenLight = 5
        self.game_mode.durationRedLight = 5
        current_time = self.game_mode.timeInit + self.game_mode.durationGreenLight + 1  # Feu rouge
        self.assertFalse(self.game_mode.can_move(current_time))

    def test_check_action_green_light(self):
        """Tester les actions des joueurs pendant le feu vert."""
        self.game_mode.timeInit = time.time()
        self.game_mode.durationGreenLight = 5

        player = self.input_data.ListPlayerInput[0]
        player.gameController.inAction = True
        current_time = self.game_mode.timeInit + 1  # Lumière verte

        self.game_mode.check_action(player, self.output_data.ListPlayerOutput[0], current_time)
        output = self.output_data.ListPlayerOutput[0]

        self.assertTrue(output.linearActuator.moveToRight)
        self.assertEqual(output.Led.color, GREEN)

    def test_check_action_red_light(self):
        """Tester les actions des joueurs pendant le feu rouge."""
        self.game_mode.timeInit = time.time()
        self.game_mode.durationGreenLight = 5
        self.game_mode.durationRedLight = 5

        player = self.input_data.ListPlayerInput[0]
        player.gameController.inAction = True
        player.linearActuator.leftLimit = 0
        current_time = self.game_mode.timeInit + self.game_mode.durationGreenLight + 1  # Lumière rouge

        self.game_mode.check_action(player, self.output_data.ListPlayerOutput[0], current_time)
        output = self.output_data.ListPlayerOutput[0]

        self.assertFalse(output.linearActuator.moveToRight)
        self.assertEqual(output.Led.color, ORANGE)

    def test_victory_condition(self):
        """Tester la détection de la victoire."""
        player = self.input_data.ListPlayerInput[0]
        player.linearActuator.currentPose = player.linearActuator.rightLimit

        result = self.game_mode.check_victory(player, self.output_data)
        self.assertTrue(result)

        output = self.output_data.ListPlayerOutput[0]
        self.assertEqual(output.Led.color, YELLOW)

    def test_long_run(self):
        """Simuler un test prolongé où les actions des manettes changent et le jeu évolue au fil du temps."""
        self.game_mode.setup(self.input_data, self.output_data)

        simulation_duration = 6  # secondes
        start_time = time.time()
        current_time = start_time

        while current_time - start_time < simulation_duration:
            for player in self.input_data.ListPlayerInput:
                player.gameController.inAction = ((current_time - start_time) % 2 == 0)  # Action change toutes les 2 secondes
                if player.gameController.inAction:
                    player.linearActuator.currentPose += 10

            self.game_mode.compute(self.input_data, self.output_data)

            # Affichage pour le suivi
            output = self.output_data.ListPlayerOutput[0]
            print(f"Time: {datetime.fromtimestamp(current_time).strftime('%H:%M:%S')}")
            print(f"Player Action: {player.gameController.inAction}")
            print(f"Player LED Color: {output.Led.color}")
            print(f"Player LinearActuatorOutput moveToRight: {output.linearActuator.moveToRight}")

            time.sleep(1)
            current_time = time.time()


if __name__ == "__main__":
    unittest.main()


