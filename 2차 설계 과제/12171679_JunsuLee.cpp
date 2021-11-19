#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int n, m, q;
vector<pair<int, int>> graph[10001]; //각 vertex와 인접한 vertex를 저장할 벡터
vector<int> print; //답을 출력하기 위한 벡터

class Heap
{
public:
	vector<pair<int, int>> data;
	int heapsize; //벡터에 0번 인덱스를 사용하지 않으므로 실질적인 힙의 크기를 저장할 변수
	int rootindex;
	Heap()
	{
		rootindex = 1; //root의 인덱스
		heapsize = 0;
		data.push_back({ -1,-1 }); //1번 인덱스부터 사용하기 위해 값 하나 넣고 시작
	}

	//decrease key에서 최소 weight가 변화되었을때 더 작아졌으므로 해당 값을 upheap해야할 수도 있음
	void update(pair<int, int> weight_vertex)
	{
		int idx;
		for (int i = 0; i < data.size(); i++)
		{
			if (weight_vertex.second == data[i].second) //weight가 수정된 값의 번호를 가지는 vertex 찾음
			{
				data[i].first = weight_vertex.first; //weight 수정
				idx = i; //upheap 하기 위해 시작 위치를 받아와야 함
				break;
			}
		}

		while (idx > rootindex) //weight 수정 후 upheap
		{
			if (data[idx] <= data[idx / 2]) //부모가 자식보다 크거나 같으면
				swap(data[idx], data[idx / 2]); //자식과 부모 swap (min heap으로 구현하므로)

			else //부모가 자식보다 작으면 swap할 필요 없이 반복문 빠져나옴
				break;

			//swap이 발생한 경우, 원래 부모의 인덱스(현재 swap된 자식의 인덱스)를 기준으로
			//다시 그의 부모와 비교하여 swap 여부 결정
			idx /= 2;
		}
	}

	void push(pair<int, int> weight_vertex) //힙에 데이터 넣음
	{
		data.push_back(weight_vertex);
		heapsize++;
		int tmp = heapsize;
		while (tmp > rootindex) //배열의 끝에서부터, 즉 가장 아래 원소부터 탐색(upheap)(이때 root보다 인덱스가 작아지면 안됨)
		{
			if (data[tmp] <= data[tmp / 2]) //부모가 자식보다 크거나 같으면
			{
				swap(data[tmp], data[tmp / 2]); //자식과 부모 swap (min heap으로 구현하므로)
			}
			else //부모가 자식보다 작으면 swap할 필요 없이 반복문 빠져나옴
				break;

			//swap이 발생한 경우, 원래 부모의 인덱스(현재 swap된 자식의 인덱스)를 기준으로
			//다시 그의 부모와 비교하여 swap 여부 결정
			tmp /= 2;
		}
	}

	void pop() //힙의 최상단, 즉 최소 weight를 가지는 vertex 삭제
	{
		if (heapsize > 0)
		{
			//가장 최하단 오른쪽 끝의 원소를 삭제하기 전에 root에 복사
			data[1] = data[heapsize--];
			data.pop_back();

			int tmp = rootindex;
			while (tmp * 2 <= heapsize) //자식 인덱스로 내려가면서 비교(downheap)(가장 끝 인덱스를 초과하면 안됨)
			{
				int left = tmp * 2;
				int right = tmp * 2 + 1;
				int min;

				//right가 가장 끝 인덱스보다 작거나 같을 경우,
				//right child와 left child 중 더 작은 값을 가진 인덱스로 이동해야함
				if (right <= heapsize)
				{
					if (data[left] <= data[right])
						min = left;
					else
						min = right;
				}

				//right가 가장 끝 인덱스를 초과할 경우, 무조건 left child를 선택해야함
				else
					min = left;

				if (data[tmp] > data[min]) //자식 < 부모 인 경우 min heap이므로 둘을 swap
				{
					swap(data[tmp], data[min]);
					tmp = min;
					//원래 자식의 인덱스(현재 부모의 인덱스)를 기준으로
					//다시 그의 자식과 비교하여 swap 여부 결정
				}
				else //자식 >= 부모 인 경우 swap할 필요 없이 반복문 빠져나옴
					break;

			} //end while
		} //end if
	} //end pop

	pair<int, int> top() //pair의 first, 즉 weight를 기준으로 최소 weight를 가지는 vertex 반환
	{
		return data[1];
	}

	int size() //heap의 크기 반환
	{
		return heapsize;
	}
};


