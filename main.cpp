//akharin test
#include <bits/stdc++.h>
#include <conio.h>



using namespace std;
string s,t[20];
int qq;
char tttt[50];
char ch[1000];
string IRtext[100000];
string token[]={"key","op","punc","num","ch","id","inv"};
string type[]={"NULL","int","float","char","bool"};
int cnt=0,pos=0,inmap=10,L=1,strpos;
set<string> keywords,operators,punc;
set<int> inv_line;
map<string , int> mp;
struct mem{
	double val;
	int id,type,number;
	bool have_val;
	mem* next;
	mem(){
		id=0;
		have_val=false;
		number=0;
		next=NULL;
	}
}first_mem;
struct node{
	int tok,line;
	char ch[30];
	bool have_else;
	node* next;
	int tedelse,tedif;
	mem* m;
	node(){
		have_else=false;
		next=NULL;
	}
}F;

node* IR_ref;
node *ref;
void fill_set();
void IR_gen(int,node*,bool);
bool is_number(string);
string tostr(node*);
void line_tokenize();
void IR_pre();
void addmem(node*);
void init(node*,mem*);
void pre_process();
void check_token();
void read_input();
bool emlaE(node*);

void eol(){
    int n=ref->line;
    while(ref->next->line==n){
        ref=ref->next;
    }
    return;
}

void kw();
int tcheck1(node *a,node *b);
int tcheck2(node *a,node *b,node *c);
void value1(node *a,node *b);
void value2(node *a,node *b,node *c,int o);
int getop(node *o);
int id();
int iff();
void tnahvi();
int id1();
int iff1();
void tmaanaii();

/************************************   main  **************************************/

int main(){
	pre_process();
	read_input();
	ref=&F;
	ref=ref->next;
	tnahvi();
	IR_pre();
	check_token();
	tmaanaii();
	return 0;
}

/************************************* rad input **********************************/

void read_input(){
	FILE * fFile;
	fFile = fopen("final.txt","r");
	bool flag=true;
	while(true){
		char ch=fgetc(fFile);
		if(ch=='#')flag=false;
		if(ch==-1 || (int)ch==5)	break;
		if((int)ch == 10 || (int) ch==13){
			if(flag)	line_tokenize();
			flag=true;
			for(int i=0 ; i<cnt ; i++)
				t[i].clear();
			cnt=0;
			continue;
		}
		else if(int(ch)==9 || int(ch)==32)
			continue;
		else{
			t[cnt]+=ch;
			while(true){
				ch=fgetc(fFile);
				if(ch=='#')
					flag=false;
				if((int)ch==5 || (int)ch==-1 || int(ch)==13 || int(ch)==10 || int(ch)==9 || int(ch)==32 ){
					fseek(fFile,-1,SEEK_CUR);
					break;
				}
				t[cnt]+=ch;
			}
			cnt++;
		}
	}
}

/**************************************** check token debug ************************/

void check_token(){
	cout<<"\npress any key to view tokenize\n";
	char fake=getch();
	ref = &F;
	while(ref->next != NULL){
		ref=ref->next;
		cout<<ref->m<<' '<<token[ref->tok]<<' ';
		for(int i=0 ; ref->ch[i]!='\0' ; i++)
			cout<< ref->ch[i];
		cout<<endl;
	}
}

/*********************************** pre process *******************************/

void pre_process(){
	FILE * pFile;
	FILE * fFile;
	FILE * vFile;
	string stt;
	pFile = fopen("input.txt","r");
	while(true){
		char ch=fgetc(pFile);
		if(ch=='#'){
			while(ch != '\"')
				ch=fgetc(pFile);
			ch=fgetc(pFile);
			int pos=0;
			while(ch!= '\"'){
				tttt[pos] = ch;
				pos++;
				ch=fgetc(pFile);
			}
			tttt[pos]='\0';
			break;
		}
	}
	fclose(pFile);
	pFile = fopen("input.txt","r");
	vFile = fopen(tttt,"r");
	fFile = fopen("final.txt","w");
	char prech;
	bool mark=false;
	while(true){
		char ch=fgetc(vFile);
		if((int)ch==-1){
			if(mark)
				fprintf(fFile,"\n");
			break;
		}
		mark=true;
		if(ch!=prech || (int)ch!=10)
		fprintf(fFile , "%c" , ch);
		prech=ch;
	}
	while(true){
		char ch=fgetc(pFile);
		if(ch==-1)
			break;
		if(ch!=prech || (int)ch!=10)
			fprintf(fFile , "%c" , ch);
		prech=ch;
	}
	char ttt=char(5);
	fprintf(fFile,"\n%c",ttt);
	fclose(fFile);
	fclose(vFile);
	fclose(pFile);
	fill_set();
}

/*********************************** fill set *******************************/

