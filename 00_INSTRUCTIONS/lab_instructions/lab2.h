/**

   @~french
   @page lab_2 Scéance 2 : navigation et exploration dans la scène.

   La navigation et l'exploration d'une scène 3D passe par la gestion d'une métaphore abstraite de Caméra.
   Dans notre application, cette notion est simplement représentée par la matrice rendersystem::Renderer::g_viewMatrix, définissant la transformation permettant de passer du repère scène au repère camera.

   La navigation et l'exploration d'une scène correspond à la modification de cette matrice en fonction des interactions utilisateur. Ces
   interactions sont capturées par l'interface QT et transmises à la fonction rendersystem::Renderer::handleMouseEvent() que vous allez compléter afin de permettre une exploration
   simple d'un objet.

   Trois type de mouvements sont gérés par notre système initial :
   <li> Mouvement de type "Observe" (bouton gauche de la souris). Ce mouvement consiste à effectuer une rotation centrée de la scène autour des axes X et Y de la caméra
   <li> Mouvement de type "Pan"  (bouton droit de la souris). Ce mouvement consiste à effectuer une translation de la scène dans le plan (X, Y) de la caméra
   <li> Mouvement de type "Zoom" (bouton central de la souris). Ce mouvement consiste à réaliser une translation de la scène selon l'axe Z de la caméra

   La matrice rendersystem::Renderer::g_viewMatrix a été initialisée dans l'exercice @ref viewSettings et défini les proriétés de la caméra. Afin de pouvoir réaliser
   les transformation, il faut récupérer les axes X, Y et Z du repère de cette caméra. Les rotations et les translations seront réalisées en
   fonction des valeurs des variables @c dx et @c dy
   contenant le déplacement normalisé de la souris dans le repère de l'image.
   
   @~english
   @page lab_2 Lab 2 : navigation and exploration of the scene.
   
   In order to implement navigation inside the 3D scene we will need to introduce code to represent the Camera of our scene.
   In this lab the camera is trivially defined by the "view matrix" Renderer::mViewMatrix which defines the transformation to go from scene coordinates to camera coordinates.
   
   Given the user mouse input (Qt events in rendersystem::Renderer::handleMouseEvent()) we will modify this matrix in order to navigate into the scene.
   
   We're interested in handling 3 simple types of motions:
   - Rotation: (left click): we turn around (0,0,0) according to the X and Y vector axis of the camera.
   - Pan: (right click): we translate the scene according to the camera plane (X,Y)
   - Zoom: (middle click): Translating the scene according to the Z vector axis of the camera.
   
   To do this you will need to extract the X, Y and Z vector axis from the camera's matrix rendersystem::Renderer::mViewMatrix. 
   Apply translation and rotation using the \c dx and \c dy values which represent the normalized mouse motion inside the window plane.
*/
