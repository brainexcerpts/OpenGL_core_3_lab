#version 150

//FR: Données en entrée (par fragments et non par sommets)
// Les valeurs "in" sont interpolés linéairement entre chaque sommet du triangle

//EN: Input values (per fragment and not per vertex)
// "in" values are linearly interpolated between each vertices over the triangle.
in vec3 varColor;
in vec3 varNormal;
in vec4 varTexCoord;

// FR: Couleur de sortie du fragment
// EN: Output color of the fragment
out vec4 outColor;

void main(void) {
    outColor = vec4( normalize(varNormal), 1.0);

    // FR: Différentes lignes à tester et comprendre:
	// EN: various lines to test and to be understood:
//    outColor = vec4( varColor,1.0);
//    outColor = varTexCoord;
//    outColor = vec4 (1.0, 1.0, 1.0, 1.0);
}

