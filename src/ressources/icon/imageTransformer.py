from PIL import Image
import os

# Parcourir tous les fichiers dans le répertoire courant
for filename in os.listdir('.'):
    # Vérifier si le fichier est une image .png
    if filename.endswith('.png'):
        img = Image.open(filename)

        # Si l'image n'est pas de taille 40x40, la redimensionner
        if img.size != (40, 40):
            #img = img.resize((40, 40))
            new_img = Image.new('RGBA', (40, 40))  # Créer une nouvelle image de taille 40x40
            new_img.paste(img, ((40 - img.width) // 2, (40 - img.height) // 2))  # Coller l'image originale au centre
            img = new_img

        # Changer les pixels transparents en gris
        data = img.getdata()

        new_data = []
        for item in data:
            # changer tous les pixels transparents
            if len(item) == 4 and item[3] == 0:
                new_data.append((82, 82, 82, 255))
            else:
                new_data.append(item)

        img.putdata(new_data)

        # Sauvegarder l'image
        img.save(filename)
