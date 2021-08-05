int a,b,c
float m,n
char c1,c2;//增加char类型
int abc(int a)
{/*注释部分自动去掉*/
	int i;
	if(a == 1 || a == 2){
	  return 1;
        }
	for(i<15){//增加了for语句循环
          i++;
	}
	return i
}
int main()//注释部分自动去掉
{
	int m,n,i
	char c;
	float M[10];
	m=read();
	i=1;
	i++;
	--i;//加了自增和自减
	m=i+15;//加了复合赋值运算
	while(i <= m){
		n=abc(i);
		write(n);
		i=i+1;
	}
	return 1;
}


