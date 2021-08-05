#include "def.h"

SYMBOLTABLE new_table={{0},0};//总的符号表
symbol_scope_Stack new_scope={{0},0};//区域符号表

char *strcat0(char *s1,char *s2){
    static char result[10];
    strcpy(result,s1);
    strcat(result,s2);
    return result;
}

char *newAlias() {
    static int no=1;
    char s[10];
    return strcat0("v",s);
}

char *newLabel() {
    static int no=1;
    char s[10];
    return strcat0("label",s);
}

char *newTemp(){
    static int no=1;
    char s[10];
    return strcat0("temp",s);
}


//生成一条标号语句，返回头指针
struct codenode *genLabel(char *label){
    struct codenode *h=(struct codenode *)malloc(sizeof(struct codenode));
    h->op=LABEL;
    strcpy(h->result.id,label);
    h->next=h->prior=h;
    return h;
}

//生成GOTO语句，返回头指针
struct codenode *genGoto(char *label){
    struct codenode *h=(struct codenode *)malloc(sizeof(struct codenode));
    h->op=GOTO;
    strcpy(h->result.id,label);
    h->next=h->prior=h;
    return h;
}

//合并多个中间代码的双向循环链表，首尾相连
struct codenode *merge(int num,...){
    struct codenode *h1,*h2,*p,*t1,*t2;
    va_list ap;
    va_start(ap,num);
    h1=va_arg(ap,struct codenode *);
    while (--num>0) {
        h2=va_arg(ap,struct codenode *);
        if (h1==NULL) h1=h2;
        else if (h2){
            t1=h1->prior;
            t2=h2->prior;
            t1->next=h2;
            t2->next=h1;
            h1->prior=t2;
            h2->prior=t1;
            }
        }
    va_end(ap);
    return h1;
}

//输出中间代码
void prnIR(struct codenode *head){
    char opnstr1[32],opnstr2[32],resultstr[32];
    struct codenode *h=head;
    do {
        if (h->opn1.kind==INT)
             sprintf(opnstr1,"#%d",h->opn1.const_int);
        if (h->opn1.kind==FLOAT)
             sprintf(opnstr1,"#%f",h->opn1.const_float);
        if (h->opn1.kind==ID)
             sprintf(opnstr1,"%s",h->opn1.id);
        if (h->opn2.kind==INT)
             sprintf(opnstr2,"#%d",h->opn2.const_int);
        if (h->opn2.kind==FLOAT)
             sprintf(opnstr2,"#%f",h->opn2.const_float);
        if (h->opn2.kind==ID)
             sprintf(opnstr2,"%s",h->opn2.id);
        sprintf(resultstr,"%s",h->result.id);
        switch (h->op) {
            case ASSIGNOP:  printf("  %s := %s\n",resultstr,opnstr1);
                            break;
            case PLUS:
            case MINUS:
            case STAR:
            case DIV: printf("  %s := %s %c %s\n",resultstr,opnstr1, \
                      h->op==PLUS?'+':h->op==MINUS?'-':h->op==STAR?'*':'\\',opnstr2);
                      break;
            case FUNCTION: printf("\nFUNCTION %s :\n",h->result.id);
                           break;
            case PARAM:    printf("  PARAM %s\n",h->result.id);
                           break;
            case LABEL:    printf("LABEL %s :\n",h->result.id);
                           break;
            case GOTO:     printf("  GOTO %s\n",h->result.id);
                           break;
            case JLE:      printf("  IF %s <= %s GOTO %s\n",opnstr1,opnstr2,resultstr);
                           break;
            case JLT:      printf("  IF %s < %s GOTO %s\n",opnstr1,opnstr2,resultstr);
                           break;
            case JGE:      printf("  IF %s >= %s GOTO %s\n",opnstr1,opnstr2,resultstr);
                           break;
            case JGT:      printf("  IF %s > %s GOTO %s\n",opnstr1,opnstr2,resultstr);
                           break;
            case EQ:       printf("  IF %s == %s GOTO %s\n",opnstr1,opnstr2,resultstr);
                           break;
            case NEQ:      printf("  IF %s != %s GOTO %s\n",opnstr1,opnstr2,resultstr);
                           break;
            case ARG:      printf("  ARG %s\n",h->result.id);
                           break;
            case CALL:     printf("  %s := CALL %s\n",resultstr, opnstr1);
                           break;
            case RETURN:   if (h->result.kind)
                                printf("  RETURN %s\n",resultstr);
                           else
                                printf("  RETURN\n");
                           break;

        }
    h=h->next;
    } while (h!=head);
}
void semantic_error(int line,char *msg1,char *msg2){
    //这里可以只收集错误信息，最后一次显示
    printf("在%d行,%s %s\n",line,msg1,msg2);
}


