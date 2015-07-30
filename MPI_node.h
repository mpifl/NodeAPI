#ifndef MPI_NODE__
#define MPI_NODE__

#define MAX_NODE 100
#define MAX_PROCESS 200

struct process{
	int rank;
	int size;
};

struct node{
	char name[128];
	int id;
	int psize;
	int allocsize;
	struct process p[MAX_PROCESS];
};

struct node arraynode[MAX_NODE];
int PROCESS_SIZE;
int NODE_SIZE;
int process[MAX_PROCESS];
//��ʼ��
int MPIN_init(char *path,int size);
//�õ���һ�����̵�rank
int MPIN_get_next_rank(int curRank);
//�õ���һ�����̵�rank
int MPIN_get_prev_rank(int curRank);
//�õ��ܵĽڵ���
int MPIN_get_node_size();
//����Node�ڵ�ţ��õ���ǰ�ڵ���̹�ģ
int MPIN_get_node_process_size(int nodeid);
//�õ���ǰ�ڵ��ϵ�master rank
int MPIN_get_master_rank(int nodeid);
//����node�ڵ�ź�λ�õõ���ǰ���̺�
int MPIN_get_node_process_rank(int nodeid,int loc);
int MPIN_get_node_by_rank(int rank);
#endif
