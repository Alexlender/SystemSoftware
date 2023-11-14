#include<iostream>
#include<vector>

using namespace std;

enum Color { white, black };
enum FType { pawn, knight, bishop, rook, queen, king};

class Figure{
public:
        Figure(Color col, FType t){
                color = col;
                type = t;
        };

        Color color;
        FType type;
	
	char print(){

		switch(type){
			case FType::pawn:
				return 'p';
				break;
			default:
				return '*';
	}
	}


        void move(vector<char> cord){
                1+1;
        };
};

class Cell{

private:
        char let;
        char num;


public:
	Figure* figure;
	void setCords(vector<char> cords){
		if(cords[0] >= 'a' && cords[0] <= 'h'){
                        let = cords[0];
                        if(cords[1] >= '1' && cords[1] <= '8')
                                num = cords[1];
                }
                else
                        throw invalid_argument("Wrong arg");
	};

	Cell(){
		1+1;
	};

	Cell(vector<char> cords){
		setCords(cords);
	};

	vector<char> getCords(){
		return vector<char> {let, num};
	};

	char print(){
		if(figure)
			return figure->print();
		return '_';
	}
};
class Field{
	public:
//private:
        Cell** cells;

//public:
	Field(){
		cells = new Cell*[8];
		int i = 0;
		for(int i = 0; i < 8; i++){
			cells[i] = new Cell[8];
			for(int j = 0; j < 8; j++){
				cells[i][j].setCords({(char)('a' + j), (char)('1' + i)});
				Figure fg = Figure(Color::white, FType::pawn);
				cells[i][j].figure = &fg;
			}
		}
				
	};

	~Field(){
		for(int i = 0; i < 8; i++)
			delete[] cells[i];
		delete[] cells;
	};

	void print(){
		cout << "--------------------------" << endl;
		for(int i = 0; i < 8; i++){
			for(int j = 0; j < 8; j++){
				//Cell cell = cells[i][j];
				cout << cells[i][j].print() << cells[i][j].getCords()[0];
			}
			cout << endl;
		}
		cout << "--------------------------" << endl;
	}
};


int main(){
	auto fg = Figure(Color::white, FType::pawn);
	//cout << fg.print();

	Field fd;
	fd.print();

	fd.cells[0][0].figure = &fg;

	fd.print();
	return 0;
}