int weight[10001]; //각 vertex까지의 최소 비용을 저장할 배열
void prim(int s)
{
	print.push_back(s); //현재 tree에 추가된 조형물 번호 저장
	char status[10001];
	for (int i = 1; i <= n; i++) //시작점은 tree vertex, 나머지는 unseen으로 초기화
	{
		if (i == s)
			status[i] = 'T';
		else
			status[i] = 'U';

		weight[i] = 10001; //비용 배열 초기화
	}

	for (int i = 1; i <= n; i++) //시작점과 인접한 정점까지의 비용 저장
	{
		for (int j = 0; j < graph[s].size(); j++)
		{
			if (i == graph[s][j].second)
			{
				weight[i] = graph[s][j].first; //인접한 정점에 해당하는 weight 배열 값에 비용 저장
				break;
			}
		}
	}

	for (int i = 0; i < graph[s].size(); i++) //시작점과 인접한 정점들을 탐색
	{
		status[graph[s][i].second] = 'F'; //시작점과 인접한 정점을 fringe vertex로 변경
	}

	int fringe_cnt = 0; //fringe vertex의 수를 저장
	for (int i = 1; i <= n; i++)
	{
		if (status[i] == 'F')
			fringe_cnt++;
	}

	Heap h;
	for (int i = 0; i < graph[s].size(); i++) //제일 처음에 시작점과 인접한 모든 vertex를 heap에 넣음
	{
		h.push({ graph[s][i].first,graph[s][i].second }); //{weight, vertex}
	}

	while (fringe_cnt > 0) //fringe vertex가 존재하는 동안 계속 반복
	{
		for (int i = 0; i < graph[s].size(); i++) //fringe vertex를 heap에 넣고 minimum weight를 가진 정점을 찾음
		{
			if (status[graph[s][i].second] == 'U') //인접한 vertex중 unseen vertex를 fringe vertex에 추가
			{
				status[graph[s][i].second] = 'F';
				h.push({ graph[s][i].first,graph[s][i].second }); //{weight, vertex}
				weight[graph[s][i].second] = graph[s][i].first; //비용 배열에 값 추가
				fringe_cnt++; //unseen -> fringe로 변경됐으므로 fringe vertex의 수 증가
			}
		}
		int minWeightVertex = h.top().second; //최소 weight을 가지는 조형물의 번호
		status[minWeightVertex] = 'T'; //찾은 정점을 tree vertex에 포함시킴
		h.pop(); // tree vertex로 변경하면서 힙에서 제거함
		fringe_cnt--; //fringe vertex의 수 감소
		print.push_back(minWeightVertex); //tree vertex에 추가되었으므로 해당 조형물 번호를 출력하기 위해 저장

		//새로 tree vertex에 추가된 정점과 인접한 정점들에 대해 fringe vertex이면
		//decreaseKey가 가능한지 확인
		for (int i = 0; i < graph[minWeightVertex].size(); i++)
		{
			//decreaseKey
			if (status[graph[minWeightVertex][i].second] == 'F')
			{
				//현재 설정된 해당 정점까지 가는 최단 거리와 새로 탐색한 최단거리를 비교
				//최솟값이 바뀌면 힙에도 적용
				if (weight[graph[minWeightVertex][i].second] > graph[minWeightVertex][i].first)
				{
					weight[graph[minWeightVertex][i].second] = graph[minWeightVertex][i].first;

					//힙에 저장된 weight가 전보다 감소하므로 upheap을 고려해야함
					pair<int, int> tmp = { weight[graph[minWeightVertex][i].second], graph[minWeightVertex][i].second };
					h.update(tmp); //힙에도 변경된 값 적용
				}
			}
		}
		s = minWeightVertex; //다음에 탐색할 정점을 변경
	}
}

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);

	cin >> n >> m >> q;

	for (int i = 0; i < m; i++) //디자인 비용, 조형물 번호 순으로 pair를 사용하여 벡터에 저장
	{
		int v1, v2, weight;
		cin >> v1 >> v2 >> weight;

		graph[v1].push_back({ weight,v2 });
		graph[v2].push_back({ weight,v1 });
	}

	for (int i = 0; i < q; i++) //질의의 수만큼 반복
	{
		char c;
		int start;
		cin >> c >> start; //질의형식
		prim(start); //시작점부터 prim 알고리즘 수행
		int sum = 0; //산책로 설치에 들어가는 비용의 총합을 저장할 변수
		for (int j = 1; j <= n; j++) //prim 알고리즘이 수행된 후에 weight 배열을 돌면서 비용의 총합을 구함
		{
			if (weight[j] != 10001)
				sum += weight[j];
		}
		cout << sum << " "; //비용의 총합을 출력한 후에
		for (int j = 0; j < print.size(); j++)
		{
			cout << print[j] << " "; //tree vertex가 된 순서대로 조형물 번호를 출력
		}
		print.clear(); //각 테스트케이스마다 벡터 초기화
		cout << "\n";
	}
	return 0;
}
