'''
La classe HoughCircle implémente un accumulateur de Hough spécialisé pour détecter des cercles avec une approche optimisée.

Motivation :
Dans le cadre du projet PING², il est essentiel d’estimer la position d’une balle sur un plateau de jeu en utilisant 
un système de vision basé sur deux caméras placées dans les coins adjacents. La détection de la balle nécessite une 
méthode robuste pour distinguer sa forme circulaire des autres objets potentiellement présents sur le plateau.
En complément de caractéristiques comme la couleur et le mouvement, la transformée de Hough est ici utilisée pour
identifier la forme de la balle, caractérisée par sa circularité.

L’algorithme classique de la transformée de Hough pour les cercles est cependant coûteux en calculs, et les ressources 
nécessaires peuvent ne pas être justifiées dans notre contexte spécifique. Cette approche propose donc une optimisation
du calcul de l'accumulateur de Hough, ciblée pour notre application.

Fonctionnalité finale attendue :
Développement d'une fonction de vraisemblance capable de calculer la probabilité de présence d’un cercle de position et 
de rayon donnés sur le plateau. Cette fonction pourra être intégrée au filtre à particules pour améliorer l'estimation 
de la position de la balle.

Description de la méthode :

    Calcul des contours :
        a. Calcul du gradient de Sobel pour obtenir les composantes Gx et Gy (float32).
        b. Calcul de la magnitude du gradient G (float32).
        c. Seuil appliqué à la magnitude du gradient pour obtenir un masque binaire Gt (bool), représentant les contours
            significatifs.
        
    Calcul de l'accumulateur de Hough :
        a. Normalisation des gradients Gx et Gy pour obtenir N_Gx et N_Gy (float32), ce qui permet de connaître les directions 
            normales aux contours.
        b. Détermination des indices de l'accumulateur : Utilisation des directions normalisées pour calculer les indices 
            acc_xyr (float32) représentant les positions et le rayon des potentiels cercles.
        c. Classification des indices en "chunks" (sous-ensembles d'accumulateurs) pour optimiser la mémoire et permettre 
            un calcul ciblé. Les "chunks" sont des tableaux de coordonnées (array) correspondant aux pixels détectés.
    
    Fonction de vraisemblance likelihood(ballPose, additiveNoise) :
        a. Sélection de 4 chunks autour de la position estimée pour la balle.
        b. Calcul de la somme des distances gaussiennes des points dans ces chunks par rapport à ballPose, avec une pondération 
            pour la tolérance au bruit additive (float32).
        c. Normalisation de la réponse pour fournir une valeur de vraisemblance indicative.
        
Différences avec la méthode classique :

    Structure de l’accumulateur :
        L’accumulateur ici n’est pas une matrice de trois dimensions classique (x, y, r), mais une matrice tridimensionnelle de 
        "chunks", chacun contenant uniquement les indices des pixels pertinents. Cela permet une gestion de la mémoire plus efficace.
    
    Réduction de la mémoire : 
        En supposant que les contours occupent peu de pixels par rapport à l'image complète, cette approche utilise beaucoup moins 
        de mémoire que celle de l’accumulateur classique.
    
    Réduction de la redondance de calculs : 
    Les gradients calculés pour les contours sont réutilisés directement pour l’accumulateur sans recalculer de nouvelles informations.

    Optimisation du calcul de la transformation de Hough :
        Plutôt que d’incrémenter les valeurs dans une matrice, l'algorithme ne calcule que les coordonnées des pixels et les regroupe 
        dans des chunks. De plus, la détection des maximums dans l'accumulateur n'est pas requise, ce qui réduit encore les calculs.

    Calcul de vraisemblance ciblé :
        La fonction de vraisemblance ne traite que les chunks sélectionnés, au lieu de travailler sur l’ensemble de l’accumulateur, 
        ce qui réduit la complexité du calcul et le rend plus rapide.
'''
'''
version sans les chunks et pour un seul rayon
'''
import numpy as np
import cv2


class ParticlesInCamFrame:
    def __init__(self):
        self.x = np.array([])
        self.y = np.array([])
        self.xy = np.column_stack((self.x, self.y))
        self.r = 10
        self.likelihood = np.array([])