void fill_set(){
	keywords.insert("main");
	keywords.insert("if");
	keywords.insert("else");
	keywords.insert("while");
	keywords.insert("int");
	keywords.insert("char");
	keywords.insert("bool");
	keywords.insert("void");
	keywords.insert("null");
	keywords.insert("float");
	keywords.insert("return");
	operators.insert("+");
	operators.insert("=");
	operators.insert("-");
	operators.insert("/");
	operators.insert("*");
	operators.insert(">");
	operators.insert("<");
	operators.insert("==");
	operators.insert("!=");
	operators.insert("&&");
	operators.insert("||");
	punc.insert("(");
	punc.insert(")");
	punc.insert("{");
	punc.insert("}");
	punc.insert(",");
	punc.insert(";");
}

/*********************************** tokenize *******************************/

void line_tokenize(){
	pos++;
	for(int i=0 ; i<cnt ; i++){
		node * ref = &F;
		while(ref->next != NULL)
			ref = ref->next;
		ref->next = (node*) malloc(sizeof (node));
		ref=ref->next;
		ref->next=NULL;
		ref->have_else=false;
		for(int j=0 ; t[i][j-1]!='\0' ; j++)
			ref->ch[j]=t[i][j];
		if(keywords.find(t[i])!=keywords.end())
			ref->tok = 0;
		else if(operators.find(t[i])!=operators.end())
			ref->tok = 1;
		else if(punc.find(t[i])!=punc.end())
			ref->tok = 2;
		else if(is_number(tostr(ref)))
			ref->tok = 3;
		else if((int)t[i][0] == 39)
			ref->tok = 4;
		else
			ref->tok = 5;
		ref->line=pos;
		if(ref->tok == 5 && emlaE(ref)==false){
			qq=1;
			inv_line.insert(ref->line);
			cout<<"Erorr in line "<<ref->line<<" your Identifier ( "<<tostr(ref)<<" ) is not valid\n";
            ref->tok=6;
		}
	}
}

/************************************* check is number?? **********************************/

bool is_number(string s){
	int i=0;
	bool flag=false;
	if(s[0]=='+' || s[0]=='-')
		i++;
	for( ; s[i]!='\0' ; i++){
		if(s[i]=='.' && flag)
			return false;
		if(s[i]=='.'){
			flag=true;
			continue;
		}
		if(s[i] > '9' || s[i] < '0')
			return false;
	}
	return true;
}


/*********************************** lexical Erorr *******************************/

bool emlaE(node * test){
	if(tolower(test->ch[0])=='f' && tolower(test->ch[1])=='a' && tolower(test->ch[2])=='l' && tolower(test->ch[3])=='s' && tolower(test->ch[4])=='e' && test->ch[5]=='\0')
		return true;
	if(tolower(test->ch[0])=='t' && tolower(test->ch[1])=='r' && tolower(test->ch[2])=='u' && tolower(test->ch[3])=='e' && test->ch[4]=='\0')
		return true;
	if(test->ch[1] == '\0'){
		if(tolower(test->ch[0]) <= 'z' && tolower(test->ch[0]) >= 'a')
			return true;
	}
	if(test->ch[2] == '\0'){
		if(tolower(test->ch[0]) <= 'z' && tolower(test->ch[0]) >= 'a')
			if((test->ch[1] <= '9' && test->ch[1] >='9') || (tolower(test->ch[1]) <= 'z' && tolower(test->ch[1]) >= 'a'))
				return true;
	}
	return false;
}

/*********************************** convert to string *******************************/

string tostr(node * test){
	string tmp;
	int pos=0;
	while(test->ch[pos] != '\0'){
		tmp+=test->ch[pos];
		pos++;
	}
	return tmp;
}

/*********************************** pre IR *******************************/

void IR_pre(){
	node* ref= &F;
	while(ref->next != NULL){
		ref=ref->next;
		if((ref->tok != 5 && ref->tok !=3) || inv_line.find(ref->line) != inv_line.end() )
			continue;
		if(mp.find(tostr(ref)) == mp.end())
			addmem(ref);
		else{
			mem* tmp_mem = &first_mem;
			while(tmp_mem->id != mp[tostr(ref)])
				tmp_mem = tmp_mem->next;
			ref->m = tmp_mem;
		}
	}
	ref= &F;
	while(ref->next != NULL){
		ref = ref->next;
		if(ref->tok != 0)
			continue;
		if(strcmp(ref->ch,"if")==0){
			node* tmpref = ref;
			while(ref->ch[0] != '{')
				ref=ref->next;
			ref=ref->next;
			int tmp=1;
			int tedelse=0,tedif=0;
			while(tmp){
				if(strcmp(ref->ch,"}") == 0)
					tmp--;
				if(strcmp(ref->ch,"{") == 0)
					tmp++;
				if(strcmp(ref->ch,"if") == 0)
					tedif++;;
				if(strcmp(ref->ch,"else") == 0)
					tedelse++;
				ref=ref->next;
			}
			tmpref->tedif = tedif;
			tmpref->tedelse = tedelse;
			if(strcmp(ref->ch,"else") == 0)
				tmpref->have_else = true;
			ref=tmpref->next;
		}
	}
	IR_ref= &F;
	IR_gen(0,&F,false);
	FILE * IRFile;
	IRFile = fopen("IR.txt","w");
	for(int i=0 ; i<strpos ; i++){
		for(int j=0 ; j < (int)IRtext[i].size() ; j++)
			fprintf(IRFile,"%c",IRtext[i][j]);
	}
}
/*********************************** IR_gen ***********************************/

