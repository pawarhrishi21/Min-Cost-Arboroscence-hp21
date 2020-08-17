#include<stdio.h>
#include<stdlib.h>

int Gcycle = 0;
int noofsupnodes = 0;

struct edge{
int label,wt;
struct edge *next;
};

struct min{
int wt,l;
};

struct arboro{
int from;
int to;
struct arboro *nxt;
}*ans;


int printdist(int s,int l, struct edge *inarr[],int N)
{
	if(l == 1)
	{
		return 0;
	}
	else
	{
		int from1,weight;
		struct edge *t = inarr[l-1];
		struct arboro *T1 = ans;
		while(T1 != NULL)
		{
			if(T1->to == l && T1->from <= N)
			{
				from1 = T1->from;
				break;
			}
			T1 = T1->nxt;	
		}
		while(t!= NULL)
		{
			if(t->label == from1)
			{
				weight = t->wt;
				break;
			}
			t = t->next;
		}
		return weight+printdist(s,from1,inarr,N);
	}
}

void insertans(struct arboro *t)
{
	struct arboro *h = ans;
	if(ans == NULL)
		ans = t;
	else
	{
		while(h->nxt != NULL)
			h = h->nxt;
		h->nxt = t;
	}
}

int mini(struct edge *hi)
{
	struct edge *h = hi;
//	printf("mini called\n");
	int min = h->wt;
	h=h->next;
	while(h!=NULL)
	{
		
		if((h->wt) < min)
			min = h->wt;
			h=h->next;
	}
//	printf(" %d returned\n",min);
	return min;
}



int DFSvisit(int N,struct edge *inarr[],struct edge *outarr[],int parent[],int dfscolor[],int i,struct edge **cyclenodes)
{
//	printf("BBBBBBBBBBBBBBBgraphsent to dfs\n");
//	struct edge *tem;
//	for(int i=0;i<N;i++)
//	{
//		tem  = outarr[i];
//		printf("out from %d\n",i+1);
	//	while(tem!=NULL)
	//	{
//			printf("label %d wt %d\n",tem->label,tem->wt);
	//		tem = tem->next;
	//	}
//	}
	if(Gcycle == 1)
	{
//		printf("dfs returning 1 as Gcycle found with wt 0\n");
		return 1;
	}
	else
	{
	int cyclefound = 0;
	int sumofedges = 0;
//	printf("DFS called\n");
    int j,flag = 0;

    dfscolor[i-1] = 1;
 //   printf("color of %d to gray\n",i);
 
  	
	struct edge *h;
	h = outarr[i-1];
    while(h != NULL)
    {
   // 	printf("\n");
        if(dfscolor[(h->label) - 1]==0)
        {
  //      	printf("entered if\n");
            parent[(h->label) - 1] = i;
//            printf("recurring dfs for source %d\n",h->label);
            DFSvisit(N,inarr,outarr,parent,dfscolor,(h->label),cyclenodes);
        }
        else if(dfscolor[h->label - 1]==1 && Gcycle != 1)
        {
       // 	printf("entered else if\n");            
        	cyclefound = 1;
        	struct edge *x = h;
      //  	for(int k=0;k<N;k++)
     //   		printf("%d ka parent in dfs is %d\n",k+1,parent[k]);
 //       	printf("back edge detected\n from %d to %d\n",i,h->label);
        	int ii = i;
			struct edge *tempt;
	    	tempt = (struct edge*)malloc(sizeof(struct edge));
	    	tempt->label = h->label;
	    	tempt->wt = h->wt;
	    	tempt->next = NULL;
	    	sumofedges = sumofedges + h->wt;
	    	struct edge *headt = *cyclenodes;
		   	if(headt == NULL)
		   		*cyclenodes = tempt;
//		   	printf("inserted label %d wt %d in cycle\n",tempt->label,tempt->wt);
        	
        	while(ii != h->label)
        	{
  //      		printf("entered whileeee\n");
        		struct edge *temphead = outarr[(parent[ii-1])-1];
        	//	printf("temphead label%d wt%d\n",temphead->label,temphead->wt);
        		while(temphead->label != ii)
        		{
        			temphead = temphead->next;
        		}
        		int currwt = temphead->wt; 
        		sumofedges = sumofedges + currwt;
	        	struct edge *temp;
	        	temp = (struct edge*)malloc(sizeof(struct edge));
	        	temp->label = ii;
	        	temp->wt = currwt;
	        	temp->next = NULL;
	        	struct edge *head = *cyclenodes;
	        	if(head == NULL)
	        		*cyclenodes = temp;
	        	else
	        	{
			    	while(head->next != NULL)
			    		head = head->next;
			    	head->next = temp;
	        	}
//	        	printf("inserted label %d wt %d in cycle\n",temp->label,temp->wt);
	        	ii = parent[ii-1];
        	}
  /*      	printf("PRINTING CYCLE\n");
        	struct edge *tempo;
        	tempo = *cyclenodes;
        	while(tempo!=NULL)
        	{
        		printf("tempo ka label %d aur weight %d",tempo->label,tempo->wt);
        		tempo = tempo->next;
        	}
        	*/
//        	printf("cyclefound%d sumofedges%d\n",cyclefound,sumofedges);
        	if(cyclefound == 1 && sumofedges == 0)
        	{
        		noofsupnodes++;
        		Gcycle = 1;
//        		printf("calling BREAK\n");
        		cyclefound = 1;
        		return 1;
        	}
        	else
        	{
        		Gcycle = 0;
        		sumofedges = 0;
        		*cyclenodes = NULL;
        		cyclefound = 0;
        	}
        }
        else if(dfscolor[h->label - 1] == 2)
        {
        	/////////////////////////////////////////
        }
        h = h->next;
    }

	dfscolor[i-1] = 2;
//	printf("color of %d to black\n",i);
	return 0;
	}
}


