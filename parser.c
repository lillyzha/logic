#include <stdio.h>
#include <string.h>   /* for all the new-fangled string functions */
#include <stdlib.h>     /* malloc, free, rand */

int Fsize=50; /* max string length of formulas*/
int inputs=10;

struct set{
  char *item;/*first word of non-empty set*/
  struct set *tail;/*remaining words in the set*/
};

struct tableau {
  struct set *S; /* pointer to first set in non-empty list */
  struct tableau *rest; /*list of pointers to other sets*/
};

void deepFree(struct tableau *t) {
  if(!t) return;
 // while(t->S) {
  if(t->S->item!= NULL)
  {
    free(t->S->item);
    t->S->item=NULL;
    if(t->rest!= NULL){
      t->rest->S->item=NULL;
    }
  }
  struct set *tempSet = t->S;
  t->S = t->S->tail;
  free(tempSet);
  tempSet=NULL;
  //}
  struct tableau *tempTableau = t;
  t = t->rest;
  free(tempTableau);
  tempTableau=NULL;
  deepFree(t);
}

int parse(char *g){
  int negation = 0;
  int leftpar = 0;
  int parenthesis = 0;               /* checks whether the number of parenthesis is correct*/
  int binary = 0;                   /* number of binary symbols*/ 
  int nleft = 0;                     /* number of left parenthesis */
                                  /* if number of left parenthesis does not equal 
                                    the number of binary symblols, it is not a formula */
  if (strlen(g) == 0){
    return 0;
  }
  if (strlen(g) == 1){
    if (g[0] == 'p' || g[0] == 'q' || g[0] == 'r'){
      return 1;
    }
    else{
      return 0;
    }
  }
  for (int i = 0; i < strlen(g); i++){
    char x = g[i];
    switch (x){
      case '-':
        if (leftpar == 0){
          negation = 1;
        }
        break;

      case('p'|| 'q' || 'r'):
        break;

      case '(':
        if (negation == 0){
          leftpar = 1;
        }
        parenthesis += 1;
        nleft += 1;
        break;

      case ')':
        parenthesis -= 1;
        break;

      case('v'):
        binary += 1;
        break;

      case('^'):
        binary += 1;
        break;

      case('>'):
        binary += 1;
        break;

      default:
        break;
    }
  }
  if (parenthesis > 0){
    return 0;
  }
  if (nleft != binary){
    return 0;
  }
  if (negation == 1){
    return 2;
  }
  if (leftpar == 1){
    return 3;
  }
  return 0;
}

void partone(char *fmla, char *dest){
    int binaryIndex = 0;               /* index of binary  */
    int leftparIndex = 0;
    int parenthesis = 0;          
    for (int i = 0; i < strlen(fmla); i++){
    char x = fmla[i];
    switch (x){
        case '-':
            break;

        case('p'|| 'q' || 'r'):
            break;

        case '(':
            parenthesis += 1;
            if(parenthesis == 1){
                leftparIndex = i;
            }
            break;

        case ')':
            parenthesis -= 1;
            break;

        case('v'):
            if (parenthesis == 1 || parenthesis == 0){
                binaryIndex = i;
            }
            break;

        case('^'):
            if (parenthesis == 1 || parenthesis == 0){
                binaryIndex = i;
            }
            break;

        case('>'):
            if (parenthesis == 1 || parenthesis == 0){
                binaryIndex = i;
            } 
            break;

        default:
            break;
        }
    }
    for(int i = leftparIndex+1; i < binaryIndex; i++){
        dest[i-leftparIndex-1] = fmla[i];
    }
}

void parttwo(char *fmla, char *dest){
    int binaryIndex = 0;               /* index of binary  */
    int leftparIndex = 0;
    int parenthesis = 0;               
    for (int i = 0; i < strlen(fmla); i++){
    char x = fmla[i];
    switch (x){
        case '-':
            break;

        case('p'|| 'q' || 'r'):
            break;

        case '(':
            parenthesis += 1;
            if(parenthesis == 1){
                leftparIndex = i;
            }
            break;

        case ')':
            parenthesis -= 1;
            break;

        case('v'):
            if (parenthesis == 1 || parenthesis == 0){
                binaryIndex = i;
            }
            break;

        case('^'):
            if (parenthesis == 1 || parenthesis == 0){
                binaryIndex = i;
            }
            break;

        case('>'):
            if (parenthesis == 1 || parenthesis == 0){
                binaryIndex = i;
            } 
            break;

        default:
            break;
        }
    }
    for(int i = binaryIndex+1; i < strlen(fmla)-1; i++){
        dest[i-binaryIndex-1] = fmla[i]; 
    }
}

