#include<iostream>
#include<vector>
#include<string>
using namespace std;

#define RED 0
#define BLACK 1

// 각 노드의 정보를 담을 클래스
class Node
{
public:
	Node* left;
	Node* right;
	Node* par;
	int ID;
	string name;
	int capacity;
	int price;
	int color = RED;

	Node() {}
};

class RBTree
{
public:
	Node* root = NULL;
	Node* leaf = new Node();
	bool check = false; //이미 등록된 애플리케이션이면 true로 바꿈
	int depth = 0;
	RBTree()
	{
		leaf->color = BLACK; //leaf노드의 색은 BLACK
		leaf->left = NULL;
		leaf->right = NULL;
	}

	void checkRebuilding(Node* data) //recoloring or restructuring을 해야하는지를 확인하는 함수
	{
		if (data != root) //자신이 root가 아닐때
		{
			if (data->par != root)//부모가 root가 아니면 재구성해야하는지 확인
			{
				if (data->color == RED && data->par->color == RED) //자신과 부모가 모두 RED일때
				{
					//부모가 조부모의 왼쪽일때
					if (data->par == data->par->par->left)
					{
						//조부모의 오른쪽(uncle)이 black인 경우
						if (data->par->par->right->color == BLACK)
							restructuring(data);

						//조부모의 오른쪽(uncle)이 RED인 경우
						else
							recoloring(data);
					}

					//부모가 조부모의 오른쪽일때
					else if (data->par == data->par->par->right)
					{
						//조부모의 왼쪽(uncle)이 black인 경우
						if (data->par->par->left->color == BLACK)
							restructuring(data);

						//조부모의 왼쪽(uncle)이 RED인 경우
						else
							recoloring(data);
					}
				}
			}
			else //부모가 root이면
				data->par->color = BLACK;
		}
	}

	void insert(Node* data)
	{
		check = false;
		depth = 0;
		data->par = NULL;
		//자식을 leaf노드로 설정
		data->left = leaf;
		data->right = leaf;

		if (root == NULL) //트리가 비어있을 경우
		{
			data->color = BLACK;
			root = data;
		}
		else //root가 아닌 노드는 일단 RED로 삽입
		{
			data->color = RED;
			Node* cur = root;
			Node* par = NULL;


			while (cur != leaf) //leaf에 도달하기 전까지
			{
				if (data->ID > cur->ID) //현재 노드보다 크면 오른쪽으로 이동
				{
					par = cur;
					cur = cur->right;
				}
				else if (data->ID < cur->ID)//현재 노드보다 작으면 왼쪽으로 이동
				{
					par = cur;
					cur = cur->left;
				}
				else
				{
					check = true;
					break; //이미 등록된 애플리케이션이면, 등록 거절하고 해당 노드의 깊이를 출력
				}
				depth++;
			}
			if (check == true)
				cout << depth << "\n";
			else
			{

				data->par = par; //자신의 부모 설정

				if (data->ID > data->par->ID) //부모보다 크면 오른쪽 자식
					data->par->right = data;
				else //부모보다 작으면 왼쪽 자식
					data->par->left = data;

				checkRebuilding(data); //restructuring이나 recoloring이 가능한지 확인
				depth = 0;

				//rebuilding 후 depth 계산
				cur = data;
				while (cur->par != NULL)
				{
					cur = cur->par;
					depth++;
				}
			}
		}
	} //end insert

