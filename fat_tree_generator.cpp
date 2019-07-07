#include <stdio.h>
#include <stdlib.h>
int main(int argc, char** argv){
	if(argc < 2){
		printf("invalid arguments\n");
		return 0;
	}
	int k=atoi(argv[1]);		// number of ports per switch
	int num_pod = k;			// number of pod
	int num_host = (k/2);		// number of hosts under a switch
	int num_edge = (k/2);		// number of edge switch in a pod
	int num_bridge = num_edge;	// number of bridge in a pod
	int num_agg = (k/2);		// number of aggregation switch in a pod
	int num_group = k/2;		// number of group of core switches
	int num_core = (k/2);		// number of core switch in a group
	int total_host = k*k*k/4;	// number of hosts in the entire network
	int idx=0;
	FILE *fp= fopen("fattree.topo", "w+");
	if(!fp){
		printf("can not open file\n");
		return 0;
	}
	fprintf(fp, "#fat tree topology file.\n");
	fprintf(fp, "#Value of k = %d\n", k);
	fprintf(fp, "#Total number of hosts = %d\n", total_host);
	fprintf(fp, "#Number of hosts under each switch = %d\n", num_host);
	fprintf(fp, "####################################################\n");

	idx=0;
	for(int i=0; i<num_pod; i++){
		for(int j=0; j<num_edge; j++){
			for(int t=0; t<num_host; t++){
				fprintf(fp, "\nHca\t1\t\"Node(%d)\"\n", idx);
				fprintf(fp, "[1] \"Edge(%d %d 1)\"[%d]\n", i, j, (t+1)*2);
				idx++;
			}
		}
	}
	idx=0;
	for(int i=0; i<num_pod; i++){
		for(int j=0; j<num_edge; j++){
			fprintf(fp, "\nSwitch\t%d\t\"Edge(%d %d 1)\"\n", k, i, j);
			for(int p=1;p<k+1;p++){
				if(p%2==1){
					fprintf(fp, "[%d] \"Aggr(%d %d 1)\"[%d]\n", p, i, k/2+(p-1)/2, (j+1)*2);
				}else{
					fprintf(fp, "[%d] \"Node(%d)\"[1]\n", p, idx);
					idx++;
				}
			}
		}
	}
	for(int i=0; i<num_pod; i++){
		for(int j=0; j<num_agg; j++){
			fprintf(fp, "\nSwitch\t%d\t\"Aggr(%d %d 1)\"\n", k, i, j+k/2);
			for(int p=1;p<k+1;p++){
				if(p%2==1){
					fprintf(fp, "[%d] \"Core(%d %d %d)\"[%d]\n", p, k, j+1, (p+1)/2, i+1);
				}else{
					fprintf(fp, "[%d] \"Edge(%d %d 1)\"[%d]\n", p, i, (p-1)/2, (j+1)*2-1);
				}
			}
		}
	}
	for(int i=0;i<num_group;i++){
		for(int j=0;j<num_core;j++){
			fprintf(fp, "\nSwitch\t%d\t\"Core(%d %d %d)\"\n", k, k, i+1, j+1);
			for(int p=1;p<k+1;p++){
				fprintf(fp, "[%d] \"Aggr(%d %d 1)\"[%d]\n", p, p-1, i+k/2, (j+1)*2-1);
			}
		}
	}
	printf("fat tree topology file generated\n");
	fclose(fp);
	return 0;
}
