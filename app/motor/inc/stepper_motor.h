// Diamètre poulie moteur = 2cm  ==> circonférence = 4*pi cm ( distance déplacée de la courroie lors d'un tour de moteur, soit 200 pas)

// Pour notre test, dimension table = 70cm x 57.5cm

// Soit NBX le nombre de pas moteur à réaliser pour parcourir une distance x sur l'axe des abscisses
// Soit NBY le nombre de pas moteur à réaliser pour parcourir une distance x' sur l'axe des ordonnées

// On a donc NBX = (x * 200) / 4*pi
// On a donc NBY = (x' * 200) / 4*pi

// Pour avoir le nombre tours, il suffit de faire NBX/200 ( même chose pour NBY )
