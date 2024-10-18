/*
 * CalculsRigidBody.cpp :
 * Copyright (C) 2016 Florence Zara, LIRIS
 *               florence.zara@liris.univ-lyon1.fr
 *               http://liris.cnrs.fr/florence.zara/
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/** \file CalculsMSS.cpp
 Programme calculant pour chaque particule i d un MSS son etat au pas de temps suivant
 (methode d 'Euler semi-implicite) : principales fonctions de calculs.
 \brief Fonctions de calculs de la methode semi-implicite sur un systeme masses-ressorts.
 */

#include <stdio.h>
#include <math.h>
#include <vector>
#include <iostream>

#include "vec.h"
#include "ObjetSimule.h"
#include "ObjetSimuleRigidBody.h"
#include "Viewer.h"

using namespace std;

/*
 * Calcul de la masse de l objet rigide
 */
void ObjetSimuleRigidBody::CalculMasse()
{
    std::cout << "-------- CalculMasse ------------" << std::endl;
    for (int i = 0; i < (int)M.size(); i++)
    {
        if (M[i] != 1)
            std::cout << " masse du point : " << M[i] << std::endl;
        _Mass += M[i];
    }
    std::cout << " masse du bovin : " << _Mass << std::endl;

    std::cout << "-------- CalculMasse Fin ------------" << std::endl;
}

/*
 * Calcul du tenseur d inertie de l objet rigide - - partie constante Ibody
 * et remplissage tableau roi (position particules dans repere objet)
 */
void ObjetSimuleRigidBody::CalculIBody()
{
    std::cout << "-------- CalculIBody ------------" << std::endl;

    // std::cout << "yo " << std::endl;
    Vector somme;
    for (int i = 0; i < (int)P.size(); i++)
    {
        somme = somme + P[i];
    }
    _BaryCentre = somme / _Mass;
    std::cout << "yoooo" << _ROi.size() << std::endl;

    for (int i = 0; i < (int)P.size(); i++)
    {
        _ROi.push_back(P[i] - _BaryCentre);
    }
    std::cout << "yoooo" << std::endl;
    for (int i = 0; i < (int)P.size(); i++)
    {
        Matrix a = (_ROi[i].x * _ROi[i].x + _ROi[i].y * _ROi[i].y + _ROi[i].z * _ROi[i].z) * Matrix::UnitMatrix();
        Matrix b = MultiplyTransposedAndOriginal(_ROi[i]);
        _Ibody += M[i] * (a - b);
    }

    // slide 47 cour 4
    std::cout << "-------- CalculIBody Fin ------------" << std::endl;
}

/*
 * Calcul de l etat de l objet rigide.
 */
// Calcul pour l etat X(t) : Inverse tenseur inertie : I^-1(t) et vitesse angulaire

void ObjetSimuleRigidBody::CalculStateX()
{
    _IbodyInv = _Ibody.InverseConst();
    _VitesseAngulaire = Vector(0.0, 0.0, 0.0);
}

/*
 * Calcul de la derivee de l etat : d/dt X(t).
 */
// Calcul pour d/dt X(t)
// R'(t)
// P'(t) = F(t)
// L'(t) = moment total de la force (torque)

///---- Info relatives a la derivee de l etat du systeme
// Derivee de l etat du systeme : X'(t) = d/dt X(t)
// - d/dt P(t) = F(t) - les forces

void ObjetSimuleRigidBody::CalculDeriveeStateX(Vector gravite)
{
    // v(t) = P(t) / mass
    for (int i = 0; i < (int)P.size(); i++)
    {
        V[i] = P[i] / _Mass;
    }
    this->_IbodyInv = _Rotation * _Ibody.InverseConst() * _Rotation.TransposeConst();

    this->_VitesseAngulaire = _IbodyInv * _MomentCinetique;
    
    this->_RotationDerivee = StarMatrix(_VitesseAngulaire) * _Rotation  ;


    _Torque = _Ibody * _VitesseAngulaire;
    _Force = 0;
    for (auto f : Force)
    {
        _Force = _Force + f;
    }

    _Force = _Force + _Mass * gravite;
    _Vitesse = _QuantiteMouvement / _Mass;

}

/**
 * Schema integration pour obbtenir X(t+dt) en fct de X(t) et d/dt X(t)
 */
void ObjetSimuleRigidBody::Solve(float visco)
{
    // x(t+dt) - position
    // R(t+dt) - rotation
    // L(t+dt) - moment cinetique
    // P(t+dt) - quantite mouvement

    _Position = _Position + _delta_t * _Vitesse;
    _Rotation = _Rotation + _delta_t * StarMatrix(_VitesseAngulaire) * _Rotation;
    _MomentCinetique = _MomentCinetique + _delta_t * _Torque;
    _QuantiteMouvement = _QuantiteMouvement + _Force * _delta_t;
    
        for (int i = 0; i < (int)P.size(); i++)
    {
        //P[i] = P[i] + _Force;
        P[i] = _Rotation * _ROi[i] +  _Position;

    }
    //std::cout << "P[0] : " << P[0]  << std::endl;  

} // void

/**
 * Gestion des collisions avec le sol.
 */
void ObjetSimuleRigidBody::Collision()
{
    float distance = 0;
	int changement = 1;
	for (size_t i = 0; i < V.size(); i++)
	{
            //std::cout << "P[0] : " << P[i]  << std::endl;

		if (P[i].y < -3.0)
		{
            //changement = -1;
            //std::cout << "P[0] : " << P[i]  << std::endl;
            this->_QuantiteMouvement = - _QuantiteMouvement;
            //V[i]= 0;		
            }
            
    }
} // void