void IR_gen(int fakeL,node* fakeref,bool flag){
	while(true){
		IR_ref = IR_ref->next;
		if(inv_line.find(IR_ref->line) != inv_line.end() )
			continue;
		if(strcmp(IR_ref->ch,"return")==0){
			IRtext[strpos++]="RETURN\nCALL MAIN\n";
			return ;
		}
		if(strcmp(IR_ref->ch,"}")==0){
			if(flag)
				continue;
			else
				return ;
		}
		else if(strcmp(IR_ref->ch,"void")==0){
			while(IR_ref->ch[0] != '{')
				IR_ref=IR_ref->next;
			IRtext[strpos++]="PROCEDURE MAIN\nBEGIN\n";
		}
		else if(IR_ref->tok == 5){
			while(IR_ref->ch[0] != ';'){
				if(IR_ref->tok == 2 || IR_ref->tok == 1){
					if(IR_ref->ch[0]== '=')
						IRtext[strpos++]=" := ";
					else{
						IRtext[strpos] += ' ';
						IRtext[strpos] += IR_ref->ch[0];
						IRtext[strpos++] += ' ';
					}
				}
				else{
					sprintf(ch," M_%d",mp[tostr(IR_ref)]);
					for(int i=1 ; ch[i]!='\0' ; i++)
							IRtext[strpos] += ch[i];
						strpos++;
				}
				IR_ref=IR_ref->next;
			}
			IRtext[strpos++]="\n";
		}
		else if(strcmp(IR_ref->ch,"else")==0){
			sprintf(ch," goto L%d\n",fakeL+(fakeref->tedelse*3)+( ((fakeref->tedif)-(fakeref->tedelse)) *2 )+2);
			for(int i=1 ; ch[i]!='\0' ; i++)
				IRtext[strpos] += ch[i];
			strpos++;
			sprintf(ch," L%d\n",fakeL+(fakeref->tedelse*3)+( ((fakeref->tedif)-(fakeref->tedelse)) *2 )+1);
			for(int i=1 ; ch[i]!='\0' ; i++)
				IRtext[strpos] += ch[i];
			strpos++;
			IR_gen(0,&F,false);
			sprintf(ch," L%d\n",fakeL+(fakeref->tedelse*3)+( ((fakeref->tedif)-(fakeref->tedelse)) *2 )+2);
			for(int i=1 ; ch[i]!='\0' ; i++)
				IRtext[strpos] += ch[i];
			strpos++;
		}
		else if(strcmp(IR_ref->ch,"if")==0){

			if(IR_ref->have_else){
				node* tmpref = IR_ref;
				IR_ref = IR_ref->next;
				IR_ref = IR_ref->next;
				sprintf(ch," if ( M_%d %s M_%d ) goto L%d else goto L%d\nL%d\n",mp[tostr(IR_ref)],(IR_ref->next)->ch,mp[tostr((IR_ref->next)->next)],L,L+(tmpref->tedelse*3)+( ((tmpref->tedif)-(tmpref->tedelse)) *2 )+1 ,L);
				for(int i=1 ; ch[i]!='\0' ; i++)
					IRtext[strpos] += ch[i];
				strpos++;
				L++;
				IR_ref = IR_ref->next;
				IR_ref = IR_ref->next;
				IR_ref = IR_ref->next;
				IR_gen(L-1,tmpref,true);
			}

			else{
				node* tmpref = IR_ref;
				IR_ref = IR_ref->next;
				IR_ref = IR_ref->next;
				sprintf(ch," if ( M_%d %s M_%d ) goto L%d else goto L%d\nL%d\n",mp[tostr(IR_ref)],(IR_ref->next)->ch,mp[tostr((IR_ref->next)->next)],L,L+(tmpref->tedelse*3)+( ((tmpref->tedif)-(tmpref->tedelse)) *2 )+1 ,L);
				for(int i=1 ; ch[i]!='\0' ; i++)
					IRtext[strpos] += ch[i];
				strpos++;
				IR_ref = IR_ref->next;
				IR_ref = IR_ref->next;
				IR_ref = IR_ref->next;
				int tmpL=L;
				L++;
				IR_gen(0,&F,false);
				sprintf(ch," L%d\n",tmpL+(tmpref->tedelse*3)+( ((tmpref->tedif)-(tmpref->tedelse)) *2 )+1);
				for(int i=1 ; ch[i]!='\0' ; i++)
					IRtext[strpos] += ch[i];
				strpos++;
			}
		}
	}
}

