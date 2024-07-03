#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct Farmacie
{
	unsigned short int id;
	char* denumire;
	float cifraAfaceri;
	char* sef;
	unsigned char nrAngajati;
	char* localitate;
}Farmacie;

typedef struct Nod
{
	Farmacie* info;
	struct Nod* left;
	struct Nod* right;
}Nod;
Farmacie* initializareFarmacie(unsigned short int id, const char* denumire, float cifraAfaceri,
	const char* sef, unsigned char nrAngajati, const char* localitate)
{
	Farmacie* farmacie = (Farmacie*)malloc(sizeof(Farmacie));
	farmacie->id = id;
	farmacie->denumire = (char*)malloc(strlen(denumire) + 1);
	strcpy(farmacie->denumire, denumire);
	farmacie->cifraAfaceri = cifraAfaceri;
	farmacie->sef = (char*)malloc(strlen(sef) + 1);
	strcpy(farmacie->sef, sef);
	farmacie->nrAngajati = nrAngajati;
	farmacie->localitate = (char*)malloc(strlen(localitate) + 1);
	strcpy(farmacie->localitate, localitate);
	return farmacie;
}
Nod* initializareNod(Farmacie* farmacie)
{
	Nod* nodNou = (Nod*)malloc(sizeof(Nod));
	nodNou->info = farmacie;
	nodNou->left = NULL;
	nodNou->right = NULL;
	return nodNou;
}
Nod* inserareFarmacie(Nod* radacina, Farmacie* farmacie)
{
	if (radacina == NULL)
	{
		return initializareNod(farmacie);
	}
	if (farmacie->id < radacina->info->id)
	{
		radacina->left = inserareFarmacie(radacina->left, farmacie);

	}
	else if(farmacie->id > radacina->info->id)
	{
		radacina->right = inserareFarmacie(radacina->right, farmacie);
	}
	return radacina;
}
void afisareInOrdine(Nod* radacina)
{
	//stanga radacina dreapta
	if (radacina != NULL)
	{
		afisareInOrdine(radacina->left);
		printf("%hu %s %5.2f %s %hhu %s\n", radacina->info->id,radacina->info->denumire, radacina->info->cifraAfaceri,
			radacina->info->sef, radacina->info->nrAngajati, radacina->info->localitate);
		afisareInOrdine(radacina->right);
	}
}


void dezalocare(Nod* radacina)
{
	if (radacina != NULL)
	{
		dezalocare(radacina->left);
		dezalocare(radacina->right);
		free(radacina->info->denumire);
		free(radacina->info->sef);
		free(radacina->info->localitate);
		free(radacina->info);
		free(radacina);

	}
}
//functie ce det nr total de ang la farm dintr o anumita localitate, data ca param
int nrTotal(Nod* radacina, const char* localitate)
{
	int contor = 0;
	if (radacina != NULL)
	{
		if (strcmp(radacina->info->localitate, localitate) == 0)
		{
			contor += radacina->info->nrAngajati;
		}
		contor += nrTotal(radacina->left, localitate);
		contor += nrTotal(radacina->right, localitate);
	}
	return contor;
}
//functia ce det cifra de afaceri medie lunara pt o farmacie specificata prin id->dat ca param
float cifraMedieLunara(Nod* radacina, unsigned short int id)
{
	
	if (radacina != NULL)
	{
		if (radacina->info->id == id)
		{
			return radacina->info->cifraAfaceri / 12;
		}
		float cifraMedieSubArboreStang = cifraMedieLunara(radacina->left, id);

		if (cifraMedieSubArboreStang != 0)
		{
			return cifraMedieSubArboreStang;
		}
		return  cifraMedieLunara(radacina->right, id);

	}
}
//functia care sterge toate nodurile frunza , plasate pe un nivel dat ca param
Nod* stergereFrunzePeNivel(Nod* radacina, int nivelCurent, int nivelSpecificat)
{
	if (radacina == NULL)
	{
		return NULL;
	}
	if (nivelCurent == nivelSpecificat)
	{
		if (radacina->left == NULL && radacina->right == NULL)//inseamna ca este nod frunza
		{
			free(radacina->info->denumire);
			free(radacina->info->sef);
			free(radacina->info->localitate);
			free(radacina->info);
			free(radacina);
			return NULL;
		}
		
	}
	else
	{
		radacina->left = stergereFrunzePeNivel(radacina->left, nivelCurent + 1, nivelSpecificat);
		radacina->right = stergereFrunzePeNivel(radacina->right, nivelCurent + 1, nivelSpecificat);
	}
	return radacina;
}
void main()
{
	
	FILE* f = fopen("Farmacie.txt", "r");
	if (f != NULL)
	{
		char bufferLine[300], sep_list[] = ",\n";
		Nod* radacina = NULL;

		while (fgets(bufferLine, sizeof(bufferLine), f))
		{
			char* token = strtok(bufferLine, sep_list);
			unsigned short int id = (unsigned short int)atoi(token);

			token = strtok(NULL, sep_list);
			char* denumire = (char*)malloc(strlen(token) + 1);
			strcpy(denumire, token);

			token = strtok(NULL, sep_list);
			float cifraAfaceri = (float)atof(token);

			token = strtok(NULL, sep_list);
			char* sef = (char*)malloc(strlen(token) + 1);
			strcpy(sef, token);

			token = strtok(NULL, sep_list);
			unsigned  char nrAngajati = (unsigned  char)atoi(token);

			token = strtok(NULL, sep_list);
			if (token == NULL) continue;
			char* localitate = (char*)malloc(strlen(token) + 1);
			strcpy(localitate, token);



			Farmacie* farmacie = initializareFarmacie(id, denumire, cifraAfaceri, sef, nrAngajati, localitate);
			radacina = inserareFarmacie(radacina, farmacie);


		}
		fclose(f);
		afisareInOrdine(radacina);
		//unsigned short int id;
		//char denumire[20];
		//float cifraAfaceri;
		//char sef[20];
		//unsigned char nrAngajati;
		//char localitate[20];
		//
		//Nod* radacina = NULL;
		//FILE* f = fopen("Farmacie.txt", "r");
		//while (fscanf(f, "%hu %s %f %s %hhu %s", &id, denumire, &cifraAfaceri, sef, &nrAngajati, localitate) == 6) {
		//	Farmacie* farmacie = initializareFarmacie(id, denumire, cifraAfaceri, sef, nrAngajati, localitate);
		//	radacina = inserareFarmacie(radacina,farmacie);

		//}
		//fclose(f);
		//afisareInOrdine(radacina);

		printf("Cerinta 2: \n");
		int nr = 0;
		nr = nrTotal(radacina, "Bucuresti");
			printf("%d\n", nr);

			printf("Cerinta 3:\n");
			float cA = 0;
			cA = cifraMedieLunara(radacina,  1);
			printf("%f", cA);

			printf("Cerinta 4:\n");
			radacina = stergereFrunzePeNivel(radacina, 0, 2);
			afisareInOrdine(radacina);


		dezalocare(radacina);
	}
	
}