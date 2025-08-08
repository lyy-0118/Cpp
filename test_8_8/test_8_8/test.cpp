//#include <iostream>
//#define M 3
//#define N 4
//#define P 5
//using namespace std;
//
//int main()
//{
//	//new delete三维数组
//	//假设数组第一维为M，第二维为N，第三维为P
//	int*** array = new int** [M];  //分配第一维
//	for (int i = 0; i < M; i++) {
//		array[i] = new int* [N];  //分配第二维
//		for (int j = 0; j < N; j++) {
//			array[i][j] = new int[P];  //分配第三维
//		}
//	}
//	//释放
//	for (int i = 0; i < M; i++)
//	{
//		for (int j = 0; j < N; j++)
//		{
//			delete[] array[i][j];
//		}
//		delete[] array[i];
//	}
//	delete[] array;
//	return 0;
//}