/*********************************** get memory *******************************/

void addmem(node* ref){
	mp[tostr(ref)]=inmap;
	mem* tmp_mem= &first_mem;
	while(tmp_mem->next != NULL)
		tmp_mem=tmp_mem->next;
	tmp_mem->next = (mem*) malloc (sizeof (mem) );
	tmp_mem=tmp_mem->next;
	ref->m = tmp_mem;
	tmp_mem->next=NULL;
	tmp_mem->type=0;
	tmp_mem->id = inmap;
	tmp_mem->val=0;
	tmp_mem->have_val=false;
	if(ref->tok ==5){
        tmp_mem->val=1e-9;
	}
	if(ref->tok ==3){
		tmp_mem->have_val = true;
		init(ref,tmp_mem);
	}
}

/************************************** Initialized ******************************************/

void init(node* ref , mem* tmp_mem){
	tmp_mem->type=1;
	for(int i=0 ; ref->ch[i]!='\0' ; i++)
		if(ref->ch[i]=='.')
			tmp_mem->type = 2;
	if(tmp_mem->type == 1){
		int i=0;
		if(ref->ch[0]=='-' || ref->ch[0]=='+')
			i++;
		for( ; ref->ch[i]!='\0' ; i++)
			tmp_mem->val = tmp_mem->val*10 + ref->ch[i] - '0';
		if(ref->ch[0]=='-')
			tmp_mem->val *= -1;
	}
	else{
		int i=0;
		if(ref->ch[0]=='-' || ref->ch[0]=='+')
			i++;
		for( ; ref->ch[i]!='.' ; i++)
			tmp_mem->val = tmp_mem->val*10 + ref->ch[i] - '0';
		i++;
		for(int j=-1 ; ref->ch[i]!='\0' ; i++,j--){
			double ash= pow(10,j) * (ref->ch[i]-'0');
			tmp_mem->val = tmp_mem->val + ash;
		}
		if(ref->ch[0]=='-')
			tmp_mem->val *= -1;
	}
}

/*********************************** tahlil maanaii *******************************/

void kw(){
    node *r=ref;
    ref=ref->next;
    if(token[ref->tok]!="id"){
        ref=r;
        return;
    }
    if(ref->m->type==0){
        if(strcmp(r->ch,"int")==0){
            ref->m->type=1;
        }
        if(strcmp(r->ch,"float")==0){
            ref->m->type=2;
        }
        if(strcmp(r->ch,"char")==0){
            ref->m->type=3;
        }
        if(strcmp(r->ch,"bool")==0){
            ref->m->type=4;
        }
    }
    else{
        printf("Multiple declaration of variable %s in line %d\n",ref->ch,ref->line);
    }
    ref=r;
}

int tcheck1(node *a,node *b){
    if(a->m->type==0){
        printf("type of %s in line %d is unknown\n",a->ch,a->line);
        return 0;
    }
    if(b->m->type==0){
        printf("type of %s in line %d is unknown\n",b->ch,b->line);
        return 0;
    }
    if(a->m->type==b->m->type){
        return 1;
    }
    else{
        printf("different types for %s & %s in line %d\n",a->ch,b->ch,a->line);
        return 0;
    }
}

int tcheck2(node *a,node *b,node *c){
    if(a->m->type==0){
        printf("type of %s in line %d is unknown\n",a->ch,a->line);
        return 0;
    }
    if(b->m->type==0){
        printf("type of %s in line %d is unknown\n",b->ch,b->line);
        return 0;
    }
    if(c->m->type==0){
        printf("type of %s in line %d is unknown\n",c->ch,c->line);
        return 0;
    }
    if((a->m->type==b->m->type)&&(a->m->type==c->m->type)&&(c->m->type==b->m->type)){
        return 1;
    }
    else{
        printf("different types for %s & %s & %s in line %d\n",a->ch,b->ch,c->ch,a->line);
        return 0;
    }
}

