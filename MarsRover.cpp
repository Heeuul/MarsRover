#include <windows.h> 
#include <iostream> 
#include <cstdlib> 
#include <iomanip> 
#include <string> 
#include <vector> 
#include <ctime> 
#include <cmath> 
 
using namespace std; 
 
// The max amount of gold can be placed is half the map size to prevent very long map generation in small maps. 
 
// Prototype // 
void intro(); 
int inputnum(); 

//    Map    // 
class Map 
{
	private: 
		int dimX, dimY, g, gCol, point, seq, cycle, gild, area, countSeq, countCyc; 
		bool f = false, expose = false; 
		vector<vector<char>>layout; 
		vector<vector<char>>noFog; 
	
	public: 
		Map() 
		{
			design(); 
			mapGen(); 
		}
		void design(); 
		void displayMap(); 
		void displayStats(); 
		void setPos(int x, int y, char ent); 
		char getEntity(int x, int y); 
		bool safe(int x, int y); 
		bool inBound(int x, int y); 
		void mapGen(); 
		void goldCount(); 
		bool checkWin(); 
		bool checkFail(); 
		void resetMap(); 
		void solution(); 
		void fail() 
		{ 
			f = true; 
		} 
		void score() 
		{ 
			int area = dimX*dimY;
			seq = (area)/(5*g); 
			cycle = (area)/(3*g); 
			gild = (area)/(g); 
		} 
		void sequence() 
		{ 
			point = point - seq; 
			countSeq++; 
		} 
		void turn() 
		{ 
			point = point - cycle; 
			countCyc++; 
		} 
		void see() 
		{ 
			expose = true;
		} 
		int getdimX() 
		{ 
			return dimX; 
		} 
		int getdimY() 
		{ 
			return dimY; 
		} 
 
}; 
	
void Map::design() 
{
	cout << endl << "Please enter the size of the map:" << endl << "Row => "; 
	dimX = inputnum(); 

	cout << "Column => "; 
	dimY = inputnum(); 
	
	area = dimX*dimY; 
	
	int max = (area)/2; 
	cout  << endl << "Number of Golds (Max: " << max << ") => "; 
	g = inputnum(); 
	
	while(g > max) 
	{ 
		cout << "You are only allowed to enter " << max << " max!" << endl << "Number of Golds => "; 
		g = inputnum(); 
	} 
	
	gCol = 0; 
} 

void Map::displayMap() 
{
	system("cls"); 
	cout << endl << "   Mission: Guide Lucky to collect " 
		 << endl << "            the golds safely. " << endl;
	
	for(int i = 0; i < dimY;i++) 
	{
		cout << "   +"; 
		for(int j = 0; j < dimX; j++) 
		{
			cout << "---+"; 
		} 
		switch(i) 
		{ 
			case 0	: 	cout << "  Guide: "; 
						break; 
			case 1	: 	cout << "  [#]Hole - Part of the mars that renders Lucky unmoveable. "; 
						break; 
			case 2	: 	cout << "  [?]Anon - Part of the map yet to be discovered. "; 
						break; 
			default : 	break; 
		} 
		
		cout << endl << setw(2) << dimY-i; 
		
		cout << " | "; 
		for(int j = 0; j < dimX; j++) 
		{
			if(!expose)
				cout << layout[i][j] << " | "; 
			else
				cout << noFog[i][j] << " | "; 
		} 
		switch(i) 
		{ 
			case 0	: 	cout << " [X]Hill - A blockade that you cannot move to. "; 
						break; 
			case 1	: 	cout << " [$]Gold - Goal to collect. "; 
						break; 
			default : 	break; 
		} 
		cout << endl; 
	} 
	
	cout << "   +"; 
	for(int j = 0; j < dimX; j++) 
	{
		cout << "---+"; 
	} 
	
	cout << endl << "  "; 
	for(int j = 1; j <= dimX; j++) 
	{
		if(j < 10)
			cout << setw(4) << j; 
		else if(j == 10)
			cout << "   " << j; 
		else 
			cout << "  " << j; 
	} 
	
	cout << endl; 
} 

