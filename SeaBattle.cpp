#include <iostream>
#include <clocale>
#include <conio.h>
#include <string>
#include <dos.h>
#include <ctime>

using namespace std;

const int s = 11;//размеры поля, не менять
const int c = 11;
bool luck = false;//логический параметр перехода компа из стадии рандомной стрельбы к осознанному убийству
int xforcomp,yforcomp;//координаты хода компьютера(необхоимость сохранять координаты последнего успешного выстрела)
int directionofluckfire = 2;//плоскость корабля в которой сейчас ИИ ищет корабль
int EndShipX,StartShipX,EndShipY,StartShipY;//координаты корабля которого сейчас убивает ИИ
void Start(double);//правила и приветствие
void Init(string*,string*,string*);//наполняет поля стартовыми данными
void Print(string*,string*);//функция вывода полей на экран, способна выводить любые 2 поля(после победы 2 полем выводит полное поле противника)
void game();//описана логика событий. описана победа и при необходимости повтор игры
void HodIgroka(string*,string*);//область программы, где игрок вводит параметры выстрела и получает результат
int Vistrel(int,int,string*);//функция высрела, ваш Кеп
bool Win(string*);//проверка есть ли живые корабли на пришедшем поле, функция работает с любым полем
bool PindosMove(string*,string*,string*);//ход ИИ, общий вид: рандомный выстрел, если попал то добивает, когда убил то снова стрелять рандомно
void PlaceShip(int,string*,string*);//функция удобна для понимания кода, использует в себе логику других функций: проверка ввода, проверка размещения на карте, добавление корабля на карту
void genField(string*);//функция генерирует на поле 10 кораблей используя логику функции CheckNearest
int Insert(string&,int&);//проверяет ввод

class ship
{
private:
	string x;
	unsigned int y;
	unsigned int lenth;
	int direction;
	char AliveSymbol;
	char DeadSymbol;
	char FiredSymbol;
public:
	ship()
	{
		x = 'a';
		y = 1;
		lenth = 0;
		direction = 0;
		AliveSymbol = 'S';
		DeadSymbol = 'X';
		FiredSymbol = 'F';
	};
	ship(unsigned int lenth_init, string x_init,unsigned int y_init,int direction_init,char AliveSymbol_init, char DeadSymbol_init, char FiredSymbol_init):lenth(lenth_init),x(x_init),y(y_init),direction(direction_init),AliveSymbol(AliveSymbol_init),DeadSymbol(DeadSymbol_init),FiredSymbol(FiredSymbol_init){};
	ship(unsigned int lenth_init, string x_init,unsigned int y_init,int direction_init,char AliveSymbol_init):lenth(lenth_init),x(x_init),y(y_init),direction(direction_init),AliveSymbol(AliveSymbol_init){};
	ship(unsigned int lenth_init, string x_init,unsigned int y_init,int direction_init):lenth(lenth_init),x(x_init),y(y_init),direction(direction_init){};
	ship(unsigned int lenth_init, string x_init,unsigned int y_init):lenth(lenth_init),x(x_init),y(y_init){};
	ship(unsigned int lenth_init):lenth(lenth_init){};

	int StringToInt(string x);//функция перевода строчный значений в целочисленные значения
	int StringToIntForY(char);//функция перевода строчных значений в целочисленные по таблице ASCII только для цифр
	string IntToString(int x);//функция обратная StringToInt
	void addToMap(string mas[s][c]);//функция добавления корабля на карту
	bool checkNearest(string Pole[s][c]);//одна из основных логик программы, проверка соседних клеток корабля(проверка можем ли мы поставить корабль на поле)
	bool checkBorder(string Pole[s][c]);//функция проверки выхода корабля за границы поля
	bool checkInsert();//необходима для основной логики игры. проверяет ввод
	bool checkDeath(int x, int y,string mas[s][c]);//проверяет состояние корабля(жив или потоплен)
	void KillingShip(int x, int y,string mas[s][c],string PoleVragaReal[s][c]);//убивает корабль работает в паре с вышестоящей
	char getFiredSymbol();//возвращает символ горщего корабля
	ship getShip(int x,int y,string mas[s][c]);//основная логика игры, получает координаты точки а возращяет все данные о корабле если по пришедщим координатам он вообще есть
	int Normalizex(int y);//системные функции предотвращения выхода за поля при расчетах
	int Normalizexforgetship(int y);
	int Normalizey(int x);
	int Normalizeyforgetship(int x);
};
int ship::StringToInt(string x)
	{
		int intx = 0;
		if((x=="a"||x=="b"||x=="c"||x=="d"||x=="e"||x=="f"||x=="g"||x=="h"||x=="i"||x=="j"))
		{
			if(x=="a") intx=0;
			else if(x=="b") intx=1;
			else if(x=="c") intx=2;
			else if(x=="d") intx=3;
			else if(x=="e") intx=4;
			else if(x=="f") intx=5;
			else if(x=="g") intx=6;
			else if(x=="h") intx=7;
			else if(x=="i") intx=8;
			else if(x=="j") intx=9;
		}
		else if((x=="1"||x=="2"||x=="3"||x=="4"||x=="5"||x=="6"||x=="7"||x=="8"||x=="9"||x=="10"))
		{
			if(x=="1") intx=0;
			else if(x=="2") intx=1;
			else if(x=="3") intx=2;
			else if(x=="4") intx=3;
			else if(x=="5") intx=4;
			else if(x=="6") intx=5;
			else if(x=="7") intx=6;
			else if(x=="8") intx=7;
			else if(x=="9") intx=8;
			else if(x=="10") intx = 9;
		}
		else intx=40;
		return intx;
	}
