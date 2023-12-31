void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}


float filtreBase1(float rawData, float lastFiltered, int fcoupure, int Te = 0.01)
{
  /*
  Filtre linéaire numérique d'ordre 1, comme on a vu en physique CN2
  */
  float tau = 1/6.28/fcoupure;
  float omegac = 6.28*fcoupure;
  float filtered = (tau-Te)/tau * lastFiltered + Te * rawData;
  float nouvelle_filtree = (lastFiltered + omegac*rawData*Te)/(1+omegac*Te);
  return filtered;
}

float filtreBase2(float rawData, float lastFiltered[2], int fpropre, float amortissement, int Te = 0.01)
{
  /*
  Filtre linéaire numérique d'ordre 2, on l'a pas encore vu mais j'ai fait les calculs 
  quand même, l'expression est une horreur ! Mais ça devrait mieux marcher que l'ordre 1
  */
  float omega0 = 6.28*fpropre;
  float filtered = lastFiltered[1] +((1/(Te*Te) - 1)/((2*amortissement/omega0/Te) + 1/Te/Te/omega0/omega0)) * lastFiltered[0] + rawData/((2*amortissement/omega0/Te) + 1/Te/Te/omega0/omega0);
  return filtered;
}


float prediction2(float e[3])
{
  float expected = 2*e[1] - e[0];
  float moyenne = (e[3] + expected)/2;
  return moyenne;
}

//----------------------------------------------------------------------------------------------------------
const int combien = 5;
float moyenneGlissante(float (&lastData)[combien], float nouvelleValeur, int indiceCourant)
{
  /* 
  moyenneGlissante requiert un tableau de 5 (ou autre valeur, à voir!!) précédentes valeurs 
  d'origine (donc à combiner avec la fonction passebas que tu as créée Antonio), la nouvelle valeur
  qui remplacera la plus ancienne du tableau (il s'agit d'un indice "glissant" qui devra être
  initialisé en global puis incrémenté à la fin de chaque exécution de void loop())
  */

  int indiceGlissant = indiceCourant % combien;
  lastData[indiceCourant] = nouvelleValeur;
  float somme = 0;
  for (int i = 0; i<5; i++)
  {
    somme += lastData[i];
  }
  return somme / combien;
}


float filtreComplet(float (&lastData)[combien], float nouvelleValeur, int indiceCourant, int &infini)
{
  /*
  nécessite simplement une initialisation de l'indice courant et de l'infini à 0 tous les deux
  */
  int indiceGlissant = indiceCourant % combien;
  if (nouvelleValeur > 1000 && infini < 10) // pics erratiques 
  {
    infini += 1;
    if (indiceGlissant == 0) { return  moyenneGlissante(lastData, lastData[combien], indiceGlissant); } 
    else {return moyenneGlissante(lastData, lastData[indiceGlissant - 1], indiceGlissant);}
    /*
    if (indiceCourant == 0) { lastData[indiceGlissant] = lastData[combien - 1]; }
    else {lastData[indiceGlissant] = lastData[indiceGlissant - 1];}
    float somme = 0;
    for (int i = 0; i<10; i++)
    {
      somme += lastData[i];
    }
    return somme / combien;
    */
  }
  else if (infini >= 10) // ca fait plus de dix mesures que c'est à l'infini, c'est pour l'arrêter
  {
    return nouvelleValeur;
  }
  else // tous les autres cas, on filtre pour lisser la courbe
  {
    infini = 0;
    return moyenneGlissante(lastData, nouvelleValeur, indiceCourant);
  }
}