void Edmonds(int N,struct edge *inarr[N],struct edge *outarr[N],int s,int call)
{
	call++;
//	printf("Edmonds called\n");
	int minedgei;
	struct edge *inarr2[N],*outarr2[N];
	
	for(int i=0;i<N;i++)
	{
		outarr2[i] = NULL;
		inarr2[i] = NULL;
	}
	
	for(int i=0;i<N;i++)
	{
		struct edge *hi = inarr[i];
		
		while(hi!=NULL)
		{
			struct edge *temp;
			temp = (struct edge*)malloc(sizeof(struct edge));
			temp->wt = hi->wt;
			temp->label = hi->label;
			temp->next = NULL;

			struct edge *hi2 = inarr2[i];
			if(hi2==NULL)
				inarr2[i] = temp;
			else
			{
				while(hi2->next!=NULL)
					hi2 = hi2->next;
				hi2->next = temp;
			}
			hi = hi->next;
		}
		
		struct edge *ho = outarr[i];
		while(ho!=NULL)
		{
			struct edge *temp;
			temp = (struct edge*)malloc(sizeof(struct edge));
			temp->wt = ho->wt;
			temp->label = ho->label;
			temp->next = NULL;

			struct edge *ho2 = outarr2[i];
			if(ho2==NULL)
				outarr2[i] = temp;
			else
			{
				while(ho2->next!=NULL)
					ho2 = ho2->next;
				ho2->next = temp;
			}
			ho = ho->next;
		}
		
	}
	//duplicate graph with same data created
//		printf("CHECKING DUPLICATE GRAPH\n");
	//	for(int i=0;i<N;i++)
	//	{
	//		struct edge *hi,*ho;
		//	hi = inarr2[i];
//			printf("incoming at %d\n",i+1);
//			if(hi==NULL)printf("hinull\n");
	//		while(hi!=NULL)
	//		{
//				printf("label%d wt%d\n",hi->label,hi->wt);
		//		hi = hi->next;
//			}
	//		ho = outarr2[i];
//			printf("out from %d\n",i+1);
	//		while(ho!=NULL)
	//		{
//				printf("label%d wt%d\n",ho->label,ho->wt);
	//			ho = ho->next;
	//		}		
	//	}
	//	
	for(int i=0;i<N;i++)
	{
		struct edge *hi = inarr2[i];
		struct edge *ho;
		if(hi != NULL)
			minedgei = mini(hi);
		
		while(hi!=NULL)
		{
			hi->wt = hi->wt - minedgei;
			if(hi->wt == 0)
			{
				struct arboro *temp;
				temp = (struct arboro*)malloc(sizeof(struct arboro));
				temp->to = i+1;
				temp->from = hi->label;
				temp->nxt = NULL;
				insertans(temp);
			}
			ho = outarr2[(hi->label)-1];
			while(ho->label != i+1)
			{
				ho = ho->next;
			}
			if(ho->label == i+1)
			{	
	//			printf("in outarr, changing label %d wt from %d to %d\n",ho->label,ho->wt,ho->wt - minedgei);
				ho->wt = ho->wt - minedgei;
			}
			hi = hi->next;
		}
	}
	//printf("printf graph2 after minimisation\n");
//	for(int i=0;i<N;i++)
//		{
//			struct edge *hi,*ho;
//			hi = inarr2[i];
	//		printf("incoming at %d\n",i+1);
	//		if(hi==NULL)printf("hinull\n");
//			while(hi!=NULL)
		//	{
	//			printf("label%d wt%d\n",hi->label,hi->wt);
//				hi = hi->next;
//			}
//			ho = outarr2[i];
//			printf("out from %d\n",i+1);
//			while(ho!=NULL)
//			{
//				printf("label%d wt%d\n",ho->label,ho->wt);
//				ho = ho->next;
//			}		
//		}
	// Duplicate graph with relaxed edges is ready
	

	struct edge *cyclenodes = NULL;
    int dfscolor[N];
    int parent[N];
    for(int i=0;i<N;i++)
    {
	dfscolor[i] = 0;
	parent[i] = -1;
    }
//	printf("calling dfs\n");
	
	Gcycle = 0;
	int garbage = DFSvisit(N,inarr2,outarr2,parent,dfscolor,s,&cyclenodes);
//	printf("AFTER DFS GLOBALCYCLE IS %d\n",Gcycle);
 //   printf("PRINTING CYCLE\n");
//	struct edge *tempo;
	//tempo = cyclenodes;
//	while(tempo!=NULL)
//	{
//		printf("label %d weight %d\n",tempo->label,tempo->wt);
//		tempo = tempo->next;
//	}
        	
	if(Gcycle == 1)
	{
		struct edge *inarr3[N+noofsupnodes], *outarr3[N+noofsupnodes];
		for(int i=0;i<(N+noofsupnodes);i++)
		{
			inarr3[i] = NULL;
			outarr3[i] = NULL;
		}
		int aaa = 0;
		struct edge *tmpc = cyclenodes;
		while(tmpc != NULL)
		{	
			aaa = 0;
			struct edge *tt = inarr2[tmpc->label - 1];
			while(tt != NULL)
			{
				aaa = 0;
				struct edge *tempcyc = cyclenodes;
				while(tempcyc != NULL)
				{
					if(tempcyc->label == tt->label)
						{aaa = 1;break;}
					tempcyc = tempcyc->next;
				}
//				printf("INSIDE edges of %d\n",tmpc->label);
				if(aaa == 1)
				{
	//				printf("continue for %d\n",tt->label);
					tt = tt->next;
					continue;
				}
				struct edge *tempnode;
				tempnode = (struct edge*)malloc(sizeof(struct edge));
				tempnode->label = tt->label;
				tempnode->wt = tt->wt;
				tempnode->next = NULL;
				struct edge *tmp;
				tmp = inarr3[N+noofsupnodes-1];
				if(inarr3[N+noofsupnodes-1] == NULL)
					inarr3[N+noofsupnodes-1] = tempnode;
				else
				{
					while(tmp->next != NULL)
						tmp = tmp->next;
					tmp->next = tempnode;
				}
				tt = tt->next;
			}
			
			tmpc = tmpc->next;
		}
			
		aaa = 0;
		tmpc = cyclenodes;
		while(tmpc != NULL)
		{
			aaa = 0;
			struct edge *tt = outarr2[tmpc->label - 1];
			while(tt != NULL)
			{
				aaa = 0;
				struct edge *tempcyc = cyclenodes;
				while(tempcyc != NULL)
				{
					if(tempcyc->label == tt->label)
						{
//						printf("skippimg for %d\n",tt->label);
						aaa = 1;break;
						}
					tempcyc = tempcyc->next;
				}
				if(aaa == 1)
				{
					tt = tt->next;
					continue;
				}
				struct edge *tempnode;
				tempnode = (struct edge*)malloc(sizeof(struct edge));
				tempnode->label = tt->label;
				tempnode->wt = tt->wt;
				tempnode->next = NULL;
				struct edge *tmp;
				tmp = outarr3[N+noofsupnodes-1];
				if(outarr3[N+noofsupnodes-1] == NULL)
					outarr3[N+noofsupnodes-1] = tempnode;
				else
				{
					while(tmp->next != NULL)
						tmp = tmp->next;
					tmp->next = tempnode;
				}
				tt = tt->next;
			}
			
			tmpc = tmpc->next;
		}
		
		int skip = 0;
		for(int i=0;i<(N+noofsupnodes-1);i++)
		{
			skip = 0;
			struct edge *tmpcc = cyclenodes;
			while(tmpcc !=NULL)
			{
				if(tmpcc->label == i+1)
				{
					skip = 1;
					break;
				}
				else
					tmpcc = tmpcc->next;
			}
			if(skip == 1)
				continue;	
		//	outarr3;
			struct edge *prevtemp = outarr2[i];
			int ce = 0;
			while(prevtemp != NULL)
			{
				ce = 0;
				tmpcc = cyclenodes;
				while(tmpcc != NULL)
				{
					ce = 0;
					if(prevtemp->label == tmpcc->label)
					{
						ce = 1;
						struct edge *T1;
						T1 = (struct edge*)malloc(sizeof(struct edge));
						T1->label = N + noofsupnodes;
						T1->wt = prevtemp->wt;
						T1->next = NULL;
						
						struct edge *T2 = outarr3[i];
						if(T2 == NULL)
							outarr3[i] = T1;
						else
						{
							while(T2->next != NULL)
							{
								T2 = T2->next;
							}
							T2->next = T1;
						}
						prevtemp = prevtemp->next;
						break;
					}
					tmpcc = tmpcc->next;	
				}
				if(ce == 1)
					continue;
				//checked if edge to cycle
				
				struct edge *T1;
				T1 = (struct edge*)malloc(sizeof(struct edge));
				T1->label = prevtemp->label;
				T1->wt = prevtemp->wt;
				T1->next = NULL;
				
				struct edge *T2 = outarr3[i];
				if(T2 == NULL)
					outarr3[i] = T1;
				else
				{
					while(T2->next != NULL)
					{
						T2 = T2->next;
					}
					T2->next = T1;
				}
				prevtemp = prevtemp->next;
			}
			
			//inarr3
			ce = 0;
			prevtemp = inarr2[i];
			while(prevtemp != NULL)
			{
				ce = 0;
				tmpcc = cyclenodes;
				while(tmpcc != NULL)
				{
					ce = 0;
					if(prevtemp->label == tmpcc->label)
					{
						ce = 1;
						struct edge *T1;
						T1 = (struct edge*)malloc(sizeof(struct edge));
						T1->label = N + noofsupnodes;
						T1->wt = prevtemp->wt;
						T1->next = NULL;
						
						struct edge *T2 = inarr3[i];
						if(T2 == NULL)
							inarr3[i] = T1;
						else
						{
							while(T2->next != NULL)
							{
								T2 = T2->next;
							}
							T2->next = T1;
						}
						prevtemp = prevtemp->next;
						break;
					}
					tmpcc = tmpcc->next;	
				}
				if(ce == 1)
					continue;
				//checked if edge to cycle
				
				struct edge *T1;
				T1 = (struct edge*)malloc(sizeof(struct edge));
				T1->label = prevtemp->label;
				T1->wt = prevtemp->wt;
				T1->next = NULL;
				
				struct edge *T2 = inarr3[i];
				if(T2 == NULL)
					inarr3[i] = T1;
				else
				{
					while(T2->next != NULL)
					{
						T2 = T2->next;
					}
					T2->next = T1;
				}
				prevtemp = prevtemp->next;
			}
			
			
		}
		//print checking
//		printf("PRINTING 3rd GRAPH before recurring Edmonds\n");
//		for(int i=0;i<N+noofsupnodes;i++)
//		{
//			struct edge *hi,*ho;
//			hi = inarr3[i];
//			printf("incoming at %d\n",i+1);
////			if(hi==NULL)printf("hinull\n");
//			while(hi!=NULL)
//			{
//				printf("label%d wt%d\n",hi->label,hi->wt);
//				hi = hi->next;
//			}
//			ho = outarr3[i];
//			printf("out from %d\n",i+1);
//			while(ho!=NULL)
//			{
//				printf("label%d wt%d\n",ho->label,ho->wt);
//				ho = ho->next;
//			}		
		//}
		
		Edmonds(N+noofsupnodes,inarr3,outarr3,s,call);
		int minwt;
		for(int i=0;i<N+noofsupnodes;i++)
		{
			struct edge *hi = inarr3[i];
			struct edge *ho;
			if(hi != NULL)
				minedgei = mini(hi);
		
			while(hi!=NULL)
			{
				if(i == N + noofsupnodes - 1 && hi->wt == minedgei)
					minwt = hi->wt;
					
				hi->wt = hi->wt - minedgei;
				ho = outarr3[(hi->label)-1];
				while(ho->label != i+1)
				{
					ho = ho->next;
				}
				if(ho->label == i+1)
					ho->wt = ho->wt - minedgei;
				hi = hi->next;
			}
		}
	//	printf("AAAAAAAAAAAAAprinting inarr3\n");
	//		printf("valof N %d noofsupnodes %d\n",N,noofsupnodes);
//			for(int i=0;i<N+noofsupnodes;i++)///////////////////////
//	//	{
	//	    struct edge *temp = inarr3[i];
//		    printf("OUT from %d\n",i+1);
	//	    while(temp!=NULL)
//		    {
	//	        printf("%d %d\n",temp->label,temp->wt);
//		        temp = temp->next;
	//	    }
	//	}
		//printf("After recurring edmonds\n");
	//	printf("AAAAAAAAAAAprinting outarr2\n");
//		for(int i=0;i<N;i++)
	//	{
	//	    struct edge *temp = outarr2[i];
//		    printf("OUT from %d\n",i+1);
//		    while(temp!=NULL)
	//	    {
//		        printf("%d %d\n",temp->label,temp->wt);
	//	        temp = temp->next;
	//	    }
//		}
//		printf("ARBORO\n");
/*
		struct edge *tsup = inarr3[N+noofsupnodes-call];
		struct arboro *tm = ans;
		int edgefrom;
		int edgeto;
		while(tsup != NULL)
		{
			if(tsup->wt == 0)
			{

				while(tm->to != N+noofsupnodes-call+1)
				{
					tm = tm->nxt;
				}
				edgefrom = tm->from;
				printf("edgefrom %d ",edgefrom);
			}
			tsup = tsup->next;
		}
		int fromnode;
		struct edge *ttt = inarr3[N+noofsupnodes-call];
		while(ttt!=NULL)
		{
			if(ttt->wt == 0)
			{
				fromnode = ttt->label;
				break;
			}
		}
		
		struct edge *T = outarr2[edgefrom - 1];
		while(T!=NULL)
		{
			struct edge *TT = cyclenodes;
			while(TT!=NULL)
			{
				if(TT->label == T->label)
				{
					edgeto = T->label;
					printf("edgeto %d\n",edgeto);
					break;
				}	
				TT = TT->next;
			}
			T = T->next;
		}
		struct arboro *AT = ans;
		while(AT!=NULL)
		{
			struct edge *TT = cyclenodes;
			while(TT!=NULL)
			{
				if(AT->from == edgeto && AT->to == TT->label)
				{
					AT->from = -1;
					AT->to = -1;
					break;
				}
				TT = TT->next;
			}
			AT = AT->nxt;
		}*/
		
		struct edge *T1 = inarr3[N+noofsupnodes-call];
		int edgefrom,edgeto,removfrom;
		
		while(T1 != NULL)
		{
			if(T1->wt == 0)
			{
				edgefrom = T1->label;
				break;
			}
			T1=T1->next;
		}
		
		struct edge *T2 = cyclenodes;
		struct edge *T3 = outarr2[edgefrom - 1];
	//	printf("outarr2\n");
	//	struct edge *c = outarr2[edgefrom-1];
	//	while(c!=NULL)
	//	{
	//		printf("label%d wt %d",c->label,c->wt);
//			c = c->next;
	////	}
//		printf("edgefrom %d\n",edgefrom);
	//	printf("minwt %d\n",minwt);
		while(T3 != NULL)
		{
			int mili = 0;
			T2 = cyclenodes;
			while(T2 != NULL)
			{	
		//		printf("T2 label %d T3 label %d T3wt %d minwt %d\n",T2->label,T3->label,T3->wt,minwt);
				if(T2->label == T3->label && T3->wt == minwt)
				{
					edgeto = T2->label;
					mili = 1;
	//				printf("Edge to be included in answer is from %d to %d\n",edgefrom,edgeto);
					break;
				}
				T2 = T2->next;
			}
			if(mili == 1)
				break;
				
			T3 = T3->next;
		}
//		printf("edgeto %d\n",edgeto);
		
		struct edge *T6 = inarr2[edgeto - 1];
		struct edge *T7 = cyclenodes;
		
		while(T6 != NULL)
		{
			int done = 0;
			T7 = cyclenodes;
			while(T7 != NULL)
			{
				if(T6->label == T7->label)
				{
					removfrom = T6->label;
	//				printf("Edge to be removed from ans is from %d to %d",removfrom,edgeto);
					done = 1;
					break;
				
				}
				T7 = T7->next;
			}
			if(done == 1)
				break;
			T6 = T6->next;
		}
		
		struct arboro *T4 = ans;
		while(T4 != NULL)
		{	
			int done = 0;
			if(T4->from == removfrom && T4->to == edgeto)
			{
						T4->from = edgefrom;
						T4->to = edgeto;
			}
		/*		struct edge *T5 = cyclenodes;
				while(T5 != NULL)
				{
					if(T4->to == T5->label)
					{
						printf("Edge to be removed from ans is from %d to %d",T4->from,T4->to);
						T4->from = edgefrom;
						T4->to = edgeto;
						done = 1;
						break;
					}
					T5 = T5->next;
				}
				if(done == 1)
					break;
			}
			*/
			T4 = T4->nxt;
			
		}
		
		
		
//		struct arboro *tmm = ans;
	//	while(tmm != NULL)
	//	{
	//		printf("from %d to %d\n",tmm->from,tmm->to);
	//		tmm = tmm->nxt;
//		}
		
		
	}

	else if(Gcycle == 0)
	{

	}


	
}
	

