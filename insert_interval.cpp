#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) > (b) ? (a) : (b))

void Insert(int* Count, int Index, int Interval[2], int (**List)[2])
{
	
	int (*TempMem)[2] = (int(*)[2])realloc(*List, (*Count + 1) * sizeof(int[2]));
	if(TempMem)
	{
		*List = TempMem;
		(*Count)++;
	}
			
	int Temp[2];
	int Temp2[2];
	memcpy(Temp, Interval, sizeof(int[2]));
	for(int I = Index; I < *Count; I++)
	{
		memcpy(Temp2, (*List)[I], sizeof(int[2]));
		memcpy((*List)[I], Temp, sizeof(int[2]));
		
		memcpy(Temp, Temp2, sizeof(int[2]));
	}
	
}

void PrintList(int Count, int (*List)[2])
{
	for(int I = 0; I < Count; I++)
	{
		printf("[%d, %d] ", List[I][0], List[I][1]);
	}
	printf("\n");
	
}

int main()
{
	int Count = 3;
	int (*Intervals)[2] = (int(*)[2])malloc(3 * sizeof(int[2]));
	int Temp[3][2] = {{2,4}, {7,8}, {11, 13}};
	memcpy(Intervals, Temp, sizeof(Temp));
	
	PrintList(Count, Intervals);
	
	//Test
#if 1
	const int TestIntervalCount = 8;
	int TestIntervals[TestIntervalCount][2] = { {0,1}, {15,16}, {3,9}, {3,12}, {5,6}, {7,8}, {11,12}, {1,14} };
#else
	const int TestIntervalCount = 1;
	int TestIntervals[TestIntervalCount][2] = { {0,12}};
#endif
	
	int NewInterval[2];
	for(int I = 0; I < TestIntervalCount; I++)
	{	
		memcpy(NewInterval, TestIntervals[I], sizeof(TestIntervals[I]));
		
		bool Inserted = false;
		int Index = 0;
		int MergeStart = -1;
		
		while(!Inserted)
		{			
			if(NewInterval[0] > Intervals[Index][1])
			{
				if(Index == (Count - 1))
				{
					Insert(&Count, Index + 1, NewInterval, &Intervals);
					Inserted = true;
					break;
				}
				else
				{
					Index++;
				}
				
			}
			else if(NewInterval[1] < Intervals[Index][0])
			{				
				Insert(&Count, Index, NewInterval, &Intervals);
				Inserted = true;
				break;
			}
			else
			{
				if((NewInterval[0] >= Intervals[Index][0]) && (NewInterval[1] <= Intervals[Index][1]))
				{
					Inserted = true;
					break;
				}
				
				if(MergeStart == -1) {MergeStart = Index;}
				
				NewInterval[0] = MIN(NewInterval[0], Intervals[Index][0]);
				NewInterval[1] = MAX(NewInterval[1], Intervals[Index][1]);
				if(Index == (Count - 1))
				{
					Insert(&Count, Index, NewInterval, &Intervals);
					Inserted = true;
					Index++;
					break;
				}
				Index++;
			}

		}
		
		if(MergeStart != -1)
		{
			int OverlapStart = MergeStart;
			int OverlapEnd = Index - 1;
			int AbsorbedCount = (OverlapEnd - OverlapStart) + 1;
			
			for(int J = 0; J < (Count - AbsorbedCount); J++)
			{
				Intervals[OverlapStart + J][0] = Intervals[OverlapEnd + J + 1][0];
				Intervals[OverlapStart + J][1] = Intervals[OverlapEnd + J + 1][1];
			}
		
			Count -= AbsorbedCount;
			
			Intervals = (int(*)[2])realloc(Intervals, Count * sizeof(int[2]));
			
#ifdef _WIN32
			//For Debugging
			size_t Size = _msize(Intervals);
			size_t Expected = Count * sizeof(int[2]);
			if(Size != Expected)
			{	
				printf("Merge failure: Actual bytes: %zu Expected bytes: %zu\n", Size, Expected);
				break;
			}
			//printf("\nMerge Check: Actual bytes: %zu Expected bytes: %zu\n", Size, Expected);
#endif
		}
		PrintList(Count, Intervals);
	}
	free(Intervals);
}