void value1(node *a,node *b){
    if( (b->m)->val == 1e-9){
        printf("the value of %s is unknown in line %d\n",b->ch,b->line);
    }
    else{
        a->m->val=b->m->val;
    }
}
void value2(node *a,node *b,node *c,int o){
    if(b->m->val == 1e-9){
        printf("the value of %s is unknown in line %d\n",b->ch,b->line);
    }
    if(c->m->val == 1e-9){
        printf("the value of %s is unknown in line %d\n",c->ch,c->line);
    }
    else{
        if(o==1){
            a->m->val=(b->m->val)+(c->m->val);

            return;
        }
        if(o==2){
            a->m->val=(b->m->val)-(c->m->val);
            return;
        }
        if(o==3){
            a->m->val=(b->m->val)*(c->m->val);
            return;
        }
        if(o==4){
            if(c->m->val==0){
                printf("ERROR : division by zero in line %d",c->line);
                return;
            }
            a->m->val=(b->m->val)/(c->m->val);
            return;
        }
        else{
            printf("qarar nabud injuri bashe!!!\n");
        }
    }
}

int id1(){
    node *p[10];
    node *a;
    node *b;
    node *c;
    int l[10];
    l[3]=ref->line;
    p[3]=ref;
    a=ref;
    ref=ref->next;
    if(strcmp(ref->ch,"=")==0){
        ref=ref->next;
        if((token[ref->tok]=="id")||(token[ref->tok]=="num")){
            l[1]=ref->line;
            p[1]=ref;
            b=ref;
            ref=ref->next;
            if(ref->tok==1){
                int o=getop(ref);
                ref=ref->next;
                if((token[ref->tok]=="id")||(token[ref->tok]=="num")){
                    l[0]=ref->line;
                    p[0]=ref;
                    c=ref;
                    ref=ref->next;
                    if(strcmp(ref->ch,";")==0){
                        int i=tcheck2(a,b,c);
                        if(i==1){
                            value2(a,b,c,o);
                        }
                        return 1;
                    }
                    else{
                        if(l[0]!=ref->line){
                            ref=p[0];
                            return 0;
                        }
                        eol();
                        return 0;
                    }
                }
                else{
                    eol();
                    return 0;
                }
            }
            else{
                if(strcmp(ref->ch,";")==0){
                    int i=tcheck1(a,b);
                    if(i==1){
                        value1(a,b);
                    }
                    return 1;
                }
                else{
                    if(l[1]!=ref->line){
                        ref=p[1];
                        return 0;
                    }
                    eol();
                    return 0;
                }
            }
        }
        else{
            if(ref->tok==4){
                l[2]=ref->line;
                p[2]=ref;
                ref=ref->next;
                if(strcmp(ref->ch,";")==0){
                    if(a->m->type!=3){
                        printf("type of %s is not char in line %d\n",a->ch,a->line);
                    }
                    return 1;
                }
                else{
                    if(l[2]!=ref->line){
                        ref=p[2];
                        return 0;
                    }
                    eol();
                    return 0;
                }
            }
            else{
                eol();
                return 0;
            }
        }
    }
    else{
        if(strcmp(ref->ch,";")==0){
            return 1;
        }
        else{
            if(strcmp(ref->ch,",")==0){
                ref=ref->next;
                if(ref->tok==5){
                    l[4]=ref->line;
                    p[4]=ref;
                    b=ref;
                    ref=ref->next;
                    if(strcmp(ref->ch,";")==0){
                        if(b->m->type==0){
                            b->m->type=a->m->type;
                        }
                        else{
                            printf("Multiple declaration of variable %s in line %d\n",ref->ch,ref->line);
                        }
                        return 1;
                    }
                    else{
                        if(l[4]!=ref->line){
                            ref=p[4];
                            return 0;
                        }
                        eol();
                        return 0;
                    }
                }
                else{
                    eol();
                    return 0;
                }
            }
            else{
                if(l[3]!=ref->line){
                    ref=p[3];
                    return 0;
                }
                eol();
                return 0;
            }
        }
    }
}

