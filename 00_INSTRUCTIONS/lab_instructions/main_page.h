/**

*/

/**

    @~
    \mainpage
    \author Mathias Paulin <Mathias.Paulin@irit.fr>
            Rodolphe Vaillant <blog at rodolphe-vaillant.fr>

    \section intro_sec Introduction

    @~french
    Ceci est la documentation d'un logiciel d'infographie 3D utilisé
    pour les travaux pratiques de l'UE "Introduction à l'Informatique Graphique 3D"
    en Master 1 informatique de l'université Paul Sabatier, Toulouse.

    @~english

    This lab assignement was originally designed for students of
    "université Paul Sabatier, Toulouse" in their 4th year of
    computer science journey. (Master 1)

    @~
    \section install_sec Installation

    @~french
    Télécharger le projet <A HREF="https://github.com/silenthell/OpenGL_core_3_lab">OpenGL_core_3_lab</a>
    vous trouverez :
    \li \c src : Répertoire contenant les source C++ du logiciel.
    Certaines de ces sources devront être complétés et modifiés pour
    la réalisation des travaux pratiques
    \li \c shaders : Répertoire contenant les sources GLSL des shader qui
    seront utilisés et modifiés pendant les TPs
    \li \c data : Répertoire contenant les modèles 3D exemples à afficher
    \li \c 00_INSTRUCTIONS : Répertoire contenant cette documentation

    @~english
    Download the project <A HREF="https://github.com/silenthell/OpenGL_core_3_lab">OpenGL_core_3_lab</a>
    you'll find:
    \li \c src : C++ source files of this project.
    You will need to complete some of these files to complete this assignement.
    \li \c shaders : GLSL source file that we use for shaders.
    You will need to modify those as well.
    \li \c data : 3D models to be drawn on the screen.
    \li \c 00_INSTRUCTIONS : folder with all these very instructions.

    @~french
    \subsection dependances Dépendances

    Le code fourni, écrit en C++ à été conçu pour fonctionner sous Windows,
    Linux et mac OS X. Tester pour la dernière fois sous windows il est possible
    que des problèmes de compilations apparaissent sous d'autres systèmes.
    Vous aurez besoin des outils suivants pour le compiler :
    \li Un compilateur C++ comme (Par example gcc pour linux:
    sudo apt-get install build-essential. Ou mingw sous windows)
    \li L'environement de compilation <A HREF="https://cmake.org/">Cmake</a>
    (version 2.6 au moins).
    (Cmake est un outil comparable à Qmake qui permet à partir d'une liste de
    fichiers sources et autres dépendences de générer un fichier makefile).
    \li L'environnement de développement OpenGL supportant OpenGL 3.2 - core.
    Il nécessite une carte graphique récente (au moins DirectX 10),
    et est fait implicitement lors de  l'installation du driver de
    la carte graphique. Une documentation <A HREF="http://www.opengl.org/sdk/docs/man3/">opengl</a>
    compléte est disponnible en ligne.
    \li L'environnement de développement QT 4.7 au moins avec, au moins,
    les modules QTCore, QTGui et QTOpenGL.  (le site de <A HREF="http://qt-project.org/downloads">QT</a>
    procure des liens vers des paquets d'insallation linux et windows)
    \li Bien que non nécessaire pour les travaux pratiques,
    nous vous invitons à installer le logiciel Blender pour créer,
    éditer les objets et scènes 3D visualisées dans le logiciel développé en TP.

    @~english
    \subsection dependances Dependencies

    We provide C++ source code designed to be crossplatform between Windows,
    Linux and Mac OS X. However, windows the latest test were conducted under
    Windows and some problems may arise under other systems. You will need
    the following tools to compile:
    \li A C++ compiler e.g. gcc under Linux
    <code>sudo apt-get install build-essential</code> or mingw under Windows
    \li <A HREF="https://cmake.org/">Cmake</a>. Cmake is similar to Qmake
    in the sens that it allows to generate a makefile from a list of
    sources files and other dependancies.
    \li OpenGL 3.2 - core and superior (which means you need a graphic card with
    DirectX 10 support with driver installed)
    The <A HREF="http://www.opengl.org/sdk/docs/man3/">opengl</a> documentation
    is available online.
    \li <A HREF="http://qt-project.org/downloads">QT 5.0</a> > with QTCore,
    QTGui and QTOpenGL.
    \li Optionally <A HREF="https://www.blender.org/">Blender</a> will help you
    to edit 3D models used in this lab.

    @~french
    \subsection compilation Compilation
    Dans le répertoire \c src vous trouverez.
    \li \c glm contient une bibliothèque de gestion de vecteurs et
    de matrices en C++. glm à été développée spécifiquement pour être utilisée
    avec OpenGL et suit les mêmes spécification (par exemple alignement
    mémoire des matrices).  Voir le site  <A HREF="http://glm.g-truc.net/">glm.g-truc.net</a>
    pour tout renseignements sur cette bibliothèque.

    \li \c filelaoders contient le code source C++ nécessaire au
    chargement/parsing des fichiers d'objets 3D.  Le code de ce répertoire
    ne sera pas édité ou modifié pour les TPs mais uniquement utilisé et étendu.

    \li \c gl_utils fonctions utilitaires qui facilite l'utilisation de
    l'API OpenGL

    \li \c qt_gui creation et gestion de la fenetre principale avec Qt

    \li \c rendersystem contient le code C++ principal de l'application pour
    calculer le rendue de l'image. Seuls les fichiers <b>renderer.h</b> et
    <b>renderer.cpp</b> devront être modifiés pendant les TPs et remis
    à l'enseignant pour évaluation des travaux.

    Nous recommandont l'utilisation d'un EDI comme QTCreator pour explorer
    le code de façon <b>éfficace</b>. Un simple "ctrl+left click" permet de
    sauter de définitions en définitions (fonctions, variables, classes etc.)
    au travers de fichiers différents. "F4" permet de switcher entre .cpp et .hpp.
    Enfin les fonctions de recherches ctrl+f ou ctrl+maj+f sont extrément utiles
    et opérent dans tous les fichiers du projet.


    @~english
    \subsection compilation Compilation
    In the \c src folder you'll find:
    \li \c glm a library defining vector and matrices data types in C++.
    glm was specifically developed to be used with OpenGL and share the
    same specifications (e.g. memory alignement of vector and matrices).
    For documentation go see <A HREF="http://glm.g-truc.net/">glm.g-truc.net</a>

    \li \c filelaoders C++ code to parse and load 3D object files.
    No modification of those files will be required during the lab.

    \li \c gl_utils tool functions to improve OpenGl API usability

    \li \c qt_gui create and manage the main window with Qt

    \li \c rendersystem Core of the application written in C+++ designed to
    draw the final image on screen. During the course of the lab you will
    need to edit the files <b>renderer.h</b> and <b>renderer.cpp</b>.


    We recommend you use an <A HREF="https://en.wikipedia.org/wiki/Integrated_development_environment">IDE</a>
    such as QTCreator to explore the project's code <b>efficiently</b>.
    For instancein QtCreator a simple "ctrl+left click" allows to jump
    from definition to declaration (function, variables, classes etc.)
    even accross several files. "F4" switches between .cpp et .hpp like a breeze...
    Search features with "ctrl+f" ou "ctrl+maj+f" may come in handy as they work
    , of course, across the whole project.

    @~french
    \subsubsection compilation_premiere Première compilation
    Pour compiler le programme, nous utiliserons la possibilité offerte par
    <A HREF="http://www.cmake.org/">cmake</a> de compiler en dehors de
    l'arborescence des sources.

    Voici la marche à suivre pour compiler votre programme :
    \li Créer un répertoire \c build/ dans le répertoire
    racine du project (au même niveau que les répertoires
    \c src, \c shaders, \c data et \c doc)
    \li Dans un terminal, se positionner dans \c build/ et taper la commande
    de génération du makefile : <code>cmake -DCMAKE_BUILD_TYPE=Debug ..</code>
    L'argument <code>-DCMAKE_BUILD_TYPE=Debug</code> sert à compiler en mode
    debug, si omit vous ne serez pas en mesure de debugger avec gdb ou
    la GUI dédiée au debuggage dans Qtcreator. L'argument ".." indique à cmake
    où se trouve le fichier CMakeLists.txt contenant la liste des fichiers
    sources et autres paramètres.
    \li Compiler le programme par la commande <code>make -j8</code> ici
    l'argument "-jn" permet de lancer en parallèle
    n threads pour accelérer la compilation.

    @~english
    \subsubsection compilation_premiere Build for the first time
    To build we use
    <A HREF="http://www.cmake.org/">cmake</a> ability to compile out of the
    source file directories

    Here is the procedure step by step:
    \li Create a \c build/ folder at the root of the project
    (same level as \c src, \c shaders, \c data et \c doc)
    \li In a terminal, go to \c build/ and call the command to generate the
    makefile: <code>cmake -DCMAKE_BUILD_TYPE=Debug ..</code>
    the parameter <code>-DCMAKE_BUILD_TYPE=Debug</code> tells Cmake to build in
    debug mode. If ommited you won't be able to use gdb or QTCreator GUI
    dedicated to debugging. The parameter ".." tells Cmake where to find the
    <code>CMakeLists.txt</code> file that list every source files and settings
    needed to build the project.
    \li To actually build the project call <code>make -j8</code> where
    <code>-jn</code> specify the number of threads to use to accelerate
    our build. (each cpp can be processed in parallel).

    @~french
    \subsubsection compilation_toutes Toutes les autres compilations
    \li Compiler le programme par la commande \b make ou \b make \b -j8
    lancée dans le répertoire \c build

    \subsection execution Exécution
    \li Executer le programme en tappant la commande <b>./minimal_renderer</b>
    dans le répertoire \c bin

    \subsection qt-creator Utilisateurs de l'EDI qtcreator
    L'EDI Qtcreator sait ouvrir les fichiers CMakeLists.txt. Vous pouvez simplement
    aller dans l'onglet fichier ou file et faire ouvrir un nouveau projet.
  
    Selectionner le fichier CMakeLists.txt à la racine du projet. Le chemin
    contenant les fichiers intermediaires de compilation (le makefile, les ".o" etc.) vous
    serons demandé; spécifiez alors le repertoir \c build précedemment créé.
    Bouton "Suivant" puis "Run cmake" et c'est bon.
    Vous pouvez ensuite compiler dans qtcreator et remplacer
    la commande \b make par \b make \b -j8 dans les paramétres du projet.

    @~english
    \subsubsection compilation_toutes Normal build
    Once Cmake generate the cmake file you don't need to call it again.
    After your first build you only need to call \b make or \b make \b -j8
    in the \c build directory.

    \subsection execution Execution
    \li You can execute the program <b>./minimal_renderer</b> in \c bin

    \subsection qt-creator QtCreator

    Qtcreator knows how to open CMakeLists.txt. just open it as a new project
    inside QtCreator.

    After selecting \c CMakeLists.txt at the root of the project. you must
    specify the \c build/ directory with all the intermediate files such as
    makefile, ".o" etc.

    @~
    \subsection list_of_topics Assignements

    - @subpage lab_1
    - @subpage lab_2
    - @subpage lab_3
    - @subpage lab_4
    - @subpage assignement_1

 */