# class AccChunk:
#     def __init__(self):
#         self.x0 = None
#         self.y0 = None
#         self.d = None
#         self.chunks = np.array([])
        
#     def computeChunkSize(self, uniqueRadiusParticles):
#         '''
#         Calcul des "chunks" de l'accumulateur de Hough.
#         '''
#         self.x0 = np.min(uniqueRadiusParticles.x)-uniqueRadiusParticles.r
#         self.y0 = np.min(uniqueRadiusParticles.y)-uniqueRadiusParticles.r
#         self.d = uniqueRadiusParticles.r
#         max_x = np.max(uniqueRadiusParticles.x)+uniqueRadiusParticles.r
#         max_y = np.max(uniqueRadiusParticles.y)+uniqueRadiusParticles.r
#         nx = np.ceil((max_x - self.x0) / self.d).astype(int)
#         ny = np.ceil((max_y - self.y0) / self.d).astype(int)
#         self.chunks.resize((nx, ny), refcheck=False)
#         # ix = (uniqueRadiusParticles.x - self.x0) // self.d
#         # iy = (uniqueRadiusParticles.y - self.y0) // self.d
#         # ... ? trop fatigué pour continuer
        
        

class HoughCircle:
    def __init__(self):
        self.Gx = None
        self.Gy = None
        self.G = None
        self.Gt = None
        self.N_Gx = None
        self.N_Gy = None
        self.acc_xy = None
        # self.rr = None
        # self.acc = [] #[AccChunk() for _ in range(4)]
        
    def computeContours(self, grayImage, threshold = 50):
        '''
        Calcul des contours significatifs de l'image.
        '''
        # a. Calcul du gradient de Sobel
        self.Gx = cv2.Sobel(grayImage, cv2.CV_32F, 1, 0, ksize=3)
        self.Gy = cv2.Sobel(grayImage, cv2.CV_32F, 0, 1, ksize=3)
        # b. Calcul de la magnitude du gradient
        self.G = cv2.magnitude(self.Gx, self.Gy)
        # c. Seuil appliqué à la magnitude du gradient
        # self.Gt = self.G > threshold
        # ou canny
        self.Gt = cv2.Canny(grayImage, 100, 200).astype(bool)
               
    
    def computeHoughAccumulator(self, radius):
        '''
        Calcul de l'accumulateur de Hough pour les cercles.
        '''
        # a. Normalisation des gradients
        self.N_Gx = self.Gx / self.G
        self.N_Gy = self.Gy / self.G
        # b. Détermination des indices de l'accumulateur
        x, y = np.nonzero(self.Gt)
        self.rr = radius
        dy = self.rr * self.N_Gx[x, y]
        dx = self.rr * self.N_Gy[x, y]
        acc_x = np.concatenate((x - dx, x + dx))
        acc_y = np.concatenate((y - dy, y + dy))
        self.acc_xy = np.array([acc_x, acc_y])
        
        
    
    
    def likelihood(self, particles, additiveNoise_factor = 0.5):
        '''
        Calcul de la vraisemblance de la présence d'un cercle de position et de rayon donnés.
        d² = dx**2 + dy**2
        likelihood = exp(-d**2/(2*additiveNoise**2))
        '''
        if particles.r != self.rr:
            return np.zeros(particles.x.shape)
        
        # b. Calcul de la somme des distances gaussiennes
        # d2 = (self.acc_xy - particles.xy)**2 #ValueError: operands could not be broadcast together with shapes (2,496) (400,2), d2 should be (400, 496, 2)
    # Supposons self.acc_xy a une forme (496, 2) et particles.xy a une forme (400, 2)
        d2 = (self.acc_xy.T[np.newaxis, :, :] - particles.xy[:, np.newaxis, :]) # Résultat de forme (400, 496, 2)
        d2 = np.sum(d2**2, axis=2) # Résultat de forme (400, 496)
        
        s = additiveNoise_factor * particles.r
        likelihood = np.exp(-d2 / (2*s**2))
        likelihood = np.sum(likelihood, axis=1)
        
        # c. Normalisation de la réponse par le la circonférence du cercle
        likelihood /= 2*np.pi*self.rr
        
        return likelihood