int iff1(){
    int n=0,e=1;
    ref=ref->next;
    if(strcmp(ref->ch,"(")==0){
        ref=ref->next;
        if(token[ref->tok]=="id"){
            ref=ref->next;
            if(strcmp(ref->ch,"==")==0){
                n=1;
            }
            if(strcmp(ref->ch,"!=")==0){
                n=1;
            }
            if(strcmp(ref->ch,"<=")==0){
                n=1;
            }
            if(strcmp(ref->ch,">=")==0){
                n=1;
            }
            if(strcmp(ref->ch,"&&")==0){
                n=1;
            }
            if(strcmp(ref->ch,"||")==0){
                n=1;
            }
            if(strcmp(ref->ch,">")==0){
                n=1;
            }
            if(strcmp(ref->ch,"<")==0){
                n=1;
            }
            if(n==1){
                ref=ref->next;
                if((token[ref->tok]=="id")||(token[ref->tok]=="num")){
                    ref=ref->next;
                    if(strcmp(ref->ch,")")==0){
                        ref=ref->next;
                        if(strcmp(ref->ch,"{")==0){
                            ref=ref->next;
                            while(strcmp(ref->ch,"}")!=0){
                                if(token[ref->tok]=="id"){
                                    id1();
                                    ref=ref->next;
                                    continue;
                                }
                                if((ref->tok==0)&&(strcmp(ref->ch,"if")!=0)&&(strcmp(ref->ch,"else")!=0)){
                                    kw();
                                    ref=ref->next;
                                    continue;
                                }
                                if(strcmp(ref->ch,"if")==0){
                                    iff1();
                                    continue;
                                }
                                else{
                                    eol();
                                    ref=ref->next;
                                    continue;
                                }
                            }
                            ref=ref->next;
                            if(strcmp(ref->ch,"else")==0){
                                ref=ref->next;
                                if(strcmp(ref->ch,"{")==0){
                                    ref=ref->next;
                                    while(strcmp(ref->ch,"}")!=0){
                                        if(token[ref->tok]=="id"){
                                            id1();
                                            ref=ref->next;
                                            continue;
                                        }
                                        if((ref->tok==0)&&(strcmp(ref->ch,"if")!=0)&&(strcmp(ref->ch,"else")!=0)){
                                            kw();
                                            ref=ref->next;
                                            continue;
                                        }
                                        if(strcmp(ref->ch,"if")==0){
                                            iff1();
                                            continue;
                                        }
                                        else{
                                            eol();
                                            ref=ref->next;
                                            continue;
                                        }
                                    }
                                    ref=ref->next;
                                    return 1;
                                }
                            }
                            else{
                                return 1;
                            }

                        }
                        else{
                            return 0;
                        }
                    }
                    else{
                        return 0;
                    }
                }
                else{
                    return 0;
                }
            }
            else{
                return 0;
            }
        }
        else{
            return 0;
        }
    }
    else{
        return 0;
    }
}

void tmaanaii(){
    ref=&F;
    ref=ref->next;
    while(strcmp(ref->ch,"void")!=0){
        if(token[ref->tok]=="id"){
            id1();
            ref=ref->next;
            continue;
        }
        if((ref->tok==0)&&(strcmp(ref->ch,"if")!=0)&&(strcmp(ref->ch,"else")!=0)){
            kw();
            ref=ref->next;
            continue;
        }
        if(strcmp(ref->ch,"if")==0){
            iff1();
            continue;
        }
        else{
            eol();
            ref=ref->next;
            continue;
        }
    }
    ref=ref->next;
    if(strcmp(ref->ch,"main")==0){
        ref=ref->next;
        if(strcmp(ref->ch,"(")==0){
            ref=ref->next;
            if(strcmp(ref->ch,")")==0){
                ref=ref->next;
                if(strcmp(ref->ch,"{")==0){
                    ref=ref->next;
                    while(strcmp(ref->ch,"return")!=0){
                        if(token[ref->tok]=="id"){
                            id1();
                            ref=ref->next;
                            continue;
                        }
                        if((ref->tok==0)&&(strcmp(ref->ch,"if")!=0)&&(strcmp(ref->ch,"else")!=0)){
                            kw();
                            ref=ref->next;
                            continue;
                        }
                        if(strcmp(ref->ch,"if")==0){
                            iff1();
                            continue;
                        }
                        else{
                            eol();
                            ref=ref->next;
                            continue;
                        }
                    }
                    ref=ref->next;
                    if(strcmp(ref->ch,";")==0){
                        ref=ref->next;
                        if(strcmp(ref->ch,"}")==0){
                            return;
                        }
                        else{
                            return;
                        }
                    }
                    else{
                        return;
                    }
                }
                else{
                    return;
                }
            }
            else{
                return;
            }
        }
        else{
            return;
        }
    }
    else{
       return;
    }
}

int getop(node *o){

    if(strcmp(o->ch,"+")==0){
        return 1;
    }
    if(strcmp(o->ch,"-")==0){
        return 2;
    }
    if(strcmp(o->ch,"*")==0){
        return 3;
    }
    if(strcmp(o->ch,"/")==0){
        return 4;
    }
    else{
        printf("qarar nabud injuri she!!!\n");
    }
}


/*********************************** tahlil nahvi *******************************/

