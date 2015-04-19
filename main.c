#include <stdio.h>  // printf, scanf
#include <stdlib.h> // atoi, malloc, free
#include <unistd.h> // fork

// A vektor értékeinek beolvasása:
void readVectorValues(float *vec, int n)
{
	int i;
	for (i = 0; i < n; ++i)
	{
		printf("[+] %d.: ", i + 1);
		scanf("%f", &vec[i]);
	}
}

int main(int argc, char *argv[])
{
	int i, n;

	// Paraméterek ellenőrzése (hiba esetén nullánál nagyobb értékkel térünk vissza, jelezvén a hibát):
	if (argc != 2)
	{
		printf("[!] Egy paramétert vár a program.\n");
		return 1;
	}

	// Az első pozícionális paramétert (karaktertömb) számmá (int) konvertáljuk:
	n = atoi(argv[1]);
	if (n < 1)
	{
		printf("[!] Az első paraméternek nullánal nagyobb számnak kell lennie.\n");
		return 1;
	}

	// Dinamikusan foglalunk memóriát két, n méretű, lebegőpontos számokat tároló vektornak:
	float *vec1 = malloc(sizeof(float) * n);
	float *vec2 = malloc(sizeof(float) * n);

	// Beolvassuk a vektorokat:
	printf("Az első vektor beolvasása:\n");
	readVectorValues(vec1, n);
	printf("A második vektor beolvasása:\n");
	readVectorValues(vec2, n);

	printf("Az eredmény:\n");

	// Processzek létrehozása:
	for (i = 0; i < n; ++i)
	{
		// A fő processzben n-szer lefut a fork fgv., így a program az alábbi ponton kettéágazik n-szer.
		// A sikeres fork fgv.hívás kétszer tér vissza értékkel: a szülő- és a gyermek processzben
		// egyszer-egyszer -- a szülőben a gyermek processz azonosítójával, a gyermekben 0 értékkel.
		if (fork() == 0)
		{
			// A fork értéke 0, tehát a gyermekben vagyunk, kiírjuk az eredményt:
			printf("%d:%f\n", i + 1, vec1[i] * vec2[i]);
			// Kiugrunk a for ciklusból.
			// Ha nem tennénk, a gyermek (is) folytatná a ciklust, azaz processzeket akarna létrehozni,
			// ami a szülő feladata.
			break;
		}
	}

	// Felszabadítjuk a dinamikusan foglalt (heap-en lévő) memóriát.
	// (Forkolásnál a szülő processz memóriája lemásolódik, így a dinamikusan létrehozott
	// memóriaterületek is, amiket minden processzben külön-külön fel kell szabadítani.)
	free(vec1);
	free(vec2);

	// A futás sikeres:
	return 0;
}