struct node *mknode(int kind,struct node *first,struct node *second, struct node *third,int pos ){
  struct node *tempnode = (struct node*)malloc(sizeof(struct node));
  tempnode->kind = kind;//结点类型
  tempnode->ptr[0] = first;//
  tempnode->ptr[1] = second;//
  tempnode->ptr[2] = third;//子树指针，由kind确定有多少颗子树
  tempnode->pos = pos;//语法单位所在位置行号
  return tempnode;
}//结点基础属性

void display(struct node* T,int indent){
    if(T){
        switch (T->kind){
            case EXT_DEF_LIST:  
                display(T->ptr[0],indent);
                display(T->ptr[1],indent);
                break;
            case EXT_VAR_DEF:
                printf("%*c%s\n",indent,' ',"外部变量定义：");     
                display(T->ptr[0],indent+5);
                printf("%*c%s\n",indent+5,' ',"变量名：");
                display(T->ptr[1],indent+5);
                break;
            case FUNC_DEF:
                printf("%*c%s\n",indent,' ',"函数定义：");
                display(T->ptr[0],indent+5);
                display(T->ptr[1],indent+5);
                display(T->ptr[2],indent+5);
                break;
            case ARRAY_DEF:
                printf("%*c%s\n",indent,' ',"数组定义：");
                display(T->ptr[0],indent+5);
                display(T->ptr[1],indent+5);
                break;
            case FUNC_DEC:
                printf("%*c%s%s\n",indent,' ',"函数名：",T->type_id);
                printf("%*c%s\n",indent,' ',"函数型参：");
                display(T->ptr[0],indent+5);
                break;
            case ARRAY_DEC:
                printf("%*c%s%s\n",indent,' ',"数组名：",T->type_id);
                printf("%*c%s\n",indent,' ',"数组大小：");
                display(T->ptr[0],indent+5);
                break;
            case EXT_DEC_LIST:
                display(T->ptr[0],indent+5);
                if(T->ptr[1]->ptr[0]==NULL)
                    display(T->ptr[1],indent+5);
                else
                    display(T->ptr[1],indent);
                break;
            case PARAM_LIST:
                display(T->ptr[0],indent);
                display(T->ptr[1],indent);
                break;
            case PARAM_DEC:
                display(T->ptr[0],indent);
                display(T->ptr[1],indent);
                break;
            case VAR_DEF:
                display(T->ptr[0],indent+5);
                display(T->ptr[1],indent+5);
                break;
            case DEC_LIST:
                printf("%*c%s\n",indent,' ',"变量名：");
                display(T->ptr[0],indent+5);
                display(T->ptr[1],indent);
                break;
            case DEF_LIST:
                printf("%*c%s\n",indent+5,' ',"LOCAL VAR_NAME：");
                display(T->ptr[0],indent+5);
                display(T->ptr[1],indent);
                break;
            case COMP_STM:
                printf("%*c%s\n",indent,' ',"复合语句：");
                printf("%*c%s\n",indent+5,' ',"复合语句的变量定义：");
                display(T->ptr[0],indent+5);
                printf("%*c%s\n",indent+5,' ',"复合语句的语句部分：");
                display(T->ptr[1],indent+5);
                break;
            case STM_LIST:
                display(T->ptr[0],indent+5);
                display(T->ptr[1],indent);
                break;
            case EXP_STMT:
                printf("%*c%s\n",indent,' ',"表达式语句：");
                display(T->ptr[0],indent+5);
                break;
            case IF_THEN:
                printf("%*c%s\n",indent,' ',"条件语句（if-else）：");
                printf("%*c%s\n",indent,' ',"条件：");
                display(T->ptr[0],indent+5);
                printf("%*c%s\n",indent,' ',"IF语句：");
                display(T->ptr[1],indent+5);
                break;
            case IF_THEN_ELSE:
                printf("%*c%s\n",indent,' ',"条件语句（if-else-if）：");
                display(T->ptr[0],indent+5);
                display(T->ptr[1],indent+5);
                break;
            case WHILE:
                printf("%*c%s\n",indent,' ',"循环语句（while）：");
                printf("%*c%s\n",indent+5,' ',"循环条件：");
                display(T->ptr[0],indent+5);
                printf("%*c%s\n",indent+5,' ',"循环体：");
                display(T->ptr[1],indent+5);
                break;
            case FOR:
                printf("%*c%s\n",indent,' ',"循环语句（for）：");
                printf("%*c%s\n",indent+5,' ',"循环条件：");
                display(T->ptr[0],indent+5);
                printf("%*c%s\n",indent+5,' ',"循环体：");
                display(T->ptr[1],indent+5);
                break;
            case FUNC_CALL:
                printf("%*c%s\n",indent,' ',"函数调用：");
                printf("%*c%s%s\n",indent+5,' ',"函数名：",T->type_id);
                printf("%*c%s\n",indent+5,' ',"第一个实际参数表达式：");
                display(T->ptr[0],indent+5);
                break;
            case ARGS:
                display(T->ptr[0],indent+5);
                display(T->ptr[1],indent+5);
                break;
            case ID:
                printf("%*cID： %s\n",indent,' ',T->type_id);//控制新的一行输出的空格数，indent代替%*c中*
                break;
            case INT:
                printf("%*cINT： %d\n",indent,' ',T->type_int);  
                break;
            case FLOAT:
                printf("%*cFLOAT： %f\n",indent,' ',T->type_float);  
                break;
            case CHAR:
                printf("%*cCHAR： %c\n",indent,' ',T->type_char);
            case ARRAY:
                printf("%*c数组名称： %s\n",indent,' ',T->type_id);  
                break;
            case TYPE:
                if(T->type==INT)
                    printf("%*c%s\n",indent,' ',"类型：int");
                else if(T->type==FLOAT)
                    printf("%*c%s\n",indent,' ',"类型：float");
                else if(T->type==CHAR)
                    printf("%*c%s\n",indent,' ',"类型：char");
                else if(T->type==ARRAY)
                    printf("%*c%s\n",indent,' ',"类型：char型数组");
                break;
            case ASSIGNOP:
            case OR:
            case AUTOADD_L:
            case AUTOSUB_L:
            case AUTOADD_R:
            case AUTOSUB_R:
            case AND:
            case RELOP:
            case PLUS:
            case MINUS:
            case STAR:
            case DIV:
            case COMADD:
            case COMSUB:
                printf("%*c%s\n",indent,' ',T->type_id);
                display(T->ptr[0],indent+5);
                display(T->ptr[1],indent+5);
                break;
            case RETURN:
                printf("%*c%s\n",indent,' ',"返回语句：");
                display(T->ptr[0],indent+5);
                break;  
        }
    }
}//将结点的各属性打印出来

