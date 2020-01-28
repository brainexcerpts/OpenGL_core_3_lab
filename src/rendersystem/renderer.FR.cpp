/***************************************************************************
 *   Copyright (C) 2012 by Mathias Paulin                                  *
 *   Mathias.Paulin@irit.fr                                                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "renderer.h"

#include "gl_utils/opengl.h"
#include "gl_utils/gldirect_draw.h"
#include "fileloaders/objloader.h"
#include "fileloaders/fileloader.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_access.hpp>

/** @defgroup RendererGlobalFunctions
  * @author Mathias Paulin <Mathias.Paulin@irit.fr>
  * (edited by Rodolphe Vaillant <vaillant@irit.fr>
  * @ingroup RenderSystem
  * Global functions for the renderer.
  *
  *  Bac à sable pour les TPs Opengl le code ci dessous doit être
  *  modifié/complété par les étudiants
  */
// =============================================================================
namespace RenderSystem {
// =============================================================================

/// Initialisation des ressources (Maillage / shaders / lumières ... )
/// (appelée une fois au chargement de l'application)
void Renderer::initRessources()
{
    // #######################################
    // TP 1 / PARTIE I: Début du code à écrire
    // #######################################
    // Dans ce TP les noms de fonctions sont donnés, mais pas leurs usage.
    // Utilisez "google" ou "http://www.opengl.org/sdk/docs/man3/" ou
    // la commande "man" pour trouver les paramètres des fonctions dans la doc.

    initShaders(); // TP 1 / PARTIE I: Shaders initialisation (fonction à remplir)
    initView();    // TP 1 / PARTIE I: Viewing parameters initialisation (fonction à remplir)

    // 1 - Activer le test de profondeur ( glEnable() )
    // N.B 1: Notez la liste impressionnante des différents paramètres de
    // glEnable() dans la doc. Ceci définissent différents "états" d'OpenGl
    // et sont stockés des variables du "contexte Opengl". glEnable() est un
    // accesseur.
    // N.B 2: tous les "états" Opengl peuvent aussi être désactivés avec glDisable()


    // 2 - Positionner le mode de dessin (filaire ou plein glPolygonMode())


    // #####################################
    // TP 1 / PARTIE I: Fin du code à écrire
    // #####################################


    initGeometry(); // TP 1 / PARTIE II: Loading or building geometric data.
}

//------------------------------------------------------------------------------

/// Initialisation des paramètres de vus.
/// (appelée une fois au chargement de l'application)
void Renderer::initView()
{
    // #######################################
    // TP 1 / PARTIE I: Début du code à écrire
    // #######################################
    // Initialiser la matrice de vue (attribut mViewMatrix)
    // à partir des indications :
    // position de vue (eye): 1.f, 0.f, 0.f
    // point visé (center): 0.0f, 0.0f, 0.0f
    // vecteur up : 0.0f, 1.0f, 0.0f
    // remplir l'attribut "mViewMatrix" en utilisant la fonction
    // "glm::lookAt()" pour calculer la matrice de vue correspondante

    // #####################################
    // TP 1 / PARTIE I: Fin du code à écrire
    // #####################################
}

// -----------------------------------------------------------------------------

// (à re-utiliser lors de l'initialisation des shaders)
/// @ingroup RendererGlobalFunctions
/// Print to stderr compilation log of a shader
/// @param shader : identifiant shader Opengl
void printShaderInfoLog(GLint shader)
{
    int infoLogLen = 0;
    int charsWritten = 0;
    GLchar* infoLog;
    glAssert(glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLen));
    if (infoLogLen > 0) {
        infoLog = new GLchar[infoLogLen];
        // error check for fail to allocate memory omitted
        glAssert(glGetShaderInfoLog(shader, infoLogLen, &charsWritten, infoLog));
        std::cerr << "InfoLog:" << std::endl
                  << infoLog << std::endl;
        delete[] infoLog;
    }
}

// -----------------------------------------------------------------------------

// (à re-utiliser lors de l'initialisation des shaders)
/// @ingroup RendererGlobalFunctions
/// Print to stderr link log of a program shader
/// @param program: identifiant program shader in opengl
void printProgramInfoLog(GLint program)
{
    int infoLogLen = 0;
    int charsWritten = 0;
    GLchar* infoLog;
    glAssert(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLen));
    if (infoLogLen > 0) {
        infoLog = new GLchar[infoLogLen];
        // error check for fail to allocate memory omitted
        glAssert(glGetProgramInfoLog(program, infoLogLen, &charsWritten, infoLog));
        std::cerr << "InfoLog:" << std::endl
                  << infoLog << std::endl;
        delete[] infoLog;
    }
}

