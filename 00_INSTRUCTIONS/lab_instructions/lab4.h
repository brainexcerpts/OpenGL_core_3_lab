/**

*/

/**

@~french
@page lab_4 Scéance 4 : calcul de l'éclairage.

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
