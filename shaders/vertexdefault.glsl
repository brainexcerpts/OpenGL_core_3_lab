#version 150

// paramètres généraux
// (Constant/uniform par objet déssiné,
// Ne changent que lors d'un appel à glUniform() )
//
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 MVP;
uniform mat4 normalMatrix;

// Données en entré (attributs par sommet)
// Le vertex shader est appelé en parallèle par sommet 
// sur les données du VAO actuellement activé.
// ici les variables sont sencées être associées à un index
// (0, 1, 2 etc.) correspondant aux attributs du VAO.
in vec3 inPosition;
in vec3 inNormal;
in vec4 inTexCoord;

// Données de sortie.
// chaque sommet se voit attribuer de nouvelles valeurs 
// (couleur, normale, coordonnées de textures)
out vec3 varColor;
out vec3 varNormal;
out vec4 varTexCoord;


// Procédure appelé pour CHAQUE sommet en parallèle sur la carte graphique
void main(void) 
{
    varColor = inPosition;
    varNormal = (normalMatrix * vec4(inNormal,0.0)).xyz;
    varTexCoord = inTexCoord;

    // gl_Position est une variable "built-in" c-a-d toujours
    // définie par OpenGl. 
    gl_Position = MVP*vec4(inPosition.xyz, 1.0);
    
    // Mieux comprendre le pipeline:
    // Tentez de décommenter les lignes suivantes une à une
    // et de comprendre le résultat à l'écran. 
    // (Si vous ne comprenez toujours pas embêtez votre encadrant :) )
//    gl_Position = vec4(2.*(inTexCoord.xy-0.5), 0.0, 1.0);
//    gl_Position = vec4(inTexCoord.xy, 0.0, 1.0);
//    gl_Position = vec4(varNormal, 1.0);
}