void Map::displayStats() 
{ 
	cout << endl 
		 << "Commands: " << endl 
		 << "M: Move   L: Turn left   R: Turn right " << endl << "Press Q to quit. "<< endl << endl 
	
		 << "Scoring: " << endl 
		 << "> 1 gold    =  " << setw(3) << gild << " points " << endl 
		 << "> 1 turn    = -" << setw(3) << cycle << " points (" << countCyc << " times)" << endl 
		 << "> 1 command = -" << setw(3) << seq << " points (" << countSeq << " times)" << endl 
		 << "Score[" << point << " points]" << endl << endl 
		
		 << "Golds collected: " << gCol <<" out of " << g << endl << endl; 
} 

void Map::setPos(int x, int y, char ent) 
{ 
	layout[x][y] = ent; 
} 

char Map::getEntity(int x, int y) 
{ 
	return noFog[x][y]; 
} 

bool Map::safe(int x, int y) 
{ 
	if(layout[x][y] == '$') 
	{ 
		goldCount(); 
		noFog[x][y] = ' '; 
		return true; 
	} 
	else if(layout[x][y] == 'X' || layout[x][y] == ' ') 
	{ 
		return true; 
	} 
	else 
		return false; 
} 

bool Map::inBound(int x, int y) 
{ 
	if(x >= 0 && y >= 0 && x < dimX && y < dimY && layout[x][y] != 'X') 
		return true; 
	else 
		return false; 
} 

void Map::mapGen() 
{ 
	layout.resize(dimY); 
	noFog.resize(dimY); 
	for(int i = 0; i < dimY; i++) 
	{ 
		layout[i].resize(dimX); 
		noFog[i].resize(dimX); 
	} 
	
	char entities[] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', 'X', 'X', '#'}; 
	int entNo = 10; 
	for(int i = 0; i < dimY; i++) 
	{ 
		for(int j = 0; j < dimX; j++) 
		{
			int entity = rand() % entNo; 
			noFog[i][j] = entities[entity]; 
			layout[i][j] = '?'; 
		} 
	} 
	
	noFog[dimX/2][dimY/2] = ' '; 
	
	for(int gold = g; 0 < gold;) 
	{ 
		int i = rand() % dimX, j = rand() % dimY; 
		
		if(noFog[i][j] != '$' && i != dimX/2 && j != dimY/2) 
		{
			noFog[i][j] = '$'; 
			
			if(inBound(i-1,j) && noFog[i-1][j] != '$')
			{ 
				noFog[i-1][j] = ' ';
			} 
			if(inBound(i,j-1) && noFog[i][j-1] != '$')
			{ 
				noFog[i][j-1] = ' ';
			} 
			if(inBound(i+1,j) && noFog[i+1][j] != '$')
			{ 
				noFog[i+1][j] = ' '; 
			} 
			if(inBound(i,j+1) && noFog[i][j+1] != '$')
			{ 
				noFog[i][j+1] = ' '; 
			} 
			
			gold = gold - 1; 
		} 
	} 
} 
 
void Map::goldCount()  
{ 
	cout << "Gold found! " << endl << endl; 
	system("pause"); 
	gCol = gCol +1; 
	point = point + gild; 
} 
  
bool Map::checkWin() 
{ 
	if(gCol == g) 
		return true;
	else 
		return false;
} 
 
bool Map::checkFail() 
{ 
	if (f) 
		return true; 
	else 
		return false; 
} 
 
void Map::resetMap() 
{ 
	point = 0, countSeq = 0, countCyc = 0;
	srand(time(NULL)); 
} 
 
//   Rover   // 
class Rover 
{
	private: 
		int x, y; 
		char dirFace; 
	
	public: 
		Rover() 
		{ 
			
		} 
		void start(Map& map); 
		void L(Map& map); 
		void R(Map& map); 
		void M(Map& map); 
		void reveal(Map& map); 
}; 

void Rover::start(Map& map) 
{ 
	x = map.getdimX()/2; 
	y = map.getdimY()/2; 
	dirFace = 'v'; 
	map.setPos(x, y, dirFace); 
} 

