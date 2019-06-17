#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<io.h>
#include<stdbool.h>

//структура режисера
typedef struct director
{
	int id;
	char name[20];
	int experience;
	bool deleted;
} dir_t;

//структура фільму
typedef struct film
{
	int idIMDB;
	char title[20];
	int releaseYear;
	int metascore;
	int directorID;
	bool deleted;
} film_t;

//структура індексного файлу
typedef struct index
{
	int index;
	bool deleted;
} ind_t;

//константи файлів БД (так як вони у каталозіі разом з програмою .с, то повний шлях не потрібний)
const char indexesF[] = { "index.txt" };
const char directorsF[] = { "directors.txt" };
const char filmsF[] = { "films.txt" };

//константи розмірів структур
const int indexS = sizeof(struct index);
const int directorS = sizeof(struct director);
const int filmS = sizeof(struct film);

//додавання записів до БД
void addDir()
{
	int countD = 0, id;
	FILE *fp_index, *fp_director, *fp_film;
	dir_t d, *d2;
	d2 = &d;
	ind_t ind;

	fp_index = fopen(indexesF, "r+b");
	fp_director = fopen(directorsF, "r+b");

	while (fread(&ind, indexS, 1, fp_index)) countD = ftell(fp_index);
	id = (countD / indexS) + 1;

	d.id = id;
	printf("\nEnter Name: ");
	scanf("%s", d.name);
	printf("\nEnter experience: ");
	scanf("%d", &d.experience);
	d.deleted = false;

	fseek(fp_director, 0, SEEK_END);      // set file position at the end of file

	if (fwrite(d2, directorS, 1, fp_director))      // write the record at the end of file
	{
		rewind(fp_index);
		fseek(fp_index, 0, SEEK_END);
		ind.index = id;
		ind.deleted = false;
		fwrite(&ind, indexS, 1, fp_index);
	}
	else
	{
		printf("\n\nAdding Error !!!");
	}
	//rewind(fp_index);
	//while (fread(d1, sizeof(int), 1, fp_index)) n++;

	fclose(fp_index);
	fclose(fp_director);
}

void addFilm(int idDir)
{
	bool flag = false;
	int countF = 0, id;
	FILE *fp_index, *fp_film;
	ind_t ind;
	film_t f, *f1;
	f1 = &f;
	fp_index = fopen(indexesF, "r+b");

	while (fread(&ind, indexS, 1, fp_index))
	{
		if (ind.deleted == false)
		{
			if (ind.index == idDir)
			{
				flag = true;
				break;
			}
		}
	}
	if (!flag)
	{
		printf("\n\Director NOT found !!!");
	}
	else
	{
		fp_film = fopen(filmsF, "r+b");
		while (fread(f1, filmS, 1, fp_film)) countF = ftell(fp_film);
		id = (countF / filmS) + 1;

		fseek(fp_film, 0, SEEK_END);
		(f.idIMDB) = id;
		printf("\nEnter title: ");
		scanf("%s", f.title);
		printf("\nEnter year of release: ");
		scanf("%d", &f.releaseYear);
		printf("\nEnter metascore: ");
		scanf("%d", &f.metascore);
		f.directorID = idDir;
		f.deleted = false;

		fseek(fp_film, 0, SEEK_END);      // set file position at the end of file
		fwrite(f1, filmS, 1, fp_film);    // write the record at the end of file

		fclose(fp_film);
	}
}

