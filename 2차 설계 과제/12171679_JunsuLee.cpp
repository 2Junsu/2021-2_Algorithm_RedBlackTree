#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int n, m, q;
vector<pair<int, int>> graph[10001]; //�� vertex�� ������ vertex�� ������ ����
vector<int> print; //���� ����ϱ� ���� ����

class Heap
{
public:
	vector<pair<int, int>> data;
	int heapsize; //���Ϳ� 0�� �ε����� ������� �����Ƿ� �������� ���� ũ�⸦ ������ ����
	int rootindex;
	Heap()
	{
		rootindex = 1; //root�� �ε���
		heapsize = 0;
		data.push_back({ -1,-1 }); //1�� �ε������� ����ϱ� ���� �� �ϳ� �ְ� ����
	}

	//decrease key���� �ּ� weight�� ��ȭ�Ǿ����� �� �۾������Ƿ� �ش� ���� upheap�ؾ��� ���� ����
	void update(pair<int, int> weight_vertex)
	{
		int idx;
		for (int i = 0; i < data.size(); i++)
		{
			if (weight_vertex.second == data[i].second) //weight�� ������ ���� ��ȣ�� ������ vertex ã��
			{
				data[i].first = weight_vertex.first; //weight ����
				idx = i; //upheap �ϱ� ���� ���� ��ġ�� �޾ƿ;� ��
				break;
			}
		}

		while (idx > rootindex) //weight ���� �� upheap
		{
			if (data[idx] <= data[idx / 2]) //�θ� �ڽĺ��� ũ�ų� ������
				swap(data[idx], data[idx / 2]); //�ڽİ� �θ� swap (min heap���� �����ϹǷ�)

			else //�θ� �ڽĺ��� ������ swap�� �ʿ� ���� �ݺ��� ��������
				break;

			//swap�� �߻��� ���, ���� �θ��� �ε���(���� swap�� �ڽ��� �ε���)�� ��������
			//�ٽ� ���� �θ�� ���Ͽ� swap ���� ����
			idx /= 2;
		}
	}

	void push(pair<int, int> weight_vertex) //���� ������ ����
	{
		data.push_back(weight_vertex);
		heapsize++;
		int tmp = heapsize;
		while (tmp > rootindex) //�迭�� ����������, �� ���� �Ʒ� ���Һ��� Ž��(upheap)(�̶� root���� �ε����� �۾����� �ȵ�)
		{
			if (data[tmp] <= data[tmp / 2]) //�θ� �ڽĺ��� ũ�ų� ������
			{
				swap(data[tmp], data[tmp / 2]); //�ڽİ� �θ� swap (min heap���� �����ϹǷ�)
			}
			else //�θ� �ڽĺ��� ������ swap�� �ʿ� ���� �ݺ��� ��������
				break;

			//swap�� �߻��� ���, ���� �θ��� �ε���(���� swap�� �ڽ��� �ε���)�� ��������
			//�ٽ� ���� �θ�� ���Ͽ� swap ���� ����
			tmp /= 2;
		}
	}

	void pop() //���� �ֻ��, �� �ּ� weight�� ������ vertex ����
	{
		if (heapsize > 0)
		{
			//���� ���ϴ� ������ ���� ���Ҹ� �����ϱ� ���� root�� ����
			data[1] = data[heapsize--];
			data.pop_back();

			int tmp = rootindex;
			while (tmp * 2 <= heapsize) //�ڽ� �ε����� �������鼭 ��(downheap)(���� �� �ε����� �ʰ��ϸ� �ȵ�)
			{
				int left = tmp * 2;
				int right = tmp * 2 + 1;
				int min;

				//right�� ���� �� �ε������� �۰ų� ���� ���,
				//right child�� left child �� �� ���� ���� ���� �ε����� �̵��ؾ���
				if (right <= heapsize)
				{
					if (data[left] <= data[right])
						min = left;
					else
						min = right;
				}

				//right�� ���� �� �ε����� �ʰ��� ���, ������ left child�� �����ؾ���
				else
					min = left;

				if (data[tmp] > data[min]) //�ڽ� < �θ� �� ��� min heap�̹Ƿ� ���� swap
				{
					swap(data[tmp], data[min]);
					tmp = min;
					//���� �ڽ��� �ε���(���� �θ��� �ε���)�� ��������
					//�ٽ� ���� �ڽİ� ���Ͽ� swap ���� ����
				}
				else //�ڽ� >= �θ� �� ��� swap�� �ʿ� ���� �ݺ��� ��������
					break;

			} //end while
		} //end if
	} //end pop

	pair<int, int> top() //pair�� first, �� weight�� �������� �ּ� weight�� ������ vertex ��ȯ
	{
		return data[1];
	}

	int size() //heap�� ũ�� ��ȯ
	{
		return heapsize;
	}
};


