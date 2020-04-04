/**

*/

/**

@~french
@page lab_4 Scéance 4 : calcul de l'éclairage.

Lors de cette scéance, nous allons programmer, à l'aide des <i>shaders</i>, le calcul de l'éclairage d'une scène.

Tout d'abord, récupérer demandes le corrigé "foundation_TP4" des 3 premières scéances de TP, corrigé étendu afin de préparer le
calcul de l'éclairage. Les fichiers renderer.cpp renderer.h doivent remplacer les anciens fichiers dans le
répertoire src/rendersystem, les fichiers vertexdefault.glsl et fragmentdefault.glsl doivent remplacer
les anciens fichiers dans le répertoire shaders.
Nous vous encourageons à regarder de près les modifications qui ont été faites, principalement l'introduction
des classes rendersystem::MyGLMaterial et rendersystem::MyGlEntity.
@~english
@page lab_4 Lab 4 : compute lighting.

In this lab we will implement the scene lighting using the so called <i>shaders</i>

To be sure we are starting from a clean base, ask for the answer files "foundation_TP4.zip".
It contains the answers and necessary changes needed to complete this lab.
renderer.cpp, renderer.h, vertexdefault.glsl, fragmentdefault.glsl must be replaced.
Among all the changes pay attention to the newly introduced classes rendersystem::MyGLMaterial and rendersystem::MyGlEntity.


@~french
Dans la nouvelle mouture de notre moteur minimal, le Renderer dessine une liste d'entités (instances de la
classe rendersystem::MyGlEntity). Une entité Correspond à un maillage, auquel est associé une apparence (un matériau
défini par la classe rendersystem::MyGLMaterial) et une transformation géométrique pour la disposer dans la scène.
Ainsi, il est possible de construire des scènes complexes simplement, en plaçant différents objets 3D dans
l'espace.

@~english
In this new version we now draw a list of "entities" (c.f. rendersystem::MyGlEntity). An entity is a mesh, its appearance 
(i.e its material as defined in rendersystem::MyGLMaterial) and its associated transformation to place it in the 3D scene.
Having several entities of a mesh with various appearances and placement allows for the creation of more complex shapes.


@~french 
Afin de mieux apréhender le principe de composition de scène, étudier, dans la méthode rendersystem::Renderer::initGeometry()
comment est créé le repère OXYZ de la scène.

@~english
Take a look at rendersystem::Renderer::initGeometry() and try to understand how we draw the object representing the 3 vector frame OXYZ in 3D.

@~french 
L'objectif de cette séance de TP est d'implanter, dans les shader, le calcul de l'éclairage par le modèle
de Blinn-Phong. Ce modèle repose sur le calcul et la combinaison de deux types de réflexions de la lumière :
une réflexion diffuse, proportionelle au cosinus de l'angle entre la normale à la surface et la direction de
lumière, et une reflexion spéculaire, proportionelle au cosinus de l'angle entre la normale et la bissectrice des
directions de vue et d'éclairage.

@~english
The goal of this lab is to implement the Blinn-Phong lighting model.
http://rodolphe-vaillant.fr/?e=85

Mainly this model decompose lighting into 3 contributions: ambiant, diffuse and specular lighting.
Ambiant lighting is a constant, diffusion is proportial to the cosine of the angle (normal, light_direction), 
while the specular component is proportial to the cosine of (normal, bissector between view_direction and light_direction.

@~french 
Dans ce modèle d'éclairage, les matériaux sont définis par trois propriétés : la couleur de base de l'objet,
contrôlant la réflexion diffuse et notée <b>materialKd</b>, la couleur de la reflexion spéculaire, notée
<b>materialKs</b>, et la rugosité de la réflexion spéculaire, notée <b>materialNs</b>.

@~english
We define our material properties as follows: the diffusion component's color is <b>materialKd</b>.
The color of the specular component is <b>materialKs</b> and the roughness of the specularity <b>materialNs</b>.


@~french 
En un point \f$ X \f$, de normale \f$ \overrightarrow{N} \f$, vu depuis la position \f$O\f$ et éclairé depuis
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

@~english


From a point \f$ X \f$, with normal \f$ \overrightarrow{N} \f$, viewed from position \f$ O \f$ and lit from position
 \f$ S \f$, we calculate the color as follows:

<ol>
 <li>View direction: \f$\overrightarrow{V} = \frac{\overrightarrow{XO}}{|\overrightarrow{XO}|}\f$</li>
 <li>Light direction: \f$\overrightarrow{L} = \frac{\overrightarrow{XS}}{|\overrightarrow{XS}|}\f$</li>
 <li>Bisector: \f$\overrightarrow{H} = \frac{\overrightarrow{V}+\overrightarrow{L}}{|\overrightarrow{V}+\overrightarrow{L}|}\f$</li>
 <li>Angle: \f$ cosTheta = \overrightarrow{N}.\overrightarrow{L} \f$ and \f$ cosAlpha = \overrightarrow{N}.\overrightarrow{H} \f$
 <li>Color: \f$ Color = \left( materialKd + materialKs \times cosAlpha^{materialNs}\right) \times cosTheta \f$
</ol>


The goal of this lab is for your shaders to make use of the input parameters setup in rendersystem::Renderer::render() 
in order to produce a 3 point lighting scene. The 3 point lighting composition uses 3 lights called Key, Fill and Back light and 
is very popular in photo shoot to easily improve the visual quality of any scene. Feel free to look up that technique and 
fiddle with the light color and position of the scene.

@~french 
\section sujet Travail à réaliser

\subsection bootstrap Mise en place des fonctions

Dans un premier temps, nous allons programmer, dans les shaders, le calcul de l'éclairage pour une source de type headlight,
dont la position est, dans le repère de vue, \f$(0.0, 0.0, 0.0)\f$ et dont la couleur est \f$(1.0, 1.0, 1.0)\f$

@~english
\section subject What's to be done

\subsection bootstrap Defining new functions

First we will implement a headlight light, its position in the view coordinate (or camera coordinates) will be 
 \f$ (0.0, 0.0, 0.0) \f$ and its color \f$(1.0, 1.0, 1.0)\f$

@~french 
Les opérations de calcul vont être séparées en deux groupes, un groupe calculée dans le vertex shader, l'autre calculé dans le
fragment shader. Ce découpage est lié aux données manipulées lors des différents calcul.



@~english
To this end computation will be divided between the "vertex shader" (parallel per vertex operations) and the "fragment shader" (parallel per triangle fragement operations)


@~french 
Les opérations 1, 2 et 3 de l'algorithme de calcul décrit ci-dessus vont être programmées dans le vertex shader.
En effet, ces opérations reposent sur une données liée au sommet : sa position.
Dans le fichier vertexdefault.glsl, programmez en GLSL la fonction
\verbatim
void computeLightingVectorsInView(in vec3 posInView, in vec3 lightPosition, out vec3 lightDir, out vec3 halfVec)
\endverbatim

qui, à partir des positions du sommet <i>posInView</i> et de la source de lumière <i>lightPosition</i>, toutes deux
exprimées dans le repère de la vue, calcule les
vecteur <i>lightDir</i> et <i>halfVec</i> représentant la direction d'éclairage et la bissectrice.



@~english
The operations 1, 2 and 3 of our algorithm will be implemented in the vertex shader.
Indeed these calculation are based on the vertex position.

Go to the file vertexdefault.glsl, and start writing the following GLSL function:
\verbatim
void computeLightingVectorsInView(in vec3 posInView, in vec3 lightPosition, out vec3 lightDir, out vec3 halfVec)
\endverbatim

From a vertex' position <i>posInView</i> and light source <i>lightPosition</i> (both in view coordinates) it 
computes the vector <i>lightDir</i> and <i>halfVec</i> which represent the light direction and bissector.


@~french 
Dans la fonction main, calculer dans les paramètres en sortie <i>lightDirInView[0]</i> et <i>halfVecInView[0]</i>
les informations pour une source headlight.


@~english
In the main function, compute the output parameters <i>lightDirInView[0]</i> et <i>halfVecInView[0]</i>

@~french 
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


@~english
Operations 4 and 5 must be implemented in the fragment shader since they compute the fragement's color. We will use the output values of the 
vertex shader which are automatically "rasterized" (i.e turned into fragments) and interpolated for us.

For now we will only use  <i>lightDirInView[0]</i> and <i>halfVecInView[0]</i>.
In fragmentdefault.glsl implement the GLSL function:
\verbatim
vec3 blinnPhongLighting (in vec3 kd, in vec3 ks, in float ns, in vec3 normal, in vec3 lightVector, in vec3 halfVector)
\endverbatim
From the material data <i>kd</i>, <i>ks</i> et <i>ns</i>, geometric data such as <i>normal</i>,
<i>lightVector</i> and <i>halfVector</i> it computes the color at a given point.
In the main function of the fragment shader, compute the final fragment color using <i>blinnPhongLighting</i>.


@~french 
Observer sur les images que vous obtenez les contributions individuelles des différentes composantes du modèle d'éclairage

\subsection light Eclairage 3 points : Key, Fill, Back
En utilisant les paramètres et variables uniformes définies dans les shaders et positionnées par le renderer, mettez en place l'éclairage 3 points de notre chameau.


@~english
Investigate the contribution of each indivudal light in your scene.

\subsection light 3 points lighting : Key, Fill, Back
Go one and turn your single headlight system into a 3 point light composition by fully adding the contribution of all our lights that are defined in the renderer.
*/