void edit(int choice, int id)
{
	int loc;
	bool flag = false;
	int temp;
	FILE *fp_index, *fp_director, *fp_film;
	dir_t *d2, d;
	d2 = &d;
	film_t *f2, f;
	f2 = &f;
	ind_t ind;

	switch (choice)
	{
	case 1:
		fp_index = fopen(indexesF, "r+b");
		fp_director = fopen(directorsF, "r+b");
		while (fread(&ind, indexS, 1, fp_index))
		{
			if (ind.deleted == false)
			{
				if (ind.index == id)
				{
					flag = true;
					break;
				}
			}
		}
		if (!flag)
		{
			printf("\n\Director NOT found !!!");
		}
		else
		{
			fseek(fp_index, (-1) * indexS, SEEK_CUR);
			loc = ftell(fp_index);

			fseek(fp_director, (loc / indexS) * directorS, SEEK_SET);

			fread(d2, directorS, 1, fp_director);

			printf("\n\nID :%d", d2->id);
			printf("\n\nName :%s", d2->name);
			printf("\n\nExperience :%d", d2->experience);

			printf("\nEnter New Name : ");
			scanf("%s", &(*d2->name));
			printf("\nEnter New Experince : ");
			scanf("%d", &temp);
			(d2->experience) = temp;

			fseek(fp_director, (-1) * directorS, SEEK_CUR);

			fwrite(d2, directorS, 1, fp_director);
		}
		printf("Director was successfully updated!");
		fclose(fp_index);
		fclose(fp_director);
		break;

	case 2:
		fp_film = fopen(filmsF, "r+b");
		while (fread(f2, filmS, 1, fp_film))
		{
			if (f2->deleted == false)
			{
				if (f2->idIMDB == id)
				{
					flag = true;
					loc = ftell(fp_film);
					break;
				}
			}
		}
		if (!flag)
		{
			printf("\n\Film NOT found !!!");
		}
		else
		{
			fseek(fp_film, (-1) * filmS, SEEK_CUR);
			fread(f2, filmS, 1, fp_film);

			printf("\n\nID: %d", f2->idIMDB);
			printf("\n\Title :%s", f2->title);
			printf("\n\nRelease Year: %d", f2->releaseYear);
			printf("\n\nMetascore: %d", f2->metascore);
			printf("\n\nDirector`s ID: %d", f2->directorID);

			printf("\n\nEnter New Title : ");
			scanf("%s", &(*f2->title));
			printf("\nEnter New Release Year : ");
			scanf("%d", &(f2->releaseYear));
			printf("\nEnter New Metascore : ");
			scanf("%d", &(f2->metascore));
			printf("\nEnter New Director`s ID : ");
			scanf("%d", &(f2->directorID));

			fseek(fp_film, (-1) * filmS, SEEK_CUR);
			fwrite(f2, filmS, 1, fp_film);
		}
		printf("Film was successfully updated!");
		fclose(fp_film);
		break;
	}
}

void searchDir(int id)
{
	int loc;
	bool flag = false;
	FILE *fp_index, *fp_director, *fp_film;
	dir_t *d2, d;
	d2 = &d;
	film_t *f2, f;
	f2 = &f;
	ind_t ind;

	fp_index = fopen(indexesF, "r+b");
	fp_director = fopen(directorsF, "r+b");
	fp_film = fopen(filmsF, "r+b");
	while (fread(&ind, indexS, 1, fp_index))
	{
		if (ind.deleted == false)
		{
			if (ind.index == id)
			{
				flag = true;
				break;
			}
		}
	}
	if (!flag)
	{
		printf("\n\Director NOT found !!!");
	}
	else
	{
		fseek(fp_index, (-1) * indexS, SEEK_CUR);
		loc = ftell(fp_index);

		fseek(fp_director, (loc / indexS) * directorS, SEEK_SET);

		fread(d2, directorS, 1, fp_director);

		printf("\nID :%d", d2->id);
		printf("\nName :%s", d2->name);
		printf("\nExperience :%d", d2->experience);
		printf("\n\nHis Films: \n");

		while (fread(f2, filmS, 1, fp_film))
		{
			if (f2->deleted == false && f2->directorID == id)
			{
				//loc = ftell(fp_film);
				fseek(fp_film, (-1) * filmS, SEEK_CUR);
				fread(f2, filmS, 1, fp_film);
				printf("\n\nID: %d", f2->idIMDB);
				printf("\n\Title :%s", f2->title);
				printf("\n\nRelease Year: %d", f2->releaseYear);
				printf("\n\nMetascore: %d", f2->metascore);
				printf("\n\nDirector`s ID: %d", f2->directorID);
				//fseek(fp_film, (-1) * filmS, SEEK_CUR);
				printf("\n---------------------------------------------");
			}

		}
	}
	printf("\n\n");
	fclose(fp_index);
	fclose(fp_director);
	fclose(fp_film);
}

