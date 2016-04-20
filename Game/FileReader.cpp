#include "stdafx.h"
#include "FileReader.h"
#include <stdio.h>
#include <iostream>
#include <fstream>

using namespace std;

// constructor
FileReader::FileReader() {

}

int ** FileReader::GetMap(string filename) {

	ifstream file;

    file.open(filename);

    if( !file ) {
        cout << "cannot find file" << '\n';
        cin.clear();
        cin.ignore(255, '\n');
        cin.get();
		
        return 0;
    }

	int** array2D;
	array2D = new int*[59];
	char ch;

	int row = 0;
	int col = 0;

	array2D[0] = new int[176];

	for(int i = 0; i < 59; i++) {
		array2D[i] = new int[633];
	}

	while (file >> noskipws >> ch) {

		if(col == 633) {
			row++;
			col = 0;
		}

		if(ch != '\n') {

			switch(ch) {

				case '0':
					array2D[row][col] = 0;
					break;

				case '1':
					array2D[row][col] = 1;
					break;

				case '2':
					array2D[row][col] = 2;
					break;

				case '3':
					array2D[row][col] = 3;
					break;

				default: 
					break;
			}

			col++;
		}
	}

    file.close();

	return array2D;
}