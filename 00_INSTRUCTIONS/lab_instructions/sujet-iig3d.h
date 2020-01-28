//
// General documentation and assesments

/**
   @mainpage
   @author Mathias Paulin <Mathias.Paulin@irit.fr>
  *
   @section intro_sec Introduction
   Ceci est la documentation d'un logiciel d'infographie 3D utilisé
   pour les travaux pratiques de l'UE "Introduction à l'Informatique Graphique 3D" en Master 1 informatique
   de l'université Paul Sabatier, Toulouse.
  *
  \section install_sec Installation
  Télécharger l'archive <A HREF="http://www.irit.fr/~Mathias.Paulin/M1Info/IG3D/TP/IG3D-Renderer-TP1_base.zip">IG3D-Renderer-TP1_base.zip</a>
  et la décompresser dans un répertoire spécifique.
  Après décompression : les répertoires suivants seront générés :
  \li \c src : Répertoire contenant les source C++ du logiciel. Certaines de ces sources devront être complétés et modifiés pour la réalisation
  des travaux pratiques
  \li \c shaders : Répertoire contenant les sources GLSL des shader qui seront utilisés et modifiés pendant les TPs
  \li \c data : Répertoire contenant les modèles 3D exemples à afficher
  \li \c doc : Répertoire contenant cette documentation
 *
  \subsection dependances Dépendances
 *
  Le code fourni, écrit en C++ n'a été testé que sous Linux et mac OS X. Vous aurez besoin des outils suivants pour le compiler :
  @li Un compilateur C++ comme (Par example gcc pour linux: sudo apt-get install build-essential. Ou mingw sous windows)
  @li L'environement de compilation cmake (version 2.6 au moins). (Cmake est un outil comparable à Qmake qui permet à partir d'une liste de
  fichiers sources et autres dépendences de générer un fichier makefile).
  @li L'environnement de développement OpenGL supportant OpenGL 3.2 - core. 
  Il nécessite une carte graphique récente (au moins DirectX 10), et est fait implicitement lors de 
  l'installation du driver de la carte graphique. Une documentation
  <A HREF="http://www.opengl.org/sdk/docs/man3/">opengl</a> compléte est disponnible en ligne.
  @li L'environnement de développement QT 4.7 au moins avec, au moins, les modules QTCore, QTGui et QTOpenGL. 
  (le site de <A HREF="http://qt-project.org/downloads">QT</a> procure des liens vers des paquets d'insallation linux et windows)
  @li Bien que non nécessaire pour les travaux pratiques, nous vous invitons à installer le logiciel Blender
  pour créer, éditer les objets et scènes 3D visualisées dans le logiciel développé en TP.
 *
  \subsection compilation Compilation
  Le système de compilation utilisé par ce code est le système <A HREF="http://www.cmake.org/">cmake</a>.
  L'ensemble des sources de ce logiciel d'infographie 3D est composé de fichiers C++ placés dans le répertoire \c src . 
  Le répertoire \c glm contient une bibliothèque de gestion de vecteurs et de matrices en C++.
  glm à été développée spécifiquement pour être utilisée avec OpenGL et suit les mêmes spécification (par exemple alignement mémoire des matrices). 
  Voir le site  <A HREF="http://glm.g-truc.net/">glm.g-truc.net</a> pour tout renseignements sur cette bibliothèque.
 *
  Le répertoire \c filelaoders contient le code source C++ nécessaire au chargement/parsing des fichiers d'objets 3D. 
  Le code de ce répertoire ne sera pas édité ou modifié pour les TPs mais uniquement utilisé et étendu.
 *
  Le répertoire \c rendersystem contient le code C++ principal de l'application. Ce code C++ est composé d'une partie IHM dévelopée
  avec QT (qui ne fera pas l'objet de modification pour les TPs) et d'une partie \c renderer correspondant au code source C++ de
  l'application qui sera développée pendant les TPs. Seuls les fichiers <b>renderer.h</b> et <b>renderer.cpp</b> devront être modifiés pendant les TPs et
  remis à l'enseignant pour évaluation des travaux.
 *
  Nous recommandont l'utilisation d'un EDI comme QTCreator pour explorer le code de façon <b>éfficace</b>.
  Un simple "ctrl+left click" permet de sauter de définitions en définitions (fonctions, variables, classes etc.) 
  au travers de fichiers différents. "F4" permet de switcher entre .cpp et .hpp. 
  Enfin les fonctions de recherches ctrl+f ou ctrl+maj+f sont extrément utiles et opérent dans tous les fichiers du projet.
  
  Pour compiler le programme, nous utiliserons la possibilité offerte par cmake de compiler en dehors de l'arborescence des sources.
  Voici la marche à suivre pour compiler votre programme :
 *
  \subsubsection compilation_premiere Première compilation
  \li Créer (la première fois) un répertoire \c build dans le répertoire d'extraction de l'archive IG3D-Renderer-TP1_base.zip
  (au même niveau que les répertoires \c src, \c shaders, \c data et \c doc)
  \li Dans un terminal, se positionner dans \c build et taper la commande de génération des makefiles : <b>cmake -DCMAKE_BUILD_TYPE=Debug ..</b> 
   L'argument "-DCMAKE_BUILD_TYPE=Debug" sert à compiler en mode debug, si omit vous ne serez pas en mesure de debugger avec gdb ou 
   la GUI dédiée au debuggage dans Qtcreator. L'argument ".." indique à cmake où se trouve le fichier CMakeLists.txt contenant la liste des fichiers sources 
   et autres paramètres.
  \li Compiler le programme par la commande \b make \b -j8 ici l'argument "-jn" permet de lancer en parallèle 
  n threads pour accelérer la compilation.

  \subsubsection compilation_toutes Toutes les autres compilations
  \li Compiler le programme par la commande \b make ou \b make \b -j8 lancée dans le répertoire \c build

  \subsection execution Exécution
  \li Executer le programme en tappant la commande <b>./minimalrenderer</b> dans le répertoire \c bin  

  \subsection qt-creator Utilisateurs de l'EDI qtcreator
  L'EDI Qtcreator sait ouvrir les fichiers CMakeLists.txt. Vous pouvez simplement 
  aller dans l'onglet fichier ou file et faire ouvrir un nouveau projet. 
  
  Selectionner le fichier CMakeLists.txt à la racine du projet. Le chemin 
  contenant les fichiers intermediaires de compilation (le makefile, les ".o" etc.) vous
  serons demandé; spécifiez alors le repertoir \c build précedemment créé.
  Bouton "Suivant" puis "Run cmake" et c'est bon.
  Vous pouvez ensuite compiler dans qtcreator et remplacer la commande \b make par \b make \b -j8 dans les paramétres du projet.
 */