void Rover::L(Map& map) 
{ 
	switch(dirFace) 
	{ 
		case '^' : 	dirFace = '<'; 
					break; 
		case '<' : 	dirFace = 'v'; 
					break; 
		case 'v' : 	dirFace = '>'; 
					break; 
		case '>' : 	dirFace = '^'; 
					break; 
	} 
	map.setPos(x, y, dirFace);
} 

void Rover::R(Map& map) 
{ 
	switch(dirFace) 
	{ 
		case '^' : 	dirFace = '>'; 
					break; 
		case '>' : 	dirFace = 'v'; 
					break; 
		case 'v' : 	dirFace = '<'; 
					break; 
		case '<' : 	dirFace = '^'; 
					break; 
	} 
	map.setPos(x, y, dirFace);
} 
 
void Rover::M(Map& map) 
{ 
	int newX = x, newY = y; 
	
	switch(dirFace) 
	{ 
		case '<': 	newY = y - 1; 
					break; 
		case '>': 	newY = y + 1; 
					break; 
		case 'v': 	newX = x + 1; 
					break; 
		case '^': 	newX = x - 1; 
					break; 
	} 
	
		if(map.inBound(newX, newY)) 
		{ 
			if(map.safe(newX, newY)) 
			{ 
				map.setPos(newX, newY, dirFace); 
				map.setPos(x, y,' '); 
				x = newX; 
				y = newY; 
			} 
			else 
			{ 
				map.fail(); 
				map.setPos(newX, newY, '@'); 
				map.setPos(x, y,' '); 
				map.displayMap(); 
				cout << endl <<  "Rover caught in a trap! Mission failed, we'll get 'em next time! " << endl << endl; 
				system("pause"); 
			}
		} 
		else 
		{
			cout<< "You cannot go there!" << endl; 
			system("pause");
		} 
} 

void Rover::reveal(Map& map) 
{ 
	int revX = x, revY = y;
	
	switch(dirFace) 
	{ 
		case '>' : 	if (map.inBound(revX-1, revY+1)) 
					map.setPos(revX-1, revY+1, map.getEntity(revX-1, revY+1)); 
					if (map.inBound(revX, revY+1)) 
					map.setPos(revX, revY+1, map.getEntity(revX, revY+1)); 
					if (map.inBound(revX+1, revY+1)) 
					map.setPos(revX+1, revY+1, map.getEntity(revX+1, revY+1)); 
					break; 
		case '^' : 	if (map.inBound(revX-1, revY+1)) 
					map.setPos(revX-1, revY+1, map.getEntity(revX-1, revY+1)); 
					if (map.inBound(revX-1, revY)) 
					map.setPos(revX-1, revY, map.getEntity(revX-1, revY)); 
					if (map.inBound(revX-1, revY-1)) 
					map.setPos(revX-1, revY-1, map.getEntity(revX-1, revY-1)); 
					break; 
		case '<' : 	if (map.inBound(revX-1, revY-1)) 
					map.setPos(revX-1, revY-1, map.getEntity(revX-1, revY-1)); 
					if (map.inBound(revX, revY-1)) 
					map.setPos(revX, revY-1, map.getEntity(revX, revY-1)); 
					if (map.inBound(revX+1, revY-1)) 
					map.setPos(revX+1, revY-1, map.getEntity(revX+1, revY-1)); 
					break; 
		case 'v' : 	if (map.inBound(revX+1, revY-1)) 
					map.setPos(revX+1, revY-1, map.getEntity(revX+1, revY-1)); 
					if (map.inBound(revX+1, revY)) 
					map.setPos(revX+1, revY, map.getEntity(revX+1, revY)); 
					if (map.inBound(revX+1, revY+1)) 
					map.setPos(revX+1, revY+1, map.getEntity(revX+1, revY+1)); 
					break; 
	} 
} 