//------------------------------------------------------------------------------

/// Chargement et compilation des shaders
void Renderer::initShaders()
{

    // #######################################
    // TP 1 / PARTIE I: Début du code à écrire
    // #######################################

    // N.B: reportez vous régulièrement à un schéma du pipeline graphique
    // pour faire la correspondance entre ce que vous programmez ici et comment
    // Opengl le traite. Faute de quoi vous ne comprendrez rien de ce que
    // vous faite, mais soyez patient tout devient plus clair en fin de TP.

    // Dans un premier temps, voici un exemple pour charger un shader
    // correspondant à l'étape du pipeline graphique appelé "vertex shader":
    // 1 - Vertex shader :
    //   1.1 - Charger le code source du vertex shader depuis le fichier "../shaders/vertexdefault.glsl"
    char* vertexShaderSource = Loaders::Text::loadFile("../shaders/vertexdefault.glsl"); // Fonction utilitaire interne au projet

    //   1.2 - Créer un objet OpenGl VERTEX_SHADER et y associer le code source
    //         On utilise l'attribut "mVertexShaderId" pour stocker l'identifiant du vertex Shader
    glAssert(mVertexShaderId = glCreateShader(GL_VERTEX_SHADER));
    glAssert(glShaderSource(mVertexShaderId, 1, (const GLchar**)&vertexShaderSource, NULL));

    //   1.3 - Compiler le shader
    glAssert(glCompileShader(mVertexShaderId));

    //   1.4 - Vérifier les erreurs de compilation
    GLint compiled;
    glAssert(glGetShaderiv(mVertexShaderId, GL_COMPILE_STATUS, &compiled));
    if (!compiled) {
        std::cerr << " Vertex shader not compiled : " << std::endl;
        printShaderInfoLog(mVertexShaderId); // Fonction utilitaire interne au projet
    }

    // !!!!!!!
    // N.B: glAssert() n'est pas une fonction OpenGL.
    // C'est une macro (interne à ce projet) chargée de vérifier les erreurs Opengl.
    // Lors d'une erreur, OpenGL ne plantera pas sur la fonction mise en cause...
    // l'application continuera l'exécution malgré le contexte OpenGl corrompue.
    // Résultat: comportement aléatoire ou crash 200 lignes plus tard.
    // Morale: toujours vérifier TOUS les appels OpenGL avec glAssert()
    // Allez donc voir la définition de glAssert() aussi.
    // !!!!!!!

    // 2 - Fragment shader : (quasiment identique au vertex shader...)
    //   2.1 - Charger le code source depuis le fichier "../shaders/fragmentdefault.glsl"
    //         en utilisant la fonction loaders::text::loadFile() de lecture d'un fichier texte
    //         Utilisez l'attribut "mFragmentShaderId"
    //   2.2 - Créer un objet OpenGl FRAGMENT_SHADER et y associer le code source
    //   2.3 - Compiler le shader
    //   2.4 - Vérifier les erreurs de compilation


    // 3 - Programme :
    //   3.1 - Créer un "programme" OpenGl, stocker son identifiant dans l'attribut "mProgram"
    // et associer les shaders vertex shader et fragment shader (glCreateProgramm() glAttachShader() )


    //   3.2 - Définir les numéros/index des attributs  aux variables "in"

    // Explications:
    // Un vertex shader travail en parallel sur la liste des sommets du maillage.
    // Le fichier "shaders/vertexdefault.glsl"  défini le traitement d'un seul
    // sommet à la fois (la boucle for est implicite et parallèle).
    // Chaque sommet possède plusieurs attributs (normales, positions, etc.),
    // Le vertex shader définie plusieurs variables "in" comme "in vec3 maVar"
    // correspondant aux attributs des sommets.
    // Il faut dire au shader ou cherché ces attributs pour remplir les variables "in".
    // Ceci ce fait en spécifiant le numéro de l'attribut.
    // On choisira la même convention que dans MyGLMesh (Partie II du TP):
    // position --> index 0,
    // normal --> index 1
    // coordonnées de textures --> index 2.
    // Vous devez impérativement jeter un oeil au fichier "shaders/vertexdefault.glsl"
    // afin de connaitre le nom des variables auxquelles vous voulez associer un index
    // du VAO ( en utilisant glBindAttribLocation() )
    // N.B: lors de l'upload du maillage de CPU vers GPU vous aurez aussi à spécifier
    // l'index de chaque attribut.

    // ...

    //   3.3 - Lier le programme (Link du vertex shader / fragment shader)
    //         ( glLinkProgram() )

    //   3.4 - Vérifier les erreurs d'édition de lien
    // (glGetProgramiv() puis printProgramInfoLog() définie plus haut )

    // #####################################
    // TP 1 / PARTIE I: Fin du code à écrire
    // #####################################
}