int id(){
    node *p[10];
    int l[10];
    l[3]=ref->line;
    p[3]=ref;
    ref=ref->next;
    if(strcmp(ref->ch,"=")==0){
        ref=ref->next;
        if((token[ref->tok]=="id")||(token[ref->tok]=="num")){
            l[1]=ref->line;
            p[1]=ref;
            ref=ref->next;
            if(ref->tok==1){
                ref=ref->next;
                if((token[ref->tok]=="id")||(token[ref->tok]=="num")){
                    l[0]=ref->line;
                    p[0]=ref;
                    ref=ref->next;
                    if(strcmp(ref->ch,";")==0){
                        return 1;
                    }
                    else{
                        printf("expected (( ; )) instead of %s in line %d\n",ref->ch,ref->line);
                        if(l[0]!=ref->line){
                            ref=p[0];
                            inv_line.insert(ref->line);
                            return 0;
                        }
                        inv_line.insert(ref->line);
                        eol();
                        return 0;
                    }
                }
                else{
					inv_line.insert(ref->line);
                    printf("expected id or number instead of %s in line %d\n",ref->ch,ref->line);
                    eol();
                    return 0;
                }
            }
            else{
                if(strcmp(ref->ch,";")==0){
                    return 1;
                }
                else{
                    printf("expected (( ; )) instead of %s in line %d\n",ref->ch,ref->line);
                    if(l[1]!=ref->line){
                        ref=p[1];
                        inv_line.insert(ref->line);
                        return 0;
                    }
                    inv_line.insert(ref->line);
                    eol();
                    return 0;
                }
            }
        }
        else{
            if(ref->tok==4){
                l[2]=ref->line;
                p[2]=ref;
                ref=ref->next;
                if(strcmp(ref->ch,";")==0){
                    return 1;
                }
                else{
                    printf("expected (( ; )) instead of %s in line %d\n",ref->ch,ref->line);
                    if(l[2]!=ref->line){
                        ref=p[2];
                        inv_line.insert(ref->line);
                        return 0;
                    }
                    inv_line.insert(ref->line);
                    eol();
                    return 0;
                }
            }
            else{
				inv_line.insert(ref->line);
                printf("expected id , number or character instead of %s in line %d\n",ref->ch,ref->line);
                eol();
                return 0;
            }
        }
    }
    else{
        if(strcmp(ref->ch,";")==0){
            return 1;
        }
        else{
            if(strcmp(ref->ch,",")==0){
                ref=ref->next;
                if(ref->tok==5){
                    l[4]=ref->line;
                    p[4]=ref;
                    ref=ref->next;
                    if(strcmp(ref->ch,";")==0){
                        return 1;
                    }
                    else{
                        printf("expected (( ; )) instead of %s in line %d\n",ref->ch,ref->line);
                        if(l[4]!=ref->line){
                            ref=p[4];
                            inv_line.insert(ref->line);
                            return 0;
                        }
                        inv_line.insert(ref->line);
                        eol();
                        return 0;
                    }
                }
                else{
					inv_line.insert(ref->line);
                    printf("expected character instead of %s in line %d\n",ref->ch,ref->line);
                    eol();
                    return 0;
                }
            }
            else{
                printf("expected (( ; )) or (( = )) or (( , )) instead of %s in line %d\n",ref->ch,ref->line);
                if(l[3]!=ref->line){
                    ref=p[3];
                    inv_line.insert(ref->line);
                    return 0;
                }
                inv_line.insert(ref->line);
                eol();
                return 0;
            }
        }
    }
}


