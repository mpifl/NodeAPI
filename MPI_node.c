#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "MPI_node.h"

//��ʼ��
int MPIN_init(char *path,int size){
	int i = 0,j,rank,filler_round = 0,vlen;
	FILE  *fp;
	char *line = NULL;
	char *para;
	char *value;
	size_t len = 0;
	ssize_t read;

	if((fp = fopen(path,"r") ) == NULL)
	{
		printf("hostfile is not exist!\n");
		return  -1;
	}
	while ((read = getline(&line, &len, fp)) != -1)
	{
		if(line[0] == '#')
			continue;
		para = strtok(line,":");
		value = strtok(NULL,":");
		//ȥ�����з�
		vlen = strlen(value);
		value[vlen - 1] = 0;
		strcpy(arraynode[i].name,para);
		arraynode[i].id = i;
		arraynode[i].psize = atoi(value);
		i++;
	}
	if(line)
		free(line);
				
	NODE_SIZE = i;
	PROCESS_SIZE = size;
	
	for(rank = 0;;){
		for(i = 0;i < NODE_SIZE;i++){
			j = 0;
			while(j < arraynode[i].psize)
			{
				(arraynode[i].p)[j+filler_round*(arraynode[i].psize)].rank = rank;
				process[rank] = i;
				rank++;
				if(rank > PROCESS_SIZE){
					arraynode[i].allocsize = j+filler_round*(arraynode[i].psize);
					goto BREAK;
				}
				j++;
			}
			arraynode[i].allocsize = j+filler_round*(arraynode[i].psize);
		}
		filler_round++;
	}
	
BREAK:
	return 0;	
}

//�õ���һ�����̵�rank
int MPIN_get_next_rank(int curRank){
	
	if(curRank > PROCESS_SIZE || curRank < 0)
		return -1;
	int node_id = process[curRank];
	int i;
	for(i = 0;i < arraynode[node_id].allocsize;i++)
		if(arraynode[node_id].p[i].rank == curRank)
			break;
		
	if(i == arraynode[node_id].allocsize)//���û���ҵ�return -1;
	{
		return -1;
	}else if(i == arraynode[node_id].allocsize - 1){//��������һ��������һ���ڵ�
		node_id++;
		if(node_id > NODE_SIZE){
			node_id = 0;
		}
		return arraynode[node_id].p[0].rank;
	}else{
		return arraynode[node_id].p[i+1].rank;
	}
	return -1;
}
//�õ���һ�����̵�rank
int MPIN_get_prev_rank(int curRank){
	if(curRank > PROCESS_SIZE || curRank < 0)
		return -1;
	int node_id = process[curRank];
	int i;
	for(i = 0;i < arraynode[node_id].allocsize;i++)
		if(arraynode[node_id].p[i].rank == curRank)
			break;
		
	if(i == arraynode[node_id].allocsize)//���û���ҵ�return -1;
	{
		return -1;
	}else if(i == 0){//����ǵ�һ��������һ���ڵ�
		node_id--;
		if(node_id < 0){
			node_id = NODE_SIZE - 1;
		}
		return arraynode[node_id].p[arraynode[node_id].allocsize - 1].rank;
	}else{
		return arraynode[node_id].p[i-1].rank;
	}
	return -1;
}
//�õ��ܵĽڵ���
int MPIN_get_node_size(){
	return NODE_SIZE;
}
//����Node�ڵ�ţ��õ���ǰ�ڵ���̹�ģ
int MPIN_get_node_process_size(int node_id){
	if(node_id >= NODE_SIZE || node_id < 0)
		return -1;
	return arraynode[node_id].allocsize;
}
//�õ���ǰ�ڵ��ϵ�master rank
int MPIN_get_master_rank(int node_id){
	if(node_id >= NODE_SIZE || node_id < 0)
		return -1;
	return arraynode[node_id].p[0].rank;
}
//����node�ڵ�ź�λ�õõ���ǰ���̺�
int MPIN_get_node_process_rank(int node_id,int loc){
	if(node_id >= NODE_SIZE || node_id < 0)
		return -1;
	if(loc >= arraynode[node_id].allocsize || loc < 0)
		return -1;
	int i;
	return arraynode[node_id].p[loc].rank;
}
int MPIN_get_node_by_rank(int rank)
{
	if(rank >= PROCESS_SIZE || rank < 0)
		return -1;
	return process[rank];
}

