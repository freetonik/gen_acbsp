#include "allinc.h"
using namespace std;

//==============================================================================
// PROTOTYPES

Individual crossoverU (Individual a, Individual b);
Individual crossoverP (Individual a, Individual b);
Individual crossoverC (Individual a, Individual b);
int CgS (int g, int N, Individual S);
int HS (int N, Individual S);
void mutate(Individual &a);
void mutatePop(Population &p);
int fitness(Individual a);
void sort(Population &p);
void printInd(Individual a);
void printPop(Population p);
void evolve(Population &p);
void genLenSeq (Individual S);
int printAlpha(Population p);
void mutateAlphaSmart (Individual &a);
int getTopFit(Population);

int lastTopFit;
int newTopFit;
int noChange=1;
int repLimit=1;
// =============================================================================
// EVOLUTION


Individual crossoverU (Individual a, Individual b)
{
	Individual c;
	int templ[LENGTH_N];
	for (int i=0; i<LENGTH_N; i++) templ[i]=rand()%10;
	for (int i=0; i<LENGTH_N; i++) 
		if (templ[i]<5) c.sequence[i]=a.sequence[i]; else c.sequence[i]=b.sequence[i];
	c.fitness=fitness(c);
	return c;
}

Individual crossoverP (Individual a, Individual b)
{
	Individual c;
	int p = rand()%LENGTH_N;
	for (int i=0; i<p; i++) c.sequence[i]=a.sequence[i];
	for (int i=p; i<LENGTH_N; i++) c.sequence[i]=a.sequence[i];	
	c.fitness=fitness(c);
	return c;
}

Individual crossoverC (Individual a, Individual b)
{
	Individual c;
	Individual d;
	int A = rand()%(LENGTH_N/2);
	int B = 0;
	while (B<=A) B=rand()%LENGTH_N;

	for (int i=0; i<A; i++) c.sequence[i]=a.sequence[i];
	for (int i=A; i<B; i++) c.sequence[i]=b.sequence[i];
	for (int i=B; i<LENGTH_N; i++) c.sequence[i]=a.sequence[i];

	for (int i=0; i<A; i++) d.sequence[i]=b.sequence[i];
	for (int i=A; i<B; i++) d.sequence[i]=a.sequence[i];
	for (int i=B; i<LENGTH_N; i++) d.sequence[i]=b.sequence[i];

	if (fitness(c) < fitness(d)) return c; else return d;
}

void mutateAlphaSmart (Individual &a, Population &p)
{
	Individual tempAlpha;
	for (int i=0; i<LENGTH_N; i++) tempAlpha.sequence[i]=a.sequence[i];
	
	if (noChange > 100 && noChange <= 200) repLimit=2; else if (noChange > 200 && noChange <=300) repLimit=3; else if (noChange > 300) repLimit=4; 
	for (int i=0; i<LENGTH_N; i++)
	{
		if (tempAlpha.sequence[i]==1) tempAlpha.sequence[i]=-1; else tempAlpha.sequence[i]=1;
		if (fitness(tempAlpha) <= a.fitness)
		{
			for (int i=0; i<repLimit; i++) 	p.pop[rand()%POP_SIZE]=tempAlpha;
		}
	}

}

void mutate(Individual &a)
{
	Individual temp;
	for (int i=0; i<LENGTH_N; i++) temp.sequence[i]=a.sequence[i];
	int lmp=MUTATE_POINTS;
	if (noChange > 350) lmp = MUTATE_POINTS*2;
	if (noChange > 1000) lmp = LENGTH_N - 2;
	if (noChange > 2000) lmp = LENGTH_N;
	for (int i=0; i<lmp; i++)
	{
		int ra = rand()%LENGTH_N;
		if (temp.sequence[ra] == 1) temp.sequence[ra]=-1; else temp.sequence[ra]=1;	
	}
	if (fitness(temp) < a.fitness) a = temp;
}

void mutatePop(Population &p)
{
	for (int i=0; i<MUTATE_INDS; i++)
	{
		mutate(p.pop[rand()%POP_SIZE]);
	}
}



int fitness(Individual a)
{
	return HS(LENGTH_N, a);
}


void printInd(Individual a)
{	
	
	for (int i=0; i<LENGTH_N; i++) cout << a.sequence[i];	
}

void printPop(Population p)
{
	for (int i=0; i<POP_SIZE; i++) 
	{
		cout << "...................................." << endl;
		cout << "H(S)= " << HS(LENGTH_N, p.pop[i]) << " | ";
		printInd(p.pop[i]);
		cout << " | ";
	       	genLenSeq(p.pop[i]);
	//	cout << "==========================================" << endl;
	}
}

int getTopFit(Population p)
{
	int answer=fitness(p.pop[0]);
	for (int i=1; i<POP_SIZE; i++) if (fitness(p.pop[i]) < answer) answer = fitness(p.pop[i]);
	return answer;
}

