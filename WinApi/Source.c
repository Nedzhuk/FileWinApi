#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <Windows.h>

double* Roots(double discr, double* list)
{
	double* roots = calloc(2, sizeof(double));

	double x1 = (-list[1] + sqrt(discr)) / (2 * list[0]);
	double x2 = (-list[1] - sqrt(discr)) / (2 * list[0]);
	roots[0] = x1;
	roots[1] = x2;
	return roots;
}

double Root(double* list)
{
	double x = (-list[1]) / (2 * list[0]);
	return x;
}

int Discriminant(double* list)
{
	double d = (list[1] * list[1]) - (4 * list[0] * list[2]);
	return d;
}

int main() 
{
	system("chcp 1251 > nul");
	char *line = "";
	char buffer1[128];
	char buffer2[128];
	char buffer3[128];
	char bufferResult[128];
	char bufferReference[128];
	double* list = calloc(3, sizeof(double));
	HANDLE fileCoefficient = OpenFile("Coeff.txt", buffer1, OF_READ);
	HANDLE fileReference = OpenFile("Reference.txt", buffer3, OF_CREATE);
	HANDLE fileResult = OpenFile("ResultRoots.txt", buffer2, OF_CREATE);
	DWORD bytes1;
	if (fileCoefficient == INVALID_HANDLE_VALUE) 
	{
		printf("The file does not exist");
		return 1;
	}
	else 
	{
		DWORD size = GetFileSize(fileCoefficient, NULL), bytes;
		int size2 = size + 1;
		char* text = calloc(size2, sizeof(char));
		ReadFile(fileCoefficient, text, size, &bytes, NULL);
		CloseHandle(fileCoefficient);

		text[size] = 0;
		printf(text);
		if (size == 0) 
		{
			printf("File empty");
			int count = sprintf(bufferResult, "File empty");
			WriteFile(fileResult, bufferResult, count, &bytes1, NULL);
			CloseHandle(fileResult);
			return 2;
		}
		else 
		{
			int chet = 0;
			int chetN = 0;
			int numCharForLine = 0;
			printf("\n");
			while (chet < 3) 
			{
				char* tmp = calloc(30, sizeof(char));
				for (int i = 0; i < size + 1; i++) {
					if (text[i] != '\n' && i != 14)
					{
						tmp[i-numCharForLine] = text[i];
					}
					else
					{
						if(i == 14)
							tmp[i - numCharForLine] = text[i];
						chetN++;
						char* num = calloc(20, sizeof(char));
						if (i == 14) 
						{
							for (int j = numCharForLine; j < i+1; j++)
							{
								num[j - numCharForLine] = tmp[j - numCharForLine];
							}
						}
						else 
						{
							for (int j = numCharForLine; j < i; j++)
							{
								num[j - numCharForLine] = tmp[j - numCharForLine];
							}
						}
						numCharForLine = i + 1;
						double c = atof(num);

						if ((tmp[0] >= '0' && tmp[0] <= '9') || (tmp[0] == '-' && (tmp[1] >= '0' && tmp[1] <= '9')))
						{
							if (c == 0 && chet == 0)
							{
								printf("1 coefficient cannot be qual 0");
								int count = sprintf(bufferResult, "1 coefficient cannot be qual 0");
								WriteFile(fileResult, bufferResult, count, &bytes1, NULL);
								CloseHandle(fileResult);
								return 1;
							}
							list[chet] = c;
							printf("%2.3f ", list[chet]);
							chet++;
						}
						else
						{
							printf("\nThe coefficient cannot be a symbol");
							int count = sprintf(bufferResult, "The coefficient cannot be a symbol");
							WriteFile(fileResult, bufferResult, count, &bytes1, NULL);
							CloseHandle(fileResult);
							return 3;
						}
						tmp = calloc(20, sizeof(char));
					}
				}
			}
		}
	}
	double discr = Discriminant(list);
	printf("\nDiscriminant = %2.3f", discr);

	DWORD bytes2;
	int count = 0;
	int count2 = 0;
	if (discr == 0)
	{
		double root = Root(list);
		printf("\nRoot = %2.3f", root);
		count = sprintf(bufferResult, "Root = %2.3f", root);
		count2 =  sprintf(bufferReference, "Discriminant = % 2.3f\nCount root = 1", discr);
	}
	else if (discr > 0)
	{
		double* roots = Roots(discr, list);
		printf("\nRoot 1 = %2.3f\nRoot 2 = %2.3f", roots[0], roots[1]);
		count = sprintf(bufferResult, "Root 1 = %2.3f\nRoot 2 = %2.3f", roots[0], roots[1]);
		count2 = sprintf(bufferReference, "Discriminant = % 2.3f\nCount root = 2", discr);
	}
	else
	{
		printf("\nNo roots");
		count = sprintf(bufferResult, "No roots");
		count2 = sprintf(bufferReference, "Discriminant = % 2.3f\nCount root = 0", discr);
	}
	WriteFile(fileResult, bufferResult, count, &bytes2, NULL);
	WriteFile(fileReference, bufferReference, count2, &bytes2, NULL);
	CloseHandle(fileResult);
	CloseHandle(fileReference);
	return 0;
}