void searchFilm(int id) {
	int loc;
	bool flag = false;
	FILE *fp_film;
	film_t *f2, f;
	f2 = &f;
	fp_film = fopen(filmsF, "r+b");

	while (fread(f2, filmS, 1, fp_film))
	{
		if (f2->deleted == false)
		{
			if (f2->idIMDB == id)
			{
				flag = true;
				loc = ftell(fp_film);
				break;
			}
		}
	}
	if (!flag)
	{
		printf("\n\Film NOT found !!!");
	}
	else
	{
		fseek(fp_film, (-1) * filmS, SEEK_CUR);
		fread(f2, filmS, 1, fp_film);
		printf("\n\nID: %d", f2->idIMDB);
		printf("\n\Title :%s", f2->title);
		printf("\n\nRelease Year: %d", f2->releaseYear);
		printf("\n\nMetascore: %d", f2->metascore);
		printf("\n\nDirector`s ID: %d", f2->directorID);
	}
	fclose(fp_film);
}

void delet(int choice, int id)
{
	int loc;
	bool flag = false;
	dir_t dir;
	ind_t ind;

	FILE *fp_index, *fp_director, *fp_film;

	switch (choice)
	{
	case 1:
		fp_index = fopen(indexesF, "r+b");
		fp_director = fopen(directorsF, "r+b");
		fp_film = fopen(filmsF, "r+b");
		while (fread(&ind, indexS, 1, fp_index))
		{
			if (ind.deleted == false)
			{
				if (ind.index == id)
				{
					flag = true;
					break;
				}
			}
		}
		if (!flag)
		{
			printf("\n\Director NOT found !!!");
		}
		else
		{
			fseek(fp_index, (-1) * indexS, SEEK_CUR);
			loc = ftell(fp_index);

			fseek(fp_director, (loc / indexS) * directorS, SEEK_SET);

			fread(&ind, indexS, 1, fp_index);
			ind.deleted = true;
			fseek(fp_index, (-1) * indexS, SEEK_CUR);
			fwrite(&ind, indexS, 1, fp_index);

			fread(&dir, directorS, 1, fp_director);

			printf("\nID :%d", dir.id);
			printf("\nName :%s", dir.name);
			printf("\nExperience :%d", dir.experience);
			printf("\n\nHis Films: \n");
			dir.deleted = true;
			fseek(fp_director, (-1) * directorS, SEEK_CUR);
			fwrite(&dir, directorS, 1, fp_director);
			film_t film;
			while (fread(&film, filmS, 1, fp_film))
			{
				if (film.deleted == false)
				{
					if (film.directorID == id)
					{

						//loc = ftell(fp_film);
						fseek(fp_film, (-1) * filmS, SEEK_CUR);
						fread(&film, filmS, 1, fp_film);
						printf("\n\nID: %d", film.idIMDB);
						printf("\n\Title :%s", film.title);
						printf("\n\nRelease Year: %d", film.releaseYear);
						printf("\n\nMetascore: %d", film.metascore);
						printf("\n\nDirector`s ID: %d", film.directorID);

						film.deleted = true;

						fseek(fp_film, (-1) * filmS, SEEK_CUR);
						fwrite(&film, filmS, 1, fp_film);
						fseek(fp_film, (-1) * filmS, SEEK_CUR);
						printf("\n\n-Was Deleted--------------------------------");
					}
				}

			}
		}
		printf("\n\n");
		fclose(fp_index);
		fclose(fp_director);
		fclose(fp_film);
		break;
	case 2:
		fp_film = fopen(filmsF, "r+b");
		film_t film;
		while (fread(&film, filmS, 1, fp_film))
		{
			if (film.deleted == false)
			{
				if (film.idIMDB == id)
				{
					flag = true;
					loc = ftell(fp_film);
					break;
				}
			}
		}
		if (!flag)
		{
			printf("\n\Film NOT found !!!");
		}
		else
		{
			fseek(fp_film, (-1) * filmS, SEEK_CUR);
			fread(&film, filmS, 1, fp_film);

			printf("\n\nFilm to DELETE");
			printf("\n\nID: %d", film.idIMDB);
			printf("\n\Title :%s", film.title);
			printf("\n\nRelease Year: %d", film.releaseYear);
			printf("\n\nMetascore: %d", film.metascore);
			printf("\n\nDirector`s ID: %d", film.directorID);

			film.deleted = true;

			fseek(fp_film, (-1) * filmS, SEEK_CUR);
			fwrite(&film, filmS, 1, fp_film);
		}
		printf("\n\nFilm was successfully deleted!");
		fclose(fp_film);
		break;

	default:
		break;
	}
}