//------------------------------------------------------------------------------

/// Eraser shader programs
void Renderer::clearShaders()
{
    // #######################################
    // TP 1 / PARTIE I: Début du code à écrire
    // #######################################

    // 1 - Dissocier les shaders du program shader ( glDetachShader() )

    // 2 - Supprimer les shaders (vertex, fragment shaders) ( glDeleteShader() )
    // N.B: si les shader ne sont pas détachés OpenGL se réserve le droit de ne
    // pas les supprimer malgré l'appel à glDeleteShader() !

    // 3 - Supprimer le "programme shader"
    // N.B: seul le programme shader est nécessaire pour dessiner un objet,
    // on aurais pu supprimer les shaders dans la fonction initShaders()
    // après le "linkage".

    // ####################################
    // TP 1 / PARTIE I:Fin du code à écrire
    // ####################################
}

//------------------------------------------------------------------------------

/// Boucle de rendue appelée à chaque rafraichissement de l'écran
void Renderer::render()
{
    // matrice de placement d'un objet dans la scène:
    // pour le premier TP pas de mouvement d'où la matrice identité.
    // le constructeur glm::mat4x4(float) fixe la valeur de la diagonale
    const glm::mat4x4 modelMatrix(1.0f); // <- matrice identité

// #######################################
// TP 1 / PARTIE I: Début du code à écrire
// #######################################

// Fonction principale de calcul d'une image,
// son rôle est de séquencer l'ensemble des ordres de dessin de l'API de bas niveau.
// En d'autres termes d'appeler les fonctions OpenGl afin de réaliser le dessin
// des objets de votre scène.

// 1 -  Préparer l'image pour le rendu :
//    1.1 - Effacer les buffers de destination (buffers de couleur ET profondeur) (glClearColor(), glClear())

// 2 - Construire les matrices de vue et de projection :
//    2.1 - Définir une matrice de projection perspective
//          en utilisant glm::perspective(float fovy, float aspect, float zNear, float zFar)
//          La fonction définie le volume de vision (frustum) de la camera.
//          Google image mot clé "Frustum" vous donnera de magnifiques schémas
//          Paramètres:
//          field of view sur l'axe y = fovy en radian
//          window_largeur/window_hauteur = aspect (svp n'oubliez pas la conversion en float...)

//    2.2 - Définir la matrice de vue à partir de mViewMatrix et de modelMatrix

//    2.3 - Calculer la matrice de transformation des normales

//    2.4 - Calculer la matrice MVP (acronyme de Model View Projection) qui passe un point du repère objet au repère image

// 3 - Activer et paramétrer les shader par défaut :
//    3.1 - Activer le programme shader créée dans initShaders() (glUseProgram())

//    3.2 - Fixer les valeurs (variable uniform des shader) qui sont indépendantes par objet :
//          les matrices des shader (c.f. voud devez regarder dans le code des shaders dossier /shader)
//          doivent être initialisées (glGetUniformLocation() et glUniform()) avec celles calculées précédemment en mémoire CPU.
//          Note: accéder au pointeur d'une matrice glm peut se faire en utilisant
//          'float* ptr = glm::value_ptr(ma_matrice)'

// ####################################
// TP 1 / PARTIE I:Fin du code à écrire
// ####################################

// #################
// TP 1 / PARTIE II:
// #################
// Ne plus utiliser la fonction de dessin d'objets prédéfinis:
#if 1
    if (mProgram != -1)
        mDummyObject->draw();
#endif
    // 4 - Dessiner les objets de la scène dans l'attribut 'mMeshes':
    // #####################################
    // TP 1 / PARTIE II:Fin du code à écrire
    // #####################################
}

