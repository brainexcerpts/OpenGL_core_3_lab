/**

*/

/**

@~french
\page lab_1 Scéance 1 : mise en place d'un environnement logiciel.

L'objectif de cette séance et de comprendre et de mettre en place
l'architecture générale d'une application de synthèse d'images 3D.
Une telle application est généralement développée selon le motif de conception
Modèle-Vue-Contrôleur

\li Le modèle est la scène 3D et est représenté par des classes gérant
les maillages, les textures, les caméras, ...
\li La vue est implémenté par le code de rendu de la scéne en utilisant OpenGL
ou DirectX (qui sont les APIs bas niveau).
\li Le contrôleur est l'interface entre le modéle et la vue et gère
les interactions avec l'utilisateur.

Dans notre application, le modèle est écrit en C++, la vue en C++/OpenGL et
le contrôleur utilise l'API Qt.
  
Ces TPs enseignent la version <a href="http://www.opengl.org/registry/doc/glspec32.core.20091207.pdf">OpenGL 3.2 core</a>
comme API de bas niveau.  Il est recommandé d'avoir suivi un cours introduisant
les bases d'opengl (explications du pipeline graphique)



@~english
\page lab_1 Lab 1 : implementation of a software environment based on OpenGl.

The goal of this lab is to implement and understand the general architecture
of an application that display 3D graphics. Such application usually follow the
called <a href="https://en.wikipedia.org/wiki/Model%E2%80%93view%E2%80%93controller">Model-View-Controler</a>
design pattern:

\li \c model is the 3D scene and is represented with C++ class for meshes, textures, cameras...
\li \c view is implemented by the code related to rendering. Here we use OpenGL which is a low level API
\li \c controler is the interface between model and view and manage user interactions.

To recap, our \c model is written in C++, the \c view in C++/OpenGL and \c controler uses Qt API.

Specifically we use <a href="http://www.opengl.org/registry/doc/glspec32.core.20091207.pdf">OpenGL 3.2 core</a> as our low level API.
Some previous knowledge about OpengGl basic usage and the graphic pipeline will help
you lot to understand this lab.


@~french
\section Introduction Organisation du sujet

Avant de pouvoir afficher un objet 3D dans la scène il va vous falloir fournir un
effort de départ conséquent. Travailler en dehors des scéances est impératif.
Il est important de noter que le travail d'exploration du code et de
la documentation fait parti intégrante du travail de TP. (N.B: un EDI peut
s'avérer bien pratique pour rechercher une classe ou des fonctions ...)

Le TP est découpé en deux parties : 
@li Partie I: Paramétrer le pipeline OpenGl. C'est à dire configurer
la manière dont openGl affiche les objets en chargeant et parametrant
les programmes appelés \b shaders dans le dossier \c shaders.

@li Partie II: Chargement de la géométrie. Les objets 3D devront être chargés
depuis un fichier vers la mémoire centrale pour enfin être transférer vers
la carte graphique. Dans cette phase vous devez spécifier comment
sont organisés les différentes données du maillage (liste de sommets,
liste de faces etc.)

Bien qu'il sera necessaire d'explorer un certain nombres de fichiers seul
<b>renderer.cpp</b> devra être modifié. Dans ce fichier le code à compléter
pour la partie I puis la partie II est clairement indiqué.

Commencez par la partie I. Ici vous ne vous préocuperez que de configurer
ce qui est relatif à l'affichage OpenGl. Des objets sont déjà chargés sur
la carte graphique. Ainsi vous pourrez observer et vérifier vos premiers
résultats sans avoir besoin de programmer toute la partie chargement
de la géométrie. Voici ce que vous devez obtenir en
fin de partie I:

@image html "/shot_TP1_Partie1.png"

La partie II vous devez charger depuis un fichier un modèle 3D puis l'envoyer sur la mémoire de 
la carte graphique en décrivant correctement l'organisation des données. Le maillage suivant
devrait s'afficher en fin de TP :

@image html "shot_TP1_Partie2.png"





@~english
\section Introduction Outline of the assignement

In OpenGl 3 before we can draw anything on the screen we must provide a
substantial effort from the very start. Working outside lab hours will be
mandatory. Work that involve exploring code and documentation is also a big part
of this lab. Note that IDE may help you tremendously to look for functions
and help you navigate the code.


We divided this lab in 2 parts:
@li Part I: Setting up the OpenGl pipeline. We configure how openGl will
draw objects by loading and setting the parameters of the programs callded
\b shaders in the \c shaders folder.

@li Part II: Loading geometry. 3D objects must be loaded to central
memory (CPU) and transfered to the graphic card. At this stage you must
specify how the data related to the mesh is organized (vertex list,
face list etc.)

Although you only <b>renderer.cpp</b> needs to be modified, it'll be necessary
to explore numerous other file. In \b renderer.cpp code to modify for Part I and II
is clearly stated in comments.

Begin with part I. You only need to worry about configuring OpenGl for drawing at
this stage. We provide a way to load geometry without you having to do anything
first. This way you'll be able to test your program without having to
check code related to handling geometry.

At the end of part I the following image should appear:

@image html "shot_TP1_Partie1.png"

In part II you must load a from a file a 3D model and send it to GPU.
You must describe the memory layout accuratly to OpenGl in order to succeed.

At the end of part II you are expected to draw:

@image html "shot_TP1_Partie2.png"




@~french
@section initOpenGL Partie I - Initialisation générale de l'application.

Fichier renderer.cpp, fonction rendersystem::Renderer::initRessources()

Au lancement de l'application, les différents paramètres et états du pipeline de discrétisation OpenGL
doivent être définis et initialisés. Ces états vont paramétrer le pipeline ou le contrôleur
pour permettre à l'application de réaliser ses tâches. Dans notre cas, les paramètres et états à initialiser concernent
@li le mode de rendu OPENGL_H
@li les paramètres de caméra pour la visualisation de la scène

Toute application de synthèse d'images nécessite l'utilisation de resources externes pour fonctionner.
Lors de l'initialisation de l'application, les resources dont la durée de vie est celle de l'application doivent être initialisées.
Dans notre cas ces resources seront :
@li le maillage à visualiser
@li les shaders pour les étages \c Vertex et \c Fragment
@li les paramètres d'apparence de l'objet à visualiser (textures, materiau, ...)





@~english
\section initOpenGL Part I - Initialize the application.

In \c renderer.cpp, \c rendersystem::Renderer::initRessources()

When launching our application we must configure the OpenGl pipeline:
\li OpenGl rendering mode
\li camera settings to visualize our scene

We also need external resources that will live during the whole lifetime of
our application:
\li Mesh to be viewed
\li Shaders (\c Vertex and \c Fragment shaders)
\li Object parameters such as textures materials etc.



@~french
@subsection generalSttings Initialisation des paramètres OpenGL.

Dans notre application, nous souhaitons visualiser nos modèles sous forme de faces pleines avec élimination des parties cachées.
Initialiser dans la fonction  rendersystem::Renderer::initRessources() les états OpenGL nécessaires.

@~english
@subsection generalSttings Initialize OpenGL settings.

We would like to draw our models with plain faces and culling hidden parts.
Go to rendersystem::Renderer::initRessources() and set up the necessary OpenGL
parameters




@~french
@subsection viewSettings Initialisation des paramètres de vue.

La fonction rendersystem::Renderer::initView() permet de définir les paramètres par défaut pour la visualisation d'un maillage dans notre application.
Initialiser la matrice de vue rendersystem::Renderer::g_viewMatrix à partir des indications données en commentaire dans la fonction rendersystem::Renderer::initView().

@~english
@subsection viewSettings Initialize view settings

in \c rendersystem::Renderer::initView() we define the default settings of
our camera to view objects in the scene. Initialize the so called view matrix
\c rendersystem::Renderer::mViewMatrix in the method \c rendersystem::Renderer::initView().



@~french
@subsection loadingShaders Chargement et compilation des shaders.

La spécification OpenGL 3.2 core nécessite imperativement la définition de \c shaders. Ces derniers décrivent comment les objets sont déssinné et transformé à l'écran.
Dans notre application, les shaders sont des programmes en langage glsl définis dans des fichiers sources glsl. 
Ils doivent être chargés, compilés et liés à partir de notre code C++ pour être utilisés pour le rendu. 
L'objectif de la fonction rendersystem::Renderer::initShaders() est de préparer l'application
afin qu'elle utilise par défaut les shaders "../shaders/vertexdefault.glsl" et "../shaders/fragmentdefault.glsl".
Il est à noter qu'un shader doit être vue comme une unité de compilation qui est ensuite intégrée dans un programme.

Les opérations devant être faites pour pouvoir utiliser des shaders OpenGL dans notre application sont les suivantes :
<ol>
<li> Vertex shader :
    <ol>
        <li> Charger le source depuis le fichier "../shaders/vertexdefault.glsl"
        <li> Créer un objet OpenGL VERTEX_SHADER et y associer le source
        <li> Compiler le shader
        <li> Vérifier les erreurs de compilation
    </ol>
<li> Fragment shader :
    <ol>
        <li> Charger le source depuis le fichier "../shaders/fragmentdefault.glsl"
        <li> Créer un objet OpenGL FRAGMENT_SHADER et y associer le source
        <li> Compiler le shader
        <li> Vérifier les erreurs de compilation
    </ol>
<li> Programme :
    <ol>
        <li> Créer un programme OpenGL et y associer les shaders
        <li> Fixer les positions des attributs géométriques en accord avec la classe rendersystem::MyGLMesh et les convention d'écriture
du source GLSL
        <ul>
            <li> inPosition --> index 0
            <li> inNormal --> index 1
            <li> inTexCoord --> index 2.
        </ul>
        <li> Lier le programme
        <li> Vérifier les erreurs d'édition de lien
    </ol>
</ol>



@~english
@subsection loadingShaders Loading and compiling shaders

Contrary to previous versions in OpenGL 3.2 core \c shaders must be defined.
Shaders describe how objects are transformed and drawn to the screen.

Here shaders are programs written in glsl langage and defined by the source file
\c **.glsl. We must load and compile them from \b within our C++ code.
We do this in \c rendersystem::Renderer::initShaders() in order to use
our "../shaders/vertexdefault.glsl" and "../shaders/fragmentdefault.glsl" shaders.

A shader really is a compilation unit that we add to our program only here
it's done dynamically on the fly while executing our software.

<ol>
    <li> Vertex shader :
    <ol>
        <li> Load the source file "../shaders/vertexdefault.glsl"
        <li> Create the OpenGL VERTEX_SHADER object and associate the source file
        <li> Compile
        <li> Check for errors
    </ol>
        <li> Fragment shader :
    <ol>
        <li> Load the source file"../shaders/fragmentdefault.glsl"
        <li> Create the OpenGL FRAGMENT_SHADER object and associate the source file
        <li> Compile
        <li> Check for errors
    </ol>
        <li> Program:
    <ol>
        <li> Create a OpenGL shader program and associte previously build shaders.
        <li> Set the vertex attribute of the shader that respect the spefication
            we use in \c rendersystem::MyGLMesh and our GLSL source code.
    <ul>
        <li> inPosition --> index 0
        <li> inNormal --> index 1
        <li> inTexCoord --> index 2.
    </ul>
        <li> Link the shader program
        <li> Check for errors
    </ol>
</ol>





@~french
@section mainRenderfunction Fonction générale de rendu.

La fonction rendersystem::Renderer::render() est la fonction principale de calcul d'une image de synthèse. Appelée à chaque
rafraichissement de l'image son rôle est de séquencer l'ensemble des ordres de dessin de l'API de bas niveau.
Dans notre application simple, cette fonction à le rôle suivant.

<ol>
<li> Préparer de l'image pour le rendu :
    <ul>
        <li> Effacer les buffers de destination
    </ul>
<li> Construire les matrices de vue et de projection :
    <ul>
        <li> Définir une matrice de projection perspective
        <li> Définir la matrice de vue
        <li> Calculer la matrice de transformation des normales
        <li> Calculer la matrice MVP de passage du repère objet au repère image
    </ul>
<li> Activer et paramétrer les shader par defaut :
    <ul>
        <li> Activer le programme par défaut
        <li> Positionner les paramètres indépendants des objets : les matrices dans notre cas
    </ul>
<li> Dessiner les objets de la scène :
    <ul>
        <li> Dessiner tous les objets prédéfinis avec g_dummy_object->draw(); (à remplacer en partie II par vos objets)
    </ul>
</ol>


@~english
@section mainRenderfunction Rendering loop

\c rendersystem::Renderer::render() is the main procedure to compute our image.
render() is called every time the screen is refreshed. It's sole role is to
call in the right order the OpenGl commands necessary to draw the final image.

<ol>
    <li> Prepare buffer for rendering:
    <ul>
        <li> Clear destination buffers
    </ul>
        <li> Build view and projection matrices:
    <ul>
        <li> Define the perspective projection matrix
        <li> Define the view matrix
        <li> Compute the matrix to transform normals
        <li> Compute the MVP matrix which transform points from the object's local space to image space
    </ul>
        <li> Enable and setup shader:
    <ul>
        <li> Enable the current shader
        <li> Set up parameters independant of the current drawn object:
             here our matrices
    </ul>
        <li> Draw object of the scene:
    <ul>
        <li> Draw predefined objects with \c g_dummy_object->draw(); (we will replace this in part II of this lab)
    </ul>
</ol>


@~french
@section managingGeometries Partie II - Représentation de la géométrie pour un affichage par OpenGL.

Fichier renderer.cpp, classe rendersystem::MyGLMesh.

Les objets 3D affichés par notre application sont des maillages. 
Ils peuvent être définis en dur dans le code source ou chargés depuis des
fichiers externes. Dans tout les cas c'est donnés devront être préparés/réagencés
pour un affichage par OpenGL.

La première étape concerne la programmation de la classe rendersystem::MyGLMesh.
Cette classe est déclarée (et partiellement vide) dans le fichier renderer.cpp.
rendersystem::MyGLMesh \b hérite de la classe Mesh et ajoute les fonctionalités
nécessaires pour un affichage par OpenGL.

Pour pouvoir être affiché en OpenGL, un maillage doit être représenté sous la forme d'un <b>Vertex Array Object</b>.
C'est objet OpenGL permettant de décrire l'organisation des données du maillage en mémoire et la façon d'y accéder. 
Lorsque notre maillage est représenté par un <b>Vertex Array Object</b>, 
son tracé est réalisé en demandant à OpenGL d'utiliser cet objet et d'en dessiner
les éléments.

Deux méthodes doivent être programmées pour implanter la classe rendersystem::MyGLMesh :
@li rendersystem::MyGLMesh::compileGL()

Le rôle de cette fonction est de créer les objets OpenGL comme VertexArrayObject
ou VertexBufferObject. Se reporter aux commentaires de la fonction dans
le fichier renderer.cpp ou dans la documentation de la fonction pour avoir
des indications sur la marche à suivre.

@li rendersystem::MyGLMesh::drawGL()

Le rôle de cette fonction est de donner les ordres OpenGL pour
le dessin d'un maillage.


@~english
\section managingGeometries Part II - Geometry representation for OpenGL rendering

In \c renderer.cpp, \c rendersystem::MyGLMesh.

Objects drawn in our project are meshes. We can define them in the code or load
them from files. Whichever it is data will need to be prepared or re-arranged
to be readable by OpenGl API.

First we focus on rendersystem::MyGLMesh. This class is declared and partially
empty. Notice that rendersystem::MyGLMesh inherits from the class Mesh.
the role of rendersystem::MyGLMesh is to extend Mesh to handle OpenGl drawing.

In order to be drawable a mesh must be represented under the form of a
<b>Vertex Array Object</b>. This is the object OpenGL knows how to read.
A Vertex Array Object describes for OpenGl how the mesh data is organized in memory.

In rendersystem::MyGLMesh you must implement:
\li rendersystem::MyGLMesh::compileGL()
This function is intended to create VertexArrayObject ou VertexBufferObject OpenGl objects.
Follow the instructions in the comments.
\li rendersystem::MyGLMesh::drawGL() this function role is to call OpenGl draw
commands (draw calls) to tell OpenGl to begin to draw.



@~french
\subsection loadingGeometry Chargement et compilation des données géométriques de l'application.

En utilisant la classe loaders::obj_mtl::ObjLoader de chargement de fichier Alias/Wavefront OBJ,
charger l'objet  "../data/camel.obj" et stockez ses différentes composantes dans un vecteur (std::vector<loaders::Mesh*>).
Transformez ces composantes en vecteur de primitives affichables en utilisant votre classe rendersystem::MyGLMesh.



@~english
\subsection loadingGeometry Loading and compiling geometry

Use loaders::obj_mtl::ObjLoader to load a Alias/Wavefront OBJ file
("../data/camel.obj"). Store the various components of that file in a vector
(std::vector<loaders::Mesh*>). Convert those components into drawable objects
thanks to rendersystem::MyGLMesh.



@~french
\subsection drawing Dessin des maillages chargés.

Modifiez la méthode Renderer::render() pour afficher les objets precedemment chargés dans g_meshes



@~english
\subsection drawing Draw calls

Modify Renderer::render() in order to draw previously loaded objects in \c mMeshes





*/
