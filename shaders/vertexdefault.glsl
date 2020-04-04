#version 150

// FR
// paramètres généraux
// (Constant/uniform par objet déssiné,
// Ne changent que lors d'un appel à glUniform() )
//

// EN
// General parameters
//(Constant/uniform per drawn object,
// will only change upon calling glUniform() )
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 MVP;
uniform mat4 normalMatrix;

// FR
// Données en entré (attributs/valeurs par sommet)
// Le vertex shader est appelé en parallèle par sommet 
// sur les données du VAO actuellement activé.
// ici les variables sont sencées être associées à un index
// (0, 1, 2 etc.) correspondant aux attributs du VAO.

// EN
// Input parameters (per vertex attribute/value)
// The vertex shader is called for each vertex in parallel 
// over the currently activated VAO.
// Here each variable is associated to an index (0, 1, 2 etc.)
// corresponding to the VAO's attributes
in vec3 inPosition;
in vec3 inNormal;
in vec4 inTexCoord;

// FR
// Données de sortie.
// chaque sommet se voit attribuer de nouvelles valeurs 
// (couleur, normale, coordonnées de textures)

// EN
// Output values
// Each vertex is associated to new values
// (color, normal, texture coordinates)
out vec3 varColor;
out vec3 varNormal;
out vec4 varTexCoord;


// FR: Procédure appelé pour CHAQUE sommet en parallèle sur la carte graphique
// EN: Procedure called for EACH vertex in parallel and processed by the GPU
void main(void) 
{
    varColor = inPosition;
    varNormal = (normalMatrix * vec4(inNormal,0.0)).xyz;
    varTexCoord = inTexCoord;

    // FR: gl_Position est une variable "built-in" c-a-d toujours
    // définie par OpenGl. 
	
	// EN: gl_Positionis is a "build-in" variable which means 
	// it is always defined for you by OpenGl.
    gl_Position = MVP*vec4(inPosition.xyz, 1.0);
    
    // FR: Mieux comprendre le pipeline:
    // Tentez de décommenter les lignes suivantes une à une
    // et de comprendre le résultat à l'écran. 
    // (Si vous ne comprenez toujours pas embêtez votre encadrant :) )
	
	// EN: Better understand the graphic pipeline:
	// de-comment one at a time the following lines and 
	// try to understand the result.
	// (If you can't figure it out go and bother your TA :) )
//    gl_Position = vec4(2.*(inTexCoord.xy-0.5), 0.0, 1.0);
//    gl_Position = vec4(inTexCoord.xy, 0.0, 1.0);
//    gl_Position = vec4(varNormal, 1.0);
}