void display()
{
	FILE *fp = NULL;
	int ch;
	bool flag = false;
	dir_t d;
	film_t f;
	ind_t ind;

	printf("\n\n1:Directors\n\n2:Films\n\n3:Index File ");
	scanf("%d", &ch);

	switch (ch)
	{
	case 1:
		fp = fopen(directorsF, "rb");
		printf("\n\nID  Name    Experience");
		while (fread(&d, directorS, 1, fp))
		{
			if (d.deleted == false) {
				printf("\n\n%d", d.id);
				printf("    %s", d.name);
				printf("    %d", d.experience);
				flag = true;
			}
		}
		break;

	case 2:
		fp = fopen(filmsF, "rb");
		printf("\n\idIMDB Title     Year  Metascore Director`s ID");
		while (fread(&f, filmS, 1, fp))
		{
			if (f.deleted == false) {
				printf("\n\n%d", f.idIMDB);
				printf("    %s", f.title);
				printf("    %d", f.releaseYear);
				printf("    %d", f.metascore);
				printf("    %d", f.directorID);
				flag = true;
			}
		}
		break;

	case 3:

		fp = fopen(indexesF, "rb");
		printf("\n\Existing (not deleted) indexes");
		while (fread(&ind, indexS, 1, fp))
		{
			if (ind.deleted == false) {
				printf("\n%d", ind.index);
				flag = true;
			}
		}
		break;

	default:
		printf("\n\nInvalid choice !!!");
	}

	if (!flag) {
		printf("\n\nNo records found !!!");
		printf("\n\n");
	}
	fclose(fp);
}

int main()
{
	FILE *fp, *fp_film;
	int choice, temp, id;

	//створюю файли або відкриваю на дозапис (перевірка, щоб точно існували потрібні файли)
	fp = fopen(directorsF, "a+b");
	fclose(fp);
	fp = fopen(filmsF, "a+b");
	fclose(fp);
	fp = fopen(indexesF, "a+b");
	fclose(fp);

	while (1)
	{
		printf("\n\n1:Add\n2:Display\n3:Search\n4:Edit\n5:Delete\n6:Exit \n\n");
		scanf("%d", &choice);

		switch (choice)
		{
		case 1:
			printf("\n\n1:Director adding\n2:Film adding:  \n");
			scanf("%d", &temp);
			switch (temp)
			{
			case 1:addDir();
				break;
			case 2:printf("\n\nEnter ID of Director: ");
				scanf("%d", &id);
				addFilm(id);
				break;
			default:
				break;
			}
			break;

		case 2:
			display();
			break;

		case 3:
			printf("\n\n1:Director search (will show all his FILMS)\n2:Film search (just 1 film):  \n");
			scanf("%d", &temp);
			printf("\n\nEnter ID to search: ");
			scanf("%d", &id);
			switch (temp)
			{
			case 1: searchDir(id);
				break;
			case 2: searchFilm(id);
				break;
			default: printf("Wrong input!");
				break;
			}
			break;

		case 4:
			printf("\n\n1:Director editing\n2:Film editing: \n ");
			scanf("%d", &temp);
			printf("\n\nEnter ID to edit: ");
			scanf("%d", &id);
			edit(temp, id);
			break;

		case 5:
			printf("\n\n1:Director deleting\n2:Film deleting:  \n");
			scanf("%d", &temp);
			printf("\n\nEnter ID to delete: ");
			scanf("%d", &id);
			delet(temp, id);
			break;

		case 6:
			exit(0);

		default:
			printf("\n\nInvalid choice !!!");
		}
	}
	return 0;
}