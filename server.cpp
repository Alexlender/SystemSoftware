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
			case FType::knight:
                                return 'k';
                                break;
			case FType::bishop:
                                return 'b';
                                break;
			case FType::rook:
                                return 'r';
                                break;
			case FType::queen:
                                return 'Q';
                                break;
			case FType::king:
                                return 'K';
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

	~Cell(){
		if(figure)
			delete figure;
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
		for(int i = 0; i < 8; i++){
			cells[i] = new Cell[8];
			for(int j = 0; j < 8; j++){
				char let = (char)('a' + j);
				char num = (char)('1' + i);
				cells[i][j].setCords({let, num});

				if(num == '1' || num == '2' || num == '7' || num == '8'){
					
					
                                Color col;
                                FType type;

                                if(num == '2' || num == '7')
                                        type = FType::pawn;
                                else if(let == 'a' || let == 'h'){
					cout << "GGG" << i << j << "GGG";
                                        type == FType::rook;
				}
                                else if(let == 'b' || let == 'g')
                                        type == FType::knight;
                                else if(let == 'c' || let == 'f')
                                        type == FType::bishop;
                                else if(let == 'd')
                                        type == FType::queen;
                                else if(let == 'e')
                                        type == FType::king;


                                	if(num == '1' || num == '2')
                                        	col = Color::white;
                                	else if(num == '8' || num == '7')
                                        	col = Color::black;
					cout << i << j << let << num << ' ' << type << ' ' << col << endl;
				
					cells[i][j].figure = new Figure(col, type);
					
				}
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
				cout << cells[i][j].print() << cells[i][j].getCords()[0] << cells[i][j].getCords()[1];
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
	fd.cells[7][4].figure->type = FType::king;
	fd.print();
	return 0;
}

