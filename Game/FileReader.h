#pragma once
#include <string>
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "stdafx.h"

using namespace std;

class FileReader {

	public:

		FileReader();
		
		int **GetMap(string filename);

	private:

};