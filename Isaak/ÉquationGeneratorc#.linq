<Query Kind="Statements" />


const int maxTotal = 15;
string équation;

	//mettre dans la methode loop()
	int premierChiffre = Random.Shared.Next(1, maxTotal);
	switch (Random.Shared.Next(1, 3))
	{
		case 1:
			//l'équation va s'additionner
			int tempMax = maxTotal - premierChiffre;
			int deuxièmeChiffre = Random.Shared.Next(1, tempMax + 1);
			équation = $"{premierChiffre} + {deuxièmeChiffre}".Dump("Addition");
			int total = premierChiffre + deuxièmeChiffre;
			total.Dump("Total Addition");
			break;
		case 2:
			//l'équation va se soustraire
			int tempMax_ = premierChiffre;
			int deuxièmeChiffre_ = Random.Shared.Next(1, tempMax_);
			équation = $"{premierChiffre} - {deuxièmeChiffre_}".Dump("Soustraction");
			int total_ = premierChiffre - deuxièmeChiffre_;
			total_.Dump("Total Soustraction");
			break;
    };