int ship::StringToIntForY(char x)
	{
		int intx = 0;
		if((x=='1'||x=='2'||x=='3'||x=='4'||x=='5'||x=='6'||x=='7'||x=='8'||x=='9'||x=='0'))
		{
			if(x=='1') intx=1;
			else if(x=='2') intx=2;
			else if(x=='3') intx=3;
			else if(x=='4') intx=4;
			else if(x=='5') intx=5;
			else if(x=='6') intx=6;
			else if(x=='7') intx=7;
			else if(x=='8') intx=8;
			else if(x=='9') intx=9;
			else if(x=='0') intx = 10;
		}
		else intx=40;
		return intx;
	}
string ship::IntToString(int x)
	{
		string stringx;
		if(x==0)stringx = "a";
		if(x==1)stringx = "b";
		if(x==2)stringx = "c";
		if(x==3)stringx = "d";
		if(x==4)stringx = "e";
		if(x==5)stringx = "f";
		if(x==6)stringx = "g";
		if(x==7)stringx = "h";
		if(x==8)stringx = "i";
		if(x==9)stringx = "j";
		if(x==10)stringx = "k";
		if(x==11)stringx = "a";
		return stringx;
	}
void ship::addToMap(string mas[s][c])
	{
		for(unsigned int i = 0;i<lenth;i++)
		{
			if(direction==1) mas[StringToInt(x)][y+i]=AliveSymbol;
			if(direction==0) mas[StringToInt(x)+i][y]=AliveSymbol;
		}
	}
bool ship::checkNearest(string Pole[s][c])
	{
		if(!checkBorder(Pole))return false;
		if(!checkInsert())return false;
		
		for(unsigned int i = 0;i<lenth;i++)
		{
			if(direction==0)
			{
				if(Pole[StringToInt(x)+i][y]!=" "||
					Pole[Normalizex(StringToInt(x)+1+i)][y]!=" "||
					Pole[Normalizex(StringToInt(x)-1+i)][y]!=" "||
					Pole[Normalizex(StringToInt(x)+i)][Normalizey(y+1)]!=" "||
					Pole[Normalizex(StringToInt(x)+i)][Normalizey(y-1)]!=" "||
					Pole[Normalizex(StringToInt(x)+i+1)][Normalizey(y+1)]!=" "||
					Pole[Normalizex(StringToInt(x)+i+1)][Normalizey(y-1)]!=" "||
					Pole[Normalizex(StringToInt(x)+i-1)][Normalizey(y-1)]!=" "||
					Pole[Normalizex(StringToInt(x)+i-1)][Normalizey(y+1)]!=" ")return false;
			}
			if(direction==1)
			{
				if(Pole[StringToInt(x)][Normalizey(y+i)]!=" "||
					Pole[Normalizex(StringToInt(x)+1)][Normalizey(y+i)]!=" "||
					Pole[Normalizex(StringToInt(x)-1)][Normalizey(y+i)]!=" "||
					Pole[StringToInt(x)][Normalizey(y+1+i)]!=" "||
					Pole[StringToInt(x)][Normalizey(y-1+i)]!=" "||
					Pole[Normalizex(StringToInt(x)+1)][Normalizey(y+i+1)]!=" "||
					Pole[Normalizex(StringToInt(x)+1)][Normalizey(y+i-1)]!=" "||
					Pole[Normalizex(StringToInt(x)-1)][Normalizey(y+i-1)]!=" "||
					Pole[Normalizex(StringToInt(x)-1)][Normalizey(y+i+1)]!=" ")return false;
			}
		}
		return true;
	}
bool ship::checkBorder(string Pole[s][c])
	{
		for(unsigned int i = 0;i<lenth;i++)
		{
			if(direction==0)
				if((StringToInt(x)+i)>10||(StringToInt(x)+i)<0)return false;
			if(direction==1)
				if((y+i)>10||(y+i)<0)return false;
		}
		return true;
	}
bool ship::checkInsert()
	{
		if(StringToInt(x)>=0&&StringToInt(x)!=40)
			if(y>=1&&y<=10)
				if(lenth>=0&&lenth<=4)
					if(direction==0||direction==1) return true;
		return false;
	}
bool ship::checkDeath(int x, int y,string mas[s][c])
	{	
		string temp = mas[x][y];
		mas[x][y] = "F";

		ship A=getShip(x,y,mas);
		if(A.lenth==1)return true;
		for(unsigned int i = 0;i<A.lenth;i++)
		{
			if(A.direction==1)
				if(mas[StringToInt(A.x)][A.y+i]!="F")
				{
					mas[x][y] = temp;
					return false;
				}
			if(A.direction==0)
				if(mas[StringToInt(A.x)+i][A.y]!="F")
				{
					mas[x][y] = temp;
					return false;
				}
		}
		return true;
	}