	void restructuring(Node* data)
	{
		Node* grand = data->par->par;
		Node* par = data->par;
		// case 1. > 모양 (자신은 부모의 왼쪽, 부모는 조부모의 오른쪽)
		if (data == par->left && par == grand->right)
		{
			Node* one = grand->left;
			Node* two = data->left;
			Node* three = data->right;
			Node* four = par->right;

			//조부모가 root이면
			if (grand == root)
			{
				//자신의 위치
				data->par = NULL;
				root = data;

				//조부모의 위치
				root->left = grand;
				root->left->par = root;

				//부모의 위치
				root->right = par;
				root->right->par = root;

				//각 노드의 나머지 자식들의 위치
				one->par = root->left;
				two->par = root->left;
				three->par = root->right;
				four->par = root->right;
				
				root->left->left = one;
				root->left->right = two;
				root->right->left = three;
				root->right->right = four;

				//바뀐 후의 색 변경
				root->color = BLACK;
				root->right->color = root->left->color = RED;
			}
			else
			{
				//조부모가 left이면
				if (grand == grand->par->left)
				{
					//자신의 위치
					data->par = grand->par;
					data->par->left = data;

				}
				else //조부모가 right이면
				{
					//자신의 위치
					data->par = grand->par;
					data->par->right = data;
				}

				//조부모의 위치
				data->left = grand;
				data->left->par = data;

				//부모의 위치
				data->right = par;
				data->right->par = data;

				//각 노드의 나머지 자식들의 위치
				one->par = data->left;
				two->par = data->left;
				three->par = data->right;
				four->par = data->right;

				data->left->left = one;
				data->left->right = two;
				data->right->left = three;
				data->right->right = four;

				//바뀐 후의 색 변경
				data->color = BLACK;
				data->right->color = data->left->color = RED;
			}
		}

		// case 2. < 모양 (자신은 부모의 오른쪽, 부모는 조부모의 왼쪽)
		else if (data == par->right && par == grand->left)
		{
			Node* one = par->left;
			Node* two = data->left;
			Node* three = data->right;
			Node* four = grand->right;

			//조부모가 root이면
			if (grand == root)
			{
				//자신의 위치
				root = data;
				root->par = NULL;

				//조부모의 위치
				root->right = grand;
				root->right->par = root;

				//부모의 위치
				root->left = par;
				root->left->par = root;

				//각 노드의 나머지 자식들의 위치
				one->par = root->left;
				two->par = root->left;
				three->par = root->right;
				four->par = root->right;

				root->left->left = one;
				root->left->right = two;
				root->right->left = three;
				root->right->right = four;

				//바뀐 후의 색 변경
				root->color = BLACK;
				root->left->color = root->right->color = RED;
			}
			else
			{
				//조부모가 left이면
				if (grand == grand->par->left)
				{
					//자신의 위치
					data->par = grand->par;
					data->par->left = data;

				}
				else //조부모가 right이면
				{
					//자신의 위치
					data->par = grand->par;
					data->par->right = data;
				}
				//조부모의 위치
				data->right = grand;
				data->right->par = data;

				//부모의 위치
				data->left = par;
				data->left->par = data;

				//각 노드의 나머지 자식들의 위치
				one->par = data->left;
				two->par = data->left;
				three->par = data->right;
				four->par = data->right;

				data->left->left = one;
				data->left->right = two;
				data->right->left = three;
				data->right->right = four;

				//바뀐 후의 색 변경
				data->color = BLACK;
				data->left->color = data->right->color = RED;
			}

		}

		// case 3. / 모양 (자신은 부모의 왼쪽, 부모는 조부모의 왼쪽)
		else if (data == par->left && par == grand->left)
		{
			Node* one = data->left;
			Node* two = data->right;
			Node* three = par->right;
			Node* four = grand->right;

			//조부모가 root이면
			if (grand == root)
			{
				//부모의 위치
				root = par;
				root->par = NULL;

				//조부모의 위치
				root->right = grand;
				root->right->par = root;

				//자신의 위치
				root->left = data;
				root->left->par = root;

				//각 노드의 나머지 자식들의 위치
				one->par = root->left;
				two->par = root->left;
				three->par = root->right;
				four->par = root->right;

				root->left->left = one;
				root->left->right = two;
				root->right->left = three;
				root->right->right = four;

				//바뀐 후의 색 변경
				root->color = BLACK;
				root->left->color = root->right->color = RED;
			}
			else
			{
				//조부모가 left이면
				if (grand == grand->par->left)
				{
					//부모의 위치
					par->par = grand->par;
					par->par->left = par;

				}
				else //조부모가 right이면
				{
					//부모의 위치
					par->par = grand->par;
					par->par->right = par;
				}
				//조부모의 위치
				par->right = grand;
				par->right->par = par;

				//자신의 위치
				par->left = data;
				par->left->par = par;

				//각 노드의 나머지 자식들의 위치
				one->par = par->left;
				two->par = par->left;
				three->par = par->right;
				four->par = par->right;

				par->left->left = one;
				par->left->right = two;
				par->right->left = three;
				par->right->right = four;

				//바뀐 후의 색 변경
				par->color = BLACK;
				par->left->color = par->right->color = RED;
			}

		}

		// case 4. \ 모양 (자신은 부모의 오른쪽, 부모는 조부모의 오른쪽)
		else if (data == par->right && par == grand->right)
		{
			Node* one = grand->left;
			Node* two = par->left;
			Node* three = data->left;
			Node* four = data->right;

			//조부모가 root이면
			if (grand == root)
			{
				//부모의 위치
				root = par;
				root->par = NULL;

				//조부모의 위치
				root->left = grand;
				root->left->par = root;

				//자신의 위치
				root->right = data;
				root->right->par = root;

				//각 노드의 나머지 자식들의 위치
				one->par = root->left;
				two->par = root->left;
				three->par = root->right;
				four->par = root->right;

				root->left->left = one;
				root->left->right = two;
				root->right->left = three;
				root->right->right = four;

				//바뀐 후의 색 변경
				root->color = BLACK;
				root->right->color = root->left->color = RED;
			}
			else
			{
				//조부모가 left이면
				if (grand == grand->par->left)
				{
					//부모의 위치
					par->par = grand->par;
					par->par->left = par;

				}
				else //조부모가 right이면
				{
					//부모의 위치
					par->par = grand->par;
					par->par->right = par;
				}
				//조부모의 위치
				par->left = grand;
				par->left->par = par;

				//자신의 위치
				par->right = data;
				par->right->par = par;

				//각 노드의 나머지 자식들의 위치
				one->par = par->left;
				two->par = par->left;
				three->par = par->right;
				four->par = par->right;

				par->left->left = one;
				par->left->right = two;
				par->right->left = three;
				par->right->right = four;

				//바뀐 후의 색 변경
				par->color = BLACK;
				par->right->color = par->left->color = RED;
			}

		}
	}

