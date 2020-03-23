#include <iostream>
#include <string>
#include <vector>
#include <fstream>
using namespace std;

static int rsize = 0;
static int csize = 0;
class Lexical {

	int **lex;
	int rowCount;
	int colCount;
	int cset[128]; // character set
	int x;
public:
	Lexical(int **l, int row, int col, int data[])
	{
		rowCount = row;
		colCount = col;
		lex = new int*[row];
		for(int i = 0; i < row; ++i)
			lex[i] = new int[col];
		
		for (int i = 0; i < row; i++)
		{
			for (int j = 0; j < col; j++)
			{
				lex[i][j] = l[i][j];
			}
		}
		for (int i = 0; i < 128; i++)
		{
			cset[i] = data[i];
		}
		
	}
	string generateLexeme(string str, int lexeme_start, int remember)
	{
		string lexeme = "";
		for (int i = lexeme_start; i <= remember; i++)
		{
			lexeme+=str[i];
		}
		return lexeme;
	}
	void Analyzer(int a)
	{
		int ini_state = 0;
		int forward = a;
		int current_state = ini_state;
		int lexeme_start = forward;
		int LSAS = NULL;
		int line_no = 1;
		int remember = NULL;
		ofstream token;
		token.open("token.txt");
		ofstream err_f;
		err_f.open("error.txt");
		ifstream file;
		string str;
		file.open("input.txt");
		str="";
		getline(file, str, '\0');
		//system("pause");
xy:		while(forward < str.length())
		{
		
			if (cset[str[forward]]!=-1 && lex[current_state][cset[str[forward]]] != -1)
			{
				//cout << cset[str[forward]];
				//cout << colCount;
				if(str[forward] == '\n')
				{
					line_no++;
				}
				current_state = lex[current_state][cset[str[forward]]];
				forward++;
				if(lex[current_state][colCount-1] == 1)
				{
					remember = forward-1;
					LSAS = current_state;
				}
			}
			else 
			{
				if (LSAS != NULL) //forward-1 is final state
				{
					string lexeme = generateLexeme(str,lexeme_start,remember);
					if(LSAS == 1) // current state is identifier
					{
						token << "Token : " << lexeme << endl;
						cout << "Token : " << lexeme << endl;
					}
					else
					{
						token << "Token : " << lexeme << endl;
						cout << "Token : " << lexeme << endl;
					}
					forward = remember+1;
					LSAS = NULL;
					lexeme_start = remember+1;
					remember = NULL;
					current_state=ini_state;
				}
				else 
				{	
					if(lexeme_start!=forward)
					{
						for (int i = lexeme_start ; i < forward; i++)
						{
							if(str[i] == '\n')
							{
								line_no++;
							}
							err_f << "Error at " << "line " << line_no << " : " << str[i] << endl;
							cout << "Error at " << "line " << line_no << " : " << str[i] << endl;
						}
						lexeme_start = forward;
						current_state=ini_state;
					}
					else
					{
						if(str[forward] == '\n')
						{
							line_no++;
						}
						err_f << "Error at " << "line " << line_no << " : " << str[forward] << endl;
						cout << "Error at " << "line " << line_no << " : " << str[forward] << endl;
						forward = forward+1;
						//cout << "forward : " << forward << endl;
						lexeme_start = forward;
						current_state=ini_state;
					}
					
				}
			}
		}

		if (LSAS != NULL)  // If all string is lexeme
				{
					string lexeme = generateLexeme(str,lexeme_start,remember);
					if(LSAS == 1) // curren state is identifier
					{
						token << "Token : " << lexeme << endl;
						cout << "Token : " << lexeme << endl;
					}
					else
					{
						token << "Token : " << lexeme << endl;
						cout << "Token : " << lexeme << endl;
					}
					forward = remember+1;
					LSAS = NULL;
					lexeme_start = remember+1;
					remember = NULL;
					current_state = ini_state;
					goto xy;
				}
		token.close();
		err_f.close();
	}
};

	static void compact(int **l, int **l1, int row, int col , int *header , int data[])
	{
		/*for(int i = 0 ; i < col ; i++)
		{
			cout << header[i] << endl;
		}*/
		int *arr = new int[col];
		for (int i = 0; i < col ; i++)
		{
			arr[i] = 0;
		}
		int colcount = 0;
		int count = 0;
		int k = 0;
		
		while(k < col-2)
		{
			int i = k+1;
			while (i < col-1)
			{
				for (int j = 0; j < row; j++)
				{
					if(l1[j][k] == l[j][i])
					{
						count++;
					}
				}
				if(count == row)
				{
					if(arr[k] == 0)
					{
						colcount++;
						arr[k] = 1;
						arr[i] = header[k];
					}
					else if (arr[k] == 1)
					{
						arr[i] = header[k];
						colcount++;
					}
					else			//contains header
					{
						arr[i] = arr[k];
					}
				}
				count = 0;
				i++;
			}
			k++;
		}
		
		int** ary = new int*[row];
		for(int i = 0; i < row; ++i)
			ary[i] = new int[col-colcount];
		csize = col-colcount; // column size set
		int r = 0, c = 0;
		/*for (int i = 0 ; i < col ; i++)
		{
			cout << arr[i] << "\t" << endl; 
		}
		system("pause");*/
		
		for (int i = 0; i < row; i++)
		{
			for (int j = 0; j < col; j++)
			{
				if(arr[j] == 1)
				{
					ary[r][c] = l[i][j];
					data[header[j]] = c;
					c++;
					
				}
				else if(arr[j] == 0)
				{
					ary[r][c] = l[i][j];
					data[header[j]] = c;
					c++;
					
				}
				else
				{
					data[header[j]] = data[arr[j]];	
				}
			}
			c=0;
			r++;
		}
		rsize = row; // row size set;
		/*for (int i = 0; i < 128; i++)
		{
			cout << i << "\t" << data[i] << endl;
		}*/
		//system("pause");
		/*for (int i = 0; i < rsize; i++)
		{
			for (int j = 0; j < csize; j++)
			{
				cout << ary[i][j] << "\t";
			}
			cout << endl;
		}*/
		
		Lexical obj(ary, rsize, csize, data);
		
		obj.Analyzer(0);
		
	}