void ship::KillingShip(int x, int y,string mas[s][c],string PoleVragaReal[s][c])
	{
		ship A=getShip(x,y,PoleVragaReal);
		if(A.lenth==1)mas[StringToInt(A.x)][A.y]="D";
		else 
		{
			for(unsigned int i = 0;i<A.lenth;i++)
			{
				if(A.direction==1)
					mas[StringToInt(A.x)][A.y+i]="D";
				if(A.direction==0)
					mas[StringToInt(A.x)+i][A.y]="D";
			}
		}
		for(unsigned int i = 0;i<lenth;i++)
		{	
			if(A.direction)
			{
				if(mas[Normalizexforgetship(StringToInt(A.x)-1)][A.y+i]==" ")mas[StringToInt(A.x)-1][A.y+i]="*";
				if(mas[Normalizexforgetship(StringToInt(A.x)+1)][A.y+i]==" ")mas[StringToInt(A.x)+1][A.y+i]="*";
				if((mas[StringToInt(A.x)][Normalizeyforgetship(A.y-1+i)]==" ")&&mas[StringToInt(A.x)][Normalizeyforgetship(A.y-1+i)]!="D")mas[StringToInt(A.x)][A.y-1+i]="*";
				if((mas[StringToInt(A.x)][Normalizeyforgetship(A.y+1+i)]==" ")&&mas[StringToInt(A.x)][Normalizeyforgetship(A.y+1+i)]!="D")mas[StringToInt(A.x)][A.y+1+i]="*";
				if(mas[Normalizexforgetship(StringToInt(A.x)+1)][Normalizeyforgetship(A.y+i+1)]==" ")mas[StringToInt(A.x)+1][A.y+i+1]="*";
				if(mas[Normalizexforgetship(StringToInt(A.x)+1)][Normalizeyforgetship(A.y+i-1)]==" ")mas[StringToInt(A.x)+1][A.y+i-1]="*";
				if(mas[Normalizexforgetship(StringToInt(A.x)-1)][Normalizeyforgetship(A.y+i-1)]==" ")mas[StringToInt(A.x)-1][A.y+i-1]="*";
				if(mas[Normalizexforgetship(StringToInt(A.x)-1)][Normalizeyforgetship(A.y+i+1)]==" ")mas[StringToInt(A.x)-1][A.y+i+1]="*";
			}
			if(!A.direction)
			{
				if((mas[Normalizexforgetship(StringToInt(A.x)-1+i)][A.y]==" ")&&mas[Normalizexforgetship(StringToInt(A.x)-1+i)][A.y]!="D")mas[StringToInt(A.x)-1+i][A.y]="*";
				if((mas[Normalizexforgetship(StringToInt(A.x)+1+i)][A.y]==" ")&&mas[Normalizexforgetship(StringToInt(A.x)+1+i)][A.y]!="D")mas[StringToInt(A.x)+1+i][A.y]="*";
				if(mas[StringToInt(A.x)+i][Normalizeyforgetship(A.y-1)]==" ")mas[StringToInt(A.x)+i][A.y-1]="*";
				if(mas[StringToInt(A.x)+i][Normalizeyforgetship(A.y+1)]==" ")mas[StringToInt(A.x)+i][A.y+1]="*";
				if(mas[Normalizexforgetship(StringToInt(A.x)+1+i)][Normalizeyforgetship(A.y+1)]==" ")mas[StringToInt(A.x)+1+i][A.y+1]="*";
				if(mas[Normalizexforgetship(StringToInt(A.x)+1+i)][Normalizeyforgetship(A.y-1)]==" ")mas[StringToInt(A.x)+1+i][A.y-1]="*";
				if(mas[Normalizexforgetship(StringToInt(A.x)-1+i)][Normalizeyforgetship(A.y-1)]==" ")mas[StringToInt(A.x)-1+i][A.y-1]="*";
				if(mas[Normalizexforgetship(StringToInt(A.x)-1+i)][Normalizeyforgetship(A.y+1)]==" ")mas[StringToInt(A.x)-1+i][A.y+1]="*";
			}
		}
	}
char ship::getFiredSymbol()
	{
		return FiredSymbol;
	}
