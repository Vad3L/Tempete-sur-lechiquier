#ifndef PLATEAU_H
#define PLATEAU_H

#include <vector> 

class  Plateau {
private:
	std::vector<std::vector<int>> plateau;
public:
	Plateau();
	std::vector<std::vector<int>> getPlateau();
};

#endif //PLATEAU_H