//-------------------------------------------

/**
   @page TP1 Scéance 1 : mise en place d'un environnement logiciel.

  L'objectif de cette séance et de comprendre et de mettre en place l'architecture générale d'une
  application de synthèse d'images 3D. Une telle application est généralement développée selon le motif de conception
  Modèle-Vue-Contrôleur

  @li Le modèle est la scène 3D et est représenté par des classes gérant les maillages, les textures, les caméras, ...
  @li La vue est implémenté par le code de rendu de la scéne en utilisant OpenGL ou DirectX (qui sont les APIs bas niveau).
  @li Le contrôleur est l'interface entre le modéle et la vue et gère les interactions avec l'utilisateur.

  Dans notre application, le modèle est écrit en C++, la vue en C++/OpenGL et le contrôleur utilise l'API Qt.
  
  Ces TPs enseignent la version <a href="http://www.opengl.org/registry/doc/glspec32.core.20091207.pdf">OpenGL 3.2 core</a> comme API de bas niveau. 
  
  Il est recommandé d'avoir suivi un cours introduisant les bases d'opengl (explications du pipeline graphique)

@section Introduction Organisation du sujet

Avant de pouvoir afficher un objet 3D dans la scène il va vous falloir fournir un
effort de départ conséquent. Travailler en dehors des scéances est impératif.
Il est important de noter que le travail d'exploration du code et de la documentation fait 
parti intégrante du travail de TP. (N.B: un EDI peut s'avérer bien pratique pour rechercher une classe ou des fonctions ...)

Le TP est découpé en deux parties : 
@li Partie I: Paramétrer le pipeline OpenGl. C'est à dire configurer la manière dont openGl
affiche les objets en chargeant et parametrant les programmes appelés \b shaders dans le dossier \c shaders.
@li Partie II: Chargement de la géométrie. Les objets 3D devront être chargés depuis un fichier vers la mémoire centrale 
pour enfin être transférer vers la carte graphique. Dans cette phase vous devez spécifier comment 
sont organisés les différentes données du maillage (liste de sommets, liste de faces etc.)

Bien qu'il sera necessaire d'explorer un certain nombres de fichiers seul <b>renderer.cpp</b> devra être modifié. 
Dans ce fichier le code à compléter pour la partie I puis la partie II est clairement indiqué.

Commencez par la partie I. Ici vous ne vous préocuperez que de configurer ce qui 
est relatif à l'affichage OpenGl. Des objets sont déjà chargés sur la carte graphique.
Ainsi vous pourrez observer et vérifier vos premiers résultats sans avoir besoin de programmer 
toute la partie chargement de la géométrie. Voici ce que vous devez obtenir en 
fin de partie I:

@image html "../sujets_TP/shot_TP1_Partie1.png"

La partie II vous devez charger depuis un fichier un modèle 3D puis l'envoyer sur la mémoire de 
la carte graphique en décrivant correctement l'organisation des données. Le maillage suivant
devrait s'afficher en fin de TP :

@image html "../sujets_TP/shot_TP1_Partie2.png"

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

@subsection generalSttings Initialisation des paramètres OpenGL.

Dans notre application, nous souhaitons visualiser nos modèles sous forme de faces pleines avec élimination des parties cachées.
Initialiser dans la fonction  rendersystem::Renderer::initRessources() les états OpenGL nécessaires.

@subsection viewSettings Initialisation des paramètres de vue.

La fonction rendersystem::Renderer::initView() permet de définir les paramètres par défaut pour la visualisation d'un maillage dans notre application.
Initialiser la matrice de vue rendersystem::Renderer::g_viewMatrix à partir des indications données en commentaire dans la fonction rendersystem::Renderer::initView().

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


@section managingGeometries Partie II - Représentation de la géométrie pour un affichage par OpenGL.

Fichier renderer.cpp, classe rendersystem::MyGLMesh.

Les objets 3D affichés par notre application sont des maillages. 
Ils peuvent être définis en dur dans le code source ou chargés depuis des fichiers externes.
Dans tout les cas c'est donnés devront être préparés/réagencés pour un affichage par OpenGL.

La première étape concerne la programmation de la classe rendersystem::MyGLMesh.
Cette classe est déclarée (et partiellement vide) dans le fichier renderer.cpp.
rendersystem::MyGLMesh \b hérite de la classe Mesh et ajoute les fonctionalités nécessaires pour un affichage par OpenGL.

Pour pouvoir être affiché en OpenGL, un maillage doit être représenté sous la forme d'un <b>Vertex Array Object</b>.
C'est objet OpenGL permettant de décrire l'organisation des données du maillage en mémoire et la façon d'y accéder. 
Lorsque notre maillage est représenté par un <b>Vertex Array Object</b>, 
son tracé est réalisé en demandant à OpenGL d'utiliser cet objet et d'en dessiner les éléments.

Deux méthodes doivent être programmées pour implanter la classe rendersystem::MyGLMesh :
@li rendersystem::MyGLMesh::compileGL()

Le rôle de cette fonction est de créer les objets OpenGL comme VertexArrayObject ou VertexBufferObject.
Se reporter aux commentaires de la fonction dans le fichier renderer.cpp ou dans la documentation de la fonction pour avoir des indications sur la marche à suivre.

@li rendersystem::MyGLMesh::drawGL()

Le rôle de cette fonction est de donner les ordres OpenGL pour le dessin d'un maillage. 

@subsection loadingGeometry Chargement et compilation des données géométriques de l'application.

En utilisant la classe loaders::obj_mtl::ObjLoader de chargement de fichier Alias/Wavefront OBJ,
charger l'objet  "../data/camel.obj" et stockez ses différentes composantes dans un vecteur (std::vector<loaders::Mesh*>).
Transformez ces composantes en vecteur de primitives affichables en utilisant votre classe rendersystem::MyGLMesh.

@subsection drawing Dessin des maillages chargés.

Modifiez la méthode Renderer::render() pour afficher les objets precedemment chargés dans g_meshes

*/