	void recoloring(Node* data)
	{
		Node* grand = data->par->par;
		grand->left->color = grand->right->color = BLACK; //부모와 uncle의 색을 black으로 변경
		if (grand != root) //조부모가 root가 아니라면 색을 red로 변경
			grand->color = RED;

		checkRebuilding(grand); //조부모의 색이 RED이므로 재구성이 필요한지 확인 
	}

	//등록
	void apply(int id, string name, int capacity, int price)
	{
		Node* newnode = new Node();
		newnode->ID = id;
		newnode->name = name;
		newnode->capacity = capacity;
		newnode->price = price;

		insert(newnode);
		if (check == false)
			cout << depth << "\n";
	}

	//검색
	void find(int id)
	{
		depth = 0;
		Node* cur = root;
		while (cur->ID != id) // 해당 id를 갖는 노드 찾는 과정
		{
			if (cur->ID < id) //현재 탐색 중인 노드의 id < 찾으려는 노드의 id -> 오른쪽 자식으로 이동
			{
				cur = cur->right;
				if (cur == leaf) //현재 탐색 중인 노드가 leaf노드면 break
					break;
			}
			else if (cur->ID > id)
			{
				cur = cur->left;
				if (cur == leaf)
					break;
			}
			else //위치 찾았을때
				break;
			depth++;
		}
		if (cur != leaf)
			cout << depth << " " << cur->name << " " << cur->capacity << " " << cur->price << "\n";
		else
			cout << "NULL\n";
	}

	//업데이트
	void update(int id, string new_name, int new_capacity, int new_price)
	{
		depth = 0;
		Node* cur = root;
		while (cur->ID != id) // 해당 id를 갖는 노드 찾는 과정
		{
			if (cur->ID < id) //현재 탐색 중인 노드의 id < 찾으려는 노드의 id -> 오른쪽 자식으로 이동
			{
				cur = cur->right;
				if (cur == leaf) //현재 탐색 중인 노드가 leaf노드면 break
					break;
			}
			else if (cur->ID > id)
			{
				cur = cur->left;
				if (cur == leaf)
					break;
			}
			else //위치 찾았을때
				break;
			depth++;
		}

		if (cur != leaf) //값을 새로 수정
		{
			cur->name = new_name;
			cur->capacity = new_capacity;
			cur->price = new_price;
			cout << depth << "\n";
		}
		else
			cout << "NULL\n";
	}

	//할인
	void discount(int x, int y, int P)
	{
		Node* cur = root;
		preorder(cur, x, y, P); //전위순회로 모든 노드를 탐색해서 조건을 만족하는 값을 찾음
	}

	void preorder(Node* data, int x, int y, int P)
	{
		if (data != leaf)
		{
			if (x <= data->ID && data->ID <= y) //조건 만족 시 할인율 적용
			{
				data->price = data->price * (100 - P) / 100;
			}
			preorder(data->left, x, y, P);
			preorder(data->right, x, y, P);
		}
	}
};

int main()
{
	RBTree rb;
	int T;
	char c;
	int ID, capacity, price;
	string name;

	int start, end; //D 할인 기능에 쓰이는 범위의 시작과 끝
	int P; //할인율

	cin >> T;
	for (int test = 0; test < T; test++)
	{
		cin >> c;
		switch (c)
		{
		case 'I':
			cin >> ID >> name >> capacity >> price;
			rb.apply(ID, name, capacity, price);
			break;
		case 'F':
			cin >> ID;
			rb.find(ID);
			break;
		case 'R':
			cin >> ID >> name >> capacity >> price;
			rb.update(ID, name, capacity, price);
			break;
		case 'D':
			cin >> start >> end >> P;
			rb.discount(start, end, P);
			break;
		default:
			break;
		}
	}
	return 0;
}