//   Main   // 
int main() 
{ 
	intro(); 
	
	Map map; 
	map.resetMap(); 
	map.displayMap(); 
	Sleep(2000); 
	cout << endl << "Prepare for landing in 3! "; 
	Sleep(1000); 
	cout << endl << "2! "; 
	Sleep(1000); 
	cout << endl << "1! "; 
	Sleep(1000); 
	cout << endl << "Lucky landed safely! "; 
	Sleep(2000); 
	map.score(); 
	
	Rover perform; 
	perform.start(map); 
	
	string Commands; 
	char input; 
	bool finish = false; 
	cin.ignore(99, '\n'); 
	
	while(finish != true) 
	{ 
		perform.reveal(map); 
		map.displayMap(); 
		map.displayStats(); 
		
		cout << "Input => "; 
		getline(cin, Commands); 
		
		int order = Commands.length(); 
		for(int i = 0; i < order; ++i) 
		{ 
			input = Commands[i]; 
			input = toupper(input); 
			perform.reveal(map); 
			
			
			if(input != 'Q' || input != 'H') 
			{ 
				map.sequence(); 
			} 
			
			map.displayMap(); 
			cout << endl << "<[Executing command: " << input << "]>" << endl << endl; 
			Sleep(555); 

			switch(input) 
			{ 
				case 'L': 	perform.L(map); 
							break; 
				case 'R': 	perform.R(map); 
							break; 
				case 'M': 	perform.M(map); 
							break; 
				case 'Q': 	map.fail(); 
							break; 
				default : 	cout << "Invalid input! "; 
							system("pause"); 
							break; 
			} 
		} 
		if(input != 'Q' || input != 'H') 
		{ 
			map.turn(); 
		} 
		
		if(map.checkWin()) 
		{ 
			cout << "Mission accomplished! We got 'em all, thanks for the help! " << endl << endl; 
			system("pause"); 
			finish = true; 
		} 
		
		if(map.checkFail()) 
			finish = true; 
		
	} 
	
	bool ans = false; 
	do 
	{ 
		map.displayMap(); 
		map.displayStats(); 
		
		cout << endl << "Do you want to see the full map?" << endl 
					 << "Y or N: "; 
		cin >> input; 
		input = toupper(input); 
		switch(input) 
		{ 
			case 'Y': 	map.see(); 
						map.displayMap(); 
						map.displayStats(); 
						ans = true; 
						break; 
			case 'N': 	ans = true; 
						break; 
			default : 	cout << "Invalid input! " ; 
						system("pause"); 
						break; 
		}  
	} while(ans != true); 
	
	ans = false; 
	do 
	{ 
		cout << endl << "Do you want to try again?" << endl 
					 << "Y or N: "; 
		cin >> input; 
		input = toupper(input); 
		switch(input)
		{ 
			case 'Y': 	ans = true; 
						cout << endl << "Restarting the game . . . "; 
						system("pause"); 
						main(); 
						break; 
			case 'N': 	ans = true; 
						cout << endl << "Thank you for playing the game! "; 
						system("pause"); 
						break; 
			default: 	cout << "Invalid input! " ; 
						system("pause"); 
						break; 
		} 
	} while(ans != true); 
	
	return 0; 
} 
 
void intro() 
{
	system("cls"); 
	
	cout << " /==================================\\" << endl 
		 << "||   WELCOME TO MARS GOLD HUNTING   ||" << endl 
		 << " \\==================================/" << endl << endl 
		 << "   The CyberScience Space Lab Inc." << endl 
		 << " of which you work, had just detected" << endl 
		 << " gold on Mars using the previous Mars" << endl 
		 << " rover, Opportunity, which died after" << endl 
		 << " 14 years of doing its service. Now, " << endl 
		 << " a new rover Lucky, was deployed re-" << endl 
		 << " cently to collect the golds. "<< endl << endl
		 << "   However, Lucky was deployed as an " << endl 
		 << " unfinished product as the lab wants " << endl 
		 << " to collect the gold as soon as possi-" << endl 
		 << " ble to prevent other competitors fin-" << endl 
		 << " ding it out first. As the lead prog-" << endl 
		 << " rammer of the lab, you are given the" << endl 
		 << " task to input commands in order to" << endl 
		 << " help Lucky search for golds safely."<< endl; 
}

int inputnum()
{
	int num = 0; 
	
	while (!(cin >> num))
	{
		cin.clear(); 
		cin.ignore(999, '\n'); 
		cout << "Invalid input, please enter positive number -> "; 
	} 
	
	return num; 
} 
 