int weight[10001]; //�� vertex������ �ּ� ����� ������ �迭
void prim(int s)
{
	print.push_back(s); //���� tree�� �߰��� ������ ��ȣ ����
	char status[10001];
	for (int i = 1; i <= n; i++) //�������� tree vertex, �������� unseen���� �ʱ�ȭ
	{
		if (i == s)
			status[i] = 'T';
		else
			status[i] = 'U';

		weight[i] = 10001; //��� �迭 �ʱ�ȭ
	}

	for (int i = 1; i <= n; i++) //�������� ������ ���������� ��� ����
	{
		for (int j = 0; j < graph[s].size(); j++)
		{
			if (i == graph[s][j].second)
			{
				weight[i] = graph[s][j].first; //������ ������ �ش��ϴ� weight �迭 ���� ��� ����
				break;
			}
		}
	}

	for (int i = 0; i < graph[s].size(); i++) //�������� ������ �������� Ž��
	{
		status[graph[s][i].second] = 'F'; //�������� ������ ������ fringe vertex�� ����
	}

	int fringe_cnt = 0; //fringe vertex�� ���� ����
	for (int i = 1; i <= n; i++)
	{
		if (status[i] == 'F')
			fringe_cnt++;
	}

	Heap h;
	for (int i = 0; i < graph[s].size(); i++) //���� ó���� �������� ������ ��� vertex�� heap�� ����
	{
		h.push({ graph[s][i].first,graph[s][i].second }); //{weight, vertex}
	}

	while (fringe_cnt > 0) //fringe vertex�� �����ϴ� ���� ��� �ݺ�
	{
		for (int i = 0; i < graph[s].size(); i++) //fringe vertex�� heap�� �ְ� minimum weight�� ���� ������ ã��
		{
			if (status[graph[s][i].second] == 'U') //������ vertex�� unseen vertex�� fringe vertex�� �߰�
			{
				status[graph[s][i].second] = 'F';
				h.push({ graph[s][i].first,graph[s][i].second }); //{weight, vertex}
				weight[graph[s][i].second] = graph[s][i].first; //��� �迭�� �� �߰�
				fringe_cnt++; //unseen -> fringe�� ��������Ƿ� fringe vertex�� �� ����
			}
		}
		int minWeightVertex = h.top().second; //�ּ� weight�� ������ �������� ��ȣ
		status[minWeightVertex] = 'T'; //ã�� ������ tree vertex�� ���Խ�Ŵ
		h.pop(); // tree vertex�� �����ϸ鼭 ������ ������
		fringe_cnt--; //fringe vertex�� �� ����
		print.push_back(minWeightVertex); //tree vertex�� �߰��Ǿ����Ƿ� �ش� ������ ��ȣ�� ����ϱ� ���� ����

		//���� tree vertex�� �߰��� ������ ������ �����鿡 ���� fringe vertex�̸�
		//decreaseKey�� �������� Ȯ��
		for (int i = 0; i < graph[minWeightVertex].size(); i++)
		{
			//decreaseKey
			if (status[graph[minWeightVertex][i].second] == 'F')
			{
				//���� ������ �ش� �������� ���� �ִ� �Ÿ��� ���� Ž���� �ִܰŸ��� ��
				//�ּڰ��� �ٲ�� ������ ����
				if (weight[graph[minWeightVertex][i].second] > graph[minWeightVertex][i].first)
				{
					weight[graph[minWeightVertex][i].second] = graph[minWeightVertex][i].first;

					//���� ����� weight�� ������ �����ϹǷ� upheap�� ����ؾ���
					pair<int, int> tmp = { weight[graph[minWeightVertex][i].second], graph[minWeightVertex][i].second };
					h.update(tmp); //������ ����� �� ����
				}
			}
		}
		s = minWeightVertex; //������ Ž���� ������ ����
	}
}

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);

	cin >> n >> m >> q;

	for (int i = 0; i < m; i++) //������ ���, ������ ��ȣ ������ pair�� ����Ͽ� ���Ϳ� ����
	{
		int v1, v2, weight;
		cin >> v1 >> v2 >> weight;

		graph[v1].push_back({ weight,v2 });
		graph[v2].push_back({ weight,v1 });
	}

	for (int i = 0; i < q; i++) //������ ����ŭ �ݺ�
	{
		char c;
		int start;
		cin >> c >> start; //��������
		prim(start); //���������� prim �˰��� ����
		int sum = 0; //��å�� ��ġ�� ���� ����� ������ ������ ����
		for (int j = 1; j <= n; j++) //prim �˰����� ����� �Ŀ� weight �迭�� ���鼭 ����� ������ ����
		{
			if (weight[j] != 10001)
				sum += weight[j];
		}
		cout << sum << " "; //����� ������ ����� �Ŀ�
		for (int j = 0; j < print.size(); j++)
		{
			cout << print[j] << " "; //tree vertex�� �� ������� ������ ��ȣ�� ���
		}
		print.clear(); //�� �׽�Ʈ���̽����� ���� �ʱ�ȭ
		cout << "\n";
	}
	return 0;
}