void semantic_error(int line,char *msg1,char *msg2){
    //这里可以只收集错误信息，最后一次显示
    printf("在%d行,%s %s\n",line,msg1,msg2);
}



void DisplaySymbolTable()
{
    int i;
    printf("\t\t\t\t\t********符号表********\n");
    printf("\t**********************************************************************************\n");
    printf("\t%s\t\t%s\t%s\t\t%s\t\t%s\t%s\n","索 引","变 量 名","层 号","类 型","标 记"," 偏 移 量");
    printf("\t**********************************************************************************\n");
    for(i=0;i<new_table.index;i++){
        printf("\t%d\t|",i);
        printf("\t%s\t|",new_table.symbols[i].name);//变量或函数名
        printf("\t%d\t|",new_table.symbols[i].level);//层号，即在哪个层次的域
        if(new_table.symbols[i].type==INT)
               printf("\t%s\t|","int");
        else if(new_table.symbols[i].type==FLOAT)
            printf("\t%s\t|","float");
        else
            printf("\t%s\t|","char");
        printf("\t%c\t|",new_table.symbols[i].flag);//符号标记:函数、变量、参数、临时变量
        if(new_table.symbols[i].flag=='F')
            printf("\t%d\n",new_table.symbols[i].paramnum);//如果是函数，则输出形参个数
        else
            printf("\t0\n");
    }
    printf("\t**********************************************************************************\n");
    printf("\n\n\n");
}//将总的符号表打印出来，symbol结构体包含一个符号表项的部分属性。