//-------------------------------------------
// Chargement et compilation des données géométriques de l'application.
//-------------------------------------------
void Renderer::initGeometry()
{

// ########################################
// TP 1 / PARTIE II: Début du code à écrire
// ########################################

// Une fois la partie I finie et vérifier désactivé le code si dessous
// et changeant la macro if de 1 vers 0
#if 1
    // Construction d'un VAO contenant un triangle et une sphère.
    // Ce code est utile pour pouvoir tester le réglage du pipeline OpenGl
    // fait en Partie I sans devoir charger un maillage
    init_dummy_object();
#endif

    // ################
    // TP 1 / PARTIE II
    // ################

    // Le code de cette méthode ne nécessite pas de commandes OpenGl. Il s'agit
    // seulement de parser les fichiers contenant les objets 3D, puis de les
    // charger en mémoire vidéo.
    // Le parseur est déjà codé. Ici on vous demande simplement de savoir
    // explorer le code déjà existant dans ce projet et de l'utiliser.

    // 1 - Construire un vecteur de maillages (std::vector<loaders::Mesh*>)
    // à partir du fichier "../data/camel.obj"
    // Créer une instance d'un objet de type "loaders::obj_mtl::ObjLoader".
    // Utiliser la méthode ".load()" pour charger le fichier ".obj".
    // Si une erreur est intervenue pendant le chargement, afficher cette erreur.
    // S'il n'y a pas d'erreur, utiliser la méthode ".getObjects()"
    // sur l'objet de type "loaders::obj_mtl::ObjLoader" pour récupérer
    // l'ensemble des maillages chargés dans un vecteur de type std::vector<loaders::Mesh*>

    // 2 - Transformer ces maillages en maillages affichables de type "MyGLMesh"
    // (ils seront stockés dans l'attribut mMeshes)

    // 3 - Faites l'upload vers GPU avec ".compileGL()"

    // ######################################
    // TP 1 / PARTIE II: Fin du code à écrire
    // ######################################
}


// -----------------------------------------------------------------------------

/**
  * @ingroup RenderSystem
  * A mesh with OpenGL rendering capabilities.
  */
class MyGLMesh : public Loaders::Mesh {
private:
    /// Identifiant OpenGL du Vertex Array Object (VAO) du maillage
    GLuint mVertexArrayObject;

    /// identifiants OpenGL des VBOs du maillage
    /// N.B: utilisez VBO_VERTICES et VBO_INDICES pour accéder au tableau
    GLuint mVertexBufferObjects[2];

    /// Index pour accéder mVertexBufferObjects[]
    enum { VBO_VERTICES = 0,
           VBO_INDICES = 1 };

public:
    MyGLMesh(const Loaders::Mesh& mesh)
        : Loaders::Mesh(mesh)
    {
    }

    MyGLMesh(const std::vector<float>& vertexBuffer,
             const std::vector<int>& triangleBuffer,
             bool hasNormals = true,
             bool hasTextureCoords = true)
        : Loaders::Mesh(vertexBuffer,
                        triangleBuffer,
                        std::vector<int>(),
                        hasNormals,
                        hasTextureCoords)
    {
    }

