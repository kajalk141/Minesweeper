### Reference: [Minewsweeper](https://ankitkeshavdbg.github.io/MineSweeper/)

```
//STAGE 1

#include<iostream>
using namespace std;
#define size 8
int matrix[size][size];

void initialize_matrix(){
	for(int i=0; i<size; i++){
		for(int j=0; j<size; j++){
			matrix[i][j]=1;
		}
	}
}

void print(){
	for(int i=0; i<size; i++){
		for(int j=0; j<size; j++){
			cout<<matrix[i][j]<<" ";
		}
		cout<<"\n";
	}
	cout<<"\n";
}

int calc_mines(int row, int col_start, int col_count){
	int count_mines=0;
	while(col_count--){
		if(matrix[row][col_start+col_count]==2) 
			count_mines++;
	}
	return count_mines;
}

int main(){
	initialize_matrix();
	cout<<"Welcome to minesweeper!\n\nHow many mines? ";
	int n; cin>>n;
	cout<<"\nEnter pairs:\n\n";
	for(int i=0; i<n; i++){
		int x,y; cin>>x>>y;
		if(x<size && y<size) matrix[x][y]=2;
	}
	cout<<"\nGame Started\n\n";
	print();
	int type, row, col_start, col_count;
	while(cin>>type){
		cin>>row>>col_start>>col_count;
		if(type==1 && row<size && (col_start+col_count)<size) 
			cout<<"\nThere are "<<calc_mines(row,col_start, col_count)<<" mine(s) in row "<<row<<", from column "<<col_start<<" to "<<col_start+col_count-1<<"\n\n";
		else cout<<"\nCoordinates not on map\n\n";
		print();
	}
}
```
