#include<iostream>
#include<vector>
#include<string>
using namespace std;

#define RED 0
#define BLACK 1

// �� ����� ������ ���� Ŭ����
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
	bool check = false; //�̹� ��ϵ� ���ø����̼��̸� true�� �ٲ�
	int depth = 0;
	RBTree()
	{
		leaf->color = BLACK; //leaf����� ���� BLACK
		leaf->left = NULL;
		leaf->right = NULL;
	}

	void checkRebuilding(Node* data) //recoloring or restructuring�� �ؾ��ϴ����� Ȯ���ϴ� �Լ�
	{
		if (data != root) //�ڽ��� root�� �ƴҶ�
		{
			if (data->par != root)//�θ� root�� �ƴϸ� �籸���ؾ��ϴ��� Ȯ��
			{
				if (data->color == RED && data->par->color == RED) //�ڽŰ� �θ� ��� RED�϶�
				{
					//�θ� ���θ��� �����϶�
					if (data->par == data->par->par->left)
					{
						//���θ��� ������(uncle)�� black�� ���
						if (data->par->par->right->color == BLACK)
							restructuring(data);

						//���θ��� ������(uncle)�� RED�� ���
						else
							recoloring(data);
					}

					//�θ� ���θ��� �������϶�
					else if (data->par == data->par->par->right)
					{
						//���θ��� ����(uncle)�� black�� ���
						if (data->par->par->left->color == BLACK)
							restructuring(data);

						//���θ��� ����(uncle)�� RED�� ���
						else
							recoloring(data);
					}
				}
			}
			else //�θ� root�̸�
				data->par->color = BLACK;
		}
	}

	void insert(Node* data)
	{
		check = false;
		depth = 0;
		data->par = NULL;
		//�ڽ��� leaf���� ����
		data->left = leaf;
		data->right = leaf;

		if (root == NULL) //Ʈ���� ������� ���
		{
			data->color = BLACK;
			root = data;
		}
		else //root�� �ƴ� ���� �ϴ� RED�� ����
		{
			data->color = RED;
			Node* cur = root;
			Node* par = NULL;


			while (cur != leaf) //leaf�� �����ϱ� ������
			{
				if (data->ID > cur->ID) //���� ��庸�� ũ�� ���������� �̵�
				{
					par = cur;
					cur = cur->right;
				}
				else if (data->ID < cur->ID)//���� ��庸�� ������ �������� �̵�
				{
					par = cur;
					cur = cur->left;
				}
				else
				{
					check = true;
					break; //�̹� ��ϵ� ���ø����̼��̸�, ��� �����ϰ� �ش� ����� ���̸� ���
				}
				depth++;
			}
			if (check == true)
				cout << depth << "\n";
			else
			{

				data->par = par; //�ڽ��� �θ� ����

				if (data->ID > data->par->ID) //�θ𺸴� ũ�� ������ �ڽ�
					data->par->right = data;
				else //�θ𺸴� ������ ���� �ڽ�
					data->par->left = data;

				checkRebuilding(data); //restructuring�̳� recoloring�� �������� Ȯ��
				depth = 0;

				//rebuilding �� depth ���
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
		// case 1. > ��� (�ڽ��� �θ��� ����, �θ�� ���θ��� ������)
		if (data == par->left && par == grand->right)
		{
			Node* one = grand->left;
			Node* two = data->left;
			Node* three = data->right;
			Node* four = par->right;

			//���θ� root�̸�
			if (grand == root)
			{
				//�ڽ��� ��ġ
				data->par = NULL;
				root = data;

				//���θ��� ��ġ
				root->left = grand;
				root->left->par = root;

				//�θ��� ��ġ
				root->right = par;
				root->right->par = root;

				//�� ����� ������ �ڽĵ��� ��ġ
				one->par = root->left;
				two->par = root->left;
				three->par = root->right;
				four->par = root->right;
				
				root->left->left = one;
				root->left->right = two;
				root->right->left = three;
				root->right->right = four;

				//�ٲ� ���� �� ����
				root->color = BLACK;
				root->right->color = root->left->color = RED;
			}
			else
			{
				//���θ� left�̸�
				if (grand == grand->par->left)
				{
					//�ڽ��� ��ġ
					data->par = grand->par;
					data->par->left = data;

				}
				else //���θ� right�̸�
				{
					//�ڽ��� ��ġ
					data->par = grand->par;
					data->par->right = data;
				}

				//���θ��� ��ġ
				data->left = grand;
				data->left->par = data;

				//�θ��� ��ġ
				data->right = par;
				data->right->par = data;

				//�� ����� ������ �ڽĵ��� ��ġ
				one->par = data->left;
				two->par = data->left;
				three->par = data->right;
				four->par = data->right;

				data->left->left = one;
				data->left->right = two;
				data->right->left = three;
				data->right->right = four;

				//�ٲ� ���� �� ����
				data->color = BLACK;
				data->right->color = data->left->color = RED;
			}
		}

		// case 2. < ��� (�ڽ��� �θ��� ������, �θ�� ���θ��� ����)
		else if (data == par->right && par == grand->left)
		{
			Node* one = par->left;
			Node* two = data->left;
			Node* three = data->right;
			Node* four = grand->right;

			//���θ� root�̸�
			if (grand == root)
			{
				//�ڽ��� ��ġ
				root = data;
				root->par = NULL;

				//���θ��� ��ġ
				root->right = grand;
				root->right->par = root;

				//�θ��� ��ġ
				root->left = par;
				root->left->par = root;

				//�� ����� ������ �ڽĵ��� ��ġ
				one->par = root->left;
				two->par = root->left;
				three->par = root->right;
				four->par = root->right;

				root->left->left = one;
				root->left->right = two;
				root->right->left = three;
				root->right->right = four;

				//�ٲ� ���� �� ����
				root->color = BLACK;
				root->left->color = root->right->color = RED;
			}
			else
			{
				//���θ� left�̸�
				if (grand == grand->par->left)
				{
					//�ڽ��� ��ġ
					data->par = grand->par;
					data->par->left = data;

				}
				else //���θ� right�̸�
				{
					//�ڽ��� ��ġ
					data->par = grand->par;
					data->par->right = data;
				}
				//���θ��� ��ġ
				data->right = grand;
				data->right->par = data;

				//�θ��� ��ġ
				data->left = par;
				data->left->par = data;

				//�� ����� ������ �ڽĵ��� ��ġ
				one->par = data->left;
				two->par = data->left;
				three->par = data->right;
				four->par = data->right;

				data->left->left = one;
				data->left->right = two;
				data->right->left = three;
				data->right->right = four;

				//�ٲ� ���� �� ����
				data->color = BLACK;
				data->left->color = data->right->color = RED;
			}

		}

		// case 3. / ��� (�ڽ��� �θ��� ����, �θ�� ���θ��� ����)
		else if (data == par->left && par == grand->left)
		{
			Node* one = data->left;
			Node* two = data->right;
			Node* three = par->right;
			Node* four = grand->right;

			//���θ� root�̸�
			if (grand == root)
			{
				//�θ��� ��ġ
				root = par;
				root->par = NULL;

				//���θ��� ��ġ
				root->right = grand;
				root->right->par = root;

				//�ڽ��� ��ġ
				root->left = data;
				root->left->par = root;

				//�� ����� ������ �ڽĵ��� ��ġ
				one->par = root->left;
				two->par = root->left;
				three->par = root->right;
				four->par = root->right;

				root->left->left = one;
				root->left->right = two;
				root->right->left = three;
				root->right->right = four;

				//�ٲ� ���� �� ����
				root->color = BLACK;
				root->left->color = root->right->color = RED;
			}
			else
			{
				//���θ� left�̸�
				if (grand == grand->par->left)
				{
					//�θ��� ��ġ
					par->par = grand->par;
					par->par->left = par;

				}
				else //���θ� right�̸�
				{
					//�θ��� ��ġ
					par->par = grand->par;
					par->par->right = par;
				}
				//���θ��� ��ġ
				par->right = grand;
				par->right->par = par;

				//�ڽ��� ��ġ
				par->left = data;
				par->left->par = par;

				//�� ����� ������ �ڽĵ��� ��ġ
				one->par = par->left;
				two->par = par->left;
				three->par = par->right;
				four->par = par->right;

				par->left->left = one;
				par->left->right = two;
				par->right->left = three;
				par->right->right = four;

				//�ٲ� ���� �� ����
				par->color = BLACK;
				par->left->color = par->right->color = RED;
			}

		}

		// case 4. \ ��� (�ڽ��� �θ��� ������, �θ�� ���θ��� ������)
		else if (data == par->right && par == grand->right)
		{
			Node* one = grand->left;
			Node* two = par->left;
			Node* three = data->left;
			Node* four = data->right;

			//���θ� root�̸�
			if (grand == root)
			{
				//�θ��� ��ġ
				root = par;
				root->par = NULL;

				//���θ��� ��ġ
				root->left = grand;
				root->left->par = root;

				//�ڽ��� ��ġ
				root->right = data;
				root->right->par = root;

				//�� ����� ������ �ڽĵ��� ��ġ
				one->par = root->left;
				two->par = root->left;
				three->par = root->right;
				four->par = root->right;

				root->left->left = one;
				root->left->right = two;
				root->right->left = three;
				root->right->right = four;

				//�ٲ� ���� �� ����
				root->color = BLACK;
				root->right->color = root->left->color = RED;
			}
			else
			{
				//���θ� left�̸�
				if (grand == grand->par->left)
				{
					//�θ��� ��ġ
					par->par = grand->par;
					par->par->left = par;

				}
				else //���θ� right�̸�
				{
					//�θ��� ��ġ
					par->par = grand->par;
					par->par->right = par;
				}
				//���θ��� ��ġ
				par->left = grand;
				par->left->par = par;

				//�ڽ��� ��ġ
				par->right = data;
				par->right->par = par;

				//�� ����� ������ �ڽĵ��� ��ġ
				one->par = par->left;
				two->par = par->left;
				three->par = par->right;
				four->par = par->right;

				par->left->left = one;
				par->left->right = two;
				par->right->left = three;
				par->right->right = four;

				//�ٲ� ���� �� ����
				par->color = BLACK;
				par->right->color = par->left->color = RED;
			}

		}
	}

	void recoloring(Node* data)
	{
		Node* grand = data->par->par;
		grand->left->color = grand->right->color = BLACK; //�θ�� uncle�� ���� black���� ����
		if (grand != root) //���θ� root�� �ƴ϶�� ���� red�� ����
			grand->color = RED;

		checkRebuilding(grand); //���θ��� ���� RED�̹Ƿ� �籸���� �ʿ����� Ȯ�� 
	}

	//���
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

	//�˻�
	void find(int id)
	{
		depth = 0;
		Node* cur = root;
		while (cur->ID != id) // �ش� id�� ���� ��� ã�� ����
		{
			if (cur->ID < id) //���� Ž�� ���� ����� id < ã������ ����� id -> ������ �ڽ����� �̵�
			{
				cur = cur->right;
				if (cur == leaf) //���� Ž�� ���� ��尡 leaf���� break
					break;
			}
			else if (cur->ID > id)
			{
				cur = cur->left;
				if (cur == leaf)
					break;
			}
			else //��ġ ã������
				break;
			depth++;
		}
		if (cur != leaf)
			cout << depth << " " << cur->name << " " << cur->capacity << " " << cur->price << "\n";
		else
			cout << "NULL\n";
	}

	//������Ʈ
	void update(int id, string new_name, int new_capacity, int new_price)
	{
		depth = 0;
		Node* cur = root;
		while (cur->ID != id) // �ش� id�� ���� ��� ã�� ����
		{
			if (cur->ID < id) //���� Ž�� ���� ����� id < ã������ ����� id -> ������ �ڽ����� �̵�
			{
				cur = cur->right;
				if (cur == leaf) //���� Ž�� ���� ��尡 leaf���� break
					break;
			}
			else if (cur->ID > id)
			{
				cur = cur->left;
				if (cur == leaf)
					break;
			}
			else //��ġ ã������
				break;
			depth++;
		}

		if (cur != leaf) //���� ���� ����
		{
			cur->name = new_name;
			cur->capacity = new_capacity;
			cur->price = new_price;
			cout << depth << "\n";
		}
		else
			cout << "NULL\n";
	}

	//����
	void discount(int x, int y, int P)
	{
		Node* cur = root;
		preorder(cur, x, y, P); //������ȸ�� ��� ��带 Ž���ؼ� ������ �����ϴ� ���� ã��
	}

	void preorder(Node* data, int x, int y, int P)
	{
		if (data != leaf)
		{
			if (x <= data->ID && data->ID <= y) //���� ���� �� ������ ����
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

	int start, end; //D ���� ��ɿ� ���̴� ������ ���۰� ��
	int P; //������

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