void main()
{
	int rowCount = 0, colCount = 0;
	int data[128];
	string header = "";
	string res="";
	string temp="";
	string temp1="";
	ifstream file;
	file.open("spec1.txt");
	getline(file, temp);
	int ln = 0;
	for (int i = 0; i < 128; i++)
	{
		data[i] = -1;
	}
	for (int i = 0; i < temp.length(); i++)
	{
		if(temp[i] == ',')
		{
			data[temp[i]] = ln;
			ln++;
		}
		if(temp[i] == ',' && temp[i+1] ==',')
			break;
		
	}
	/*cout << ln;
	system("pause");*/
	//cout << header[16];
	int *h = new int[ln+1];
	for(int i = 0; i < ln+1; i++ )
		h[i] = 0;
	int index = 0;
	string tmp = "";
	for (int i = 0; i < temp.length(); i++)
	{
		if(temp[i]!=',')
			tmp+=temp[i];
		if(temp[i] == ',')
		{
			if(tmp!="")
			{
				h[index] = stoi(tmp);
				index++;
				tmp="";
			}
		}
	}

	/*for(int i = 0 ; i < ln ; i++)
	{
		cout << h[i] << endl;
	}
	system("pause");*/
	while(!file.eof())
	{
		getline(file, res);
		temp1+=res;
		//cout << res << endl;
		rowCount++;
	}
	//cout << temp1;
	for (int i=0; i < res.length(); i++)
	{
		if(res[i]!='-' && res[i]!=',')
		{
			colCount++;
		}
		temp+=res[i];
	}
	
	//cout << "Hello : " <<res.length() << "kay";
	int** ary = new int*[rowCount];
	for(int i = 0; i < rowCount; ++i)
		ary[i] = new int[colCount];
	int k = 0;
	int bit = 0;
	for(int i = 0; i < rowCount; i++)
	{
		for(int j = 0; j < colCount; j++)
		{
			if(temp1[k] == '-')
			{
				bit = 1;
				j--;
			}
			else if(temp1[k] == ',')
			{
				j--;
			}
			else
			{
				if(bit == 1)
				{
					ary[i][j] = -(temp1[k]-48);
					bit = 0;
				}
				else
				{
					ary[i][j] = temp1[k]-48;
				}
			}
			k++;
		}
	}
	//cout << temp1;
	/*for (int i = 0; i < rowCount; i++)
		{
			for (int j = 0; j < colCount; j++)
			{
				cout << ary[i][j] << "\t";
			}
			cout << endl;
		}*/
	compact(ary,ary,rowCount,colCount,h, data);
}
	