void main()
{
	int test;
	scanf("%d",&test);

	while(test--)
	{
		int call = 0;
		int N,s,M;
		scanf("%d %d",&N,&s);
		scanf("%d",&M);
	//	struct arboro *ans;
		ans = NULL;
		struct edge *inarr[N],*outarr[N];
		for(int i=0;i<N;i++)
		{
			inarr[i] = NULL;
			outarr[i] = NULL;
		}
		int u,v,weight;
		for(int i=0;i<M;i++)
		{
			scanf("%d %d %d",&u,&v,&weight);
			struct edge *hi,*ho;
			hi = inarr[v-1];
			ho = outarr[u-1];
			
	//		printf("YAHA AYA U V W %d\n",weight);
			struct edge *temp;
			temp = (struct edge*)malloc(sizeof(struct edge));
			temp->wt = weight;
			temp->label = u;
			temp->next = NULL;

	
			if(hi==NULL)
				inarr[v-1] = temp;
			else
			{
				while(hi->next!=NULL)
					hi = hi->next;
				hi->next = temp;
			}
			
			
			struct edge *tempo;
			tempo = (struct edge*)malloc(sizeof(struct edge));
			tempo->wt = weight;
			tempo->label = v;
			tempo->next = NULL;
			
			if(ho==NULL)
			{
				outarr[u-1]=tempo;
			}
			else
			{
				while(ho->next!=NULL)
					ho = ho->next;
				ho->next = tempo;
			}
		}
		
		Edmonds(N,inarr,outarr,s,call);
		

		int sum = 0;
		for(int i=1;i<N;i++)
		{
			struct arboro *x = ans;
			while(x->to != i+1 && x->from <= N)
			{
				x = x->nxt;
			}
			int fromm = x->from;
			struct edge *oo = outarr[fromm-1];
			while(oo->label != i+1)
				oo = oo->next;
			sum = sum +oo->wt;
		}
		printf("%d ",sum);
		printf("0 ");
		for(int i=0;i<N;i++)
		{
			printf("%d ",printdist(s,i+1,inarr,N));
		}	
		//printing parent
//		printf("Priniting parent\n");
		printf("# 0 ");
		struct arboro *tp = ans;
		for(int i=0;i<N;i++)
		{
			tp = ans;
			while(tp != NULL)
			{
				if(tp->to == i+1 && (tp->from) <= N)
					printf("%d ",tp->from);	
				
				tp = tp->nxt;
			}

		}
		printf("\n");
	//	printf("printing dist\n");

/*		//print checking
		for(int i=0;i<N;i++)
		{
			struct edge *hi,*ho;
			hi = inarr[i];
			printf("incoming at %d\n",i+1);
			if(hi==NULL)printf("hinull\n");
			while(hi!=NULL)
			{
				printf("label%d wt%d\n",hi->label,hi->wt);
				hi = hi->next;
			}
			ho = outarr[i];
			printf("out from %d\n",i+1);
			while(ho!=NULL)
			{
				printf("label%d wt%d\n",ho->label,ho->wt);
				ho = ho->next;
			}		
		}
	*/	
	}
}



/*

============INPUT ===========
2
6 1
9
1 2 10
1 3 2
1 4 10
2 3 1
3 4 4
4 5 2
2 6 8
5 2 2
4 6 4
8 1
13
1 2 5
1 5 11
5 2 6
6 5 10
6 2 2
2 3 3
2 7 13
7 6 7
3 7 9
3 4 12
4 8 1
8 3 4
7 8 8


============OUTPUT ===========
14 0 10 2 6 8 10
47 0 5 8 20 34 24 17 21 


1
5 1
8
1 3 3
1 5 1
5 3 4
3 5 0
3 4 0
4 2 0 
2 3 1
3 2 7

*/