void evolve(Population &p)
{
	
	Individual offspring[POP_SIZE/2];
	Individual winners[POP_SIZE/2];
	Individual localWinners[L_TOURN_N];
	Individual temp;	
	int g=0;
	int c=0;
	int crotype;

//============================================================================================================
	if (TERM_CRITERIA == 0)
{
while (ev<FIT_EVAL)
	{
	lastTopFit = getTopFit(p);
	//cout << endl << "     Top fit before one step of evolution: " << lastTopFit << endl;

		for (int i=0; i<=POP_SIZE/2; i++)
			{
				
				for (int j=0; j<L_TOURN_N; j++)
				{
					localWinners[j]=p.pop[rand()%(POP_SIZE)];
				}
				
				if (localWinners[0].fitness <= localWinners[1].fitness && localWinners[0].fitness <= localWinners[2].fitness) winners[i] = localWinners[0];
				else if (localWinners[1].fitness <= localWinners[0].fitness && localWinners[1].fitness <= localWinners[2].fitness) winners[i] = localWinners[1];
				else if (localWinners[2].fitness <= localWinners[0].fitness && localWinners[2].fitness <= localWinners[1].fitness) winners[i] = localWinners[2];
				else winners[i]=localWinners[0];
			}
			

		for (int i=0; i<POP_SIZE/2; i++)
			{
				crotype=rand()%3;
				switch (CROSSOVER_TYPE)
				{
					case 0: offspring[i]=crossoverU (winners[i+1], winners[i]); break;
					case 1: offspring[i]=crossoverP (winners[i+1], winners[i]); break;
              				case 2: offspring[i]=crossoverC (winners[i+1], winners[i]); break;
				}	
			       
			}

	mutatePop(p);
		for (int i=0; i<POP_SIZE/2; i++)
			{
				int index = rand()%POP_SIZE;
				for (int j=0; j<POP_SIZE/2; j++) if (fitness(offspring[i]) < fitness(p.pop[index]))	p.pop[index]=offspring[i];					
			}
		

		

	cout << "N=" << LENGTH_N << " Gen: " << g << " | ";
	newTopFit=printAlpha(p);
	//cout << "      ...new top fit after one step of evolution " << newTopFit << endl;
	if (lastTopFit==newTopFit) noChange++; else noChange=1;	
	g++;	
	}
}

//=========================================================================================================================================================

	if (TERM_CRITERIA == 1)
{
	while (g<GENERATIONS)
	{
	lastTopFit = getTopFit(p);
	//cout << endl << "     Top fit before one step of evolution: " << lastTopFit << endl;

		for (int i=0; i<=POP_SIZE/2; i++)
			{
				
				for (int j=0; j<L_TOURN_N; j++)
				{
					localWinners[j]=p.pop[rand()%(POP_SIZE)];
				}
				
				if (localWinners[0].fitness <= localWinners[1].fitness && localWinners[0].fitness <= localWinners[2].fitness) winners[i] = localWinners[0];
				else if (localWinners[1].fitness <= localWinners[0].fitness && localWinners[1].fitness <= localWinners[2].fitness) winners[i] = localWinners[1];
				else if (localWinners[2].fitness <= localWinners[0].fitness && localWinners[2].fitness <= localWinners[1].fitness) winners[i] = localWinners[2];
				else winners[i]=localWinners[0];
			}
			

		for (int i=0; i<POP_SIZE/2; i++)
			{
				crotype=rand()%3;
				switch (CROSSOVER_TYPE)
				{
					case 0: offspring[i]=crossoverU (winners[i+1], winners[i]); break;
					case 1: offspring[i]=crossoverP (winners[i+1], winners[i]); break;
              				case 2: offspring[i]=crossoverC (winners[i+1], winners[i]); break;
				}	
			       
			}

	mutatePop(p);
		for (int i=0; i<POP_SIZE/2; i++)
			{
				int index = rand()%POP_SIZE;
				for (int j=0; j<POP_SIZE/2; j++) if (fitness(offspring[i]) < fitness(p.pop[index]))	p.pop[index]=offspring[i];					
			}
		

		

	cout << "N=" << LENGTH_N << " Gen: " << g << " | ";
	newTopFit=printAlpha(p);
	//cout << "      ...new top fit after one step of evolution " << newTopFit << endl;
	if (lastTopFit==newTopFit) noChange++; else noChange=1;	
	g++;	
	}
}
}

int printAlpha(Population p)
{
	Individual te = p.pop[0];
	for (int i=1; i<LENGTH_N; i++)
	{
		if (p.pop[i].fitness < te.fitness) te = p.pop[i];
	}

	cout << " H=" << HS(LENGTH_N, te) << " S=";
	mutateAlphaSmart(te,p);
	printInd(te);
	cout << " LSeq=";
	genLenSeq(te);
	return fitness(te);
}

Individual generateInd()
{
	Individual newInd;
	newInd.fitness=-1;
	for (int k=0; k<LENGTH_N; k++) 	
	{
		int i=rand()%2;
		if (i==0) i=1; else i=-1;
		newInd.sequence[k]=i;
	}

	newInd.fitness = fitness(newInd);
return newInd;
}

void genLenSeq (Individual S)
{
	int q = 1;
	for (int i=1; i<LENGTH_N; i++)
	{
		if (S.sequence[i] == S.sequence[i-1]) q++;
		else 
		{
			cout << q;
			q=1;
		}
	}
	cout << q << endl;

}

int CgS (int g, int N, Individual S)
{
	int C=0;
	for (int i=0; i<N-g; i++)
	{
	//	cout << " i=" << i << " C=" << C;
		C=C + S.sequence[i] * S.sequence[(i+g)];
	//	cout << "+" << S.sequence[i] << "*" << S.sequence[i+g] << endl;
	}
	return C;
}

int HS (int N, Individual S)
{
	int H=0;
	for (int g=1; g<N; g++) 
	{
	//	cout << "g=" << g << ", H=" << H; 
		H = H + pow((CgS(g, N, S)),2);
	//	cout << "+" << (CgS(g, N, S)) << "^2"  << endl;
	}
	ev++;
	return H;
}
// =============================================================================
// MAIN

int main()
{
	srand(time(NULL));
	ev=0;
	Population population1;
	
	for (int r=0; r<POP_SIZE; r++) population1.pop[r]=generateInd();

	cout << "Initial population: " << endl;
	printPop(population1);	
	cout << endl << "Starting evolution..." << endl << endl;
 	evolve(population1);
	
	return 0;
}