ship ship::getShip(int x,int y,string mas[s][c])
	{
		lenth = 0;
		if(mas[x][y]=="S"||mas[x][y]=="F"||mas[x][y]=="D")
		{
			lenth = 1;
			if((mas[x][Normalizeyforgetship(y+1)]=="D"||mas[x][Normalizeyforgetship(y+1)]=="F"||mas[x][Normalizeyforgetship(y+1)]=="S")||(mas[x][Normalizeyforgetship(y-1)]=="D"||mas[x][Normalizeyforgetship(y-1)]=="F"||mas[x][Normalizeyforgetship(y-1)]=="S"))
			{
				direction=1;
				for(int i = 1;i<6;i++)
				{
					if(!(mas[x][Normalizeyforgetship(y+i)]=="D"||mas[x][Normalizeyforgetship(y+i)]=="F"||mas[x][Normalizeyforgetship(y+i)]=="S"))
					{
						y=Normalizeyforgetship(y+i-1);
						i=6;
					}
				}
				for(int i = 0;i<5;i++)
				{
					if(!(mas[x][Normalizeyforgetship(y-1-i)]=="D"||mas[x][Normalizeyforgetship(y-1-i)]=="F"||mas[x][Normalizeyforgetship(y-1-i)]=="S"))
					{
						y=Normalizeyforgetship(y-i);
						lenth=++i;
						ship A(lenth,IntToString(x),y,direction);
						return A;
					}
				}
			}
			else if((mas[Normalizexforgetship(x+1)][y]=="D"||mas[Normalizexforgetship(x+1)][y]=="F"||mas[Normalizexforgetship(x+1)][y]=="S")||(mas[Normalizexforgetship(x-1)][y]=="D"||mas[Normalizexforgetship(x-1)][y]=="F"||mas[Normalizexforgetship(x-1)][y]=="S"))
			{
				direction=0;
				for(int i = 1;i<6;i++)
				{
					if(!(mas[Normalizexforgetship(x+i)][y]=="D"||mas[Normalizexforgetship(x+i)][y]=="F"||mas[Normalizexforgetship(x+i)][y]=="S"))
					{
						x=Normalizexforgetship(x+i-1);
						i=6;
					}
				}
				for(int i = 0;i<5;i++)
				{
					if(!(mas[Normalizexforgetship(x-1-i)][y]=="D"||mas[Normalizexforgetship(x-1-i)][y]=="F"||mas[Normalizexforgetship(x-1-i)][y]=="S"))
					{
						x=Normalizexforgetship(x-i);
						lenth=++i;
						ship A(lenth,IntToString(x),y,direction);
						return A;
					}
				}
			}
			else 
			{
				ship A(lenth,IntToString(x),y,direction);
				return A;
			}
		}
		else 
		{
			ship A(lenth,IntToString(x),y,direction);
			return A;
		}
	}
int ship::Normalizex(int y)
	{
		if(y>-1&&y<10)return y;
		if(y<0) return 0;
		if(y>9) return 9;
	}
int ship::Normalizexforgetship(int y)
	{
		if(y>-1&&y<10)return y;
		else return 10;
	}
int ship::Normalizey(int x)
	{
		if(x>0&&x<11)return x;
		if(x<1) return 1;
		if(x>10) return 10;
	}
int ship::Normalizeyforgetship(int x)
	{
		if(x>0&&x<11)return x;
		else return 0;
	}