//-------------------------------------------

/**
   @page TP2 Scéance 2 : navigation et exploration dans la scène.

   La navigation et l'exploration d'une sc!ène 3D pass par la gestion d'une métaphore abstraite de Caméra.
   Dans notre application, cette notion est simplement représentée par la matrice rendersystem::Renderer::g_viewMatrix, définissant la transformation permettant de passer du repère scène au repère camera.

   La navigation et l'exploration d'une scène correspond à la modification de cette matrice en fonction des interactions utilisateur. Ces
   interactions sont capturées par l'interface QT et transmises à la fonction rendersystem::Renderer::handleMouseEvent() que vous allez compléter afin de permettre une exploration
   simple d'un objet.

   Trois type de mouvements sont gérés par notre système initial :
   <li> Mouvement de type "Observe" (bouton gauche de la souris). Ce mouvement consiste à effectuer une rotation centrée de la scène autour des axes X et Y de la caméra
   <li> Mouvement de type "Pan"  (bouton droit de la souris). Ce mouvement consiste à effectuer une translation de la scène dans le plain (X, Y) de la caméra
   <li> Mouvement de type "Zoom" (bouton central de la souris). Ce mouvement consiste à réaliser une translation de la scène selon l'axe Z de la caméra

   La matrice rendersystem::Renderer::g_viewMatrix a été initialisée dans l'exercice @ref viewSettings et défini les proriétés de la caméra. Afin de pouvoir réaliser
   les transformation, il faut récupérer les axes X, Y et Z du repère de cette caméra. Les rotations et les translations seront réalisées en
   fonction des valeurs des variables @c dx et @c dy
   contenant le déplacement normalisé de la souris dans le repère de l'image.


*/

