import matplotlib.pyplot as plt


#définition d'une fonction qui prend en argument un fichier .txt
#et qui renvoie un vecteur dont les éléments sont les vecteurs du fichier.

def read_data(file_name):
    all_vectors = []
    current_vector = []  
    with open(file_name, 'r') as file:
        for line in file:
            line = line.strip()
            if line:  
                try:
                    current_vector.append(float(line))
                except ValueError:
                    print(f"Ignorer les données non numériques : {line}")
            else:  
                if current_vector:  
                    all_vectors.append(current_vector)  
                    current_vector = []  

    if current_vector:  
        all_vectors.append(current_vector)  

    return all_vectors

#définition d'une fonction qui prend en argument 
# un vecteur de vecteur (vectors) et un vecteur (vec)
#et qui revoie le graphe de tout les vecteurs de vectors 
#avec pour abscisse les éléments de vec.

def plot_vectors(vectors,vec):
    for i, vector in enumerate(vectors, 1):
        plt.plot(vec,vector)

    plt.xlabel('t')
    plt.ylabel('St')
    plt.title('Graphique pour tous les St')
    plt.legend()
    plt.grid(True)
    plt.show()

#file_name = fichier pathsimul.txt 
#file_name2 = fichier pathsimul2.txt
file_name = 'pathsimul.txt'
file_name2 = 'pathsimul2.txt'

vec= read_data(file_name2)[0]
vectors = read_data(file_name)
plot_vectors(vectors,vec)