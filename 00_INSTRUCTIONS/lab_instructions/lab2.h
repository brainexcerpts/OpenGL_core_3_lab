/**

   @~french
   @page lab_2 Scéance 2 : navigation et exploration dans la scène.

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