//-------------------------------------------

/**
   @page TP3 Scéance 3 : génération procédurale de maillage.

Le sujet de cette séance est disponible dans ce <a href="../../sujetTP3.pdf">document</a>

*/

//-------------------------------------------

/**
   @page DV1 Devoir 1 : construction d'une sphère géodésique.

   L'objectif de ce devoir est de programmer et d'ajouter à la liste des objets que l'on peut visualiser dans notre
application un solide Platonique, l'icosahèdre et d'utiliser ce solide comme élément de base pour la construction d'une sphère géodésique.

Dans ce devoir programmerez 2 classes, Icosahèdre et Geodesique permettant de construire un icosahedre selon la définition de ce solide accessible
<A HREF="http://fr.wikipedia.org/wiki/Icosa%C3%A8dre">ici</a>.
La sphère géodésique sera obtenue en divisant récursivement jusqu'à un niveau N les faces de l'icosahèdre et en projettant les sommets sur la sphère.


*/

//-------------------------------------------

/**
   @page TP4 Scéance 4 : calcul de l'éclairage.

Lors de cette scéance, nous allons programmer, à l'aide des <i>shaders</i>, le calcul de l'éclairage d'une scène.

Tout d'abord, récupérer <a href="../../corrigeTP1a3.tgz">ici</a> le corrigé des 3 premières scéances de TP, corrigé étendu afin de préparer le
calcul de l'éclairage. Les fichiers renderer.cpp renderer.h doivent remplacer les anciens fichiers dans le
répertoire src/rendersystem, les fichiers vertexdefault.glsl et fragmentdefault.glsl doivent remplacer
les anciens fichiers dans le répertoire shaders.
Nous vous encourageons à regarder de près les modifications qui ont été faites, principalement l'introduction
des classes rendersystem::MyGLMaterial et rendersystem::MyGlEntity.

Dans la nouvelle mouture de notre moteur minimal, le Renderer dessine une liste d'entités (instances de la
classe rendersystem::MyGlEntity). Une entité Correspond à un maillage, auquel est associé une apparence (un matériau
défini par la classe rendersystem::MyGLMaterial) et une transformation géométrique pour la disposer dans la scène.
Ainsi, il est possible de construire des scènes complexes simplement, en plaçant différents objets 3D dans
l'espace.

Afin de mieux apréhender le principe de composition de scène, étudier, dans la méthode rendersystem::Renderer::initGeometry()
comment est créé le repère OXYZ de la scène.

L'objectif de cette séance de TP est d'implanter, dans les shader, le calcul de l'éclairage par le modèle
de Blinn-Phong. Ce modèle repose sur le calcul et la combinaison de deux types de réflexions de la lumière :
une réflexion diffuse, proportionelle au cosinus de l'angle entre la normale à la surface et la direction de
lumière, et une reflexion spéculaire, proportionelle au cosinus de l'angle entre la normale et la bissectrice des
directions de vue et d'éclairage.

Dans ce modèle d'éclairage, les matériaux sont définis par trois propriétés : la couleur de base de l'objet,
contrôlant la réflexion diffuse et notée <b>materialKd</b>, la couleur de la reflexion spéculaire, notée
<b>materialKs</b>, et la rugosité de la réflexion spéculaire, notée <b>materialNs</b>.

En un point \f$X\f$, de normale \f$\overrightarrow{N}\f$, vu depuis la position \f$O\f$ et éclairé depuis
la position \f$S\f$, la couleur se calcule de la manière suivante :

<ol>
 <li>Calcul de la direction de vue \f$\overrightarrow{V} = \frac{\overrightarrow{XO}}{|\overrightarrow{XO}|}\f$</li>
 <li>Calcul de la direction de d'éclairage \f$\overrightarrow{L} = \frac{\overrightarrow{XS}}{|\overrightarrow{XS}|}\f$</li>
 <li>Calcul de la bissectrice \f$\overrightarrow{H} = \frac{\overrightarrow{V}+\overrightarrow{L}}{|\overrightarrow{V}+\overrightarrow{L}|}\f$</li>
 <li>Calcul de \f$cosTheta = \overrightarrow{N}.\overrightarrow{L}\f$ et de \f$cosAlpha = \overrightarrow{N}.\overrightarrow{H}\f$
 <li>Calcul de la couleur : \f$Color = \left( materialKd + materialKs \times cosAlpha^{materialNs}\right) \times cosTheta\f$
</ol>


A la fin de cette séance de travaux pratiques, vos shaders sauront exploiter les paramètres d'éclairage définis par la
méthode rendersystem::Renderer::render() et correspondant à un éclairage photographique 3 points (Key, Fill, Back)
utilisé en studio photo ou en extérieur. Nous laissons les étudiants se renseigner sur cette technique d'éclairage
qui fera l'objet de devoir numéro 2. Nous attirons l'attention que ni les positions des sources, ni leur couleurs sont
correctement positionnées pour le moment mais que l'on pourra observer, en fin de séance, les avantages de cette technique
d'éclairage.

\section sujet Travail à réaliser

\subsection bootstrap Mise en place des fonctions

Dans un premier temps, nous allons programmer, dans les shaders, le calcul de l'éclairage pour une source de type headlight,
dont la position est, dans le repère de vue, \f$(0.0, 0.0, 0.0)\f$ et dont la couleur est \f$(1.0, 1.0, 1.0)\f$

Les opérations de calcul vont être séparées en deux groupes, un groupe calculée dans le vertex shader, l'autre calculé dans le
fragment shader. Ce découpage est lié aux données manipulées lors des différents calcul.

Les opérations 1, 2 et 3 de l'algorithme de calcul décrit ci-dessus vont être programmées dans le vertex shader.
En effet, ces opérations reposent sur une données liée au sommet : sa position.
Dans le fichier vertexdefault.glsl, programmez en GLSL la fonction
\verbatim
void computeLightingVectorsInView(in vec3 posInView, in vec3 lightPosition, out vec3 lightDir, out vec3 halfVec)
\endverbatim

qui, à partir des positions du sommet <i>posInView</i> et de la source de lumière <i>lightPosition</i>, toutes deux
exprimées dans le repère de la vue, calcule les
vecteur <i>lightDir</i> et <i>halfVec</i> représentant la direction d'éclairage et la bissectrice.

Dans la fonction main, calculer dans les paramètres en sortie <i>lightDirInView[0]</i> et <i>halfVecInView[0]</i>
les informations pour une source headlight.


Les opérations 4 et 5 vont êtres programmées dans le fragment shaders puisqu'elle calculent la couleur du fragment.
Elles seront calculées à partir des informations calculées par sommet et interpolées par le rasterizer sur chaque fragment.
Dans un premier temps, on n'utilisera que les informations <i>lightDirInView[0]</i> et <i>halfVecInView[0]</i>.
Dans le fichier fragmentdefault.glsl, programmez en GLSL la fonction
\verbatim
vec3 blinnPhongLighting (in vec3 kd, in vec3 ks, in float ns, in vec3 normal, in vec3 lightVector, in vec3 halfVector)
\endverbatim
qui, à partir des données de matériau <i>kd</i>, <i>ks</i> et <i>ns</i>, des données géométriques <i>normal</i>,
<i>lightVector</i> et <i>halfVector</i> calcule la couleur d'un point.
Dans la fonction maindu fragment shader, calculer la couleur du fragment en utilisant votre fonction <i>blinnPhongLighting</i>.

Observer sur les images que vous obtenez les contributions individuelles des différentes composantes du modèle d'éclairage

\subsection light Eclairage 3 points : Key, Fill, Back
En utilisant les paramètres et variables uniformes définies dans les shaders et positionnées par le renderer, mettez en place l'éclairage 3 points de notre chameau.


*/

// Light sources : 3 sources
// Key light
// Position, puissance et couleur détermine l'amboiance principale de l'éclairage
// ex : soleil pour scènes extérieures, flash pour scènes interieures
//
// Fill light
// Corrige les défauts engendrés par key light: ombres portées trop fortes, parties sombres , ...
// Position : sur le coté par rapport à la fill light, moins haute (au niveau de la face du sujet)
// Puissance : 2 fois moins importante que la key light.
// Couleur : plus douce que la key light.
//
// Back light
// Permet de mettre en évidence les contours de l'objet et de le détacher de l'arrière plan.
//

//-------------------------------------------

/**
   @page TP5 Scéance 5 : textures pour le contrôle de l'apparence.

*/

//-------------------------------------------

/**
   @page TP6 Scéance 6 : subdivision de lignes et extrusion.

*/

//-------------------------------------------

/**
   @page DV2 Devoir 2 : mettez en scène vos idées.

*/