    /**
      * Upload du maillage sur GPU
      * Build VertexArrayObjects for the mesh.
      *
      */
    void compileGL()
    {
        // Cette fonction a pour but de préparer les données du maillage afin
        // de l'afficher avec OpenGl. Pour ce faire vous devez charger
        // en mémoire vidéo (GPU) le maillage depuis la mémoire centrale
        // (CPU). OpenGl fournit différents types de buffers GPU (appelés VBOs)
        // dans lesquels sera chargé le maillage. Il faudra aussi décrire
        // l'organisation mémoire de ces VBO à l'aide d'un autre objet OpenGl
        // appelé VAO.
        //
        // Avant de coder vous devez impérativement comprendre où et comment
        // sont organisées les données du maillage sur CPU.
        // La classe dont hérite "MyGLMesh" contient les réponses à ces questions,
        // jetez y donc un oeil... Attardez-vous sur les attributs de "Mesh"
        // et ses sous structures "TriangleIndex" et "Vertex".
        //
        // doc: http://www.opengl.org/sdk/docs/man3/

        // Maintenant que vous avez une idée du stockage d'un maillage sur CPU
        // Nous allons effectuez ce fameux upload sur GPU. Nous allons donc
        // transférer notre classe "Mesh" vers les structures OpenGLs.
        // Par contre il ne s'agit pas ici de faire quelque chose comme:
        // opengl_class.mAttribut = ma_class.mAttribut
        // car OpenGl ne se manipule qu'au travers de fonctions C du type gl_XXXX_().

        // Les structures OpenGL que nous allons remplir sont des VAO et des VBOS
        // qui ressemble de façon schématique à cela:
        /*
            struct VertexArrayObject { // <- The VAO
                // Attributes:
                VertexBufferObject _list_of_positions[NB_VERTS]; // Array of vertex positions
                VertexBufferObject _list_of_normales[NB_VERTS]; // Array of vertex normals
                // ...

                // List of vertex index in VBOs defining a triangle every 3 elements :
                // First triangle: (_list_of_triangles[0], _list_of_triangles[1], _list_of_triangles[2])
                ElementBufferObject _list_of_triangles[NB_TRIS];

            };
            Même si OpenGL ne stocke pas exactement les VAOs de cette façon cela
            vous donne une bonne idée de l'organisation. Créer les structures
            et remplir les champs se fera avec des fonctions du type glXXX().
        */

        // ########################################
        // TP 1 / PARTIE II: Début du code à écrire
        // ########################################

        // 1 - Générez un identifiant pour un VertexArrayObject et le stocker
        // dans l'attribut mVertexArrayObject ( fonction  glGenVertexArrays() )

        // 2 - Générez deux identifiants pour deux VertexBufferObject
        // (un pour les sommets VBO_VERTICES, l'autre pour les faces VBO_INDICES)
        // et les stockers dans l'attribut mVertexBufferObjects ( glGenBuffers() )

        // 3 - Activez le VertexArrayObject (VAO) ( fonction glBindVertexArray() )

        // 4 - Activez le VertexBufferObject (glBindBuffer()) contenant les attributs des sommets.
        // Les buffers d'attributs de sommets sont toujours du type (GL_ARRAY_BUFFER)
        // Exemple d'attributs : position, normale, vitesse, couleur, accélération...
        // ce qu'on veut quoi :)

        // 5 - Remplissez le VertexBufferObject contenant les sommets (glBufferData())

        // 6 - Décrivez l'organisation mémoire (glVertexAttribPointer())
        // permettant à OpenGL d'accéder aux informations. Vous devrez associer
        // de position (index 0), de normale (index 1) et de coordonnées
        // de texture (index 2) pour chaque sommet.

        // 7 - Activez les attributs (position, normale, coordonnée de texture) (glEnableVertexAttribArray)

        // 8 - Activez le VertexBufferObject contenant les faces.
        // Attention! Les index de faces sont contenus dans un buffer
        // d'un type bien particulier (contrairement aux buffers des attributs des sommets)
        // il faut utiliser un GL_ELEMENT_ARRAY_BUFFER

        // 9 - Remplir le VertexBufferObject contenant les faces

        // TP 1 / PARTIE II: Fin du code à écrire

        // Un bind sur l'index 0 est en fait un 'unBind()' garantissant qu'aucun
        // buffer n'est activé
        glAssert(glBindVertexArray(0));
    }

    /// Draws the VertexArrayObjects (VAO "mVertexArrayObject") of the mesh.
    void drawGL()
    {
        // Affiche le maillage chargé en mémoire vidéo en utilisant le VAO

        // ########################################
        // TP 1 / PARTIE II: Début du code à écrire
        // ########################################
        // 1 - Activer le VAO (bind)

        // 2 - Dessiner les triangles.
        // Les sommets des triangles étant indexés et non consécutifs (sauf cas très particulier)
        // on utilisera la fonction glDrawElements(...)
        // N.B: !attention! le paramètre "count" de glDrawElements() ne désigne
        // pas le nombre de triangles mais la taille du tableau d'index de sommets
        // (c-a-d le nombre d'entiers dans le vertex buffer element VBE)

        // ######################################
        // TP 1 / PARTIE II: Fin du code à écrire
        // ######################################
    }

    /// Destructor
    ~MyGLMesh()
    {
        // ########################################
        // TP 1 / PARTIE II: Début du code à écrire
        // ########################################
        // 1 - Supprimer VBO et VBE glDeleteBuffers()

        // 2 - Supprimer VAO glDeleteVertexArrays()

        // ######################################
        // TP 1 / PARTIE II: Fin du code à écrire
        // ######################################
    }
};


// -----------------------------------------------------------------------------

