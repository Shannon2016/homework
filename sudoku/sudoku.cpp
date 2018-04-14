#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>
#include<cstdlib>
#include<iostream>
#include<fstream>
#include<algorithm>
#include"getopt.h"
using namespace std;

int plate[9][9];
bool flag;
FILE* input, *output_solve;

void create(int n)
{
	FILE* output_create;
	output_create = fopen("sudoku.txt", "w");
	if (!output_create)
	{
		printf("CANNOT open the output file!\n");
		exit(1);
	}
	int change[9] = { 0,3,6,1,4,7,2,5,8 };
	for (int i = 0; i < 6 && n; i++)
	{
		if (i)
			next_permutation(change + 3, change + 6);
		for (int j = 0; j < 6 && n; j++)
		{
			if (j) next_permutation(change + 6, change + 9);
			char num[10] = "412356789";
			for (int k = 0; k < 40320 && n; k++)
			{
				if (k)next_permutation(num + 1, num + 9);
				for (int m = 0; m < 9; m++)
				{
					fputc(num[change[m] % 9], output_create);
					for (int n = 1; n < 9; n++)
					{
						fputc(' ', output_create);
						fputc(num[(n + change[m]) % 9], output_create);
					}
					fputc('\n', output_create);
				}
				if (--n) fputc('\n', output_create);
			}
		}
		change[6] = 2, change[7] = 5, change[8] = 8;
	}
	fclose(output_create);
}

bool check(int x, int m, int n)
{
	for (int i = 0; i < 9; i++)
		if (plate[m][i] == x) return false;
	for (int i = 0; i < 9; i++)
		if (plate[i][n] == x) return false;
	int begin_i = m / 3 * 3;
	int begin_j = n / 3 * 3;
	for (int i = begin_i; i < begin_i + 3; i++)
		for (int j = begin_j; j < begin_j + 3; j++)
			if (plate[i][j] == x) return false;
	return true;
}

void solve(int count)
{
	if (flag) return;
	if (count == 81)
	{
		flag = true;
		for (int i = 0; i < 9; i++)
		{
			fputc(plate[i][0]+ '0', output_solve);
			for (int j = 1; j < 9; j++)
			{
				fputc(' ', output_solve);
				fputc(plate[i][j]+ '0', output_solve);
			}
			fputc('\n', output_solve);
		}
		fputc('\n', output_solve);
		return;
	}
	int m = count / 9;
	int n = count % 9;
	if (plate[m][n] == 0)
	{
		for(int i = 1; i <= 9; i ++)
			if (check(i, m, n))
			{
				plate[m][n] = i;
				solve(count + 1);
			}
		plate[m][n] = 0;
	}
	else solve(count + 1);
}

inline int to_integer(char* arg)
{
	int ret = 0;
	while (*arg)
	{
		if (!isdigit(*arg)) return -1;
		ret = ret * 10 + *arg - '0';
		arg++;
	}
	return ret;
}

int main(int argc, char* argv[])
{
	int opt = getopt(argc, argv, "c:s:");
	if (opt == 'c')
	{
		int n;
		n = to_integer(optarg);
		if (n > 0 && n <= 1000000)
			create(n);
		else printf("Please input a NUMBER from 1 to 1000000!\n");
	}
	else if (opt == 's')
	{
		input = fopen(optarg, "r");
		output_solve = fopen("sudoku.txt", "w");
		if (!input)
		{
			printf("CANNOT find the input file!\n");
			exit(1);
		}
		if (!output_solve)
		{
			printf("CANNOT open the output file!\n");
			exit(1);
		}
		while (fscanf(input, "%d", &plate[0][0]) != EOF)
		{
			for (int j = 1; j < 9; j++)
				fscanf(input, "%d", &plate[0][j]);
			for (int i = 1; i < 9; i++)
				for (int j = 0; j < 9; j++)
					fscanf(input, "%d", &plate[i][j]);
			flag = false;
			solve(0);
		}
		fclose(input);
		fclose(output_solve);
	}
	else printf("INPUT ERROR!\n");
    return 0;
}