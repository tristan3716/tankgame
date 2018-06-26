#include "stdafx.h"
#include "map.h"

void parseMap(int ***map, int stage_number) {
	char *path = NULL;
	FILE *fp;
	char buf[BUF_SIZE];
	int i, j, k;

	const char wall[] = CHARACTER_WALL, barricade[] = CHARACTER_BARRICADE, blank[] = CHARACTER_BLANK;


	path = (char *)malloc(sizeof(char) * strlen(getMapPath(stage_number)));
	strcpy(path, getMapPath(stage_number));
	fp = fopen(path, "r");

	*map = (int **)malloc(sizeof(int *) * 30);
	for (i = 0; i < 30; i++) {
		(*map)[i] = (int *)malloc(sizeof(int) * 30);
	}
	j = 0; // map[j][]
	while (fgets(buf, BUF_SIZE, fp) != NULL) {
		i = 0; // For reading 2 bytes
		k = 0; // map[][k]
		while (buf[i] != '\0') {
			if (buf[i] == wall[0] && buf[i + 1] == wall[1]) {
				(*map)[j][k] = WALL;
				i += 2;
				k += 1;
			}
			else if (buf[i] == barricade[0] && buf[i + 1] == barricade[1]) {
				(*map)[j][k] = BARRICADE;
				i += 2;
				k += 1;
			}
			else if (buf[i] == blank[0] && buf[i + 1] == blank[1]) {
				(*map)[j][k] = BLANK;
				i += 2;
				k += 1;
			}
			else {
				i++;
			}
		}
		j++;
	}
}

const char *getMapPath(const int stage_number) {
	switch (stage_number) {
	case 1: return "map1.txt";
	default: exit(2425);
	}
}