int Renderer::handleMouseEvent(const MouseEvent& event)
{
    //static int modifiers = 0;
    //static bool moved = false;
    static int x = 0;
    static int y = 0;
    static int button = 0;

    if (event.click) {
        x = event.x;
        y = event.y;
        button = event.button;
        //modifiers = event.modifiers;
    }

    if (event.button == MouseEvent::MOVE) {

        // Note: Le repère orthonormé i, j, k de la camera est encodé dans
        // la matrice mViewMatrix, il vous faut retrouver comment récupérer
        // ces axes. Attention, les matrices glm sont column major (comme OpenGL)

        float dx = (float)(event.x - x) / (float)mWidth;
        float dy = (float)(event.y - y) / (float)mHeight;

        // ##########################
        // TP2 Début du code à écrire
        // ##########################
        switch (button) {
        case MouseEvent::LEFT: {
            float a = dx, b = dy;
            a = b;
            b = a; // HACK: Avoid unsued warning

            // 1 - Récupérer les axes xvec et yvec de la caméra
            // 2 - Modifier mViewMatrix pour y ajouter une rotation de dy*360.f degrés autour de xvec
            // 3 - Modifier mViewMatrix pour y ajouter une rotation de dx*360.f degrés autour de yvec
        } break;
        case MouseEvent::RIGHT: {
            // 1 - Calculer les vecteurs déplacement xvec et yvec selon les axes X et Y de la caméra
            // 2 - Modifier mViewMatrix pour y ajouter une translation de xvec+yvec
        } break;
        case MouseEvent::MIDDLE: {
            // 1 - Calculer le vecteur déplacement zvec selon l'axe Z de la caméra
            // 2 - Modifier mViewMatrix pour y ajouter une translation de zvec
        } break;
        }
        // #########################
        // TP2 Fin du code à écrire
        // #########################

        x = event.x;
        y = event.y;
        //moved = true;
        return 1;
    }
    else {
        //moved = false;
    }

    if (event.button == MouseEvent::WHEEL) {
        return 1;
    }

    if (!event.click) {
        button = 0;
        //modifiers = 0;
    }

    return 1;
}

// -----------------------------------------------------------------------------

void Renderer::setViewport(int width, int height)
{
    mWidth = width;
    mHeight = height;
    glAssert(glViewport(0, 0, mWidth, mHeight));
}

// -----------------------------------------------------------------------------

int Renderer::handleKeyEvent(char key)
{

    switch (key) {
    case 'w':
        glAssert(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
        break;
    case 'f':
        glAssert(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
        break;
    }
    return 1;
}

// -----------------------------------------------------------------------------

Renderer::~Renderer()
{
    for (unsigned i = 0; i < mMeshes.size(); ++i)
        delete mMeshes[i];

    clearShaders();
    delete mDummyObject;
}

// -----------------------------------------------------------------------------

void Renderer::init_dummy_object()
{
    mDummyObject = new GlDirectDraw();

    // On utilise les shaders du TP et pas ceux dans GlDirect_draw
    mDummyObject->enable_internal_shader(false);

    // On associe chaque attribut (position, normale, etc.) à un index (0, 1, etc.)
    // il faudra spécifier cet index au vertex shader ( dans "initShaders()" )
    mDummyObject->set_attr_index(GlDirectDraw::ATTR_POSITION, 0);
    mDummyObject->set_attr_index(GlDirectDraw::ATTR_NORMAL, 1);
    mDummyObject->set_attr_index(GlDirectDraw::ATTR_TEX_COORD, 2);
    mDummyObject->set_auto_flat_normals(true);

    // Construction des données du triangle
    mDummyObject->begin(GL_TRIANGLES);
    mDummyObject->color3f(1.f, 0.f, 1.f);
    mDummyObject->vertex3f(0.f, 0.f, 0.2f);
    mDummyObject->vertex3f(0.f, 0.2f, 0.f);
    mDummyObject->vertex3f(0.f, 0.f, -0.2f);
    mDummyObject->end();

    // Construction des données de la sphère
    mDummyObject->begin(GL_POINTS);
    for (float u_ = 0; u_ < 1.f; u_ += 0.01f) {
        for (float v_ = 0; v_ < 1.f; v_ += 0.01f) {
            float u = u_ * (2. * M_PI);
            float v = v_ * M_PI;

            float scale = 0.5;
            float x = cos(u) * sin(v) * scale;
            float y = sin(u) * sin(v) * scale;
            float z = cos(v) * scale;

            mDummyObject->normal3f(x, y, z);
            mDummyObject->vertex3f(x, y, z);
        }
    }
    mDummyObject->end();
}

// -----------------------------------------------------------------------------

// We need to run init glew outside the cpp of QT because GLEW headers aren't
// compatible with <QOpenGLContext>
void initGlew()
{
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        // Problem: glewInit failed, something is seriously wrong.
        std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
    }
    std::cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;
    glCheckError();
}

} // END namespace RenderSystem ================================================