if __name__ == "__main__":
    def add_salt_and_pepper_noise(image, salt_prob=0.01, pepper_prob=0.01):
        """
        Ajoute du bruit "sel et poivre" à une image.
        
        :param image: Image d'entrée (grayscale ou couleur)
        :param salt_prob: Probabilité d'ajouter un pixel "sel" (blanc)
        :param pepper_prob: Probabilité d'ajouter un pixel "poivre" (noir)
        :return: Image avec bruit "sel et poivre"
        """
        # Créer une copie de l'image pour éviter de modifier l'image d'origine
        noisy_image = image.copy()
        
        # Générer le bruit "sel" (pixels blancs)
        num_salt = int(salt_prob * image.size)
        coords_salt = [np.random.randint(0, i - 1, num_salt) for i in image.shape]
        noisy_image[tuple(coords_salt)] = 255
        
        # Générer le bruit "poivre" (pixels noirs)
        num_pepper = int(pepper_prob * image.size)
        coords_pepper = [np.random.randint(0, i - 1, num_pepper) for i in image.shape]
        noisy_image[tuple(coords_pepper)] = 0
        
        return noisy_image

    print("HoughCircle class")
    import matplotlib.pyplot as plt
    # Création d'une image de test
    img = np.zeros((200, 200), dtype=np.uint8)
    cv2.circle(img, (210, 100), 30, 255, -1)
    cv2.rectangle(img, (120, 120), (180, 180), 255, -1)
    # gaussian blur
    img = cv2.GaussianBlur(img, (7, 7), 0)
    # ajout de bruit
    img = img + 10*np.random.randn(*img.shape) # gaussian noise
    # poivre et sel
    img = add_salt_and_pepper_noise(img)
    img = np.clip(img, 0, 255).astype(np.uint8)
    # visualisation
    plt.imshow(img, cmap='gray')
    plt.show()
    
    H = HoughCircle()
    # filtre median
    prepared_img = cv2.medianBlur(img, 3)
    H.computeContours(prepared_img)
    H.computeHoughAccumulator(30)

    # visualisation
    # plt.imshow(H.Gt, cmap='gray')
    # plt.show()
    
    # maillage de particules avec np.meshgrid
    particles = ParticlesInCamFrame()
    x_min = np.floor(np.min(H.acc_xy[0])).astype(int)
    x_max = np.ceil(np.max(H.acc_xy[0])).astype(int)
    y_min = np.floor(np.min(H.acc_xy[1])).astype(int)
    y_max = np.ceil(np.max(H.acc_xy[1])).astype(int)
    xd = x_max - x_min + 1
    yd = y_max - y_min + 1
    x = np.linspace(x_min, x_max, xd)
    y = np.linspace(y_min, y_max, yd)
    particles.x, particles.y = np.meshgrid(x, y)
    particles.xy = np.column_stack((particles.x.flatten(), particles.y.flatten()))
    particles.r = 30
    # visualisation
    # plt.scatter(particles.x.flatten(), particles.y.flatten())
    # plt.show()
    # calcul de l'accumulateur de Hough
    
    
    # visualisation
    # plt.imshow(img, cmap='gray')
    # plt.scatter(H.acc_xy[0], H.acc_xy[1], color='r', s=1)
    # plt.show()
    
    # calcul de la vraisemblance
    likelihood = H.likelihood(particles, additiveNoise_factor=1/12)
    
    # # visualisation
    # plt.imshow(likelihood.reshape(60, 60))
    # plt.show()
    
    
    
    # 2*2 instead of 1*4
    fig, ax = plt.subplots(2, 2, figsize=(10, 10))
    ax[0, 0].imshow(img, cmap='gray')
    ax[0, 0].set_title('Image')
    ax[0, 1].imshow(H.Gt, cmap='gray')
    ax[0, 1].set_title('Contours')
    ax[1, 0].scatter(H.acc_xy[1], H.acc_xy[0], color='r', s=1)
    ax[1, 0].set_aspect('equal')
    ax[1, 0].set_title('Accumulateur de Hough')
    likelihood_image = ax[1, 1].imshow(likelihood.reshape(yd, xd).T, cmap='viridis')
    ax[1, 1].set_title('Vraisemblance')
    plt.colorbar(likelihood_image, ax=ax[1, 1], orientation='vertical')
    plt.show()
    