void main()
{
	double version = 0.2;
	setlocale(LC_CTYPE,"Russian");
	Start(version);
}
void Start(double version)
{
	char vvod = 0;

	system("cls");
	cout<<"Вас приветствует моя первая игра"<<endl;
	cout<<"Морской бой версия "<<version<<endl;
	
	cout<<"----------------------------------------------------------------------------"<<endl;
	cout<<"                                Правила игры:                                "<<endl;
	cout<<"В игре морской бой у игрока и у компьютера есть по 10 кораблей"<<endl;
	cout<<"Один четырехпалубный или линкор"<<endl;
	cout<<"Два трехпалубных или крейсеры"<<endl;
	cout<<"Три двухпалубных или эсминцы"<<endl;
	cout<<"Четыре однопалубных или подводные лодки"<<endl;
	cout<<"Цель игры морской бой - убить все корабли противника."<<endl<<endl;

	cout<<"Палубы одного корабля должны быть расположены в одну линию"<<endl;
	cout<<"Клетки рядом с кораблем не могут быть заняты другими кораблями"<<endl;
	cout<<"В игре стрельба ведется по очереди"<<endl;
	cout<<"Результатом выстрела может быть один из 3 вариантов:"<<endl;
	cout<<"Промах, что значит что по координатам стрельбы нету палуб корабля"<<endl;
	cout<<"Ранил, что значит что по координатам стрельбы палуба коробля потоплена"<<endl;
	cout<<"Убил, что значит что последняя палуба судна потоплена"<<endl;
	cout<<"Если выстрел был удачен(ранил или убил), то игрок ходит еще раз."<<endl;
	cout<<"Игра не понимает русский ввод, и чувствительна к регистру"<<endl;
	cout<<"Игра воспринимает ввод первой координаты цифрой.(а=1,b=2,c=3,d=4 и т.д.)"<<endl;
	cout<<"Приятной игры!"<<endl;
	cout<<"----------------------------------------------------------------------------"<<endl;
	

	for(;;)
	{
		cout<<"Для начала игры нажмите Enter, для выхода Esc."<<endl;
		vvod = _getch();
		if(static_cast<int>(vvod)==13)
		{
			game();
			exit(0);
		};
		if(static_cast<int>(vvod)==27)
		{
			exit(0);
		};
	}
}
void Print(string PoleVraga[s][c],string PoleIgroka[s][c])
{
	system("cls");
	cout<<"Ваше поле: \t\tПоле врага: "<<endl<<endl;
	for(int i = 0;i<11;i++)
	{
		for(int j = 0;j<11;j++)									
		{
			cout<<PoleIgroka[i][j];
		}
		cout<<"\t"<<"\t";
		for(int j = 0;j<11;j++)
		{
			cout<<PoleVraga[i][j];
		}
		cout<<endl;
	}
	cout<<endl;
}
bool Win(string Mas[s][c])
{
	for(int i = 0;i<11;i++)
		for(int j = 0;j<11;j++)									
			if(Mas[i][j]=="S") return false;
	return true;
}
void Init(string PoleVraga[s][c],string PoleIgroka[s][c],string PoleVragaReal[s][c])
{
	
	for(int i = 0;i<10;i++)
		for(int j = 1;j<11;j++)
		{
			PoleIgroka[i][j]=' ';
			PoleVraga[i][j]=' ';
			PoleVragaReal[i][j]=' ';
		}
	PoleIgroka[0][0]='a';
	PoleIgroka[1][0]='b';
	PoleIgroka[2][0]='c';
	PoleIgroka[3][0]='d';
	PoleIgroka[4][0]='e';
	PoleIgroka[5][0]='f';
	PoleIgroka[6][0]='g';
	PoleIgroka[7][0]='h';
	PoleIgroka[8][0]='i';
	PoleIgroka[9][0]='j';
	PoleIgroka[10][0]="/";
	PoleIgroka[10][1]="1";
	PoleIgroka[10][2]="2";
	PoleIgroka[10][3]="3";
	PoleIgroka[10][4]="4";
	PoleIgroka[10][5]="5";
	PoleIgroka[10][6]="6";
	PoleIgroka[10][7]="7";
	PoleIgroka[10][8]="8";
	PoleIgroka[10][9]="9";
	PoleIgroka[10][10]="0";

	PoleVraga[0][0]='a';
	PoleVraga[1][0]='b';
	PoleVraga[2][0]='c';
	PoleVraga[3][0]='d';
	PoleVraga[4][0]='e';
	PoleVraga[5][0]='f';
	PoleVraga[6][0]='g';
	PoleVraga[7][0]='h';
	PoleVraga[8][0]='i';
	PoleVraga[9][0]='j';
	PoleVraga[10][0]="/";
	PoleVraga[10][1]="1";
	PoleVraga[10][2]="2";
	PoleVraga[10][3]="3";
	PoleVraga[10][4]="4";
	PoleVraga[10][5]="5";
	PoleVraga[10][6]="6";
	PoleVraga[10][7]="7";
	PoleVraga[10][8]="8";
	PoleVraga[10][9]="9";
	PoleVraga[10][10]="0";

	PoleVragaReal[0][0]='a';
	PoleVragaReal[1][0]='b';
	PoleVragaReal[2][0]='c';
	PoleVragaReal[3][0]='d';
	PoleVragaReal[4][0]='e';
	PoleVragaReal[5][0]='f';
	PoleVragaReal[6][0]='g';
	PoleVragaReal[7][0]='h';
	PoleVragaReal[8][0]='i';
	PoleVragaReal[9][0]='j';
	PoleVragaReal[10][0]="/";
	PoleVragaReal[10][1]="1";
	PoleVragaReal[10][2]="2";
	PoleVragaReal[10][3]="3";
	PoleVragaReal[10][4]="4";
	PoleVragaReal[10][5]="5";
	PoleVragaReal[10][6]="6";
	PoleVragaReal[10][7]="7";
	PoleVragaReal[10][8]="8";
	PoleVragaReal[10][9]="9";
	PoleVragaReal[10][10]="0";
}
int Insert(string& x,int& y)
{	
	cout<<"координата появится как только ввод будет корректен"<<endl;
	while(1)
	{
		y=1;
		x=_getch();
		ship A(0,x,y,0);
		if(A.checkInsert())
		{
			cout<<x<<"-";
			while(1)
			{
				y=A.StringToIntForY(_getch());
				ship A(0,x,y,0);
				if(A.checkInsert())
				{
					cout<<y<<endl;
					return 0;
				}
			}
		}
	}
}
void PlaceShip(int lenth,string PoleVraga[s][c],string PoleIgroka[s][c])
{
	string jopa1 = "a";
	int jopa2 = 1;
	char dirchar = ' ';
	int direction = 0;
	bool b;
	ship B;

	cout<<"Введите данные "<<lenth<<" палубного корабля: "<<endl;
	Insert(jopa1,jopa2);
	if(lenth>1)
	{
		int i = 1;
		cout<<"Направление носа(0-ВНИЗ, 1-ВПРАВО): ";
		while(i)
		{
			direction=B.StringToIntForY(_getch());
			ship B(0,"a",1,direction);
			if(B.checkInsert())
			{
				cout<<direction<<endl;
				i--;
			}
		}	
	}
	ship A(lenth,jopa1,jopa2,direction,'S');
	b=A.checkNearest(PoleIgroka);
	if(!b)
	{
		cout<<"Ошибка размещения, нажмите любую клавишу для повторного ввода"<<endl;
		_getch();
		Print(PoleVraga,PoleIgroka);
		PlaceShip(lenth,PoleVraga,PoleIgroka);
	}
	else if(b)
	{
		A.addToMap(PoleIgroka);
		Print(PoleVraga,PoleIgroka);
	}
}
int Vistrel(int x, int y,string mas[s][c])
{
	if(x<=9&&x>=0&&y>=1&&y<=10)
	{
		if(mas[x][y]==" ") return 2;
		else if(mas[x][y]=="S") return 1;
		else if(mas[x][y]=="*"||mas[x][y]=="F"||mas[x][y]=="D") return 0;
		else return 40;
	}
	else return 40;
}
int PindosMove(string PoleIgroka[s][c],string WhatPindosAlreadyDo[s][c],string PoleVraga[s][c])
{
	int vistrel = 40;
	ship A;
	bool povtor = true;
	int vector;
	bool EndShip,StartShip;
	srand(time(NULL));
	while(1)
	{
		while(povtor&&!luck)
		{
			povtor = false;
			xforcomp=rand()%10;
			yforcomp=rand()%10+1;
			vistrel=Vistrel(xforcomp,yforcomp,PoleIgroka);
			if(vistrel==2)
			{
				PoleIgroka[xforcomp][yforcomp]="*";
				WhatPindosAlreadyDo[xforcomp][yforcomp]="*";
				Print(PoleVraga,PoleIgroka);
				cout<<"Противник промахнулся"<<endl;
				cout<<"Координаты выстрела были \""<<A.IntToString(xforcomp)<<"\" и "<<yforcomp<<endl;
				_getch();
				return 0;
			}
			if(vistrel==1)
			{
				bool b = A.checkDeath(xforcomp,yforcomp,PoleIgroka);
				if(!b)
				{
					PoleIgroka[xforcomp][yforcomp]=A.getFiredSymbol();
					WhatPindosAlreadyDo[xforcomp][yforcomp]=A.getFiredSymbol();
					Print(PoleVraga,PoleIgroka);
					cout<<"Ваш корабль ранен!"<<endl;
					cout<<"Координаты выстрела были \""<<A.IntToString(xforcomp)<<"\" и "<<yforcomp<<endl;
					cout<<"Противник ходит еще раз!"<<endl;
					_getch();
					povtor = true;
					luck = true;
					EndShipX = xforcomp;
					EndShipY =  yforcomp;
					StartShipX = xforcomp;
					StartShipY = yforcomp;
				}
				if(b)
				{
					PoleIgroka[xforcomp][yforcomp]=A.getFiredSymbol();
					WhatPindosAlreadyDo[xforcomp][yforcomp]=A.getFiredSymbol();
					A.KillingShip(xforcomp,yforcomp,PoleIgroka,PoleIgroka);
					A.KillingShip(xforcomp,yforcomp,WhatPindosAlreadyDo,WhatPindosAlreadyDo);
					Print(PoleVraga,PoleIgroka);
					cout<<"Противник потопил ваш корабль!"<<endl;
					cout<<"Координаты выстрела были \""<<A.IntToString(xforcomp)<<"\" и "<<yforcomp<<endl;
					cout<<"Повторный ход!"<<endl;
					_getch();
					povtor = true;
				}
			}
			if(vistrel==0)povtor = true;
			if(vistrel==40)povtor = true;
		}
		while(povtor&&luck)
		{
			povtor = false;
			EndShip = false; 
			StartShip = false;

			if(!(directionofluckfire==1||directionofluckfire==0))
			{
				vistrel = 40;
				do
				{
					vector = rand()%4;
					if(vector==0&&PoleIgroka[A.Normalizex(xforcomp+1)][yforcomp]!="*")vistrel=Vistrel(xforcomp+1,yforcomp,PoleIgroka);
					else if(vector==2&&PoleIgroka[A.Normalizex(xforcomp-1)][yforcomp]!="*")vistrel=Vistrel(xforcomp-1,yforcomp,PoleIgroka);
					else if(vector==1&&PoleIgroka[xforcomp][A.Normalizey(yforcomp+1)]!="*")vistrel=Vistrel(xforcomp,yforcomp+1,PoleIgroka);
					else if(vector==3&&PoleIgroka[xforcomp][A.Normalizey(yforcomp-1)]!="*")vistrel=Vistrel(xforcomp,yforcomp-1,PoleIgroka);
				}
				while(vistrel==0||vistrel==40);

				if(vector==0)xforcomp++;
				else if(vector==2)xforcomp--;
				else if(vector==1)yforcomp++;
				else if(vector==3)yforcomp--;
			
				if(vistrel==2)
				{
					PoleIgroka[xforcomp][yforcomp]="*";
					WhatPindosAlreadyDo[xforcomp][yforcomp]="*";
					Print(PoleVraga,PoleIgroka);
					cout<<"Противник промахнулся"<<endl;
					cout<<"Координаты выстрела были \""<<A.IntToString(xforcomp)<<"\" и "<<yforcomp<<endl;
					_getch();
					if(vector==0)xforcomp--;
					else if(vector==2)xforcomp++;
					else if(vector==1)yforcomp--;
					else if(vector==3)yforcomp++;
					return 0;
				}
				if(vistrel==1)
				{
					bool b = A.checkDeath(xforcomp,yforcomp,PoleIgroka);
					if(!b)
					{
						PoleIgroka[xforcomp][yforcomp]=A.getFiredSymbol();
						WhatPindosAlreadyDo[xforcomp][yforcomp]=A.getFiredSymbol();
						Print(PoleVraga,PoleIgroka);
						cout<<"Ваш корабль ранен!"<<endl;
						cout<<"Координаты выстрела были \""<<A.IntToString(xforcomp)<<"\" и "<<yforcomp<<endl;
						cout<<"Противник ходит еще раз!"<<endl;
						_getch();
						povtor = true;
						if(vector==0||vector==2)
						{
							directionofluckfire=0;
							if(vector==0)EndShipX = xforcomp;
							if(vector==2)StartShipX = xforcomp;
						}
						else if(vector==1||vector==3)
						{
							directionofluckfire=1;
							if(vector==1)EndShipY = yforcomp;
							if(vector==3)StartShipY = yforcomp;
						}
					}
					if(b)
					{
						PoleIgroka[xforcomp][yforcomp]=A.getFiredSymbol();
						WhatPindosAlreadyDo[xforcomp][yforcomp]=A.getFiredSymbol();
						A.KillingShip(xforcomp,yforcomp,PoleIgroka,PoleIgroka);
						A.KillingShip(xforcomp,yforcomp,WhatPindosAlreadyDo,WhatPindosAlreadyDo);
						Print(PoleVraga,PoleIgroka);
						cout<<"Противник потопил ваш корабль!"<<endl;
						cout<<"Координаты выстрела были \""<<A.IntToString(xforcomp)<<"\" и "<<yforcomp<<endl;
						cout<<"Повторный ход!"<<endl;
						_getch();
						povtor = true;
						luck = false;
						directionofluckfire = 2;
						yforcomp = xforcomp = 0;
					}
				}
			}
			else if(directionofluckfire!=2)
			{
				vistrel = 40;
				do
				{
					vector = rand()%2+directionofluckfire*2;
					if(vector==2&&PoleIgroka[xforcomp][EndShipY+1]!="*")vistrel=Vistrel(xforcomp,EndShipY+1,PoleIgroka);
					else if(vector==3&&PoleIgroka[xforcomp][StartShipY-1]!="*")vistrel=Vistrel(xforcomp,StartShipY-1,PoleIgroka);
					else if(vector==0&&PoleIgroka[EndShipX+1][yforcomp]!="*")vistrel=Vistrel(EndShipX+1,yforcomp,PoleIgroka);
					else if(vector==1&&PoleIgroka[StartShipX-1][yforcomp]!="*")vistrel=Vistrel(StartShipX-1,yforcomp,PoleIgroka);
				}
				while(vistrel==0||vistrel==40);

				if(vector==2)yforcomp=EndShipY+1;
				else if(vector==3)yforcomp=StartShipY-1;
				else if(vector==0)xforcomp=EndShipX+1;
				else if(vector==1)xforcomp=StartShipX-1;
		
				if(vistrel==2)
				{
					PoleIgroka[xforcomp][yforcomp]="*";
					WhatPindosAlreadyDo[xforcomp][yforcomp]="*";
					Print(PoleVraga,PoleIgroka);
					cout<<"Противник промахнулся"<<endl;
					cout<<"Координаты выстрела были \""<<A.IntToString(xforcomp)<<"\" и "<<yforcomp<<endl;
					_getch();
					if(vector==2)yforcomp=EndShipY;
					else if(vector==3)yforcomp=StartShipY;
					else if(vector==0)xforcomp=EndShipX;
					else if(vector==1)xforcomp=StartShipX;
					return 0;
				}
				if(vistrel==1)
				{
					bool b = A.checkDeath(xforcomp,yforcomp,PoleIgroka);
					if(!b)
					{
						PoleIgroka[xforcomp][yforcomp]=A.getFiredSymbol();
						WhatPindosAlreadyDo[xforcomp][yforcomp]=A.getFiredSymbol();
						Print(PoleVraga,PoleIgroka);
						cout<<"Ваш корабль ранен!"<<endl;
						cout<<"Координаты выстрела были \""<<A.IntToString(xforcomp)<<"\" и "<<yforcomp<<endl;
						cout<<"Противник ходит еще раз!"<<endl;
						_getch();
						povtor = true;
						if(vector==2)EndShipY = yforcomp;
						else if(vector==3)StartShipY = yforcomp;
						else if(vector==0)EndShipX = xforcomp;
						else if(vector==1)StartShipX = xforcomp;
					}
					if(b)
					{
						PoleIgroka[xforcomp][yforcomp]=A.getFiredSymbol();
						WhatPindosAlreadyDo[xforcomp][yforcomp]=A.getFiredSymbol();
						A.KillingShip(xforcomp,yforcomp,PoleIgroka,PoleIgroka);
						A.KillingShip(xforcomp,yforcomp,WhatPindosAlreadyDo,WhatPindosAlreadyDo);
						Print(PoleVraga,PoleIgroka);
						cout<<"Противник потопил ваш корабль!"<<endl;
						cout<<"Координаты выстрела были \""<<A.IntToString(xforcomp)<<"\" и "<<yforcomp<<endl;
						cout<<"Повторный ход!"<<endl;
						_getch();
						povtor = true;
						luck = false;
						directionofluckfire = 2;
						yforcomp = xforcomp = 0;
					}
				}
			}
		}
	}
}
int HodIgroka(string PoleVraga[s][c],string PoleIgroka[s][c],string PoleVragaReal[s][c])
{
	while(1)
	{
		int vistrely = 0;
		int vistrel = 40;
		int vistrelx = 0;
		string vvodx=" ";
		ship A;
		Print(PoleVraga,PoleIgroka);
		cout<<"Ваш ход"<<endl;
		cout<<"введите координаты выстрела: "<<endl;

		Insert(vvodx,vistrely);

		vistrelx=A.StringToInt(vvodx);		
		vistrel = Vistrel(vistrelx,vistrely,PoleVragaReal);
		if(vistrel==2)
		{
			PoleVraga[vistrelx][vistrely]="*";
			PoleVragaReal[vistrelx][vistrely]="*";
			Print(PoleVraga,PoleIgroka);
			cout<<"Вы промахнулись!"<<endl;
			cout<<"Ход перешел к компьютеру"<<endl;
			_getch();
			return 0;
		}
		else if(vistrel==1)
		{	
			bool b = A.checkDeath(vistrelx,vistrely,PoleVragaReal);
			if(!b)
			{
				PoleVraga[vistrelx][vistrely]="F";
				PoleVragaReal[vistrelx][vistrely]="F";
				Print(PoleVraga,PoleIgroka);
				cout<<"Ранил!"<<endl;
				cout<<"Повторный ход!"<<endl;
			}
			if(b)
			{
				A.KillingShip(vistrelx,vistrely,PoleVragaReal,PoleVragaReal);
				A.KillingShip(vistrelx,vistrely,PoleVraga,PoleVragaReal);
				Print(PoleVraga,PoleIgroka);
				cout<<"Убил!"<<endl;
				if(Win(PoleVragaReal))return 1;
				cout<<"Повторный ход!"<<endl;
			}
		}
		else if(vistrel==0) 
		{
			Print(PoleVraga,PoleIgroka);
			cout<<"Вы уже стреляли по этим координатам!"<<endl;
			_getch();
		}
		else if(vistrel==40) 
		{
			Print(PoleVraga,PoleIgroka);
			cout<<"Ошибка ввода"<<endl;
			cout<<"Для повторный ввода нажмите любую клавишу"<<endl;
			_getch();
		}
	}
}
void genField(string mas[s][c])
{
	ship A;
	int y = 0;
	string x = "a";
	int direction = 0;
	unsigned int lenth = 0;
	for(int i = 0;i<1;i++)
	{
		x = A.IntToString(rand()%11+1);
		y = rand()%10;
		direction = rand()%2;
		lenth = 4;
		ship A(lenth,x,y,direction,'S');
		if(A.checkNearest(mas))A.addToMap(mas);
			else i--;
	}
	for(int i = 0;i<2;i++)
	{
		x = A.IntToString(rand()%11+1);
		y = rand()%10;
		direction = rand()%2;
		lenth = 3;
		ship A(lenth,x,y,direction,'S');
		if(A.checkNearest(mas))A.addToMap(mas);
			else i--;
		
	}
	for(int i = 0;i<3;i++)
	{
		x = A.IntToString(rand()%11+1);
		y = rand()%10;
		direction = rand()%2;
		lenth = 2;
		ship A(lenth,x,y,direction,'S');
		if(A.checkNearest(mas)) A.addToMap(mas);
			else i--;
	}
	for(int i = 0;i<4;i++)
	{
		x = A.IntToString(rand()%11+1);
		y = rand()%10;
		direction = rand()%2;
		lenth = 1;
		ship A(lenth,x,y,direction,'S');
		if(A.checkNearest(mas))A.addToMap(mas);
			else i--;
	}
}
void game()
{
		
	string PoleIgroka[s][c];
	string PoleVraga[s][c];
	string PoleVragaReal[s][c];
	string WhatPindosAlreadyDo[s][c];

	Init(PoleVraga,PoleIgroka,PoleVragaReal);
	srand(time(NULL));
	Print(PoleVraga,PoleIgroka);

	cout<<"Вы хотите расставить свои корабли вручную или сгенерировать случайно?"<<endl;
	cout<<"Для расстановки вручную нажмите Enter, для случайной генерации нажмиите Esc"<<endl;
	for(int j = 0;j<1;)
		{
			char vvod = _getch();
			if(static_cast<int>(vvod)==13)
			{
				Print(PoleVraga,PoleIgroka);
				cout<<"Введите данные своих кораблей"<<endl;
				PlaceShip(4,PoleVraga,PoleIgroka);
				for(int i = 0;i<2;i++)PlaceShip(3,PoleVraga,PoleIgroka);
				for(int i = 0;i<3;i++)PlaceShip(2,PoleVraga,PoleIgroka);
				for(int i = 0;i<4;i++)PlaceShip(1,PoleVraga,PoleIgroka);
				j++;
			};
			if(static_cast<int>(vvod)==27)
			{
				genField(PoleIgroka);
				j++;
			};
		}
	genField(PoleVragaReal);
	
	do
	{
	if(HodIgroka(PoleVraga,PoleIgroka,PoleVragaReal)!=1)
	PindosMove(PoleIgroka,WhatPindosAlreadyDo,PoleVraga);
	}while(!Win(PoleIgroka)&&!Win(PoleVragaReal));

	if(!Win(PoleIgroka))
	{
		Print(PoleVragaReal,PoleIgroka);
		cout<<"Вы победили!"<<endl;
		for(;;)
		{
			cout<<"Для повтора игры нажмите Enter, для выхода Esc."<<endl;
			char vvod = _getch();
			if(static_cast<int>(vvod)==13)
			{
				game();
				exit(0);
			};
			if(static_cast<int>(vvod)==27)
			{
				exit(0);
			};
		}
	}
	if(!Win(PoleVragaReal))
	{
		Print(PoleVragaReal,PoleIgroka);
		cout<<"Вы проиграли!"<<endl;
		for(;;)
		{
			cout<<"Для реванша нажмите Enter, для выхода Esc."<<endl;
			char vvod = _getch();
			if(static_cast<int>(vvod)==13)
			{
				game();
				exit(0);
			};
			if(static_cast<int>(vvod)==27)
			{
				exit(0);
			};
		}
	}
}