/*0 for alpha, 1 for beta, 2 for double negation, 3 negative alpha, 4 negative beta, 5 negative imp,6 imp*/
int expansionRule(char *g){
    int ruleLeft = 0;                          
    int ruleNeg = 0;
    int ruleNleft = 0;      /*numer of left parenthesis*/
    for (int i = 0; i < strlen(g); i++){
      char x = g[i];
      switch (x){
        case '-':
	  if(ruleLeft == 0){
	    if (g[i+1] == '-'){
	      return 2;
	    }
	    ruleNeg = 1;
	  }
	  break;

	case('p'|| 'q' || 'r'):
	  break;

	case '(':
	  if(ruleNeg == 0){
	    ruleLeft = 1;
	  }
	  ruleNleft += 1;
	  break;

	case ')':
	  ruleNleft -= 1;
	  break;

	case('v'):
	  if (ruleNleft == 1 || ruleNleft == 0){
	    if(ruleNeg == 0){
	      return 1;
	    }
	    else{
	      return 4;
	    }
	  }
	  break;

	case('^'):
	  if (ruleNleft == 1 || ruleNleft == 0){
	    if(ruleNeg == 0){
	      return 0;
	    }
	    else{
	      return 3;
	    }
	  }
	  break;

	case('>'):
  	  if (ruleNleft == 1 || ruleNleft == 0){
	    if(ruleNeg == 0){
	      return 6;
	    }
	    else{
	      return 5;
	    }
	  }
	  break;

	default:
	  break;
      }
    }
}

/* return 1 if is fully expanded*/
int isExp(char *g){
    if(g == NULL){
        return 1;
    }
    if(g[0] =='p'||g[0]=='q'||g[0] =='r'){
        return 1;
    }
    if(g[0] =='-'){
        if(g[1] =='p'||g[1]=='q'||g[1] =='r'){
            return 1;
        }
        else{
            return 0;
        }       
    }
    return 0;
}

void alpha(struct set *s){
    struct set* temp = (struct set*)calloc(1, sizeof(struct set));
    char *one = (char *)calloc(Fsize, sizeof(char));
    char *two = (char *)calloc(Fsize, sizeof(char));
    partone(s -> item, one);
    parttwo(s -> item, two);

	if(expansionRule(s->item)==3 || expansionRule(s->item)==4 || expansionRule(s->item)==6){
		char *negative = (char *)calloc(Fsize, sizeof(char));
		strcat(negative,"-");
		strcat(negative,one);
		strcpy(one,negative);
	}
	if(expansionRule(s->item)==3 || expansionRule(s->item)==4 || expansionRule(s->item)==5){
		char *negative = (char *)calloc(Fsize, sizeof(char));
		strcat(negative,"-");
		strcat(negative,two);
		strcpy(two,negative);
	}

	s -> item = one;
	temp -> item = two;
	temp -> tail = s -> tail;
	s -> tail = temp;
}

void beta(struct set *set, struct set* head, struct tableau *tab){
    struct tableau* newTableau = (struct tableau *)calloc(1, sizeof(struct tableau));
    struct set* newSet = (struct set *)calloc(1, sizeof(struct set));
    newTableau->S = newSet;
    struct set* currentSet = head; 

    char *one = (char *)calloc(Fsize, sizeof(char));
    char *two = (char *)calloc(Fsize, sizeof(char));
    partone(set -> item, one);
    parttwo(set -> item, two);

    if(expansionRule(set->item)==3 || expansionRule(set->item)==4 || expansionRule(set->item)==6){
	char *temp = (char *)calloc(Fsize, sizeof(char));
	strcat(temp,"-");
	strcat(temp,one);
	strcpy(one,temp);
    }
    if(expansionRule(set->item)==3 || expansionRule(set->item)==4 || expansionRule(set->item)==5){
	char *temp = (char *)calloc(Fsize, sizeof(char));
	strcat(temp,"-");
	strcat(temp,two);
	strcpy(two,temp);
    }

    if(set->item == currentSet->item){
	newSet->item = two;
	newSet->tail = (struct set *)calloc(1, sizeof(struct set));
	while(currentSet->tail != NULL){
		newSet = newSet->tail;
		currentSet = currentSet->tail;
		newSet->item = currentSet->item;
	}
	newSet->tail == NULL;
    }
    else{
	while(currentSet->tail != NULL){
		newSet->item = currentSet->item;
		newSet->tail = (struct set *)calloc(1, sizeof(struct set));
		newSet = newSet->tail;
		currentSet = currentSet->tail;
		if(currentSet->tail == NULL){
			newSet->item = two;
		}
		newSet->tail = NULL;
	}
    }
    set->item = one;
    newTableau->rest = tab->rest;
    tab->rest = newTableau;
}

