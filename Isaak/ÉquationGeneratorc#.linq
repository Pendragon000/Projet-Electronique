<Query Kind="Statements" />

string test = "";
Equation(ref test);
const int maxTotal = 15;
int[] Equation(ref string equation)
{
	int premierChiffre = Random.Shared.Next(1, maxTotal);
	int rep;
	int rep2;
	int rep3;
	switch (Random.Shared.Next(1, 3))
	{
		case 1:
			//l'équation va s'additionner
			int tempMax = maxTotal - premierChiffre;
			int deuxièmeChiffre = Random.Shared.Next(1, tempMax + 1);
			equation = $"{premierChiffre} + {deuxièmeChiffre}".Dump("Addition");
			int total = premierChiffre + deuxièmeChiffre;
			total.Dump("Total Addition");
			rep = premierChiffre + deuxièmeChiffre;
			break;
		case 2:
			//l'équation va se soustraire
			int tempMax_ = premierChiffre;
			int deuxièmeChiffre_ = Random.Shared.Next(1, tempMax_);
			equation = $"{premierChiffre} - {deuxièmeChiffre_}".Dump("Soustraction");
			int total_ = premierChiffre - deuxièmeChiffre_;
			total_.Dump("Total Soustraction");
			rep = premierChiffre - deuxièmeChiffre_;
			break;
			default:
			rep = 0; 
			break;
	};
	rep2 = Random.Shared.Next(rep - 3, rep + 3);
	if (rep2 < 0){
		rep = 0;
	}
	rep3 = Random.Shared.Next(rep - 3, rep + 3);
	int[] reponse = new int[3];
	reponse[0] = rep;
	reponse[1] = rep2;
	reponse[2] = rep3;
	return reponse;
}