int iff(){
    int n=0,e=1;
    ref=ref->next;
    if(strcmp(ref->ch,"(")==0){
        ref=ref->next;
        if(token[ref->tok]=="id"){
            ref=ref->next;
            if(strcmp(ref->ch,"==")==0){
                n=1;
            }
            if(strcmp(ref->ch,"!=")==0){
                n=1;
            }
            if(strcmp(ref->ch,"<=")==0){
                n=1;
            }
            if(strcmp(ref->ch,">=")==0){
                n=1;
            }
            if(strcmp(ref->ch,"&&")==0){
                n=1;
            }
            if(strcmp(ref->ch,"||")==0){
                n=1;
            }
            if(strcmp(ref->ch,">")==0){
                n=1;
            }
            if(strcmp(ref->ch,"<")==0){
                n=1;
            }
            if(n==1){
                ref=ref->next;
                if((token[ref->tok]=="id")||(token[ref->tok]=="num")){
                    ref=ref->next;
                    if(strcmp(ref->ch,")")==0){
                        ref=ref->next;
                        if(strcmp(ref->ch,"{")==0){
                            ref=ref->next;
                            while(strcmp(ref->ch,"}")!=0){
                                if(token[ref->tok]=="id"){
                                    id();
                                    ref=ref->next;
                                    continue;
                                }
                                if((ref->tok==0)&&(strcmp(ref->ch,"if")!=0)&&(strcmp(ref->ch,"else")!=0)){
                                    ref=ref->next;
                                    continue;
                                }
                                if(strcmp(ref->ch,"if")==0){
                                    iff();
                                    continue;
                                }
                                else{
									inv_line.insert(ref->line);
                                    printf("expected ' id ' , ' if ' or ' key word ' instead of %s in line %d\n",ref->ch,ref->line);
                                    eol();
                                    ref=ref->next;
                                    continue;
                                }
                            }
                            ref=ref->next;
                            if(strcmp(ref->ch,"else")==0){
                                ref=ref->next;
                                if(strcmp(ref->ch,"{")==0){
                                    ref=ref->next;
                                    while(strcmp(ref->ch,"}")!=0){
                                        if(token[ref->tok]=="id"){
                                            id();
                                            ref=ref->next;
                                            continue;
                                        }
                                        if((ref->tok==0)&&(strcmp(ref->ch,"if")!=0)&&(strcmp(ref->ch,"else")!=0)){
                                            ref=ref->next;
                                            continue;
                                        }
                                        if(strcmp(ref->ch,"if")==0){
                                            iff();
                                            continue;
                                        }
                                        else{
											inv_line.insert(ref->line);
                                            printf("expected ' id ' , ' if ' or ' key word ' instead of %s in line %d\n",ref->ch,ref->line);
                                            eol();
                                            ref=ref->next;
                                            continue;
                                        }
                                    }
                                    ref=ref->next;
                                    return 1;
                                }
                            }
                            else{
                                return 1;
                            }

                        }
                        else{
                            printf("expected '}' instead of %s in line %d \n",ref->ch,ref->line);
                            return 0;
                        }
                    }
                    else{
                        printf("expected ')' instead of %s in line %d \n",ref->ch,ref->line);
                        return 0;
                    }
                }
                else{
                    printf("expected id or number instead of %s in line %d \n",ref->ch,ref->line);
                    return 0;
                }
            }
            else{
                printf("expected operator instead of %s in line %d \n",ref->ch,ref->line);
                return 0;
            }
        }
        else{
            printf("expected id instead of %s in line %d \n",ref->ch,ref->line);
            return 0;
        }
    }
    else{
        printf("expected '(' instead of %s in line %d \n",ref->ch,ref->line);
        return 0;
    }

}

void tnahvi(){
    while(strcmp(ref->ch,"void")!=0){
        if(token[ref->tok]=="id"){
            id();
            ref=ref->next;
            continue;
        }
        if((ref->tok==0)&&(strcmp(ref->ch,"if")!=0)&&(strcmp(ref->ch,"else")!=0)){
            ref=ref->next;
            continue;
        }
        if(strcmp(ref->ch,"if")==0){
            iff();
            continue;
        }
        else{
			inv_line.insert(ref->line);
            printf("expected ' id ' , ' if ' or ' key word ' instead of %s in line %d\n",ref->ch,ref->line);
            eol();
            ref=ref->next;
            continue;
        }
    }
    ref=ref->next;
    if(strcmp(ref->ch,"main")==0){
        ref=ref->next;
        if(strcmp(ref->ch,"(")==0){
            ref=ref->next;
            if(strcmp(ref->ch,")")==0){
                ref=ref->next;
                if(strcmp(ref->ch,"{")==0){
                    ref=ref->next;
                    while(strcmp(ref->ch,"return")!=0){
                        if(token[ref->tok]=="id"){
                            id();
                            ref=ref->next;
                            continue;
                        }
                        if((ref->tok==0)&&(strcmp(ref->ch,"if")!=0)&&(strcmp(ref->ch,"else")!=0)){
                            ref=ref->next;
                            continue;
                        }
                        if(strcmp(ref->ch,"if")==0){
                            iff();
                            continue;
                        }
                        else{
							inv_line.insert(ref->line);
                            printf("expected ' id ' , ' if ' or ' key word ' instead of %s in line %d\n",ref->ch,ref->line);
                            eol();
                            ref=ref->next;
                            continue;
                        }
                    }
                    ref=ref->next;
                    if(strcmp(ref->ch,";")==0){
                        ref=ref->next;
                        if(strcmp(ref->ch,"}")==0){
                            return;
                        }
                        else{
							inv_line.insert(ref->line);
                            printf("expected ' } ' instead of %s in line %d\n",ref->ch,ref->line);
                            return;
                        }
                    }
                    else{
						inv_line.insert(ref->line);
                        printf("expected ' ; ' instead of %s in line %d\n",ref->ch,ref->line);
                        return;
                    }
                }
                else{
					inv_line.insert(ref->line);
                    printf("expected ' { ' instead of %s in line %d\n",ref->ch,ref->line);
                    return;
                }
            }
            else{
				inv_line.insert(ref->line);
                printf("expected ' ) ' instead of %s in line %d\n",ref->ch,ref->line);
                return;
            }
        }
        else{
			inv_line.insert(ref->line);
            printf("expected ' ( ' instead of %s in line %d\n",ref->ch,ref->line);
            return;
        }
    }
    else{
		inv_line.insert(ref->line);
       printf("expected (( main )) instead of %s in line %d\n",ref->ch,ref->line);
       return;
    }

}