void doubleNegation(struct set *s){
    char *formula = (char *)calloc(Fsize, sizeof(char));
    for(int i = 2; i < strlen(s->item); i++){
        formula[i-2] = s->item[i];
    }
    s->item = formula;
}

void complete(struct tableau *t){
    struct tableau *tab =t;
    struct set *set;
	while (tab->S != NULL){   
          set = tab -> S;
	  struct set *head = tab->S;
          while (set->item != NULL){
            if(isExp(set->item)==1){
	      if(set->tail != NULL){
	        set = set->tail;
	        if(isExp(set->item)){
	          break;
	        }
	      }
	      else{
	        break;     //s = t->rest
	      }
	    }
	    switch(expansionRule(set->item)){
	      case 0:case 4:case 5:
		alpha(set);
		break;
	      case 1:case 3:case 6:
		beta(set,head,tab);
		break;
	      case 2:
		doubleNegation(set);
		break;
	    }
          }
	  if(tab->rest == NULL){
	    break;
	  }
	  tab = tab->rest;
      }
}

bool closed(struct tableau *t){
	int open = 0;
	while(t->S!=NULL){
		struct set *set = t->S;
		int literals[6] = {0,0,0,0,0,0};
		while(set->item != NULL){
			if(strcmp(set->item,"p")==0){
				literals[0] = 1;
			}
			if(strcmp(set->item,"-p")==0){
				literals[1] = 1;
			}
			if(strcmp(set->item,"q")==0){
				literals[2] = 1;
			}
			if(strcmp(set->item,"-q")==0){
				literals[3] = 1;
			}
			if(strcmp(set->item,"r")==0){
				literals[4] = 1;
			}
			if(strcmp(set->item,"-r")==0){
				literals[5] = 1;
			}
			if(set->tail!=0){
				set=set->tail;
			}
			else{
				break;
			}
		}
		if(!(literals[0]==1 && literals[1]==1 || literals[2]==1 && literals[3]==1 || literals[4]==1 && literals[5]==1)){
			open = 1;
		}
		if(t->rest!=0){
			t = t->rest;
		}
		else{
			break;
		}
	}
	if(open != 1){
		return true;
	}
	return false;
}

int main()

{ /*input 10 strings from "input.txt" */

   int j;
    char *name = (char *) calloc(Fsize, sizeof(char));
    char *left = (char *) calloc(Fsize, sizeof(char));
    char *right = (char *) calloc(Fsize, sizeof(char));
    FILE *fp, *fpout;

    /* reads from input.txt, writes to output.txt*/
    if ((  fp=fopen("input.txt","r"))==NULL){printf("Error opening file");exit(1);}
    if ((  fpout=fopen("output.txt","w"))==NULL){printf("Error opening file");exit(1);}

 
    for(j=0;j<inputs;j++)
    {
        fscanf(fp, "%s",name);/*read formula*/
        switch (parse(name))
        {
            case(0): fprintf(fpout, "%s is not a formula.  \n", name);break;
            case(1): fprintf(fpout, "%s is a proposition. \n ", name);break;
            case(2): fprintf(fpout, "%s is a negation.  \n", name);break;
            case(3):
                partone(name, left);
		parttwo(name, right);
		fprintf(fpout, "%s is a binary. The first part is %s and the second part is %s  \n", name,left,right);
            break;
            default:fprintf(fpout, "What the f***!  ");
        }

		if (parse(name)!=0)
        {
            /* Iitialise the tableau with the formula */
            char* s = (char *)calloc(Fsize, sizeof(char));
            strcpy(s, name);
            struct set* S = (struct set*)calloc(1, sizeof(struct set));
            S->item = s;
            struct tableau* t = (struct tableau *)calloc(1, sizeof(struct tableau));
            t->S = S;

          /* Completes the tableau and checks if it is closed */
            complete(t);
            if (closed(t))  fprintf(fpout, "%s is not satisfiable.\n", name);
            else fprintf(fpout, "%s is satisfiable.\n", name);

          /* Frees all the bits of memory on the tableau*/
            deepFree(t);
        }
        else  fprintf(fpout, "I told you, %s is not a formula.\n", name);

	memset(left,0,Fsize);
        memset(right,0,Fsize);
    }

    fclose(fp);
    fclose(fpout);
    free(name);
    free(left);
    free(right);

    return(0);
}
