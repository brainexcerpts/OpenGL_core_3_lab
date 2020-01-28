#version 150

// Données en entrée (par fragments et non par sommets)
// Les valeurs "in" sont interpolés linéairement entre chaque sommet du triangle
in vec3 varColor;
in vec3 varNormal;
in vec4 varTexCoord;

// Couleur de sortie du fragment
out vec4 outColor;

void main(void) {
    outColor = vec4( normalize(varNormal), 1.0);

    // Différentes lignes à tester et comprendre:
//    outColor = vec4( varColor,1.0);
//    outColor = varTexCoord;
//    outColor = vec4 (1.0, 1.0, 1.0, 1.0);
}