int i,j,counter=0,t;

int Semantic_Analysis(struct node* T,int type,int level,char flag,int command)//语义分析
{//对抽象语法树的先根遍历,按displayAST的控制结构修改完成符号表管理和语义检查、TAC生成（语句部分）
    int type1,type2;
    if(T){
        switch(T->kind){
            case EXT_DEF_LIST:
                Semantic_Analysis(T->ptr[0],type,level,flag,command); //访问外部定义列表中的第一个
                Semantic_Analysis(T->ptr[1],type,level,flag,command); //访问该外部定义列表中的其它外部定义
                break;
            case EXT_VAR_DEF://处理外部说明,将第一个孩子(TYPE结点)中的类型送到第二个孩子的类型域
                type=Semantic_Analysis(T->ptr[0],type,level,flag,command); //这个外部变量的偏移量向下传递
                Semantic_Analysis(T->ptr[1],type,level,flag,command);//将一个变量的宽度向下传递
                break;
            case ARRAY_DEF:
                type = Semantic_Analysis(T->ptr[0],type,level,flag,command);
                Semantic_Analysis(T->ptr[1],type,level,flag,command);
                break;
            case ARRAY_DEC:
                flag = 'A';//Array
                strcpy(new_table.symbols[new_table.index].name,T->type_id);
                new_table.symbols[new_table.index].level=level;
                new_table.symbols[new_table.index].type=type;
                new_table.symbols[new_table.index].flag=flag;
                new_table.index++;
                break;
            case TYPE:
                return T->type;
            case EXT_DEC_LIST:
                flag='V';
                Semantic_Analysis(T->ptr[0],type,level,flag,command);
                Semantic_Analysis(T->ptr[1],type,level,flag,command);
                break;
            case ID://检测新的变量名是否唯一
                i=0;
                while(new_table.symbols[i].level!=level&&i<new_table.index)//转到相同作用域
                    i++;
                if(command==0){//定义变量
                    while(i<new_table.index){
                        if(strcmp(new_table.symbols[i].name,T->type_id)==0 && new_table.symbols[i].flag==flag){
                            if(flag=='V')
                                printf("ERROR！第%d行：全局变量中出现相同变量名%s\n",T->pos,T->type_id);
                            else if(flag=='F')
                                printf("ERROR！第%d行：函数定义中出现了相同的函数名%s\n",T->pos,T->type_id);
                            else if(flag=='T')
                                printf("ERROR！第%d行：局部变量中出现了相同的变量名%s\n",T->pos,T->type_id);
                            else
                                printf("ERROR！第%d行：函数参数中中出现了相同的变量名%s\n",T->pos,T->type_id);
                            return 0;
                        }
                        i++;
                    }
                    strcpy(new_table.symbols[new_table.index].name,T->type_id);
                    new_table.symbols[new_table.index].level=level;
                    new_table.symbols[new_table.index].type=type;
                    new_table.symbols[new_table.index].flag=flag;
                    new_table.index++;
                }
                else{//使用变量
                    i=new_table.index-1;
                    while(i>=0){
                        if(strcmp(new_table.symbols[i].name,T->type_id)==0&&(new_table.symbols[i].flag=='V'||new_table.symbols[i].flag=='T')){
                            return new_table.symbols[i].type;
                        }
                        i--;
                    }
                    if(i<0){
                        printf("ERROR！第%d行：变量名%s未定义\n",T->pos,T->type_id);
                    }
                }
                break;
            case FUNC_DEF://函数声明
                type=Semantic_Analysis(T->ptr[0],type,level+1,flag,command);
                Semantic_Analysis(T->ptr[1],type,1,flag,command);
                Semantic_Analysis(T->ptr[2],type,1,flag,command);
                break;
            case FUNC_DEC:
                strcpy(new_table.symbols[new_table.index].name,T->type_id);
                new_table.symbols[new_table.index].level=0;
                new_table.symbols[new_table.index].type=type;
                new_table.symbols[new_table.index].flag='F';
                new_table.index++;
                counter=0;
                Semantic_Analysis(T->ptr[0],type,level,flag,command);//函数形参
                new_table.symbols[new_table.index - counter - 1].paramnum=counter;
                break;
            case PARAM_LIST:
                counter++;
                Semantic_Analysis(T->ptr[0],type,level,flag,command);
                Semantic_Analysis(T->ptr[1],type,level,flag,command);
                break;
            case PARAM_DEC:
                flag='P';
                type=Semantic_Analysis(T->ptr[0],type,level+1,flag,command);
                Semantic_Analysis(T->ptr[1],type,level,flag,command);
                break;
            case COMP_STM:
                flag='T';
                command=0;
                new_scope.ScopeArray[new_scope.top]=new_table.index;
                new_scope.top++;
                Semantic_Analysis(T->ptr[0],type,level,flag,command);//分析定义列表
                command=1;
                Semantic_Analysis(T->ptr[1],type,level+1,flag,command);//分析语句列表
                new_table.index=new_scope.ScopeArray[new_scope.top-1];
                new_scope.top--;
                if (new_scope.top == 0)
                  DisplaySymbolTable();
                break;
            case DEF_LIST:
                Semantic_Analysis(T->ptr[0],type,level,flag,command);
                Semantic_Analysis(T->ptr[1],type,level,flag,command);
                break;
            case VAR_DEF:
                type=Semantic_Analysis(T->ptr[0],type,level+1,flag,command);
                Semantic_Analysis(T->ptr[1],type,level,flag,command);
                break;
            case DEC_LIST:
                Semantic_Analysis(T->ptr[0],type,level,flag,command);
                Semantic_Analysis(T->ptr[1],type,level,flag,command);
                break;
            case STM_LIST:
                Semantic_Analysis(T->ptr[0],type,level,flag,command);//第一个语句
                Semantic_Analysis(T->ptr[1],type,level,flag,command);//其他语句
                break;
            case EXP_STMT:
                Semantic_Analysis(T->ptr[0],type,level,flag,command);
                break;
            case RETURN:
                Semantic_Analysis(T->ptr[0],type,level,flag,command);
                break;
            case IF_THEN:
            case WHILE:
            case FOR:
                Semantic_Analysis(T->ptr[0],type,level,flag,command);
                Semantic_Analysis(T->ptr[1],type,level,flag,command);
                break;
            case IF_THEN_ELSE:
                Semantic_Analysis(T->ptr[0],type,level,flag,command);
                Semantic_Analysis(T->ptr[1],type,level,flag,command);
                Semantic_Analysis(T->ptr[2],type,level,flag,command);
                break;
            case ASSIGNOP:
            case OR:
            case AND:
            case RELOP:
            case PLUS:
            case MINUS:
            case STAR:
            case DIV:
            case COMADD:
            case COMSUB:
                type1=Semantic_Analysis(T->ptr[0],type,level,flag,command);
                type2=Semantic_Analysis(T->ptr[1],type,level,flag,command);
                if(type1==type2)
                    return type1;
                break;
            case AUTOADD_L:
            case AUTOSUB_L:
            case AUTOADD_R:
            case AUTOSUB_R:
                Semantic_Analysis(T->ptr[0],type,level,flag,command);
                break;
            case INT:
                return INT;
            case FLOAT:
                return FLOAT;
            case CHAR:
                return CHAR;
            case FUNC_CALL:
                j=0;
                while(new_table.symbols[j].level==0&&j<new_table.index){
                    if(strcmp(new_table.symbols[j].name,T->type_id)==0){
                        if(new_table.symbols[j].flag!='F')
                            printf("ERROR！第%d行：函数名%s在符号表中定义为变量\n",T->pos,T->type_id);
                        break;
                    }
                    j++;
                }
                if(new_table.symbols[j].level==1||j==new_table.index){
                    printf("ERROR！第%d行：函数%s未定义\n",T->pos,T->type_id);
                    break;
                }
                type=new_table.symbols[j+1].type;
                counter=0;
                Semantic_Analysis(T->ptr[0],type,level,flag,command);//分析参数
                if(new_table.symbols[j].paramnum!=counter)
                    printf("ERROR！第%d行：函数调用%s参数个数不匹配\n",T->pos,T->type_id);
                break;
            case ARGS:
                counter++;
                t=Semantic_Analysis(T->ptr[0],type,level,flag,command);
                if(type!=t)
                    printf("ERROR！第%d行：函数调用的第%d个参数类型不匹配\n",T->pos,counter);
                type=new_table.symbols[j+counter+1].type;
                Semantic_Analysis(T->ptr[1],type,level,flag,command);
                break;
        }
    }
    return 0;
}

