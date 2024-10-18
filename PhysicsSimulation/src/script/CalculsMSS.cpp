/*
 * CalculsMSS.cpp :
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
#include "ObjetSimuleMSS.h"
#include "Viewer.h"

using namespace std;

/**
 * Calcul des forces appliquees sur les particules du systeme masses-ressorts.
 */
void ObjetSimuleMSS::CalculForceSpring()
{
	// faire une boucle autoures des arretes

	for (size_t i = 0; i < Force.size(); i++)
	{
		Force[i] = 0;
	}
	double k = _SystemeMasseRessort->GetRessortList().at(0)->GetRaideur();
	double nb_ressort = _SystemeMasseRessort->GetNbRessort();
	double masse = _SystemeMasseRessort->GetParticule(0)->GetMass();
	double l = 0;
	Vector p1;
	Vector p2;
	double dist_abs = 0;
	double dist = 0;
	Vector u = 0.0;
	Vector vi;
	Vector vj;
	double vij = 0;
	Vector f1 = 0.0;

	Vector f2 = 0;
	Vector Ffinal = 0;
	Vector temp;
	int x = 0;
	int y = 0;
	double dechirure = 0.0;

	//(l(i,j)-l_0(i,j)) = la distance actuelle entre les 2 masses
	// std::cout << "nb parti = " << _SystemeMasseRessort->GetNbParticule() << std::endl;
	// std::cout << "size = " << _SystemeMasseRessort->GetRessortList().size() << std::endl;
	for (size_t i = 0; i < _SystemeMasseRessort->GetRessortList().size(); i++)
	{
		x = _SystemeMasseRessort->GetRessortList().at(i)->_ParticuleA->GetId();
		y = _SystemeMasseRessort->GetRessortList().at(i)->_ParticuleB->GetId();

		l = _SystemeMasseRessort->GetRessortList().at(i)->GetLrepos();
		k = _SystemeMasseRessort->GetRessortList().at(i)->GetRaideur();
		p1 = P[x];
		p2 = P[y];

		dist = (length(p2 - p1));

		u = (p2 - p1) / (length(p2 - p1));

		f1 = k * (dist - l) * u;

		vij = _SystemeMasseRessort->GetRessortList().at(i)->GetAmortissement();

		vi = V[x];
		vj = V[y];

		f2 = vij * dot(vi - vj, u) * u;

		Ffinal = f1 + f2;
		dechirure = length(Ffinal);
		// dechirure 30000
		// dist 0.5
		if (dechirure > 25000)
		{
			//_SystemeMasseRessort->GetRessortList().erase(_SystemeMasseRessort->GetRessortList().begin() + i);
			_SystemeMasseRessort->GetRessortList()[i]->SetAmortiss(0.0);
			_SystemeMasseRessort->GetRessortList()[i]->SetRaideur(0.0);

			//std::cout << "dechirure" << dechirure << std::endl;

			//std::cout << "dist" << dist << std::endl;
		}

		Force[x] = Force[x] + Ffinal;
		Force[y] = Force[y] - Ffinal;

		/*
				std::cout << "GetLrepos = " << l << std::endl;
				std::cout << "_ParticuleA = " << p1 << std::endl;
				std::cout << "_ParticuleB = " << p2 << std::endl;
				std::cout << "distance = " << dist << std::endl;
				std::cout << "k = " << k << std::endl;
				std::cout << "u = " << u << std::endl;
				std::cout << "vij = " << vij << std::endl;
				std::cout << "vi = " << vi << std::endl;
				std::cout << "vj = " << vj << std::endl;
				std::cout << "f1 = " << f1 << std::endl;
				std::cout << "f2 = " << f2 << std::endl;
				std::cout << "force = " << Ffinal << std::endl;
				std::cout << "real force = " << Force[x] << std::endl
						  << std::endl;*/
	}

	/// Rq : Les forces dues a la gravite et au vent sont ajoutees lors du calcul de l acceleration

} // void

/**
 * Gestion des collisions avec le sol.
 */
void ObjetSimuleMSS::Collision()
{
	/* ! Gestion des collisions  */
	// Reponse : reste a la position du sol - arret des vitesses
	// Penser au Translate de l objet dans la scene pour trouver plan coherent
	float distance = 0;
	
	for (size_t i = 0; i < V.size(); i++)
	{
		distance = length(P[i] - Vector(3.3, -10.1, 0));
		if (distance <= 1)
		{
			//std::cout << "distance " << distance << std::endl;

			V[i].y = 0;
		}
		if (P[i].y <= -10)
		{

			V[i].y = 0;
		}
	}
	
	/// Arret de la vitesse quand touche le plan

} // void
